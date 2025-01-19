/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichakank <ichakank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 15:30:16 by ichakank          #+#    #+#             */
/*   Updated: 2025/01/19 18:57:13 by ichakank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "pipex.h"
#include <string.h>

char **get_paths(char **env)
{
    int i = 0;
    while (env[i] && strncmp(env[i], "PATH=", 5) != 0)
        i++;
    return ft_split(env[i] + 5, ':');
}

char *get_command(char **paths, char *command)
{
    int i = 0;
    char *path, *test;
    while (paths[i])
    {
        test = ft_strjoin("/", command);
        path = ft_strjoin(paths[i], test);
        free(test);
        if (access(path, R_OK | X_OK) == 0)
            return path;
        free(path);
        i++;
    }
    return NULL;
}

int open_files(char *infile, char *outfile)
{
    int fd1 = open(infile, O_RDONLY);
    int fd2 = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd1 == -1 || fd2 == -1)
    {
        perror("File error");
        return -1;
    }
    return fd1;
}

void free_paths(char **paths)
{
    for (int i = 0; paths[i]; i++)
        free(paths[i]);
    free(paths);
}

void free_args(char **args)
{
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

void execute_command(char *executable, char **arguments, char **envp, t_pipex *pipex)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (pipex->index == pipex->ac - 2)
            dup2(pipex->outfd, STDOUT_FILENO);
        else 
            dup2(pipex->fdpipe[1], STDOUT_FILENO);
        close(pipex->fdpipe[0]);
        close(pipex->fdpipe[1]);
        execve(executable, arguments, envp);
        exit(126);
    }
    free(executable);
    free_args(arguments);
    dup2(pipex->fdpipe[0], STDIN_FILENO);
    close(pipex->fdpipe[0]);
    close(pipex->fdpipe[1]);
}

void handle_fd(t_pipex *pipex, char **av, char **envp)
{
    pipex->infd = open(av[1], O_RDONLY);
    pipex->outfd = open(av[pipex->ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
    pipex->paths = get_paths(envp);
}

void handle_pipe(t_pipex *pipex)
{
    close(pipex->infd);
    close(pipex->outfd);
}

int main(int ac, char **av, char **envp)
{
    t_pipex pipex;
    
    if (ac != 5)
        return 1;
    pipex.index = 2;
    pipex.ac = ac;
    handle_fd(&pipex, av, envp);
    if (pipex.infd == -1 || pipex.outfd == -1) return 1;
    dup2(pipex.infd, STDIN_FILENO);
    while (pipex.index < ac - 1)
    {
        if (pipe(pipex.fdpipe) == -1)
            handle_pipe(&pipex);
        char **arguments = ft_split(av[pipex.index], ' ');
        char *executable = get_command(pipex.paths, arguments[0]);
        if (executable == NULL)
            exit(127);
        execute_command(executable, arguments, envp, &pipex);
        wait(NULL);
        pipex.index++;
    }
    free_paths(pipex.paths);
    close(pipex.infd);
    close(pipex.outfd);
    return 0;
}
