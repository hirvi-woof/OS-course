#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#define SIZE 256


int main(int argc, char* argv[])
{
	FILE *file, *channel;
        char *ptr;
	int err, size;
	char buff[SIZE];
	char empty_line[] = "\n";
	if (argc < 2) 
	{
        	err = printf("error - not enough arguments; usage: %s file_name\n", argv[0]);
        	exit(-1);
    	}
    	file = fopen(argv[1], "r");
	if(file == 0)
	{
		perror("can't call fopen");
		exit(-1);
	}
	channel = popen("wc -l", "w");
	if(channel == 0)
	{
		perror("can't call popen");
		exit(-1);
	}
	do
	{
		ptr = fgets(buff, SIZE, file);
		if(!strcmp(empty_line, buff))
		{
			fputs(empty_line, channel);
		}
	} while(ptr);

	fclose(file);
	pclose(channel);
	return 0;
}

