#ifndef _WHIMORY_BASE_H_
#define _WHIMORY_BASE_H_

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/version.h>
#include "./wmr_if.h"
#include "tinywhimory/WMRRFS.h"
#include "debug.h"

/*
 * kernel version macro
 */
#undef WMR_FOR_2_6
#undef WMR_FOR_2_6_14
#undef WMR_FOR_2_6_15
#undef WMR_FOR_2_6_19

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
#define WMR_FOR_2_6             1
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19)
#define WMR_FOR_2_6_19          1
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 15)
#define WMR_FOR_2_6_15          1
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 14)
#define WMR_FOR_2_6_14          1
#endif

#define SECTOR_SIZE             512
#define SECTOR_BITS             9
#define OOB_SIZE                16
#define OOB_BITS                4
#define SECTOR_MASK             MASK(SECTOR_BITS)

#define WMR_PROC_DIR            "rfs"
#define WMR_PROC_BMLINFO        "bmlinfo"
#define WMR_PROC_STLINFO        "stlinfo"

#define MAX_BML_PART_NUM            7
// [Hyunkwon]
//#define MAX_STL_PART_NUM            2
#define MAX_STL_PART_NUM            3	

extern struct semaphore wmr_mutex;

WMRLLDSpec *wmr_get_vol_spec(void);
int wmr_update_vol_spec(void);
int bml_update_blkdev_param(u32 minor, u32 blkdev_size, u32 blkdev_blksize);
int bml_blkdev_init(void);
void bml_blkdev_exit(void);
struct block_device_operations *bml_get_block_device_operations(void);
struct block_device_operations *stl_get_block_device_operations(void);
int stl_update_blkdev_param(u32 minor, u32 blkdev_size, u32 blkdev_blksize);
int stl_blkdev_init(void);
void stl_blkdev_exit(void);


stl_info_t *wmr_get_stl_info(u32 partno);


static inline unsigned int wmr_stl_sectors_nr(stl_info_t *ssp)
{
        return (ssp->total_sectors);
}

static inline unsigned int wmr_minor(unsigned int part)
{
        return (part + 1);
}

/*Get partition*/
static inline unsigned int wmr_part(unsigned int minor)
{
        return (minor - 1);
}

static inline unsigned int wmr_is_whole_dev(unsigned int part_no)
{
        return (part_no >> PARTITION_BITS);
}

/*Get volume*/
static inline unsigned int wmr_vol_spb(WMRLLDSpec *volume)
{
        return (volume->nPgsPerBlock * volume->nSctsPerPgLLD);
}

static inline unsigned int wmr_vol_spp(WMRLLDSpec *volume)
{
        return (volume->nSctsPerPgLLD);
}

static inline unsigned int wmr_vol_sectors_nr(WMRLLDSpec *volume)
{
        return (volume->nSctsPerPgLLD * volume->nPgsPerBlock * volume->nBlksPerNand);
}

static inline unsigned int wmr_vol_blksize(WMRLLDSpec *volume)
{
        return ((volume->nPgsPerBlock * volume->nSctsPerPgLLD) << SECTOR_BITS);
}

static inline unsigned int wmr_vol_block_nr(WMRLLDSpec *volume)
{
         return (volume->nBlksPerNand);
}

static inline unsigned int wmr_stl_page_size(stl_info_t *ssp)
{
        return (ssp->page_size);
}





/* kernel 2.6 */
#include <linux/version.h>
#ifdef WMR_FOR_2_6
#include <linux/blkdev.h>

struct wmr_dev {
	struct request		*req;
        struct list_head list;
        int                     size;
        spinlock_t              lock;
        struct request_queue    *queue;
        struct gendisk          *gd;
        int                     dev_id;
	struct scatterlist *sg;
};
#else
/* Kernel 2.4 */
#ifndef __user
#define __user
#endif
#endif

#endif	/* _WHIMORY_BASE_H_ */

