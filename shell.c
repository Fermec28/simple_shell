#include "holberton.h"
/**
 * main - Entry point of shell
 * @ac: argument counter
 * @argv: argument values
 * @envp: enviroments
 * Return: 0 if success
 */
int main(int ac, char *argv[], char *envp[])
{
	char *lineptr = NULL, *full_path = NULL, interactive;
	size_t n = 0;
	ssize_t bytes;
	char *prompt = "$jessiFer> ", **myargv;
	int bytes_path;


	interactive = isatty(STDIN_FILENO);
	if(interactive)
	{
		write(1, prompt, 12);
		signal(SIGINT, sighandler);
		signal(EOF, sighandler);
	}
	while ((bytes = getline(&lineptr, &n, stdin)) != -1)
	{
		if (bytes > 0)
		{
			myargv = build_argv(lineptr);
			if(myargv && myargv[0] != NULL)
			{
				bytes_path = build_path(&full_path,myargv[0], envp);
				myexec(full_path, myargv, envp);
				if (bytes_path > 0)
				{
					free(full_path);
				}
			}
		}
		else if (bytes < 0)
		{
			write(STDOUT_FILENO, "Error\n", sizeof("Error\n"));
			exit(1);
		}
		else
		{
			free(lineptr);
			free(full_path);
			printf("%d\n", EOF);
			exit(0);
		}
		if(interactive)
			write(1, prompt, 12);
//		free(lineptr);
//		free(myargv);
		myargv = NULL;
		full_path = NULL;

	}
	return (0);
}
/**
 * myexec - Execute program
 * @exec_path: path to execute program
 * @args: arguments to execute
 * @env_args: enviroment variables
 * Return: retu if was successful or not
 */
int myexec(char *exec_path, char *args[], char *env_args[])
{
	pid_t pid;
	int wstatus = 0;

	pid = fork();
	if (pid == -1)
	{
		write(STDOUT_FILENO, "Error process\n", 14);
		return (-1);
	}
	else if (pid == 0)
	{
		if (execve(exec_path, args, env_args) == -1)
		{
			write(STDOUT_FILENO, "Error execv\n", 12);
			exit (-1);
		}
	}
	else
	{
		waitpid(pid, &wstatus, 0);
	}
	return (0);
}
/**
 * str_concat - Duplicate string
 * @s1: string one
 * @s2: string two
 * Return: pointer to copy array
 */
char *str_concat(char *s1, char *s2)
{
	char *p;
	unsigned int i;
	unsigned int size_s1, size_s2;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	for (size_s1 = 0; *(s1 + size_s1) && s1; size_s1++)
	{
	}
	for (size_s2 = 0; *(s2 + size_s2) && s2; size_s2++)
	{
	}
	p =  malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (p == NULL)
		return (NULL);
	for (i = 0; i < size_s1 + size_s2; i++)
	{
		if (i < size_s1)
			p[i] = *(s1 + i);
		else
			p[i] = *(s2 + i - size_s1);
	}
	p[i] = '\0';
	return (p);
}
/**
 * get_value_env - find the value of enviroment variable
 * @envp: enviroment variables
 * @variable: variable to search
 * Return: pointer start on value of variable
 *
 */
char *get_value_env(char **envp, char *variable)
{
	unsigned int envp_i = 0, variable_i = 0;
	char flag;

	while (envp[envp_i])
	{
		flag = 1;
		while (variable[variable_i] && flag)
		{
			if (envp[envp_i][variable_i] != variable[variable_i])
				flag = 0;
			variable_i++;
		}
		if (flag)
		{
			return (&envp[envp_i][variable_i + 1]);
		}
		variable_i = 0;
		envp_i++;
	}
	return (NULL);
}
/**
 * _realloc -  reallocates a memory block
 * @ptr: is a pointer to the memory previously allocated
 * @old_size: is the size, in bytes, of the allocated space for ptr
 * @new_size: is the new size, in bytes of the new memory block
 *
 * Return: pointer to the new allocation with the values from prev
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p, *ptr1 = ptr;
	unsigned int iterator = 0;

	if (new_size == old_size)
		return (ptr);
	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	p = malloc(new_size);
	if (p == NULL)
	{
		free(p);
		return (NULL);
	}
	while (iterator < new_size - 1)
	{
		p[iterator] = ptr1[iterator];
		iterator++;
	}
	free(ptr);
	return (p);
}

void *_realloc_pointer(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char **p, **ptr1 = ptr;
	unsigned int iterator = 0;

	if (new_size == old_size)
		return (ptr);
	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	p = malloc(new_size * sizeof(char*));
	if (p == NULL)
	{
		free(p);
		return (NULL);
	}
	while (iterator < new_size - 1)
	{
		p[iterator] = ptr1[iterator];
		iterator++;
	}
	free(ptr);
	return (p);
}
/**
 * build_argv - Build argv to send myexec
 * @lineptr: Line with value stored the information
 * Return: Array to pointers with Arguments
 */
char** build_argv(char *lineptr)
{
/*
  hacer free cuando no se pueda hacer realloc hacer free
*/
	char **argv= NULL;
	char len = 1;

	lineptr = strtok(lineptr, " \n");
	while (lineptr != NULL)
	{
		argv = _realloc_pointer(argv, len - 1, len);
		argv[len - 1] = lineptr;
		lineptr = strtok(NULL, " \n");
		len++;
	}
	argv = _realloc_pointer(argv, len - 1, len);
	argv[len - 1] = NULL;
	return (argv);
}
/**
 * build_path - fin path to execute
 * @path: comand to execute
 * Return: Pointer with the value of full path
 */
int build_path(char **full_path, char *argv_0, char *envp[])
{
	char *token = NULL, *s = ":", *path = NULL, *aux;
	struct stat st;

	if (argv_0[0] == '/')
	{
		if (stat(argv_0, &st) == 0)
		{
			*full_path = argv_0;
			return (0);
		}
		else
		{
			return (-1);
		}
	}
	else
	{
		aux = str_concat(get_value_env(envp, "PATH"), "");
		token = strtok(aux, s);
		while (token != NULL)
		{
			path = str_concat(token, "/");
			if (path == NULL)
			{
				free(aux);
				printf("ERRRORR en concat\n");
				return (-1);
			}
			*full_path = str_concat(path, argv_0);
			free(path);
			if (*full_path == NULL)
			{
				free(aux);
				printf("ERRRORR en concat\n");
				return (-1);
			}
			if (stat(*full_path, &st) == 0)
			{
				free(aux);
				return (1);
			}
			free(*full_path);
			token = strtok(NULL, s);
		}
		free(aux);
		return(-1);
	}
}
/**
 * sighandler - Function handle the signals
 * @signum: signal num
 */
void sighandler(int signum)
{
	;
}
