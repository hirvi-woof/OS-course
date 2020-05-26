#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#define SIZE 256

int main()
{
	int dscrpt[2];
	int pid, err, i;
	char buffer_send[SIZE] = "ThiS is sOmE tExt\n";
	char buffer_recv[SIZE];

	err = pipe(dscrpt);
	if(err == -1)
	{
		perror("can't call pipe");
		exit(-1);
	}
	pid = fork();
	if(pid == -1)
	{
		perror("can't create new child process");
		exit(-1);
	}
	if(pid == 0)
	{
		err = write(dscrpt[1], buffer_send, SIZE);
		if (err == -1)
		{
			while (err == -1 && errno == EINTR)
                        {
             			err = write(dscrpt[1], buffer_send, SIZE);
                        }
			if (errno != EINTR)
                        {
				perror("can't write to the terminal");
				exit(-1);
			}
		}
		if(close(dscrpt[1]) == -1)
		{
			perror("can't close file");
			exit(-1);
		}
		exit(0);
	}

	err = read(dscrpt[0], buffer_recv, SIZE);
    	if (err == -1)
       	{
               	while (err == -1 && errno == EINTR)
       	 	{
                	err = read(dscrpt[0], buffer_recv, SIZE);
        	}
       	        if (errno != EINTR)
       	        {
    	               	perror("can't read from buffer");
               	       	return -1;
                }
	}
	
	for(int i = 0; i < strlen(buffer_recv); i++)
	{
		buffer_recv[i] = toupper(buffer_recv[i]);
	}

	printf("%s\n", buffer_recv);
	if(close(dscrpt[0]) == -1)
	{
		perror("can't close file");
		exit(-1);
	}
	return 0;
}

