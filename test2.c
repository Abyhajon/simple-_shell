#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char *str, *buffer;
    char *av[2];
    size_t bufsize = 120;
    int i, j = 0, k = 0;
    pid_t pid;
    int status;

char* read_line()
{
    
}

char 

/**
 * main - entry point for program execution.
 *
 * Return: 0 (success)
 */

int main(void)
{
    char *str, *buffer;
    char *av[2];
    size_t bufsize = 120;
    int i, j = 0, k = 0;
    pid_t pid;
    int status;

    buffer = malloc(sizeof(char) * bufsize);

    while (1)
    {
        // printing prompt.
        printf("$ ");
        // reading the input from user.
        i = getline(&buffer, &bufsize, stdin);
        if (i == -1)
        {
            printf("\n");
            break;
        }
        str = malloc(sizeof(char) * (i + 1));
        while(buffer[j] != '\0' && buffer[j] != '\n')
        {
            str[k] = buffer[j];
            k++;
            j++;
        }
        str[k] = '\0';

        // execution.
        av[0] = str;
        av[1] = NULL;
        pid = fork();
        if (pid == 0) // child process.
        {
            if (execve(av[0], av, NULL) == -1)
                perror("error");
            exit(0);
        }
        else
        {
            wait(&status);
        }

    }
    return(0);
}

