#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

int main(int argn,char *argv[])
{
    int ptoc_fd[2],ctop_fd[2];
    //make pipe
    pipe(ptoc_fd);
    pipe(ctop_fd);
    char buffer[8];
    if(fork()==0)
    {
        //child process
        read(ptoc_fd[0],buffer,4);
        printf("%d: received %s\n",getpid(),buffer);
        write(ctop_fd[1],"pong",strlen("pong"));
    }
    else
    {
        //parent process
        write(ptoc_fd[1],"ping",strlen("ping"));
        wait(NULL);
        read(ctop_fd[0],buffer,4);
        printf("%d: received %s\n",getpid(),buffer);
    }
    exit(0);
}