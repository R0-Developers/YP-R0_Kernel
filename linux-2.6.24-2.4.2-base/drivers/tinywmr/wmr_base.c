
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>

#include "wmr_base.h"

static WMRLLDSpec      vol_spec;
static stl_info_t stl_info_list[MAX_FLASH_PARTITIONS];

//int (*wmr_stl_delete)(dev_t dev, u32 start, u32 nums, u32 b_size) = NULL;

//EXPORT_SYMBOL(wmr_stl_delete);

/* To protect wmr operations, this semaphore lock wmr codes */
DECLARE_MUTEX(wmr_mutex);

/**
 * whimory_get_vol_spec - get a volume instance
 */
WMRLLDSpec *wmr_get_vol_spec(void)
{
        return &vol_spec;
}

/**
 * wmr_update_vol_spec - update volume & partition instance from the device
 */
int wmr_update_vol_spec(void)
{
        int error;
        WMRLLDSpec *vs;

        vs = wmr_get_vol_spec();
        memset(vs, 0x00, sizeof(WMRLLDSpec));

        error = BML_GetDevInfo(vs);
        /* I/O error */
        if (error)
                return -1;
        return 0;
}

/**
 * wmr_get_vol_spec - get a STL info instance
 * @param volume        : a volume number
 & @param partno        : a partition number
 */
stl_info_t *wmr_get_stl_info(u32 partno)
{
        return (stl_info_list + partno);
}

