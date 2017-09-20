#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/capability.h>
#include <linux/slab.h>

/* Much thanks to xorl.wordpress.com article and Bhallaji V for the link on slack */
/* Source Article Title: Linux Kernel User to Kernel Space Range Checks */
#include <linux/uaccess.h>
#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>

#define SYSCALL_NO 333 /* just FYI */


SYSCALL_DEFINE3(mysort, int32_t*, buffer, size_t, buffer_size, int32_t*, sort_buffer)
{
	int idx = 0;
	int retval          = 0; /* return value   */
	uint32_t first_idx  = 0; /* outer iterator */
	uint32_t second_idx = 0; /* inner iterator */
	int32_t  temp       = 0; /* swap variable  */
	int32_t *kbuffer = NULL; /* internal kernel buffer */

	/* check all inputs for validity */
	printk(KERN_NOTICE "[sys_sort] INVOKING SYSCALL SYS_SORT\n");

	//if (!access_ok(VERIFY_WRITE,buffer,buffer_size))
	//{
	//	return EACCES; /* error out if out of userspace */
	//}
	if (!buffer)
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
	kbuffer = (int32_t *) kmalloc (sizeof(int32_t) * buffer_size, GFP_KERNEL);
	if (!kbuffer)
	{
		printk(KERN_ERR "[sys_sort] UNABLE TO MALLOC K_BUFFER \n");	
		return ENOMEM; /* no memory to complete a malloc */
	}
	else
	{
		printk(KERN_NOTICE "[sys_sort] MALLOC K_BUFFER OF SIZE %d \n", buffer_size);	
	}
        printk("[ %d", kbuffer[0]);
        for(idx=1; idx<buffer_size; idx++)
        {
                printk(", %d", kbuffer[idx]);
        }
        printk("]\n");


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
	
        printk("[ %d", kbuffer[0]);
        for(idx=1; idx<buffer_size; idx++)
        {
                printk(", %d", kbuffer[idx]);
        }
        printk("]\n");


	
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
        
	printk("[ %d", kbuffer[0]);
        for(idx=1; idx<buffer_size; idx++)
        {
                printk(", %d", kbuffer[idx]);
        }
        printk("]\n");

	/* return result of sort to user-space and exit*/
	retval = copy_to_user(sort_buffer, kbuffer, buffer_size);
	if (retval != 0)
	{
		printk(KERN_ERR "[sys_sort] UNABLE TO COPY BACK TO USER BUFFER; ERRNO %d  \n", retval);	
		return retval; /* use the retval from copy_to_user */
	}
	
	printk(KERN_NOTICE "[sys_sort] LEAVING SYSCALL SYS_SORT\n");
	return 0;
}
