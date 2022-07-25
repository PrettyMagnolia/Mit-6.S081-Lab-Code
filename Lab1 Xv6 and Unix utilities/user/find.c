#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

// find 函数
void
find(char *dir, char *file)
{   
    // 声明 文件名缓冲区 和 指针
    char buf[512], *p;
    // 声明文件描述符 fd
    int fd;
    // 声明与文件相关的结构体
    struct dirent de;
    struct stat st;

    // open() 函数打开路径，返回一个文件描述符，如果错误返回 -1
    if ((fd = open(dir, 0)) < 0)
    {
        // 报错，提示无法打开此路径
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    // int fstat(int fd, struct stat *);
    // 系统调用 fstat 与 stat 类似，但它以文件描述符作为参数
    // int stat(char *, struct stat *);
    // stat 系统调用，可以获得一个已存在文件的模式，并将此模式赋值给它的副本
    // stat 以文件名作为参数，返回文件的 i 结点中的所有信息
    // 如果出错，则返回 -1
    if (fstat(fd, &st) < 0)
    {
        // 出错则报错
        fprintf(2, "find: cannot stat %s\n", dir);
        // 关闭文件描述符 fd
        close(fd);
        return;
    }

    // 如果不是目录类型
    if (st.type != T_DIR)
    {
        // 报类型不是目录错误
        fprintf(2, "find: %s is not a directory\n", dir);
        // 关闭文件描述符 fd
        close(fd);
        return;
    }

    // 如果路径过长放不入缓冲区，则报错提示
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        // 关闭文件描述符 fd
        close(fd);
        return;
    }
    // 将 dir 指向的字符串即绝对路径复制到 buf
    strcpy(buf, dir);
    // buf 是一个绝对路径，p 是一个文件名，通过加 "/" 前缀拼接在 buf 的后面
    p = buf + strlen(buf);
    *p++ = '/';
    // 读取 fd ，如果 read 返回字节数与 de 长度相等则循环
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
            continue;
        // strcmp(s, t);
        // 根据 s 指向的字符串小于（s<t）、等于（s==t）或大于（s>t） t 指向的字符串的不同情况
        // 分别返回负整数、0或正整数
        // 不要递归 "." 和 "..."
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        // memmove，把 de.name 信息复制 p，其中 de.name 代表文件名
        memmove(p, de.name, DIRSIZ);
        // 设置文件名结束符
        p[DIRSIZ] = 0;
        // int stat(char *, struct stat *);
        // stat 系统调用，可以获得一个已存在文件的模式，并将此模式赋值给它的副本
        // stat 以文件名作为参数，返回文件的 i 结点中的所有信息
        // 如果出错，则返回 -1
        if(stat(buf, &st) < 0)
        {
            // 出错则报错
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        // 如果是目录类型，递归查找
        if (st.type == T_DIR)
        {
            find(buf, file);
        }
        // 如果是文件类型 并且 名称与要查找的文件名相同
        else if (st.type == T_FILE && !strcmp(de.name, file))
        {
            // 打印缓冲区存放的路径
            printf("%s\n", buf);
        }
    }
}

int
main(int argc, char *argv[])
{
    // 如果参数个数不为 3 则报错
    if (argc != 3)
    {
        // 输出提示
        fprintf(2, "usage: find [dirName] [fileName]\n");
        // 异常退出
        exit(1);
    }
    // 调用 find 函数查找指定目录下的文件
    find(argv[1], argv[2]);
    // 正常退出
    exit(0);
}
