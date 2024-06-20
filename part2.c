#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 329924567 Gideon Neeman
void write_message(const char *message, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", message);
        usleep((rand() % 100) * 1000); // Random delay between 0 and 99 milliseconds
    }
}
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <message1> <message2> ... <count>", argv[0]);
        return 1;
    }
    int count = atoi(argv[argc - 1]);
    for (int i = 0; i< argc -2; i++){
        __pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            while(access("lockfile.lock", F_OK) == 0){
                usleep(100);
            }
            FILE *lockfile = fopen("lockfile.lock", "w");
            write_message(argv[i+1], count);
            fclose(lockfile);
            remove("lockfile.lock");
            exit(0);
        }
    }
    wait(NULL);
    return 0;
}