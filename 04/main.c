// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/printk.h>

int init_module(void)
{
		pr_info("USB keyboard detected and this module is loaded\n");
		return 0;
}

void cleanup_module(void)
{
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pengu");
MODULE_DESCRIPTION("Module for detecting usb plugging");
