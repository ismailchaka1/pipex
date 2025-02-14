/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:03:07 by root              #+#    #+#             */
/*   Updated: 2025/02/14 21:29:56 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(char *executable, char **arguments, char **envp,
		t_pipex *pipex)
{
	pid_t	pid;

	pid = fork();
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

void	handle_fd(t_pipex *pipex, char **av, char **envp)
{
	pipex->infd = open(av[1], O_RDONLY);
	pipex->outfd = open(av[pipex->ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (pipex->infd == -1)
	{
		perror(av[1]);
		exit(0);
	}
	pipex->paths = get_paths(envp);
	if (pipex->paths == NULL)
		exit(1);
}

void	handle_pipe(t_pipex *pipex)
{
	close(pipex->infd);
	close(pipex->outfd);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	char	**arguments;
	char	*executable;

	if (ac != 5)
		return (1);
	pipex.index = 2;
	pipex.ac = ac;
	handle_fd(&pipex, av, envp);
	dup2(pipex.infd, STDIN_FILENO);
	while (pipex.index < ac - 1)
	{
		if (pipe(pipex.fdpipe) == -1)
			handle_pipe(&pipex);
		arguments = ft_split(av[pipex.index], ' ');
		executable = get_command(pipex.paths, arguments[0]);
		if (executable == NULL)
			exit(127);
		execute_command(executable, arguments, envp, &pipex);
		waitpid(-1, &pipex.status, 0);
		pipex.index++;
	}
	free_paths(pipex.paths);
	handle_pipe(&pipex);
	return (pipex.status);
}
