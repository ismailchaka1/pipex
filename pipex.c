/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaka <chaka@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:35:13 by chaka             #+#    #+#             */
/*   Updated: 2024/12/15 01:34:29 by chaka            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char **argv, char **envp)
{
    int fd[2];
    int fd1;
    int fd2;
    if (argc <= 4)
    {
        printf("More Arguments");
        return 0;
    }
    if (access(argv[1], R_OK) == -1)
    {
        printf("IN FILE IS INVALID");
        return 0;
    }
    if (access(argv[argc -1], R_OK) == -1)
    {
        printf("OUT FILE IS INVALID");
        return 0;
    }
    fd1 = open(argv[1], O_RDWR);
    if (fd1 == -1)
    {
        printf("Error opening in file");
        return 0;
    }
    fd2 = open(argv[argc - 1], O_RDWR);
    if (fd2 == -1)
    {
        printf("Error opening out file");
        return 0;
    }
    
    printf("%s \n", argv[1]);
    printf("%s \n", argv[argc -1]);
    return 0;
    
}