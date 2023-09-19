// Lab Xv6 and Unix utilities
// xargs.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSIZE 16

int
main(int argc, char *argv[])
{
    sleep(10);
    char buf[MAXSIZE];
    read(0,buf,MAXSIZE);

    char *xargv[MAXARG];
    int xargc=0;
    for(int i=1;i<argc;++i)
    {
        xargv[xargc]=argv[i];
        xargc++;
    }

    char *p=buf;
    for(int i=0;i<MAXSIZE;++i)
    {
        if(buf[i]=='\n')
        {
            int pid=fork();
            if(pid>0)
            {
                //parent
                p=&buf[i+1];
                wait(0);
            }
            else
            {
                //child
                buf[i]=0;
                xargv[xargc]=p;
                xargc++;
                xargv[xargc]=0;
                xargc++;  

                exec(xargv[0],xargv);     
                exit(0);         
            }
        }
    }
    wait(0);
    exit(0);
}
