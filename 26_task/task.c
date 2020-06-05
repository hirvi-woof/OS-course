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
	char buff[SIZE] = "ThiS is sOmE tExt\n";
	FILE *out;
	int err;
	out = popen("./_task", "w");
	if(out == 0)
	{
		perror("can't call popen");
		exit(-1);
	}
	fputs(buff, out);
	err = pclose(out);
        if(err == -1)
        {
                perror("can't call pclose\n");
                exit(-1);
        }
        if(WIFEXITED(err) != 0 && WEXITSTATUS(err) != 0)
        {
                printf("some problems: child process ended up with status: %d\n", WEXITSTATUS(err));
                exit(-1);
        }  
        else
        {
                perror("some problems: child process ended up incorrectly\n");
                exit(-1);
        }
	return 0;
}

