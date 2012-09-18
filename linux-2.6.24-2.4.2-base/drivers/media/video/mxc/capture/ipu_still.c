/*
 * Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file ipu_still.c
 *
 * @brief IPU Use case for still image capture
 *
 * @ingroup IPU
 */

#include <asm/semaphore.h>
#include "mxc_v4l2_capture.h"
#include <asm/arch/ipu.h>
#include "ipu_prp_sw.h"

static int callback_flag;
/*
 * Function definitions
 */

/*!
 * CSI EOF callback function.
 *
 * @param irq       int irq line
 * @param dev_id    void * device id
 *
 * @return status   IRQ_HANDLED for handled
 */
static irqreturn_t prp_csi_eof_callback(int irq, void *dev_id)
{
	if (callback_flag == 2) {
		ipu_select_buffer(CSI_MEM, IPU_OUTPUT_BUFFER, 0);
		ipu_enable_channel(CSI_MEM);
	}

	callback_flag++;
	return IRQ_HANDLED;
}

/*!
 * CSI callback function.
 *
 * @param irq       int irq line
 * @param dev_id    void * device id
 *
 * @return status   IRQ_HANDLED for handled
 */
static irqreturn_t prp_still_callback(int irq, void *dev_id)
{
	cam_data *cam = (cam_data *) dev_id;

	cam->still_counter++;
	wake_up_interruptible(&cam->still_queue);

	return IRQ_HANDLED;
}

/*!
 * start csi->mem task
 * @param private       struct cam_data * mxc capture instance
 *
 * @return  status
 */
static int prp_still_start(void *private)
{
	cam_data *cam = (cam_data *) private;
	u32 pixel_fmt;
	int err;

	if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV420)
		pixel_fmt = IPU_PIX_FMT_YUV420P;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV422P)
		pixel_fmt = IPU_PIX_FMT_YUV422P;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_UYVY)
		pixel_fmt = IPU_PIX_FMT_UYVY;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_BGR24)
		pixel_fmt = IPU_PIX_FMT_BGR24;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB24)
		pixel_fmt = IPU_PIX_FMT_RGB24;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB565)
		pixel_fmt = IPU_PIX_FMT_RGB565;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_BGR32)
		pixel_fmt = IPU_PIX_FMT_BGR32;
	else if (cam->v2f.fmt.pix.pixelformat == V4L2_PIX_FMT_RGB32)
		pixel_fmt = IPU_PIX_FMT_RGB32;
	else {
		printk(KERN_ERR "format not supported\n");
		return -EINVAL;
	}

	err = ipu_init_channel(CSI_MEM, NULL);
	if (err != 0)
		return err;
	ipu_csi_enable_mclk(CSI_MCLK_RAW, true, true);

	err = ipu_init_channel_buffer(CSI_MEM, IPU_OUTPUT_BUFFER,
				      pixel_fmt, cam->v2f.fmt.pix.width,
				      cam->v2f.fmt.pix.height,
				      cam->v2f.fmt.pix.width, IPU_ROTATE_NONE,
				      cam->still_buf, 0, 0, 0);
	if (err != 0)
		return err;

	err = ipu_request_irq(IPU_IRQ_SENSOR_OUT_EOF, prp_still_callback,
			      0, "Mxc Camera", cam);
	if (err != 0) {
		printk(KERN_ERR "Error registering irq.\n");
		return err;
	}
	callback_flag = 0;
	err = ipu_request_irq(IPU_IRQ_SENSOR_EOF, prp_csi_eof_callback,
			      0, "Mxc Camera", NULL);
	if (err != 0) {
		printk(KERN_ERR "Error IPU_IRQ_SENSOR_EOF \n");
		return err;
	}

	return err;
}

/*!
 * stop csi->mem encoder task
 * @param private       struct cam_data * mxc capture instance
 *
 * @return  status
 */
static int prp_still_stop(void *private)
{
	cam_data *cam = (cam_data *) private;
	int err = 0;

	ipu_free_irq(IPU_IRQ_SENSOR_EOF, NULL);
	ipu_free_irq(IPU_IRQ_SENSOR_OUT_EOF, cam);

	ipu_disable_channel(CSI_MEM, true);
	ipu_uninit_channel(CSI_MEM);
	ipu_csi_enable_mclk(CSI_MCLK_RAW, false, false);

	return err;
}

/*!
 * function to select CSI_MEM as the working path
 *
 * @param private       struct cam_data * mxc capture instance
 *
 * @return  status
 */
int prp_still_select(void *private)
{
	cam_data *cam = (cam_data *) private;

	if (cam) {
		cam->csi_start = prp_still_start;
		cam->csi_stop = prp_still_stop;
	}

	return 0;
}

/*!
 * function to de-select CSI_MEM as the working path
 *
 * @param private       struct cam_data * mxc capture instance
 *
 * @return  status
 */
int prp_still_deselect(void *private)
{
	cam_data *cam = (cam_data *) private;
	int err = 0;

	err = prp_still_stop(cam);

	if (cam) {
		cam->csi_start = NULL;
		cam->csi_stop = NULL;
	}

	return err;
}

/*!
 * Init the Encorder channels
 *
 * @return  Error code indicating success or failure
 */
__init int prp_still_init(void)
{
	return 0;
}

/*!
 * Deinit the Encorder channels
 *
 */
void __exit prp_still_exit(void)
{
}

module_init(prp_still_init);
module_exit(prp_still_exit);

EXPORT_SYMBOL(prp_still_select);
EXPORT_SYMBOL(prp_still_deselect);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("IPU PRP STILL IMAGE Driver");
MODULE_LICENSE("GPL");
