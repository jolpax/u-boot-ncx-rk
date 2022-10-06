#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define LOGFILE "ncxapp.log"

void print_time (FILE *fd)
{
	time_t time_r;
  	struct tm * timeinfo;
  	char a[500];

  	time ( &time_r );
  	timeinfo = localtime ( &time_r );
  	printf("[%d/%d/%d]", timeinfo->tm_mday, timeinfo->tm_mon, (1900+timeinfo->tm_year)-2000);
  	printf("[%d:%d:%2d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	if (fd != NULL && fd > 0) {
  		sprintf(a,"[%d/%d/%d][%d:%d:%2d][pid:%d]",timeinfo->tm_mday, timeinfo->tm_mon, ((1900+timeinfo->tm_year)-2000),
							timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, getpid());
		fprintf(fd,"%s", a);
	}

}

#define ncx_log( fmt , ... )    \
        do{ \
        	printf("\033[0;34m");\
            FILE* f = fopen( LOGFILE , "ab+" ) ; \
            if( !f )    \
                break ; \
	    print_time(f);\
            printf("[pid :%d]" ,getpid());\
            if (1)\
			{\
				fprintf(f, "[%s:%d]-> ",__FILE__,__LINE__);    \
				printf("[%s:%d]-> ",__FILE__,__LINE__);    \
			}\
            fprintf(f, fmt,##__VA_ARGS__);    \
            printf(fmt,##__VA_ARGS__);    \
            fclose( f ) ;   \
            printf("\033[0m");\
        }while( 0 )

int main()
{
	pthread_t tid;
	ncx_log("main starts\n");
	ncx_log("Hello %s %d\n", "zahid", 90);
	

}

