
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*Built in commands*/

/*Exit the shell.*/
void sshl_exit(__attribute__((unused))char **args)
{
    exit(0);
}

/*Change the working directory.*/
void sshl_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "sshl: cd: missing argument\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("sshl: cd");
        }
    }
}

/*Print the shell's help text.*/
void sshl_help(__attribute__((unused))char **args)
{
    char *helptext =
        "sshl - the Kinda Aimless Shell. "
        "The following commands are built in:\n"
        "  cd       Change the working directory.\n"
        "  exit     Exit the shell.\n"
        "  help     Print this help text.\n"
        ;
    printf("%s", helptext);
}

/*A builtin instance associates a command name with a handler function.*/
struct builtin {
    char *name;
    void (*func)(char **args);
};

/*Array of built in commands.*/
struct builtin builtins[] = {
    {"help", sshl_help},
    {"exit", sshl_exit},
    {"cd", sshl_cd},
};

/*Returns the number of registered commands.*/
int sshl_num_builtins()
{
    return sizeof(builtins) / sizeof(struct builtin);
}

/**
 * 
 * Process/command launcher.
 * 
 */

void sshl_exec(char **tokens)
{
    pid_t child_pid;
    int status, i;


    for (i = 0; i < sshl_num_builtins(); i++)
    {
        if (strcmp(tokens[0], builtins[i].name) == 0)
        {
            builtins[i].func(tokens);
            return;
        }
    }

    child_pid = fork();

	if (child_pid == -1)
	{
		perror("Error");
		exit(1);
	}
	else if (child_pid == 0)
	{
		if (execvp(tokens[0], tokens) == -1)
		{
			perror("Error");
			exit(-1);
		}
	}
	else
		wait(&status);

}

/**
 * 
 * Input parser.
 * 
 */

/**
 * Tokenize a string, splitting on whitespace characters. Leading and trailing
 * whitespace is ignored. Consecutive whitespace characters are treated as a
 * single delimiter. The return value is a NULL terminated array of string
 * pointers which needs to be freed once we're finished with it.
 */
char** sshl_split_line(char *line)
{
    int position = 0;
    int bufsize = 16;
    char *delimiters;
    char *token;

    char **tokens = malloc(bufsize * sizeof(char*));
    if (!tokens)
    {
        perror("sshl");
        exit(1);
    }

    delimiters = " \t\r\n";
    token = strtok(line, delimiters);

    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize = (int) (bufsize * 1.5);
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                perror("sshl");
                exit(1);
            }
        }

        token = strtok(NULL, delimiters);
    }

    tokens[position] = NULL;
    return tokens;
}

/**
 * Read a single line of input from stdin. The return value is a string pointer
 * which needs to be freed once we're finished with it.
 */
char* sshl_read_line()
{
    char *line = NULL;
    size_t buflen = 0;
    ssize_t str;
    errno = 0;
    
    
    str = getline(&line, &buflen, stdin);
    if (str == -1)
    {
        if (errno)
        {
            perror("sshl");
        }
        exit(1);
    }
    return line;
}

/**
 * 
 * Entry point.
 * 
 */

int main()
{
    while (1)
    {
        char **tokens;
        char *line;

        printf("$ ");
        line = sshl_read_line();
        tokens = sshl_split_line(line);

        if (tokens[0] != NULL)
        {
            sshl_exec(tokens);
        }

        free(tokens);
        free(line);
    }
}
