#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define FORKS 2

unsigned long _timestamp_sec(clockid_t clk_id){
    struct timespec tp;
    clock_gettime(clk_id, &tp);
    return tp.tv_sec;
}

void heavy_load()
{
    unsigned long i = 10L;
    while (i--);
    //{
    //    printf("%lu\n",i);
    //}
}

void heavy_load2()
{
    unsigned long i = 15L;
    while (i--);
    //{
    //    printf("%lu\n",i);
    //}
}

int main()
{
    //unsigned long process_ts_start = _timestamp_sec(CLOCK_PROCESS_CPUTIME_ID);
    //unsigned long system_ts_start = _timestamp_sec(CLOCK_REALTIME);

    printf("Starting process: pid = %d\n", getpid());
    for(int i=0; i < FORKS; i++)
    {
        int prc = fork();
        if(prc == 0)
        {
            printf("Child process: pid = %d\n", getpid());
            heavy_load2();
        }
        else
        {
            wait(); //the parent process waits for the child  
            printf("Parent process: pid = %d\n", getpid());
            heavy_load();
        }
    }

    //printf("Running process: pid = %d\n", getpid());
    //heavy_load(); 

    return 0;
}
