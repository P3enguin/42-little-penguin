// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define LOGIN "ybensell"

static ssize_t msc_read(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
	return simple_read_from_buffer(buf, count, off, LOGIN, strlen(LOGIN));
}

static ssize_t msc_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
		ssize_t ret;
		char kbuf[69];

		ret = simple_write_to_buffer(kbuf, 69, off, buf, count);
		if (ret > 0) {
			if (strcmp(kbuf, LOGIN) == 0)
				return ret;
		}
		return -EINVAL;
}

static const struct file_operations fops = {
		.owner = THIS_MODULE,
		.read = msc_read,
		.write = msc_write,
};

static struct miscdevice etx_device = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "fortytwo",
		.fops = &fops,
};

int init_module(void)
{
		int error;

		error =  misc_register(&etx_device);
		if (error) {
			pr_err("Error registering char device!\n");
			return error;
		}
		pr_info("misc device registered successfully");
		return 0;
}

void cleanup_module(void)
{
		misc_deregister(&etx_device);
		pr_info("unregisted successfully\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pengolian");
MODULE_DESCRIPTION("Module for creating misc device");
