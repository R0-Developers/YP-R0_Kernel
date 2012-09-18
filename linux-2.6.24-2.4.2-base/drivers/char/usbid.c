#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/KernelParam.h>

static int show_usbid(struct seq_file *seq, void *v)
{
	KernelParam *param = KernelParamGet();

	seq_printf(seq, "%s", param->szUsbId);
	return 0;
}

static int usbid_open(struct inode *inode, struct file *file)
{
	int res;

	res = single_open(file, show_usbid, NULL);

	return res;
}

const struct file_operations proc_usbid_operations = {
	.open    = usbid_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};
