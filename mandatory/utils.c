/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:03:15 by root              #+#    #+#             */
/*   Updated: 2025/02/18 19:47:04 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **env)
{
	int	i;

	i = 0;
	if (!env || !env[0])
		return (exit(127), NULL);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (exit(127), NULL);
	return (ft_split(env[i] + 5, ':'));
}

void	cleangarbage(char **paths, char **arguments)
{
	free_paths(paths);
	free_args(arguments);
	exit(127);
}

char	*get_command(char **paths, char *command, char **arguments)
{
	int		i;
	char	*path;
	char	*test;

	i = 0;
	if (command[0] == '/')
	{
		if (access(command, F_OK | X_OK) == 0)
			return (ft_strdup(command));
		else
			return (perror("Error:"), cleangarbage(paths, arguments), NULL);
	}
	while (paths[i])
	{
		test = ft_strjoin("/", command);
		path = ft_strjoin(paths[i], test);
		free(test);
		if (access(path, R_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (perror(command), cleangarbage(paths, arguments), NULL);
}

int	open_files(char *infile, char *outfile)
{
	int	fd1;
	int	fd2;

	fd1 = open(infile, O_RDONLY);
	fd2 = open(outfile, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd1 == -1 || fd2 == -1)
	{
		perror("No such file or directory");
		exit(0);
	}
	return (fd1);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
