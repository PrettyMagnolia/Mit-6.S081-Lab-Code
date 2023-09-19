#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

void mapping(int n,int pd[])
{
    close(n);
    dup(pd[n]);
    close(pd[0]);
    close(pd[1]);
}

void primes()
{
    int previous,next;
    int fd[2];
    if(read(0,&previous,sizeof(int)))
    {
        printf("prime %d\n",previous);
        pipe(fd);
        if(fork()==0)
        {
            mapping(1,fd);
            while (read(0,&next,sizeof(int)))
            {
                if(next % previous !=0)
                {
                    write(1,&next,sizeof(int));
                }
            }  
        }
        else
        {
            wait(NULL);
            mapping(0,fd);
            primes();
        }
    }
}

int main(int argc,char *argv[])
{
    int fd[2];
    pipe(fd);
    if(fork()==0)
    {
        mapping(1,fd);
        for(int i=2;i<36;i++)
        {
            write(1,&i,sizeof(int));
        }
    }
    else
    {
        wait(NULL);
        mapping(0,fd);
        primes();
    }
    exit(0);
}