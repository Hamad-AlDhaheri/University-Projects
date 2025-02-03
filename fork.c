#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define Length 50

int main()
{
    int fd_pipe[2];
    pid_t p;
    char executable[Length];
    char input[Length];
    pipe(fd_pipe);
    p=fork();


    if (p == 0)
    {
        ssize_t n = read(fd_pipe[0], executable, (Length));
        close(fd_pipe[1]);
        executable[n]='\0';

        if (strcmp(executable, "id") == 0) 
        {
            execl("/usr/bin/id", "id", NULL);
        } 
        else if (strcmp(executable, "pwd") == 0)
        {
            execl("/usr/bin/pwd", "pwd", NULL);
        } 
        else if (strcmp(executable, "date") == 0) 
        {
            execl("/usr/bin/date", "date", NULL);
        } 
    }
    else if (p > 0)
    {
        printf("prompt$- ");
        scanf(input, sizeof(input), stdin);
        write(fd_pipe[1], input, (strlen(input)+1));
        close(fd_pipe[1]);

        int status;
        waitpid(p, &status, 0);
        printf("Done- %s", input);
    }
    return 0;
 }
