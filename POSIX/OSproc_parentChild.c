#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> //not available in windows, shown as an error

#define nChilds 2

int globalVar = 0;

int main()
{
    for(int i = 0; i < nChilds; i++)
    {
        int ret = fork(); //what returns from the fork function (0 || child's pid)
        if(ret==0)
        {
            //child process: return 0 from fork()
            printf("Child process: pid = %d\n", getpid());
            printf("Child(%d) says: Done!\n", getpid());
            exit(0);
        }
        else
        {
            //parent process: return child's pid from fork()
            printf("Parent process: pid = %d\n", getpid());
            int child = wait(NULL); //waits for the child process to end
            printf("Child %d is done!\n", child);
            printf("Return to caller process (parent)\n");
        }
    }


    return 0;
}