#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 256


int main(int argc, char* argv[])
{
        FILE *channel;
        int err, result;
        char res[SIZE], cmd[SIZE];
        if (argc < 2)
        {
                err = printf("error - not enough arguments; usage: %s file_name\n", argv[0]);
                exit(-1);
        }
        strcat(cmd, "egrep \"^$\" ");
        strcat(cmd, argv[1]);
        strcat(cmd, " | wc -l");

        channel = popen(cmd, "r");
        if(channel == NULL)
        {
                perror("can't call popen\n");
                exit(-1);
        }

        fscanf(channel, "%d", &result);

        err = pclose(channel);
        if(err == -1)
        {
                perror("can't call pclose\n");
                exit(-1);
        }
        if(WIFEXITED(err) != 0)
        {
                if(WEXITSTATUS(err) != 0)
                {
                        printf("some problems: child process ended up with status: %d\n", WEXITSTATUS(err));
                        exit(-1);
                }
                printf("result: %d\n", result);
        }
        else
        {
                perror("some problems: child process ended up incorrectly\n");
                exit(-1);
        }
        return 0;
}
