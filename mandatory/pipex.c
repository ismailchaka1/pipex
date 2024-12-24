#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../libft/libft.h"

char **get_paths(char **env)
{
    int i = 0;
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
            break;
        i++;
    }
    return ft_split(env[i] + 5, ':');
}

char *get_command(char **paths, char *command)
{
    int i = 0;
    char *path;
    char *test;

    while (paths[i])
    {
        test = ft_strjoin("/", command);
        path = ft_strjoin(paths[i], test);  
        free(test);

        if (access(path, R_OK | X_OK) == 0)
        {
            return path;
        }
        free(path);
        i++;
    }
    return NULL;
}

int main(int ac, char **av, char **envp)
{
    if (ac < 5)
    {
        return 1;
    }

    int fd[2];
    int fd1 = open(av[1], O_RDONLY);
    int fd2 = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0644);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("No such file or directory");
        return 1;
    }

    int i = 2;
    char **arguments;
    char *executable;
    char **paths = get_paths(envp);


    dup2(fd1, STDIN_FILENO);
    while (i < ac - 1)
    {
        if (pipe(fd) == -1)
        {
            perror("Pipe error");
            close(fd1);
            close(fd2);
            return 1;
        }

        arguments = ft_split(av[i], ' ');
        executable = get_command(paths, arguments[0]);

        if (executable == NULL)
        {
            fprintf(stderr, "Command not found: %s\n", arguments[0]);
            free(executable);
            free(arguments);
            i++;
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
          
            if (i == ac - 2)
                dup2(fd2, STDOUT_FILENO);
            else 
                dup2(fd[1], STDOUT_FILENO); 

            close(fd[0]);
            close(fd[1]);
            execve(executable, arguments, envp);
            perror("Execve failed"); 
            exit(0);
        }
        
        free(executable);
        for (int j = 0; arguments[j]; j++)
            free(arguments[j]);
        // free(paths);
        free(arguments);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        i++; 
    }
    for (int j = 0; paths[j]; j++)
        free(paths[j]);
    free(paths);
    close(fd1);
    close(fd2);
    return 0;
}
