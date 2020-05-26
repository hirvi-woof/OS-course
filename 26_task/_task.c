#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100

int main()
{
	char buff[SIZE];
	int size;
	do
	{
		size = read(fileno(stdin), buff, SIZE);
		for(int i = 0; i < size; i++)
		{
			buff[i] = toupper(buff[i]);
		}
		write(fileno(stdout), buff, size);
	} while (size > 0);
	return 0;
}

