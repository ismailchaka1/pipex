
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int ac, char **av, char **envp)
{
    int fd[2];
    int fd1 = open("in", O_RDONLY);
    int fd2 = open("out", O_RDWR | O_TRUNC | O_CREAT);
    if (fd1 == -1 || fd2 == -1)
    {
        return 1;
    }
    if (ac < 5)
    {
        return 1;
    }
    int i = 2;
    if (pipe(fd) == -1) {
        close(fd1);
        close(fd2);
        return 1;
    }
    while (i < ac - 1)
    {
        // int pid = fork();
        // if (pid == -1) {
        //     close(fd1);
        //     close(fd2);
        //     return 1;
        // }
        // if (pid == 0)
        // {       
        if (i == 2)
        {
            dup2(fd1, STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            printf("first command %s\n", av[i]);
        }
        else if (i == ac - 2)
        {
            dup2(fd[0], STDIN_FILENO);
            dup2(fd2, STDOUT_FILENO);
            printf("last command %s\n", av[i]);
        }
        else
        {
            dup2(fd[0], STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            printf("middle command %s\n", av[i]);
        }
        // }
        i++;
    }

}
