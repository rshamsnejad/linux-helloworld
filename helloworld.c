#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kgdb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robin SHAMSNEJAD");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int __init helloworld_init(void)
{
    printk(KERN_INFO "Salut les gonzes !\n");
    kgdb_breakpoint();
    return 0;
}

static void __exit helloworld_exit(void)
{
    printk(KERN_INFO "Tchao les gonzes !\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
