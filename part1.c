#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 329924567 Gideon Neeman
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <parent_message> <child1_message> <child2_message> <count>", argv[0]);
        return 1;
    }
    FILE *f;
    f = fopen("output.txt", "w");
    int times = atoi(argv[4]);
    __pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        for (int i = 0; i < times; i++)
        {
            fprintf(f, "%s\n", argv[2]);
        }
        exit(0);
    }
    else
    {
        __pid_t pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid2 == 0)
        {
            wait(NULL);
            for (int i = 0; i < times; i++)
            {
                fprintf(f, "%s\n", argv[3]);
            }
            exit(0);
        }
        else
        {
            wait(NULL);
            for (int i = 0; i < times; i++)
            {
                fprintf(f, "%s\n", argv[1]);
            }
            fclose(f);
        }
    }
    return 0;
}