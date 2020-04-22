#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE_BUFF 100000
#define SIZE_LINE 100

int read_file(char *buffer, int dscrpt);

int fill_shift_table(char *buffer, int *shifts, int *lengths, int size);

int run_request_mode(char *buffer, int *shifts, int *lengths, int dscrpt, int size, int line_cnt);

int main(int argc, char **argv)
{
	char buffer[SIZE_BUFF];
	int shifts[SIZE_LINE], lengths[SIZE_LINE];
	int line_cnt, size, file_dscrpt;
	int err;
	if (argc < 2)
	{
		printf("error - not enough arguments; usage: %s file_name\n", argv[0]);
		exit(-1);
	}

	file_dscrpt = open(argv[1], O_RDONLY);
	if (file_dscrpt == -1)
	{
		perror("can't open file");
		exit(-1);
	}
	size = read_file(buffer, file_dscrpt);
	if(size == -1)
	{
		err = close(file_dscrpt);
                if (err == -1)
			perror("can't close file");
		exit(-1);
	}
	
	line_cnt = fill_shift_table(buffer, shifts, lengths, size);

	if(run_request_mode(buffer, shifts, lengths, file_dscrpt, size, line_cnt) == -1)
	{
		err = close(file_dscrpt);
		if (err == -1)
		{
			perror("can'r close file");
			exit(-1);
		}
		exit(-1);
	}


	err = close(file_dscrpt);
                if (err == -1)
                {
                        perror("can'r close file");
                        exit(-1);
                }

	return 0;
}

int read_file(char *buffer, int dscrpt)
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
	err = read(dscrpt, buffer, size);
    	if (err == -1)
        {
                while (err == -1 && errno == EINTR)
               	{
                	 err = read(dscrpt, buffer, size);
        	}
       	        if (errno != EINTR)
       	        {
    	                perror("can't write to the terminal");
               	        return -1;
                }   
	}
	
	if (lseek(dscrpt, 0, SEEK_SET) == -1L)
        {
	        perror("can't get set pointer to the beginning of file(lseek)");
                return -1;
        }
	return size;
}

int fill_shift_table(char *buffer, int *shifts, int *lengths, int size)
{
	for (int k = 0; k < size; k++)
        {
                printf("%c", buffer[k]);
        }
	printf("\n");
	int i = 0, line_cnt = 0, shift_cnt = 0, length = 0;
	shifts[line_cnt] = shift_cnt;
	while (i < size)
	{
		while (buffer[i] != '\n' && i++ < size)
			length++;
		length++;
		lengths[line_cnt++] = length;
		shift_cnt += length;
		shifts[line_cnt] = shift_cnt;
		length = 0;
		i++;		
	}
	return line_cnt;
}

int run_request_mode(char *buffer, int *shifts, int *lengths, int dscrpt, int size, int line_cnt)
{
	int flag = 0, line_num;
	printf("exit: print -1.\nenter number of the line you want to be printed:\n");
	while(!flag)
	{
		int err;
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
		if (lseek(dscrpt, shifts[line_num], SEEK_SET) == -1L)
        	{
                	perror("can't set pointer to the beginning of file(lseek)");
                	return -1;
         	}
        	if(err = read(dscrpt, buffer, lengths[line_num]) == -1 && errno != EINTR)
		{	
			while (err == -1 && errno == EINTR)
			{
				if (lseek(dscrpt, shifts[line_num], SEEK_SET) == -1L)
                		{
                        		perror("can't set pointer to the beginning of file(lseek)");
                	        	return -1;
		                }
                        	err = read(dscrpt, buffer, lengths[line_num]);
			}
			if (errno != EINTR)
			{
				perror("can't read line from file");
            			return -1;
			}
        	}
        	err = write(1, buffer, lengths[line_num]);
		if (err == -1)
		{
			while (err == -1 && errno == EINTR)
                        {
             			 err = write(1, buffer, lengths[line_num]);
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
