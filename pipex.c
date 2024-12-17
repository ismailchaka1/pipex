/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaka <chaka@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:35:13 by chaka             #+#    #+#             */
/*   Updated: 2024/12/17 00:48:49 by chaka            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

static size_t	countword(char const *s, char c)
{
	size_t	i;
	size_t	count;
	size_t	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c)
		{
			in_word = 0;
		}
		i++;
	}
	return (count);
}

static void	split_free(char **str, size_t k)
{
	if (str)
	{
		while (k > 0)
			free(str[--k]);
		free(str);
	}
}

static int	word_alloc(char **str, size_t k, size_t t)
{
	str[k] = (char *)malloc((t + 1) * sizeof(char));
	if (!str[k])
		return (split_free(str, k), 0);
	return (1);
}

static void	skip_char(const char *s, size_t *index, char c)
{
	while (s[*index] == c)
	{
		(*index)++;
	}
}

char	**ft_split(char const *s, char c)
{
	size_t	k;
	size_t	i;
	size_t	t;
	char	**str;

	k = 0;
	i = 0;
	if (!s)
		return (NULL);
	str = (char **)malloc((countword(s, c) + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	while (k < countword(s, c))
	{
		skip_char(s, &i, c);
		t = 0;
		while (s[i + t] != c && s[i + (++t)] != '\0')
			;
		if (!word_alloc(str, k, t))
			return (NULL);
		strlcpy(str[k], s + i, t + 1);
		str[k++][t] = '\0';
		i += t;
	}
	return (str[k] = NULL, str);
}

char **get_path(char *command, char **env)
{
    int i = 0;
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
            break;
            // printf ("%s \n", env[i] + 5);
        i++;
    }
    return ft_split(env[i] + 5, ':');
}

char *get_command(char *command, char **paths) {
    int i = 0;
    char *path = NULL;

    while (paths[i]) {
        path = malloc(strlen(paths[i]) + strlen(command) + 2);
        if (!path) {
            perror("Malloc failed");
            return NULL;
        }
        strcpy(path, paths[i]);
        strcat(path, "/");
        strcat(path, command);

        if (access(path, R_OK | X_OK) == 0)
            return path; // Found a valid command, return the path

        free(path); // Free memory if the path is invalid
        path = NULL;
        i++;
    }
    return NULL; // No valid path found
}


int main(int argc, char **argv, char **envp) {
    int fd[2];
    int fd1, fd2;
    int pid;

    if (argc <= 4) {
        fprintf(stderr, "Error: More arguments needed\n");
        return 1;
    }

    // Open input and output files
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror("Error opening input file");
        return 1;
    }

    fd2 = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (fd2 == -1) {
        perror("Error opening output file");
        close(fd1);
        return 1;
    }

    // Get paths from environment
    char **paths = get_path(argv[2], envp);
    if (!paths) {
        fprintf(stderr, "Error retrieving paths\n");
        close(fd1);
        close(fd2);
        return 1;
    }

    // Loop through commands
    for (int i = 2; i < argc - 1; i++) {
        if (pipe(fd) == -1) {
            perror("Error creating pipe");
            close(fd1);
            close(fd2);
            return 1;
        }
        
        char **args = ft_split(argv[i], ' ');
        char *command = get_command(args[0], paths);
        printf("%s \n",command);
        
        if (!command) {
            fprintf(stderr, "Error: Command '%s' not found\n", argv[i]);
            // free_split(paths); // Clean up paths if exiting
            exit(1);
        }

        pid = fork();
        if (pid == -1) {
            perror("Error forking process");
            free(command);
            close(fd1);
            close(fd2);
            return 1;
        }

        if (pid == 0) { // Child process
            // Redirect input and output
            if (i == 2) { // First command
                dup2(fd1, STDIN_FILENO);
            } else { // Middle commands
                dup2(fd[0], STDIN_FILENO);
            }
            // printf("%d %d \n", argc - 2, i);
            if (i == argc - 3) { // Last command
                printf("last command");
                dup2(fd2, STDOUT_FILENO);
            } else { // Middle commands
                printf("middle command");
                dup2(fd[1], STDOUT_FILENO);
            }

            close(fd[0]);
            close(fd[1]);
            close(fd1);
            close(fd2);

            // Parse the command arguments
            char **args = ft_split(argv[i], ' ');
            if (!args) {
                perror("Error splitting command arguments");
                exit(1);
            }
            

            execve(command, args, envp);
            perror("Error executing command");
            exit(1);
        }

        // Parent process
        close(fd[0]);
        close(fd[1]);
        free(command);
        wait(NULL); // Wait for the child process to finish
    }

    close(fd1);
    close(fd2);
    return 0;
}
