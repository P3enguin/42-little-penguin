// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
		.owner = THIS_MODULE,
		.read = &myfd_read,
		.write = &myfd_write
};

static struct miscdevice myfd_device = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "reverse",
		.fops = &myfd_fops,
};

static char str[PAGE_SIZE];

static int __init myfd_init(void)
{
		int retval;

		retval = misc_register(&myfd_device);
		if (retval) {
			pr_err("Error registering char device!\n");
			return retval;
		}
		pr_info("misc device registered successfully");
		return 0;
}

static void __exit myfd_cleanup(void)
{
		misc_deregister(&myfd_device);
		pr_info("unregisted successfully\n");
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
		int t, i;
		char *tmp;

		tmp = kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (!tmp)
			return -1;
		for (t = strlen(str) - 1, i = 0; t >= 0; t--, i++)
			tmp[i] = str[t];
		tmp[i] = 0x0;
		return simple_read_from_buffer(user, size, offs, tmp, strlen(tmp));
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
		ssize_t res;

		res = simple_write_to_buffer(str, PAGE_SIZE, offs, user, size);
		// 0x0 = ’\0’
		str[size] = '\0';
		return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
