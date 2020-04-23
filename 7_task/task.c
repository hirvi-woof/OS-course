#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/mman.h>
#define SIZE_BUFF 100000
#define SIZE_LINE 100

int read_file(char **buffer, int dscrpt);

int fill_shift_table(char *buffer, char **shifts, int *lengths, int size);

int run_request_mode(char *buffer, char **shifts, int *lengths, int dscrpt, int size, int line_cnt);

int main(int argc, char **argv)
{
	char *buffer;
	int lengths[SIZE_LINE];
	int line_cnt, size, file_dscrpt;
	int err;
	if (argc < 2)
	{
		err = printf("error - not enough arguments; usage: %s file_name\n", argv[0]);
		exit(-1);
	}

	file_dscrpt = open(argv[1], O_RDONLY);
	if (file_dscrpt == -1)
	{
		perror("can't open file");
		exit(-1);
	}
	size = read_file(&buffer, file_dscrpt);
	if(size == -1)
	{
		err = close(file_dscrpt);
                if (err == -1)
			perror("can't close file");
		exit(-1);
	}
	char **shifts = (char**)malloc(size);

    	if (shifts == NULL)
    	{
        	perror("can't allocate memory");
        	err = close(file_dscrpt);
     		if (err == -1)
    			perror("can't close file");
    		exit(-1);
    	}
	for (int i = 0; i < size; i++)
		printf("%c", buffer[i]);
	line_cnt = fill_shift_table(buffer, shifts, lengths, size);

	if(run_request_mode(buffer, shifts, lengths, file_dscrpt, size, line_cnt) == -1)
	{
		err = close(file_dscrpt);
		free(shifts);
		if (err == -1)
		{
			perror("can't close file");
			exit(-1);
		}
		exit(-1);
	}

	
	err = close(file_dscrpt);
     	if (err == -1)
    	{
		free(shifts);
        	perror("can't close file");
        	exit(-1);
    	}
	
	free(shifts);
	munmap(buffer, size);
	return 0;
}

int read_file(char **buffer, int dscrpt)
{
	int err = 0, size;
	size = lseek(dscrpt, 0, SEEK_END);
	if (size == -1L)
        {
                perror("can't get size of the file(lseek)");
                return -1;
        }
	if (lseek(dscrpt, 0, SEEK_SET) == -1L)
        {
                perror("can't get set pointer to the beginning of file(lseek)");
                return -1;
        }   		
    	*buffer = mmap(0, size, PROT_READ, MAP_PRIVATE, dscrpt, 0);
    	if (*buffer == NULL)
    	{
        	perror("can't call mmap");
    	}
	if (lseek(dscrpt, 0, SEEK_SET) == -1L)
    	{
	    	perror("can't get set pointer to the beginning of file(lseek)");
        	return -1;
    	}
	return size;
}

int fill_shift_table(char *buffer, char **shifts, int *lengths, int size)
{
	int i = 0, line_cnt = 0, shift_cnt = 0, length = 0;
	shifts[line_cnt] = buffer;
	while (i < size)
	{
		while (*(buffer + i) != '\n' && i++ < size)
			length++;
		length++;
		lengths[line_cnt++] = length;
		shift_cnt += length;
		shifts[line_cnt] = buffer + shift_cnt;
		length = 0;
		i++;
	}
	return line_cnt;
}

int run_request_mode(char *buffer, char **shifts, int *lengths, int dscrpt, int size, int line_cnt)
{
	int flag = 0, line_num;
	struct pollfd fds;
 	fds.fd = STDIN_FILENO;
    	fds.events = POLLIN;
	printf("exit: print -1.\nenter number of the line you want to be printed:\n");
	while(!flag)
	{
		int err;
		err = poll(&fds, 1, 5000);
        	if (err == 0)
        	{
            		printf("request was timed out! file buffer:\n");
			err = write(1, buffer, size);
			if (err == -1)
			{
				while (err == -1 && errno == EINTR)
                        	{
             				err = write(1, buffer, size);
                        	}	
				if (errno != EINTR)
                        	{
					perror("can't write to the terminal");
					return -1;
				}		
			}
			flag = 1;
			continue;
        	}
        	if (err == -1)
        	{
           	 	perror("can't do a poll");
           		return -1;
        	}
		err = scanf("%d", &line_num);
		if (err == 0)
       		{
                	while (err == -1 && errno == EINTR)
            		{
                		err = scanf("%d", &line_num);
                	}
                	if (errno != EINTR)
                	{
                   		perror("can't read from the terminal");
                   	 	return -1;
                	}
        	}
		if (line_num == -1)
		{
			flag = 1;
			continue;
		}
		if (line_num >= line_cnt || line_num < 0)
		{
			printf("incorrect line number.\nexit: print -1\n");
			continue;
		}
		err = write(1, shifts[line_num], lengths[line_num]);
		if (err == -1)
		{
			while (err == -1 && errno == EINTR)
                        {
             			 err = write(1, shifts[line_num], lengths[line_num]);
                        }	
			if (errno != EINTR)
                        {
				perror("can't write to the terminal");
				return -1;
			}		
		}
	}
	return 0;
}

