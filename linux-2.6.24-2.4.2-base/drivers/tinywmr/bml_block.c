
#include <linux/module.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

#include <asm/errno.h>
#include <asm/uaccess.h>

#include "wmr_base.h"

/* conditions that ignore ECC error */

/* if main_area[24 ~ 27] == ~main_area[56 ~ 59], ready mark */
#define READY_MARK_CONDITION \
        ((kpage->mbuf[24]^kpage->mbuf[56]) == 0xFF) && \
        ((kpage->mbuf[25]^kpage->mbuf[57]) == 0xFF) && \
        ((kpage->mbuf[26]^kpage->mbuf[58]) == 0xFF) && \
        ((kpage->mbuf[27]^kpage->mbuf[59]) == 0xFF)
/* if main_area[32 ~ 35] == ~main_area[44 ~ 47], erase count */
#define ERASE_COUNT_CONDITION \
        ((kpage->mbuf[32]^kpage->mbuf[44]) == 0xFF) && \
        ((kpage->mbuf[33]^kpage->mbuf[45]) == 0xFF) && \
        ((kpage->mbuf[34]^kpage->mbuf[46]) == 0xFF) && \
        ((kpage->mbuf[35]^kpage->mbuf[47]) == 0xFF)
/* if main_area[64 ~ 67] == ~main_area[68 ~ 71], partition id */
#define PARTITION_ID_CONDITION \
        ((kpage->mbuf[64]^kpage->mbuf[68]) == 0xFF) && \
        ((kpage->mbuf[65]^kpage->mbuf[69]) == 0xFF) && \
        ((kpage->mbuf[66]^kpage->mbuf[70]) == 0xFF) && \
        ((kpage->mbuf[67]^kpage->mbuf[71]) == 0xFF)
/* if main_area[24 ~ 27] == 0xA5 */
#define MAIN_AREA_CONDITION \
        (kpage->mbuf[24] == 0xA5) && \
        (kpage->mbuf[25] == 0xA5) && \
        (kpage->mbuf[26] == 0xA5) && \
        (kpage->mbuf[27] == 0xA5)
/* if spare_area[0 ~ 15] == 0xFF */
#define SPARE_AREA_CONDITION \
        (kpage->sbuf[0] == 0xFF) && \
        (kpage->sbuf[1] == 0xFF) && \
        (kpage->sbuf[2] == 0xFF) && \
        (kpage->sbuf[3] == 0xFF) && \
        (kpage->sbuf[4] == 0xFF) && \
        (kpage->sbuf[5] == 0xFF) && \
        (kpage->sbuf[6] == 0xFF) && \
        (kpage->sbuf[7] == 0xFF) && \
        (kpage->sbuf[8] == 0xFF) && \
        (kpage->sbuf[9] == 0xFF) && \
        (kpage->sbuf[10] == 0xFF) && \
        (kpage->sbuf[11] == 0xFF) && \
        (kpage->sbuf[12] == 0xFF) && \
        (kpage->sbuf[13] == 0xFF) && \
        (kpage->sbuf[14] == 0xFF) && \
        (kpage->sbuf[15] == 0xFF)

#if 0
/**
 * update the information about whole partitions on linux blkdev
 * @param volume        volume number
 * @return              0 on success, -1 on failure
 */
static int bml_update_whole_blkdev(void)
{
        int ret = 0;
        u32 i, minor, partsize, pagesize, blocksize, nMaxPages;
        WMRLLDSpec *vs = wmr_get_vol_spec();

        blocksize = wmr_vol_blksize(vs) >> 10U; /* by 1KB */
        /* update the block size */
        for (i = 0 ; i < MAX_PART_NUM; i++) {
                minor = wmr_minor(i);
		BML_GetInfo(i, &nMaxPages);
                partsize = nMaxPages / vs->nPgsPerBlock * blocksize; /* 1KB */
                pagesize = wmr_vol_spp(vs) << 9U;/* by Byte*/
                ret = bml_update_blkdev_param(minor, partsize, pagesize);
        }

        return ret;
}
#endif

/**
 * BML block open operation
 * @param inode         block device inode
 * @param file          block device file
 * @return              0 on success, otherwise on failure
 */
#if 0
static int bml_block_open(struct inode *inode, struct file *file)
{
        int ret;

        ret = BML_Open();
        if (ret) {
                printk(KERN_ERR "BML: open error = 0x%x\n", ret);
                return -ENODEV;
        }

        return 0;
}

/**
 * bml_block_release - BML block release operation
 * @param inode         block device inode
 * @param file          block device file
 * @return              0 on success
 */
static int bml_block_release(struct inode *inode, struct file *file)
{
        BML_Close();

        return 0;
}
#endif

/**
 * WMR common block device operations
 */
static struct block_device_operations bml_block_fops = {
        .owner          = THIS_MODULE,
//        .open           = bml_block_open,
//        .release        = bml_block_release,
//        .ioctl          = bml_block_ioctl,
};

/**
 * bml_get_block_device_ops
 * @return              WMR common block device operations
 */
struct block_device_operations *bml_get_block_device_operations(void)
{
        return &bml_block_fops;
}

/**
 * BML block module init
 * @return      0 on success
 */
static int __init bml_block_init(void)
{
        if (bml_blkdev_init() == 0)
                printk(KERN_INFO "WMR: Registered BML Driver.\n");
        else
                printk(KERN_INFO "WMR: Couldn't register BML Driver.\n");

        return 0;
}

/**
 * BML block module exit
 */
static void __exit bml_block_exit(void)
{
        bml_blkdev_exit();
}

module_init(bml_block_init);
module_exit(bml_block_exit);

MODULE_LICENSE("Samsung Proprietary");
MODULE_AUTHOR("Samsung Electronics");
MODULE_DESCRIPTION("BML common block device layer");

