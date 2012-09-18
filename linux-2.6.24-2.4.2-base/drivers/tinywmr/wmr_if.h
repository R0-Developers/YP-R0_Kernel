/**
 * @file	include/linux/xsr_if.h
 * @brief	WMR interface to export commands and macros to utils, fat
 *
 *  %COPYRIGHT%
 */
#ifndef _WMR_IF_H_
#define _WMR_IF_H_


/* this is support 15 partition*/
#define MASK(x)			((1U << (x)) -1)			
#define PARTITION_BITS              4 
#define PARTITION_MASK		MASK(PARTITION_BITS)
#define MAX_FLASH_PARTITIONS	((0x1 << PARTITION_BITS) - 1)
#define MAX_PAGE_SIZE 2048
#define MAX_OOB_SIZE 64

/* Device major number*/
#define WMR_BLK_DEVICE_RAW	137
#define WMR_BLK_DEVICE_FTL	138

typedef struct {
	unsigned int total_sectors;
	unsigned int page_size;
}stl_info_t;

#endif /* _WMR_IF_H_ */
