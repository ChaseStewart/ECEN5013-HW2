/*****************************************************
 * Redistribution, modification or use of this software in source or binary forms 
 * is permitted as long as the files maintain this copyright. Users are permitted to 
 * modify this and use it to learn about the field of embedded software but don't copy 
 * my (Chase E Stewart's) work for class, I worked really hard on this. Alex Fosdick and 
 * the University of Colorado and Chase E Stewart are not liable for any misuse of this material. 
 * License copyright (C) 2017 originally from Alex Fosdick, code by Chase E Stewart.
 *****************************************************/
/**
 * @file system_call.c
 * @brief A system call that sorts an int32_t buffer
 * 
 * The system call is called sys_sort, it sorts an input array of chars up to the provided length,
 * and then returns them via another input pointer
 *
 * @author Chase E Stewart
 * @date Sept 19 2017
 * @version 1.0
 *
 */

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/capability.h>
#include <linux/slab.h>

/* Much thanks to xorl.wordpress.com article and Bhallaji V for the link on slack */
/* Source Article Title: Linux Kernel User to Kernel Space Range Checks */
#include <linux/uaccess.h>
#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>

#define SYSCALL_NO 333

/*
 * @brief take inputs, check them for correctness, sort them, then pass them back
 *
 * @param int32_t *buffer - an input buffer hopefully of size "buffer_size"
 * @param uint32_t buffer_size - length of the buffer to sort
 * @param int32_t *sort_buffer - a pointer to the now-sorted buffer
 *
 * @return none
 */
SYSCALL_DEFINE3(sys_sort, int32_t*, buffer, uint32_t, buffer_size, int32_t*, sort_buffer)
{
	int retval          = 0; /* return value   */
	uint32_t first_idx  = 0; /* outer iterator */
	uint32_t second_idx = 0; /* inner iterator */
	int32_t  temp       = 0; /* swap variable  */
	uint32_t *kbuffer = NULL;

	/* check all inputs for validity */
	printk(KERN_NOTICE "[sys_sort] INVOKING SYSCALL SYS_SORT\n");

	if (!access_ok(VERIFY_WRITE,buffer,buffer_size))
	{
		return EACCES; /* error out if out of userspace */
	}
	else if (!buffer)
	{
		printk(KERN_ERR "[sys_sort] INVALID INPUT BUFFER \n");
		return EINVAL; /* invalid input buffer */ 
	}
	else if (buffer_size <= 0)
	{
		printk(KERN_ERR "[sys_sort] INVALID INPUT BUFFER_SIZE \n");	
		return EOVERFLOW; /* input is too low, overflow is the closest errno */
	}

	/* Create a buffer in the kernel using kmalloc */
	kbuffer = (uint32_t *) kmalloc (sizeof(uint32_t) * buffer_size, GFP_KERNEL);
	if (!kbuffer)
	{
		printk(KERN_ERR "[sys_sort] UNABLE TO MALLOC K_BUFFER \n");	
		return ENOMEM; /* no memory to complete a malloc */
	}
	if (!capable(CAP_SYS_ADMIN))
	{
		printk(KERN_ERR "[sys_sort] CAP_SYS_ADMIN RETURNED ERROR - USE SUDO\n");	
		return EPERM; /* insufficient permissions */
	}

	/* copy user's provided pointer into kernel space*/
	retval = copy_from_user(kbuffer, buffer, buffer_size);
	if (retval != 0)
	{
		printk(KERN_ERR "[sys_sort] UNABLE TO COPY FROM USER BUFFER \n");	
		return retval; /* use the retval from copy_from_user */
	}
	
	
	/* now sort the buffer with bubble sort*/
	printk(KERN_NOTICE "[sys_sort] STARTING BUBBLE SORT\n");
	for (first_idx=buffer_size; first_idx>1; first_idx= first_idx-1 )
	{
		for(second_idx=0; second_idx<first_idx-1; second_idx++)
		{
			/* iterate thru, if i > i+1, swap values */
			if (kbuffer[second_idx] > kbuffer[second_idx + 1])
			{
				/* swap the vars using temp variable */
				temp = kbuffer[second_idx];
				kbuffer[second_idx] = kbuffer[second_idx+1];
				kbuffer[second_idx + 1] = temp;
			}
		}
	}
	printk(KERN_NOTICE "[sys_sort] COMPLETED BUBBLE SORT\n");

	/* return result of sort to user-space and exit*/
	retval = copy_to_user(sort_buffer, kbuffer, buffer_size);
	if (retval != 0)
	{
		printk(KERN_ERR "[sys_sort] UNABLE TO COPY BACK TO USER BUFFER \n");	
		return retval; /* use the retval from copy_to_user */
	}
	
	printk(KERN_NOTICE "[sys_sort] LEAVING SYSCALL SYS_SORT\n");
	return 0;
}
