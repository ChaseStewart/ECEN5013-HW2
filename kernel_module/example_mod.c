/*****************************************************
 * Redistribution, modification or use of this software in source or binary forms 
 * is permitted as long as the files maintain this copyright. Users are permitted to 
 * modify this and use it to learn about the field of embedded software but don't copy 
 * my (Chase E Stewart's) work for class, I worked really hard on this. Alex Fosdick and 
 * the University of Colorado and Chase E Stewart are not liable for any misuse of this material. 
 * License copyright (C) 2017 originally from Alex Fosdick, code by Chase E Stewart.
 *****************************************************/
/**
 * @file example_mod.c
 * @brief A kernel module that prints to syslog every 500msec
 * 
 * The kernel module is called example_mod, it calls my_timer_tick() every 500msec, prints to syslog, 
 * and increments a counter which is formatted into the syslog print.
 *
 * @author Chase E Stewart
 * @date Sept 19 2017
 * @version 1.0
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>



/* the number of msec for timer */
#define TIMER_INTERVAL 500

/* the linux timer struct */ 
static struct timer_list my_timer;
static int32_t monotonic_counter;


/*
 * @brief increment the counter, print to syslog, and reset the timer
 * to go off 500msec in the future
 *
 * @param unsigned long data - an artifact of being a callback from linux/timer
 *
 * @return none
 */
static void my_timer_tick(unsigned long data)
{
	int retval = 0;
	printk("[example_mod] timer tick number: %d \n", monotonic_counter );
	monotonic_counter++;

	retval = mod_timer( &my_timer, jiffies+msecs_to_jiffies(TIMER_INTERVAL) );
	if (retval != 0)
	{
		printk("[example_mod] ERROR: Failed to continue with timer\n");
	}
}

/*
 * @brief print to syslog, initialize timer, set it to go off in 500 msec and set counter to 0
 *
 * @param none
 *
 * @return int status
 */
static int example_mod_init(void)
{
	int retval=0;
	monotonic_counter = 0;
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

/*
 * @brief print to syslog, stop and destroy timer, and set counter to 0
 *
 * @param none
 *
 * @return none
 */
static void example_mod_exit(void)
{
	int retval=0;
	monotonic_counter = 0;

	/* attempt to destroy timer and report results */
	retval = del_timer(&my_timer);
	printk("[example_mod] Retcode for destroying timer was %d !\n", retval);
	
	/* inform user of exit */
	printk("[example_mod] Exiting HW2 timer module!\n");

}

/* set the init and exit function using a macro */
module_init(example_mod_init);
module_exit(example_mod_exit);

/* Set module parameters */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECEN5013 HW2 Example Kernel Module Using Timer");
MODULE_AUTHOR("Chase E Stewart");
