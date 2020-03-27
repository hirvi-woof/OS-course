#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];
int main()
{
	time_t now;
	struct tm *_time;
	if(putenv("TZ=PST8PDT") == -1)
     	{
        	perror("Can't change timezone.\n");
        	return 1;
     	}
	printf("Timezone was changed successfully.\n");

     	if(time(&now) == -1)
     	{
        	perror("Can't get time at the moment.\n");
          	return 1;
     	}
	printf("Time was got successfully.\n");

	printf("%s", ctime(&now));
	_time = localtime(&now);
	
	printf("%d/%d/%02d %d:%02d %s\n", _time->tm_mon + 1, _time->tm_mday, _time->tm_year, _time->tm_hour, _time->tm_min, tzname[_time->tm_isdst]);
	return 0;
}
