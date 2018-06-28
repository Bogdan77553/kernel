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

/* 
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits, 
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number 
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");



static struct my_st{
	int my_temp;
	int my_temp1;
	int number_irq;
}my_st_1;
#define len 100
static char ker_buf[len];
struct dentry *fileret,*dirret;

spinlock_t my_lock;

/* read file operation */
static ssize_t myReader(struct file *fp, char __user *user_buffer,
                                size_t count, loff_t *position)
{
     return simple_read_from_buffer(user_buffer, count, position, ker_buf, len);
}
 
/* write file operation */
static ssize_t myWriter(struct file *fp, const char __user *user_buffer,
                                size_t count, loff_t *position)
{
        if(count > len )
                return -EINVAL;
  
        return simple_write_to_buffer(ker_buf, len, position, user_buffer, count);

        //if(user_buffer+"sdf")
}

static const struct file_operations fops_debug = {
        .read = myReader,
        .write = myWriter,
};



static irqreturn_t my_tHello(int irq, void *dev_id)
{
	
	spin_lock(&my_lock);
	sprintf(ker_buf,"%d",my_st_1.number_irq);
	spin_unlock(&my_lock);	
	my_st_1.my_temp1 = current->pid;
	//printk("T_hello%d  ***   %d\n",my_st_1.my_temp, my_st_1.my_temp1);

	

	return IRQ_HANDLED;
}

struct tasklet_struct Tmy;


static irqreturn_t my_pl011_int(int irq, void *dev_id)
{	
	struct my_st *a;
	spin_lock(&my_lock);
	//tasklet_schedule(&Tmy);
	
	a = dev_id; 
	//((struct my_st*)dev_id)->my_temp = current->pid;
	a->number_irq += 1;
	a->my_temp = current->pid;
	spin_unlock(&my_lock);
	return IRQ_WAKE_THREAD;
}


static int __init lkm_example_init(void) {
	int ret;

	
	dirret = debugfs_create_dir("myDebug", NULL);
	fileret = debugfs_create_file("uart-pl011-count", S_IRUGO | S_IWUSR, dirret, NULL, &fops_debug);

	//tasklet_init(&Tmy,&my_tHello,0);
	
	//ret = request_irq(53, &my_pl011_int, IRQF_SHARED, "my-uart-pl011", &my_st_1);
	ret = request_threaded_irq (53, &my_pl011_int, my_tHello, IRQF_SHARED, "my-uart-pl011", &my_st_1);
		



 	printk(KERN_INFO "Hello, World!\n");
 return ret;
}


static void __exit lkm_example_exit(void) {
	//tasklet_kill(&Tmy);
	free_irq(53,&my_st_1);
 	printk(KERN_INFO "Goodbye, World!\n");
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);