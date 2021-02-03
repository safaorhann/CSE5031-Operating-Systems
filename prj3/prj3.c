#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include<sys/wait.h>

#define RD 0
#define WR 1


int main(int argc, char const *argv[])
{
    int pid1, pid2, pfd1[2], pfd2[2], n, status;
    char buf[40];

    if (pipe(pfd1) < 0) 
    {
        printf("Pipe1 creation error\n");
        return 1;
    }
    if (pipe(pfd2) < 0)
    {
        printf("Pipe2 creation error\n");
        return 2;
    }

    if((pid1 = fork()) < 0){
        printf("Fork error \n");
        return 2;
    }

    if(pid1 == 0) //child1 process
    {
        printf("Child1 mypid1:%d  ppid1:%d\n", getpid(), getppid());
        close(pfd1[RD]);
        close(pfd2[RD]);
        close(pfd2[WR]);

        while (fgets(buf, 40, stdin) != 0)
        {
            int i, flag = 1;
            for(i = 0; i < strlen(buf); i++)
            {
                if(buf[i] == '\n'){
                    if(flag){
                        write(pfd1[WR], buf, strlen(buf));
                    }
                }
                else if(buf[i] < '0' || buf[i] > '9'){
                    flag = 0;
                }
            }
        }
        close(pfd1[WR]);
        exit(0);
    }

    else
    {
        wait(&status);
        if ((pid2 = fork()) < 0)
        {
            printf("Fork error");
        }

        if(pid2 == 0) //child2 process
        {
            printf("Child2 mypid1:%d  ppid1:%d\n", getpid(), getppid());
            close(pfd2[RD]);
            close(pfd1[WR]);
            while ((n = read(pfd1[RD], buf, 40)) > 0)
            {
                //int num = atoi(buf);
                //if(num < 1000){
                write(pfd2[WR], buf, n);
                //}        
            }
            close(pfd1[RD]);
            close(pfd2[WR]);
        }
        close(pfd1[RD]);
        close(pfd2[WR]);
        close(pfd1[WR]);
        int count = 0;
        while ((n = read(pfd2[RD], buf, 40)) > 0)
        {
            int i;
            for (i = 0; i < n; i++)
            {
                if (buf[i] == '\n')
                    count++;
            }
            printf("%d Lines contain numeric.\n", count);
        }
        close(pfd2[RD]);
        exit(0);
    }

    return 0;
}