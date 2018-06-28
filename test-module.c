#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/interrupt.h>

#include <linux/proc_fs.h>
#include <linux/kthread.h>
 
#include <linux/semaphore.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/debugfs.h> 
#include <linux/fs.h>

//MODULE_LICENSE("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W. Oliver II");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";
static int myintArray[2] = { -1, -1 };
static int arr_argc = 0;

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");


module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");


static int __init lkm_example_init(void) {
	int sum;
 	printk(KERN_INFO "Hello, World!\n");
 	sum = myshort + myint;
 	printk(KERN_INFO "sum = %d\n",sum);
 	return 0;
}


static void __exit lkm_example_exit(void) {
	//tasklet_kill(&Tmy);
 	printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);