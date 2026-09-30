#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main()
{
    char input[MAX_INPUT];

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        if (strcmp(input, "exit") == 0)
            break;

        char *args[MAX_ARGS];
        int argc = 0;

        char *token = strtok(input, " ");

        while (token != NULL && argc < MAX_ARGS - 1)
        {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }

        args[argc] = NULL;

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            char *envp[] = { NULL };

            execve(args[0], args, envp);

            /* If command was not given with a full path */
            char path[1024];
            snprintf(path, sizeof(path), "/bin/%s", args[0]);

            execve(path, args, envp);

            snprintf(path, sizeof(path), "/usr/bin/%s", args[0]);

            execve(path, args, envp);

            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
