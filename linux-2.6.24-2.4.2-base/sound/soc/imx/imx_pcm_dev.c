/*
 * imx_pcm_dev.c  --  user mode filesystem api for pcm stream recording
 *
 * Copyright 2008 Freescale Semiconductor
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  Revision history
 *    14th Dec 2008   Initial version.
 *
 */
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/cdev.h> 
#include <asm/uaccess.h> 
#include <linux/device.h>
#include <sound/pcm.h>


#define MXC_PCM_DEV_NAME	"mxc_pcm_dev"
#define FIXED_DEV_NODE

#ifdef FIXED_DEV_NODE
#define SND_PCM_DEV_MAJOR	202
#endif

/* debug */
// #define IMX_PCM_DEV_DEBUG 1 
#if IMX_PCM_DEV_DEBUG
#define dbg(format, arg...) printk(format, ## arg)
#else
#define dbg(format, arg...)
#endif

#define	PCM_RECORD_CANCEL	_IO('g',1)

struct mxc_runtime_data {
	int dma_ch;
	spinlock_t dma_lock;
	int active, period, periods;
	int dma_wchannel;
	int dma_active;
	int dma_alloc;
};

static struct class *mxc_pcm_dev_class;
static int mxc_pcm_dev_major = 0;
static int mxc_pcm_dev_minor = 0;

#ifdef CONFIG_SND_MXC_SOC_IRAM
static int buf_size = SND_RAM_SIZE;
#else
static int buf_size = 64 * 1024,
#endif
static int buf_index = 0;
char * pcm_buf;

extern struct record_playback mxc_record_playback;
extern void msleep(unsigned int msecs);

static void complete_delay_work ()
{
	int i = 30;
	while (i--) {
		if (mxc_record_playback.state == RECORD_ON){	
			complete(mxc_record_playback.context);
			break;
		}
		else
			msleep(1);
	}
	if ((i == 0) && (mxc_record_playback.state == RECORD_DEV_OPEN))
		dbg("pcm_capture underrun\n");
	
	return;
}

static DECLARE_DELAYED_WORK(complete_work, complete_delay_work);

static int mxc_pcm_dev_open (struct inode *inode, struct file *file)
{
	pcm_buf = kzalloc(buf_size * 2, GFP_KERNEL);
	if (pcm_buf == NULL)
		return -ENOMEM;
	
	mxc_record_playback.state = RECORD_DEV_OPEN;
	mxc_record_playback.cancel_record= 0;
	mxc_record_playback.period_number = 0;
	buf_index = 0;

    return 0;
}

static int mxc_pcm_dev_release (struct inode *inode, struct file *file)
{
	mxc_record_playback.state = RECORD_DEV_CLOSE;	
	kfree(pcm_buf);
    return 0;
}

static void rec_pla_complete(struct record_playback *rec_pb)
{
	struct snd_pcm_runtime *runtime;
	struct mxc_runtime_data *prtd;
	unsigned int period_size, period_offset, pcm_buf_offset, i;
	char *period_buf;;

	if (mxc_record_playback.state == RECORD_DEV_CLOSE)
		return;

	runtime = mxc_record_playback.runtime;
	period_size = frames_to_bytes(runtime, runtime->period_size);
	prtd = runtime->private_data;
	
	if (mxc_record_playback.period_number == 1){
		memset(pcm_buf, 0, buf_size * 2);
		buf_index = 0;
		dbg("pb_size=%d, periods=%d\n", period_size, runtime->periods);
	}

	period_offset = period_size * prtd->period;
	period_buf = runtime->dma_area + period_offset;
//	pcm_buf_offset = ((mxc_record_playback.period_number - 1) % runtime->periods) * period_size;
	pcm_buf_offset = ((mxc_record_playback.period_number - 1) % 2) * period_size;
	pcm_buf_offset += buf_index * runtime->periods * period_size;

#if 0
	printk("period_number=%d\n  ", mxc_record_playback.period_number);
	for (i=0; i<16; i++){
		printk("%2x ", *(runtime->dma_area+period_offset+i));
	}
	printk("\n");
#endif
		
	memcpy(pcm_buf + pcm_buf_offset, period_buf, period_size);
// if (mxc_record_playback.period_number % runtime->periods == 0) {
   if (mxc_record_playback.period_number % 2 == 0) {

		buf_index ++; 
		buf_index %= 2;
		if (mxc_record_playback.state == RECORD_ON)	
			complete((struct completion *)rec_pb->context);
		else{
			schedule_delayed_work(&complete_work, 1);
			dbg("schedule_delayed_work: mxc_record_playback.period_number=%d\n", mxc_record_playback.period_number);
		}
	}
}

