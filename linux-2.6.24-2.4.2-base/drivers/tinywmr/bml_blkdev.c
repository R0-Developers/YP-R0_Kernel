
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 15)
#include <linux/platform_device.h>
#else
#include <linux/device.h>
#endif

#include "wmr_base.h"

#define DEVICE_NAME             "bml"
#define MAJOR_NR                WMR_BLK_DEVICE_RAW

/**
 * list to keep track of each created block devices
 */
static DECLARE_MUTEX(bml_list_mutex);
static LIST_HEAD(bml_list);

/**
 * transger data from BML to buffer cache
 * @param volume        : device number
 * @param partno        : 0~15: partition, other: whole device
 * @param req           : request description
 * @return              1 on success, 0 on failure
 *
 * It will erase a block before it do write the data
 */

// static WMRLLDSpec      vol_spec;

static int bml_transfer(u32 partno, const struct request *req)
{
        unsigned long sector, nsect;
        char *buf;
        u32 spb_shift, vbn = 0, vsn = 0;
        WMRLLDSpec *vs;
        int ret = 0;

        if (!blk_fs_request(req))
                return 0;
        sector = req->sector;
        nsect = req->current_nr_sectors;
        buf = req->buffer;

        vs = wmr_get_vol_spec();

        spb_shift = ffs(wmr_vol_spb(vs)) - 1;
        vsn += sector;

        switch (rq_data_dir(req)) {
	        case READ:
		if(vs->nSctsPerPgLLD == 8)
        	        ret = BML_Mread(partno, vsn / vs->nSctsPerPgLLD,
				nsect / vs->nSctsPerPgLLD, buf);
		else
	                ret = BML_MreadPL(partno, vsn / vs->nSctsPerPgNAND, 
				nsect / vs->nSctsPerPgNAND, buf);
		break;

        	default:
                	printk(KERN_NOTICE "Unknown request 0x%x\n", 
				(unsigned int) rq_data_dir(req));
	                return 0;
	}

        /* I/O error */
        if (ret) {
                ERRPRINTK("bml: transfer error =%d\n", ret);
                return 0;
        }
        return 1;
}


/**
 * request function which is do read/write sector
 * @param rq    : request queue which is created by blk_init_queue()
 * @return              none
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
static void bml_request(struct request_queue *rq)
#else
static void bml_request(request_queue_t *rq)
#endif
{
	u32 minor, partno;
	struct request *req;
	struct wmr_dev *dev;
        int ret;

	dev = rq->queuedata;
	if (dev->req)
		return;
 
	while ((dev->req = req = elv_next_request(rq)) != NULL) {

                spin_unlock_irq(rq->queue_lock);

                minor = dev->gd->first_minor;
                partno = wmr_part(minor);

                ret = bml_transfer(partno, req);

                spin_lock_irq(rq->queue_lock);
                end_request(req, ret);
        }
}

/**
 * free all disk structure
 * @param *dev  bml block device structure (ref. inlcude/linux/wmr_if.h)
 * @return      none
 *
 */
static void bml_del_disk(struct wmr_dev *dev)
{
        if (dev->gd) {
                del_gendisk(dev->gd);
                put_disk(dev->gd);
        }
        if (dev->queue)
                blk_cleanup_queue(dev->queue);
        list_del(&dev->list);
        kfree(dev);
        return;
}

/**
 * add each partitions as disk
 * @param volume        a volume number
 * @param partno        a partition number
 * @return              0 on success, otherwise on error
 *
 */
static int bml_add_disk(u32 partno)
{
        u32 minor, sectors;
        struct wmr_dev *dev;
        WMRLLDSpec *vs;
	u32 nMaxPages = 0;

        dev = kmalloc(sizeof(struct wmr_dev), GFP_KERNEL);
        /* memory error */
        if (!dev)
                return -ENOMEM;
        memset(dev, 0, sizeof(struct wmr_dev));
        spin_lock_init(&dev->lock);
        INIT_LIST_HEAD(&dev->list);
        down(&bml_list_mutex);
        list_add(&dev->list, &bml_list);
        up(&bml_list_mutex);

        /* init queue */
        dev->queue = blk_init_queue(bml_request, &dev->lock);
	dev->queue->queuedata = dev;
	dev->req = NULL;
	
        /* Each GBBM2 partition is a physical disk which has one partition */
        dev->gd = alloc_disk(1);
        /* memory error */
        if (!dev->gd) {
                list_del(&dev->list);
                kfree(dev);
                return -ENOMEM;
        }

        minor = wmr_minor(partno);

        dev->gd->major = MAJOR_NR;
        dev->gd->first_minor = minor;
        dev->gd->fops = bml_get_block_device_operations();
        dev->gd->queue = dev->queue;

        vs = wmr_get_vol_spec();

        /* check minor number whether it is used for chip */
        if (minor & PARTITION_MASK) {
                /* partition */
                snprintf(dev->gd->disk_name, 32, "%s%d", DEVICE_NAME, minor);
		BML_GetInfo(partno, &nMaxPages); 
                sectors = (u32)(nMaxPages * wmr_vol_spp(vs));
        } else {
                /* chip */
                snprintf(dev->gd->disk_name, 32, "%s%d/%c", DEVICE_NAME,
                                        minor >> PARTITION_BITS, 'c');
                sectors = wmr_vol_sectors_nr(vs);
        }
        /* setup block device parameter array */
        set_capacity(dev->gd, sectors);
        add_disk(dev->gd);

        return 0;
}

