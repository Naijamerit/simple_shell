#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

#define MAX_LINE 80

extern char **environ;

/**
 * search_path - search for a command in the PATH
 * @cmd: the command to search for
 *
 * Return: If the command is found, return the full path. Otherwise, return NULL.
 */
char *search_path(char *cmd)
{
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *dir = strtok(path_copy, ":");
	char *full_path = malloc(MAX_LINE);

	while (dir != NULL)
	{
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strncat(full_path, cmd, MAX_LINE - strlen(full_path) - 1);
	if (access(full_path, X_OK) == 0)
	{
	free(path_copy);
	return (full_path);
	}
	dir = strtok(NULL, ":");
	}

	free(path_copy);
	free(full_path);
	return (NULL);
}

/**
 * main - simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *args[MAX_LINE / 2 + 1];
	int should_run = 1;
	char line[MAX_LINE];
	pid_t pid;
	int status;

	while (should_run)
	{
	write(STDOUT_FILENO, "osh> ", 5);
	fgets(line, MAX_LINE, stdin);
	line[strcspn(line, "\n")] = '\0';

	int i = 0;
	args[i] = strtok(line, " ");
	while (args[i] != NULL)
	{
	i++;
	rgs[i] = strtok(NULL, " ");
	}

	if (strcmp(args[0], "exit") == 0)
	{
	should_run = 0;
	}
	else if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
	write(STDOUT_FILENO, "\n", 1);
	}
	}
	else
	{
	char *cmd_path = search_path(args[0]);
	if (cmd_path != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
		execv(cmd_path, args);
		}
		else
		{
		wait(&status);
		}
		free(cmd_path);
		}
		else
		{
		write(STDERR_FILENO, "Command not found\n", 17);
			}
		}
	}

		return (0);
}

