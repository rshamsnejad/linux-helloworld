#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kgdb.h>

#include "macros.h"
#include "device_file.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robin SHAMSNEJAD");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");


static int __init helloworld_init(void)
{
	PRINT_CALL();
	//kgdb_breakpoint();
	return 0;
}

static void __exit helloworld_exit(void)
{
	PRINT_CALL();
}

module_init(helloworld_init);
module_exit(helloworld_exit);
