#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

void print_process(clock_t start)
{
    clock_t end = clock();

    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("PID: %d, PPID: %d, Execution time: %.3f ms\n",
           getpid(), getppid(), time_ms);
}

int main()
{
    pid_t child1, child2;
    clock_t start;

    /* Create the first child */
    child1 = fork();

    if (child1 < 0)
    {
        perror("fork");
        return 1;
    }

    if (child1 == 0)
    {
        /* First child */
        start = clock();

        print_process(start);

        return 0;
    }

    /* Only the main process reaches here */

    /* Create the second child */
    child2 = fork();

    if (child2 < 0)
    {
        perror("fork");
        return 1;
    }

    if (child2 == 0)
    {
        /* Second child */
        start = clock();

        print_process(start);

        return 0;
    }

    /* Main process */
    start = clock();

    print_process(start);

    /* Wait for both children */
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    return 0;
}
