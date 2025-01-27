/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 15:29:41 by ichakank          #+#    #+#             */
/*   Updated: 2025/01/27 17:50:55 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	char	**paths;
	int		infd;
	int		outfd;
	char	**envp;
	int		index;
	int		fdpipe[2];
	int		ac;
	int		status;
}			t_pipex;

char		**get_paths(char **env);
char		*get_command(char **paths, char *command);
int			open_files(char *infile, char *outfile);
void		free_paths(char **paths);
void		free_args(char **args);

#endif