ssize_t mxc_pcm_dev_read (struct file *filp, char *rec_buff,
		size_t count, loff_t *offp)
{
    ssize_t rec_size = 0;
    int value;
    unsigned int offset, period_size, buffer_size, data_size;
	struct snd_pcm_runtime *runtime;
	
    DECLARE_COMPLETION_ONSTACK (done);

    if (mxc_record_playback.state == RECORD_DEV_OPEN) {
		mxc_record_playback.context = &done;
		mxc_record_playback.state = RECORD_ON;
    }else {
    	printk("exit: mxc_record_playback.state=%d\n", mxc_record_playback.state);
    	return 0;
	}

	value = wait_event_interruptible (done.wait, done.done);
	if (value != 0) {
		printk(KERN_ERR "%s: i/o interrupted\n", __func__);
		mxc_record_playback.state = RECORD_DEV_OPEN;
		return -1;
	} else {
		mxc_record_playback.state = RECORD_DEV_OPEN;
		if (mxc_record_playback.cancel_record == 1){
			mxc_record_playback.cancel_record = 0;
			return 0;
		}
		
		runtime = mxc_record_playback.runtime;
		period_size = frames_to_bytes(runtime, runtime->period_size);
		buffer_size = runtime->periods * period_size;
		data_size = period_size * 2;
		offset = buf_index ? 0 : buffer_size;
		rec_size = copy_to_user (rec_buff, pcm_buf + offset, data_size);
		if (rec_size < 0)
			printk(KERN_ERR "%s: error\n", __func__);
		
	//	return (buffer_size - rec_size);
	    return data_size;
	}
}

ssize_t mxc_pcm_dev_ioctl (struct inode *inode, struct file *filp, 
		unsigned int code, unsigned long value)
{
	int ret = 0;
	
	switch (code) {
		case PCM_RECORD_CANCEL:
			mxc_record_playback.cancel_record = 1;
			if (mxc_record_playback.state == RECORD_ON)
				complete(mxc_record_playback.context);
			else if(mxc_record_playback.state == RECORD_DEV_OPEN)
				schedule_delayed_work(&complete_work, 1);
			break;
		default:
			ret = -ENOTTY;
	}
	return ret;	
}

struct file_operations mxc_pcm_dev_fops = {
	.owner		= THIS_MODULE,
	.open		= mxc_pcm_dev_open,
	.release	= mxc_pcm_dev_release,
	.read		= mxc_pcm_dev_read,
	.ioctl		= mxc_pcm_dev_ioctl,
};



static int __init mxc_pcm_dev_init (void)
{
	int ret = 0;
	struct class_device *mxc_pcm_dev;

#ifdef FIXED_DEV_NODE
	ret = register_chrdev(SND_PCM_DEV_MAJOR, MXC_PCM_DEV_NAME, &mxc_pcm_dev_fops);
	if (ret < 0)
		printk(KERN_ERR "unable to register pcm capture char device\n");
#else   
	mxc_pcm_dev_major = register_chrdev(0, MXC_PCM_DEV_NAME, &mxc_pcm_dev_fops);
	if (mxc_pcm_dev_major < 0) {
		printk(KERN_ERR "unable to get a major for mxc_pcm_dev\n");
		return mxc_pcm_dev_major;
	}

	mxc_pcm_dev_class = class_create(THIS_MODULE, MXC_PCM_DEV_NAME);
	if (IS_ERR(mxc_pcm_dev_class)) {
		printk(KERN_ERR "Error creating mxc_pcm_dev class\n");
		ret = -1;
		goto err;
	}	

	mxc_pcm_dev = class_device_create(mxc_pcm_dev_class, NULL, 
			MKDEV(mxc_pcm_dev_major, mxc_pcm_dev_minor), NULL, MXC_PCM_DEV_NAME);
	if (IS_ERR(mxc_pcm_dev)) {
		printk(KERN_ERR "Error creating mxc_pcm_dev class\n");
		ret = -1;
		goto err1;
	}
#endif

	mxc_record_playback.complete = rec_pla_complete;
	dbg (KERN_INFO "mxc_pcm_dev char device registered\n");
	return ret;

err1:
	class_destroy(mxc_pcm_dev_class);
err:
	unregister_chrdev(mxc_pcm_dev_major, MXC_PCM_DEV_NAME);

	
	return ret;    
}

static void __exit mxc_pcm_dev_exit (void)
{
#ifndef FIXED_DEV_NODE
	class_device_destroy(mxc_pcm_dev_class, 
		MKDEV(mxc_pcm_dev_major, mxc_pcm_dev_minor));
	class_destroy(mxc_pcm_dev_class);
#endif
	unregister_chrdev(SND_PCM_DEV_MAJOR, MXC_PCM_DEV_NAME);

	dbg(KERN_INFO "mxc_pcm_dev char device : exit\n");
}
module_init (mxc_pcm_dev_init);
module_exit (mxc_pcm_dev_exit);

MODULE_AUTHOR("Wang Shaojun");
MODULE_DESCRIPTION("PCM Recording When Playback");
MODULE_LICENSE("GPL");
