#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define B_size 1024

int count(FILE *file, char *word)
{
    char wordset[B_size];
    int count=0;
    size_t wordlen = strlen(word);

    while(fgets(wordset, sizeof(wordset), file) != NULL)
    {
        char *position = wordset;
        while ((position = strstr(position, word)) != NULL)
        {
            count++;
            position += wordlen;
        }
    }
    return count;
}
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
    }
    else if (p > 0)
    {
        printf("needle- ");
        scanf(%w);
        write(fd_pipe[1], input, (strlen(input)+1));
        close(fd_pipe[1]);

        int status;
        waitpid(p, &status, 0);
        printf("Done- %s", input);
    }
    return 0;
 }
