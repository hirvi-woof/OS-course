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
		perror("can't call pclose");
		exit(-1);
	}
	return 0;
}

