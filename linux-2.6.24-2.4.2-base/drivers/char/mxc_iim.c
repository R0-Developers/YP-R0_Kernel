/*
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/clk.h>
#include <linux/kdev_t.h>

#define MXC_IIM_MAJOR	251
#define MXC_IIM_MINOR	0
#define MODULE_NAME	"mxc_iim"

static dev_t mxc_iim_dev_num;
static struct cdev mxc_iim_dev;
static struct class *mxc_iim_class;
static unsigned long iim_reg_base, iim_reg_end, iim_reg_size;
static struct clk *iim_clk;
static struct device *iim_dev;

/*!
 * MXC IIM interface - memory map function
 *
 * @param file	     struct file *
 * @param vma	     structure vm_area_struct *
 *
 * @return	     Return 0 on success or negative error code on error
 */
static int mxc_iim_mmap(struct file *file, struct vm_area_struct *vma)
{
	size_t size = vma->vm_end - vma->vm_start;

	/* Check the physical addresses which can be mapped */
	if ((vma->vm_pgoff != 0) || (size > iim_reg_size))
		return -EINVAL;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    iim_reg_base >> PAGE_SHIFT,
			    size,
			    vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

/*!
 * MXC IIM interface - open function
 *
 * @param inode	     struct inode *
 * @param filp	     struct file *
 *
 * @return	     Return 0 on success or negative error code on error
 */
static int mxc_iim_open(struct inode *inode, struct file *filp)
{
	iim_clk = clk_get(NULL, "iim_clk");

	if (IS_ERR(iim_clk)) {
		dev_err(iim_dev, "No IIM clock defined\n");
		return -ENODEV;
	}
	clk_enable(iim_clk);

	return 0;
}

/*!
 * MXC IIM interface - release function
 *
 * @param inode	     struct inode *
 * @param filp	     struct file *
 *
 * @return	     Return 0 on success or negative error code on error
 */
static int mxc_iim_release(struct inode *inode, struct file *filp)
{
	clk_disable(iim_clk);
	clk_put(iim_clk);
	return 0;
}

static const struct file_operations mxc_iim_fops = {
	.mmap = mxc_iim_mmap,
	.open = mxc_iim_open,
	.release = mxc_iim_release,
};

/*!
 * This function is called by the driver framework to get iim base/end address
 * and register iim misc device.
 *
 * @param	dev	The device structure for IIM passed in by the driver
 *			framework.
 *
 * @return      Returns 0 on success or negative error code on error
 */
static int mxc_iim_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct device *tmp_dev;
	int ret = 0;

	iim_dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (IS_ERR(res)) {
		dev_err(iim_dev, "Unable to get IIM resource\n");
		return -ENODEV;
	}

	iim_dev = &pdev->dev;

	iim_reg_base = res->start;
	iim_reg_end = res->end;
	iim_reg_size = iim_reg_end - iim_reg_base + 1;

	mxc_iim_dev_num = MKDEV(MXC_IIM_MAJOR, MXC_IIM_MINOR);
	ret = register_chrdev_region(mxc_iim_dev_num, 1, MODULE_NAME);
	if (ret) {
		dev_err(iim_dev, "Unable to register mxc iim chrdev region\n");
		return ret;
	}

	cdev_init(&mxc_iim_dev, &mxc_iim_fops);
	mxc_iim_dev.owner = THIS_MODULE;
	mxc_iim_dev.ops = &mxc_iim_fops;

	ret = cdev_add(&mxc_iim_dev, mxc_iim_dev_num, 1);
	if (ret) {
		kobject_put(&mxc_iim_dev.kobj);
		goto error;
	}

	mxc_iim_class = class_create(THIS_MODULE, MODULE_NAME);
	if (IS_ERR(mxc_iim_class)) {
		dev_err(iim_dev, "Unable to create mxc iim class\n");
		cdev_del(&mxc_iim_dev);
		ret = PTR_ERR(mxc_iim_class);
		goto error;
	}

	tmp_dev = device_create(mxc_iim_class, NULL, mxc_iim_dev_num,
				MODULE_NAME);
	if (IS_ERR(tmp_dev)) {
		dev_err(iim_dev, "Unable to create mxc iim device\n");
		cdev_del(&mxc_iim_dev);
		class_destroy(mxc_iim_class);
		ret = PTR_ERR(tmp_dev);
		goto error;
	}

	return ret;
error:
	unregister_chrdev_region(mxc_iim_dev_num, 1);
	return ret;
}

static int mxc_iim_remove(struct platform_device *pdev)
{
	device_destroy(mxc_iim_class, mxc_iim_dev_num);
	class_destroy(mxc_iim_class);

	unregister_chrdev_region(mxc_iim_dev_num, 1);
	cdev_del(&mxc_iim_dev);
	return 0;
}

static struct platform_driver mxc_iim_driver = {
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "mxc_iim",
		   },
	.probe = mxc_iim_probe,
	.remove = mxc_iim_remove,
};

static int __init mxc_iim_dev_init(void)
{
	return platform_driver_register(&mxc_iim_driver);
}

static void __exit mxc_iim_dev_cleanup(void)
{
	platform_driver_unregister(&mxc_iim_driver);
}

module_init(mxc_iim_dev_init);
module_exit(mxc_iim_dev_cleanup);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("MXC IIM driver");
MODULE_LICENSE("GPL");
