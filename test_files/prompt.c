#include <stdio.h>
#include <stdlib.h>

/*read lines from stdin*/
char read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin))
        printf("  %s", line);
    {
        if (feof(stdin))
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        // else
        // {
        //     perror("readline");
        // }
    }
    return (*line);
}



int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av)
{

    while (1)
    {
        printf("$ ");
        read_line();
       
    }
    return EXIT_SUCCESS;
}