/**
 * update the block device parameter
 * @param minor                 minor number to update device
 * @param blkdev_size           it contains the size of all block-devices by 1KB
 * @param blkdev_blksize        it contains the size of logical block
 * @return                      0 on success, -1 on failure
 */
int bml_update_blkdev_param(u32 minor, u32 blkdev_size, u32 blkdev_blksize)
{
        struct wmr_dev *dev;
        struct gendisk *gd = NULL;
        struct list_head *this, *next;
        unsigned int nparts = 0;
        u32 partno;

        down(&bml_list_mutex);
        list_for_each_safe(this, next, &bml_list) {
                dev = list_entry(this, struct wmr_dev, list);
                if (dev && dev->gd && dev->gd->first_minor) {
			nparts = MAX_BML_PART_NUM;

                        if (dev->gd->first_minor == minor) {
                                gd = dev->gd;
                                break;
                        } else if (dev->gd->first_minor > nparts) {
                                /* update for removed disk */
                                bml_del_disk(dev);
                        }
                }
        }
        up(&bml_list_mutex);

        if (!gd) {
                partno = wmr_part(minor);
                if (bml_add_disk(partno)) {
                        /* memory error */
                        ERRPRINTK("gd updated failed, minor = %d\n", minor);
                        return -1;
                }
        } else {
                /* blkdev_size use KB, set_capacity need numbers of sector */
                set_capacity(gd, blkdev_size << 1U);

        }

        return 0;
}

/**
 * free resource
 * @return              none
 * @remark bml_blkdev is the build-in code, so it will be never reached
 */
static void bml_blkdev_free(void)
{
        struct wmr_dev *dev;
        struct list_head *this, *next;

        down(&bml_list_mutex);
        list_for_each_safe(this, next, &bml_list) {
                dev = list_entry(this, struct wmr_dev, list);
                bml_del_disk(dev);
        }
        up(&bml_list_mutex);
	BML_Close();
}

/**
 * bml_blkdev_create - create device node
 * @return              none
 */
static int bml_blkdev_create(void)
{
        u32 partno, i;
        int ret;

        ret = BML_Open();

        ret = wmr_update_vol_spec();
        if (ret) {
                BML_Close();
                return 0;
        }

        /*
         * which is better auto or static?
         */
        for (i = 0; i < MAX_BML_PART_NUM; i++) {
                /* when partno has -1, it means whole device */
                partno = i - 1;

                ret = bml_add_disk(partno);
                if (ret)
	                continue;
        }
//        BML_Close();

        return 0;
}

/**
 * initialize bml driver structure
 */
static struct device_driver bml_driver = {
        .name           = DEVICE_NAME,
        .bus            = &platform_bus_type,
};

/**
 * initialize bml device structure
 */
static struct platform_device bml_device = {
        .name   = DEVICE_NAME,
};

/**
 * create device node, it will scan every chips and partitions
 * @return      0 on success, otherwise on error
 */
int __init bml_blkdev_init(void)
{
        if (register_blkdev(MAJOR_NR, DEVICE_NAME)) {
                printk(KERN_WARNING "raw: unable to get major %d\n", MAJOR_NR);
                return -EAGAIN;
        }

        if (bml_blkdev_create()) {
                unregister_blkdev(MAJOR_NR, DEVICE_NAME);
                return -ENOMEM;
        }

        if (driver_register(&bml_driver)) {
                bml_blkdev_free();
                unregister_blkdev(MAJOR_NR, DEVICE_NAME);
                return -ENODEV;
        }

        if (platform_device_register(&bml_device)) {
                driver_unregister(&bml_driver);
                bml_blkdev_free();
                unregister_blkdev(MAJOR_NR, DEVICE_NAME);
                return -ENODEV;
        }

        return 0;
}

/**
 * initialize the bml devices
 * @return              0 on success, otherwise on failure
 */
void __exit bml_blkdev_exit(void)
{
        platform_device_unregister(&bml_device);
        driver_unregister(&bml_driver);

        bml_blkdev_free();

        unregister_blkdev(MAJOR_NR, DEVICE_NAME);
}

MODULE_LICENSE("Samsung Proprietary");
MODULE_AUTHOR("Samsung Electronics");
MODULE_DESCRIPTION("The kernel 2.6 block device interface for BML");


