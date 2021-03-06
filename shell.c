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
	char *lineptr = NULL, *full_path = NULL;
	char interactive, *prompt = "$jessiFer> ", **myargv;
	size_t n = 0;
	ssize_t bytes;
	int bytes_exec = 0, c_prompt = 1;
	op_t operate;
	(void)ac, (void)argv;

	interactive = isatty(STDIN_FILENO);
	if (interactive)
		signal(SIGINT, sighandler);
	while ((bytes = getline(&lineptr, &n, stdin)))
	{
		if (bytes > 0)
		{	myargv = build_argv(lineptr);
			if (myargv && myargv[0] != NULL)
			{	operate.f = getfunction(myargv[0]);
				if (operate.f != NULL)
					operate.f(myargv, envp, lineptr, bytes_exec);
				else
				{	bytes_exec = build_path(c_prompt, &full_path, myargv[0], envp);
					if (bytes_exec != 127)
						bytes_exec = myexec(lineptr, full_path, myargv, envp);
					free(full_path);	}
			}
			free(myargv);	}
		else if (bytes < 0)
		{	free(lineptr);
			exit(0);
		}
		if (interactive)
			write(1, prompt, 12);
		free(lineptr);
		lineptr = NULL;
		myargv = NULL;
		full_path = NULL;
		c_prompt++;
	}
	return (0);
}
