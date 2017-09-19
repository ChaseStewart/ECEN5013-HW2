#include <linux/kernel.h>
#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>

#define SYSCALL_NAME "sys_sort"
#define SYSCALL_NO 333

/* Much thanks to xorl.wordpress.com article and Bhallaji V for the link on slack */
/* Source Article Title: Linux Kernel User to Kernel Space Range Checks */
#define access_ok(type,addr,size) (likely(__range_not_ok(addr, size) == 0))



SYSCALL_DEFINE3(sys_sort, int32_t *buffer, uint32_t buffer_size, int32_t *sort_buffer)
{
	uint32_t first_idx  = 0; /* outer iterator */
	uint32_t second_idx = 0; /* inner iterator */
	int32_t  temp       = 0; /* swap variable  */
	uint32_t *kbuffer = NULL;

	/* check all inputs for validity */
	printk(KERN_NOTICE "[%s] INVOKING SYSCALL SYS_SORT\n" SYSCALL_NAME);



	if (!access_ok(int32_t,buffer,buffer_size))
	{
		return EACCES; /* error out if out of userspace */
	}
	else if (!buffer)
	{
		printk(KERN_ERROR "[%s] INVALID INPUT BUFFER \n" SYSCALL_NAME);
		return EINVAL; /* invalid input buffer */ 
	}
	else if (buffer_size <= 0)
	{
		printk(KERN_ERROR "[%s] INVALID INPUT BUFFER_SIZE \n" SYSCALL_NAME);	
		return EOVERFLOW; /* input is too low, overflow is the closest errno */
	}

	/* Create a buffer in the kernel using kmalloc */
	kbuffer = (uint32_t *) kmalloc (sizeof(uint32_t) * buffer_size, GFP_KERNEL);
	if (!kbuffer)
	{
		printk(KERN_ERROR "[%s] UNABLE TO MALLOC K_BUFFER \n" SYSCALL_NAME);	
		return ENOMEM; /* no memory to complete a malloc */
	}
	if (!capable(CAP_SYS_ROOT))
	{
		printk(KERN_ERROR "[%s] CAP_SYS_ROOT RETURNED ERROR \n" SYSCALL_NAME);	
		return EPERM; /* insufficient permissions */
	}

	/* copy user's provided pointer into kernel space*/
	copy_from_user(kbuffer, buffer, buffer_size);
	
	/* now sort the buffer with bubble sort*/
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

	/* return result of sort to user-space and exit*/
	copy_to_user(sort_buffer, kbuffer, buffer_size)
	printk(KERN_NOTICE "[%s] LEAVING SYSCALL SYS_SORT\n", SYSCALL_NAME);
	return 0;
}

