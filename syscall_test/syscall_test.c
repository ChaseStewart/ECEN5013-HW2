#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUF_SIZE 10

int main()
{
	int32_t input_buffer[BUF_SIZE] = {1,2354,2132,45,12,7,3,9,3,10000};
	int32_t *output_buffer;
	output_buffer = (int32_t *) malloc(sizeof(int32_t *) * BUF_SIZE);

	syscall(333, input_buffer, BUF_SIZE, output_buffer);
	

}
