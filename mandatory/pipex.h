/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichakank <ichakank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 15:29:41 by ichakank          #+#    #+#             */
/*   Updated: 2024/12/25 16:48:15 by ichakank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../libft/libft.h"

typedef struct s_pipex
{
    char **paths;
    int infd;
    int outfd;
    char **envp;
    int index;
    int fdpipe[2];
    int ac;
} t_pipex;

