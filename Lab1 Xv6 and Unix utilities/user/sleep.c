#include "kernel/types.h"
#include "user/user.h"

int main(int argn,char *argv[])
{
    if(argn!=2)
    {
        fprintf(2,"sleep require 1 argument\n");
        exit(1);
    }
    //ascii to int
    int sleep_num = atoi(argv[1]);
    sleep(sleep_num);
    exit(0);
}