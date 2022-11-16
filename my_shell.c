#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define JOSH_TOK_BUFSIZE 64


void josh_exec(char **args) {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        execve(args[0], args, NULL);
        perror("josh");
        exit(1);
    } else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("josh");
    }
}
char** josh_split_line(char *line)
{
    int bufsize = JOSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;
    char *delimeters = " ";

    if (!tokens)
    {
        fprintf(stderr, "josh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delimeters);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += JOSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "josh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delimeters);
    }
    tokens[position] = NULL;
    return tokens;
}

char* josh_read_line()
{
    char *line = NULL;
    size_t buflen = 0;
    getline(&line, &buflen, stdin);
    return line;

}

int main()
{
    while (1)
    {
        printf("$ ");
        char *line = josh_read_line();
        char **tokens = josh_split_line(line);
        
        if (tokens[0] != NULL)
        {
            josh_exec(tokens);
        }
        free(tokens);
        free(line);
    }
    
}