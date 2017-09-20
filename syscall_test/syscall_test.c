#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUF_SIZE 10

int main()
{
	int idx;
	const int32_t input_buffer[BUF_SIZE] = {5,3,4,8,5,9,1,2,3,0} ; 
	const int32_t *output_buffer;

	/* create input buffer */
	//input_buffer = (int32_t *) malloc(sizeof(int32_t) * BUF_SIZE);
	//if(!input_buffer)
	//{
	//	printf("ERROR: Couldn't allocate input buff\n");
	//}
	//
	///* fill input buffer */
	//for (idx=0; idx<BUF_SIZE; idx++)
	//{
	//	*(input_buffer+idx) = rand();
	//}
	
	printf("INPUT_BUFFER START [ %d", input_buffer[0]);
	for(idx=1; idx<BUF_SIZE; idx++)
	{
		printf(", %d", input_buffer[idx]);
	}
	printf("]\n");
	


	/* create output buffer */
	output_buffer = (int32_t *) malloc(sizeof(int32_t) * BUF_SIZE);
	if(!output_buffer)
	{
		printf("ERROR: Couldn't allocate ouput buff\n");
	}
	printf("OUTPUT_BUFFER START [ %d", output_buffer[0]);
	for(idx=1; idx<BUF_SIZE; idx++)
	{
		printf(", %d", output_buffer[idx]);
	}
	printf("]\n");


	/* Run the syscall */
	syscall(333, input_buffer, BUF_SIZE, output_buffer);


	/* print the results */
	printf("[ %d", input_buffer[0]);
	for(idx=1; idx<BUF_SIZE; idx++)
	{
		printf(", %d", input_buffer[idx]);
	}
	printf("]\n");

	printf("[ %d", output_buffer[0]);
	for(idx=1; idx<BUF_SIZE; idx++)
	{
		printf(", %d", output_buffer[idx]);
	}
	printf("]\n");
}
