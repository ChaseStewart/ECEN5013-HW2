#include <linux/kernel.h>


asmlinkage long sys_sort( int32_t *buffer, uint32_t buffer_size, int32_t *sort_buffer)
{

	printk(KERN_NOTICE "INVOKING SYSCALL SYS_SORT\n");

	if (!buffer)
	{
		return -1;
	}
	else if (buffer_size <= 0)
	{
		return -2;
	}

	uint32_t *kbuffer = NULL;
	kbuffer = (uint32_t *) kmalloc (sizeof(uint32_t) * buffer_size, GFP_KERNEL);
	if (!kbuffer)
	{
		return -1;
	}
	if (!capable(CAP_SYS_ROOT))
	{
		return -3;
	}
	copy_from_user(kbuffer, buffer, buffer_size);
	

	printk(KERN_NOTICE "LEAVING SYSCALL SYS_SORT\n");
	


}



