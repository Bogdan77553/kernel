#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

//MODULE_LICENSE("")
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W. Oliver II");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

struct my_st{
	int my_temp;
}my_st_1;


static void my_tHello(unsigned long parem)
{
	printk("T_hello\n");
}

struct tasklet_struct Tmy;


static irqreturn_t my_pl011_int(int irq, void *dev_id)
{	
	tasklet_schedule(&Tmy);	
	return IRQ_HANDLED;
}

static int __init lkm_example_init(void) {
	int ret;
	tasklet_init(&Tmy,&my_tHello,0);
	ret = request_irq(53, &my_pl011_int, IRQF_SHARED, "my-uart-pl011", &my_st_1);
 	printk(KERN_INFO "Hello, World!\n");
 return ret;
}


static void __exit lkm_example_exit(void) {
	tasklet_kill(&Tmy);
	free_irq(53,0);
 	printk(KERN_INFO "Goodbye, World!\n");
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);