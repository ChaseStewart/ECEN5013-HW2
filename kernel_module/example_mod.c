#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>



/* the number of msec for timer */
#define TIMER_INTERVAL 500

/* the linux timer struct */ 
static struct timer_list my_timer;
extern static int32_t monotonic_counter = 0;


/* function to run on every timer tick */
static void my_timer_tick(unsigned long data)
{
	int retval=0;
	printk("[example_mod][%ld] timer tick \n", monotonic_counter );
	monotonic_counter++;

	retval = mod_timer( &my_timer, jiffies+msecs_to_jiffies(TIMER_INTERVAL) );
	if (retval != 0)
	{
		printk("[example_mod] ERROR: Failed to continue with timer\n");
	}
	
}


static int example_mod_init(void)
{
	int retval=0;
	printk("[example_mod] Installing %dmsec timer module...\n", TIMER_INTERVAL);

	/* attempt to setup the timer, point it to tick */
	setup_timer( &my_timer, my_timer_tick, 0);

	/* attempt to set timer to go off in TIMER_INTERVAL msec */
	retval = mod_timer( &my_timer, jiffies+msecs_to_jiffies(TIMER_INTERVAL) );	
	
	/* if mod_timer fails, print warning */
	if (retval)
	{
		printk("[example_mod] Failed to start timer\n");
		return -1;
	}
	
	/* else print success message */
	else
	{
		printk("[example_mod] Timer installation was a success\n");
	}
	return 0;
}


static void example_mod_exit(void)
{
	int retval=0;

	/* attempt to destroy timer and report results */
	retval = del_timer(&my_timer);
	if (retval != 0)
	{
		printk("[example_mod] Failed to destroy timer!\n");
	}
	else
	{
		printk("[example_mod] Timer successfully destroyed\n");
	}
	
	/* inform user of exit */
	printk("[example_mod] Example module exit\n");

}

/* set the init and exit function */
module_init(example_mod_init);
module_exit(example_mod_exit);

/* Set module parameters */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECEN5013 HW2 Example Kernel Module Using Timer");
MODULE_AUTHOR("Chase E Stewart");
