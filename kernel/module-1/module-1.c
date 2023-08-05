// SPDX-License-Identifier: UNLICENSED

//Simple kernel module without functionality

#include <linux/init.h> //For macros __init, __exit
#include <linux/module.h>
#include <linux/kernel.h>

static int __init init_func(void)
{
	pr_info("Module init function call");
	return 0;
}

static void __exit exit_func(void)
{
	pr_info("Module exit function call");
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Name Surname");
MODULE_DESCRIPTION("A simple Linux kernel module");
MODULE_VERSION("0.0.1");
