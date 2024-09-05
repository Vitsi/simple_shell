#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t nread;
    pid_t pid;
    char *args[2];

    while (1)
    {
        printf("($) ");  /* Shell prompt */
        nread = getline(&buffer, &bufsize, stdin);  /* Read input */
        if (nread == -1)
        {
            free(buffer);
            exit(0);  /* Handle Ctrl+D (EOF) */
        }

        buffer[nread - 1] = '\0';  /* Remove newline character */

        pid = fork();  /* Create child process */
        if (pid == 0)  /* Child process */
        {
            args[0] = buffer;
            args[1] = NULL;

            if (execve(buffer, args, NULL) == -1)
                perror("./hsh");  /* Command not found */
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)  /* Parent process */
        {
            wait(NULL);  /* Wait for child process to finish */
        }
        else  /* Fork failed */
        {
            perror("fork");
        }
    }

    free(buffer);
    return 0;
}

