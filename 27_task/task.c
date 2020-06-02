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
        strcat(cmd, "cat ");
        strcat(cmd, argv[1]);
        strcat(cmd, " | egrep \"^$\" | wc -l");

        channel = popen(cmd, "r");

        if(channel == 0)
        {
                perror("can't call popen");
                exit(-1);
        }

        fgets(res, SIZE, channel);
        result = atoi(res);
        printf("result: %d\n", result);

        err = pclose(channel);
        if(err == -1)
        {
                perror("can't call pclose");
                exit(-1);
        }
        return 0;
}
