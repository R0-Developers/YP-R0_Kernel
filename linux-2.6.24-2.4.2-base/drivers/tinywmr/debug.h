/**
 * @file	drivers/tinywmr/debug.h
 * @brief	Debug macro, time measure macro
 *
 *  %COPYRIGHT%
 */
#ifndef _WMR_DEBUG_H_
#define _WMR_DEBUG_H_

#define MAX_NUMBER_SECTORS	16
#define IO_DIRECTION		2
#define STL_IOSTAT_PROC_NAME	"stl-iostat"
#define BML_IOSTAT_PROC_NAME	"bml-iostat"

#ifdef WMR_TIMER
#define DECLARE_TIMER	struct timeval start, stop
#define START_TIMER()	do { do_gettimeofday(&start); } while (0)
#define STOP_TIMER(name)					\
do {								\
	do_gettimeofday(&stop);					\
	if (stop.tv_usec < start.tv_usec) {			\
		stop.tv_sec -= (start.tv_sec + 1);		\
		stop.tv_usec += (1000000U - start.tv_usec);	\
	} else {						\
		stop.tv_sec -= start.tv_sec;			\
		stop.tv_usec -= start.tv_usec;			\
	}							\
	printk("STOP: %s: %d.%06d",				\
	 name, (int) (stop.tv_sec), (int) (stop.tv_usec));	\
} while (0)

#else
#define DECLARE_TIMER		do { } while (0)
#define START_TIMER()		do { } while (0)
#define STOP_TIMER(name)	do { } while (0)
#endif

/* These symbols will be use the user-level*/
//#define _RFS_DEBUG_
#ifdef _RFS_DEBUG_
#define DPRINTK(fmt, args...)						\
do {									\
	printk("%s[%d]: " fmt "\n", __func__, __LINE__, ##args);	\
} while(0)			
#else
#define DPRINTK(...)		do { } while (0)
#endif

/* This macro for print request descriptor */
//#define RFS_WMR_PRINT_REQ
#ifdef RFS_WMR_PRINT_REQ
#define print_request(req)						\
do {									\
	printk("cmd: %d, sector[%d], nr_sectors[%d], nr_segment[%d] :0x%08x\n",\
		req->cmd, req->sector, req->nr_sectors,			\
		req->nr_segments, req->buffer);				\
} while (0)
#else
#define print_request(...)	do { } while (0)
#endif

#define ERRPRINTK(format, args...)					\
do {									\
	printk("%s[%d] " format "\n", __func__, __LINE__, ##args);	\
} while (0)

/* These macros will be use for iostat */
#ifdef CONFIG_RFS_WMR_DEBUG
void stl_count_iostat(int num_sectors, int rw);
void bml_count_iostat(int num_sectors, int rw);
#else
#define stl_count_iostat(...)		do { } while (0)
#define bml_count_iostat(...)		do { } while (0)
#endif /* CONFIG_RFS_WMR_DEBUG */

#define WMR_DOWN(x)			down(x)
#define WMR_UP(x)			up(x)

#ifdef CONFIG_RFS_WMR_DEBUG
#define WMRPRINTK(format, args...)					\
do {									\
	printk("%s[%d] " format "\n", __func__, __LINE__, ##args);	\
} while (0)
#else
#define WMRPRINTK(format, args...)	do { } while (0)
#endif /* CONFIG_RFS_WMR_DEBUG */

typedef void (*wmr_reset_t)(void);

struct wmr_platform {
	wmr_reset_t	reset;
};

#endif	/* _WMR_DEBUG_H_ */
