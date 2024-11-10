// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

int do_work(int *nbr, int retval)
{
		int x;
		int y = *nbr;
		int z;

		for (x = 0; x < nbr; ++x)
			usleep_range(10);

		if (y < 10)
			pr_info("We slept a long time!");
		z = x * y;
		return z;
}

bool my_init(void)
{
		int x = 10;

		x = do_work(&x, x);
		return true;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pengu");
MODULE_DESCRIPTION("A simple Hello World kernel module");
