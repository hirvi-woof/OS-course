#include <stdio.h>
#include <unistd.h>
int main()
{
	FILE *file;
	
	printf("1. uid: %d, euid: %d\n", getuid(), geteuid());
	file = fopen("file", "r");
	if(!file)
	{
		perror("fopen");
	}
	else
	{
		printf("File was opened successfully.\n");
		fclose(file);
	}

	int err = seteuid(getuid());
	printf("2. seteuid = %d, uid: %d, euid: %d\n", err, getuid(), geteuid());

	file = fopen("file", "r");
	if(!file)
	{
		perror("fopen");
	}
	else
	{
		printf("File was opened successfully.\n");
		fclose(file);
	}
	return 0;
}

