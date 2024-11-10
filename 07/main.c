// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>

#define LOGIN "ybensell"

static struct dentry *dir_42;
static char *foo_data;
static size_t content_size;
static DEFINE_MUTEX(foo_mutex);

/* read and write func for id file */
static ssize_t id_write(struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
		char kbuf[69];
		ssize_t ret;

		ret = simple_write_to_buffer(kbuf, 69, off, buf, count);
		if (ret > 0) {
			if (strcmp(kbuf, LOGIN) == 0)
				return ret;
		}
		return -EINVAL;
}

static ssize_t id_read(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
		return simple_read_from_buffer(buf, count, off, LOGIN, strlen(LOGIN));
}

/* read func for jiffies file */
static ssize_t jiffies_read(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
		int len;
		unsigned char jiffies_str[69];

		len = snprintf(jiffies_str, 69, "%lu\n", jiffies);

		return simple_read_from_buffer(buf, count, off, jiffies_str, strlen(jiffies_str));
}

/* read and write func for foo file */
static ssize_t foo_read(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
		ssize_t ret;

		mutex_lock(&foo_mutex);
		ret = simple_read_from_buffer(buf, count, off, foo_data, content_size);

		mutex_unlock(&foo_mutex);
		return ret;
}

static ssize_t foo_write(struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
		ssize_t ret;

		mutex_lock(&foo_mutex);
		ret = simple_write_to_buffer(foo_data, PAGE_SIZE, off, buf, count);
		if (ret > 0)
			content_size = max_t(size_t, content_size, *off);
		mutex_unlock(&foo_mutex);
		return ret;
}

/* File operations structure for the files */
static const struct file_operations fops_id = {
		.owner = THIS_MODULE,
		.read = id_read,
		.write = id_write,
};

static const struct file_operations fops_jiffies = {
		.owner = THIS_MODULE,
		.read = jiffies_read,
};

static const struct file_operations fops_foo = {
		.owner = THIS_MODULE,
		.read = foo_read,
		.write = foo_write,
};

/* Main module */
int init_module(void)
{
		struct dentry *id_file, *jiffies, *foo_file;

		dir_42 = debugfs_create_dir("fortytwo", NULL);

		if (IS_ERR(dir_42)) {
			pr_err("Something wrong\n");
			return 1;
		}
		pr_info("fortytwo folder created successfully on /sys/kernel/debug\n");

		id_file = debugfs_create_file("id", 0666, dir_42, NULL, &fops_id);
		if (IS_ERR(id_file)) {
			pr_err("Error while creating file id\n");
			return 1;
		}
		pr_info("id device created successfully on /sys/kernel/debug/fortytwo\n");

		jiffies = debugfs_create_file("jiffies", 0444, dir_42, NULL, &fops_jiffies);
		if (IS_ERR(jiffies)) {
			pr_err("Error while creating file jiffies");
			return 1;
		}
		pr_info("jiffies device created successfully on /sys/kernel/debug/fortytwo\n");

		foo_data = kzalloc(PAGE_SIZE, GFP_KERNEL);
		if (!foo_data) {
			debugfs_remove_recursive(dir_42);
			pr_err("Can not allocate for foo_data");
			return -ENOMEM;
		}

		foo_file = debugfs_create_file("foo", 0644, dir_42, NULL, &fops_foo);
		if (IS_ERR(foo_file)) {
			pr_err("Error whiel creating foo file");
			return -ENOMEM;
		}
		pr_info("foo device created successfully on /sys/kernel/debug/fortytwo\n");

		return 0;
}

void cleanup_module(void)
{
	debugfs_remove_recursive(dir_42);
	kfree(foo_data);
	pr_info("debugfs module cleaning finished\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pengolian");
MODULE_DESCRIPTION("Module for testing debugfs");

