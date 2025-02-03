#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define SIZE 4064
#define MTHREADS 100
// Timed out error probably size
struct thread_data
{
    char *filename;
    char *needle;
    long start;
    long end;
};
void *thread_search(void *argument)
{
    struct thread_data *data =(struct thread_data *)argument;
    FILE *text = fopen(argv[i], "r");
    char haystack[SIZE];    
    int i_count = 0;
    
    while(fgets(haystack, SIZE, text))
        {
            char *c_pos = haystack;
            while((c_pos = strstr(c_pos, needle)) != NULL)
            {
                i_count++;
                c_pos += needle_length;
            }
            
            }
    fclose(text);
    pthread_exit((void*)(intptr_t)count);
}

int main (int argc, char *argv[])
{
    int num = atoi(argv[1]);
    if (num <= 0 || num > MTHREADS || argc < (num+3))
    {
        printf("Invalid\n");
        return EXIT_FAILURE;
    }
    char *word = argv[2];
    pthread_t threads[MTHREADS];
    struct thread_data threaddata[MTHREADS];
    pid_t p_pipe;
    char needle[SIZE];
    printf("Enter a word: ");
    scanf("%s", needle);
    size_t needle_length = strlen(needle);
    for(int i = 1; i < argc; i++)
    {
        p_pipe = fork();
        if(p_pipe == 0)
        {
            FILE *text = fopen(argv[i], "r");
            char haystack[SIZE];
            //int f_count = 0;
            int i_count = 0;
            //char *line_position = NULL;
            //size_t word_pointer = 0;
            // Not working in autograder trying something else
            //size_t position = getline(&line_position, &word_pointer, text);
            while(fgets(haystack, SIZE, text))
            {
                char *c_pos = haystack;
                while((c_pos = strstr(c_pos, needle)) != NULL)
                {
                    i_count++;
                    c_pos += needle_length;
                }
                //f_count = i_count;
            }
            printf("%s - %d\n", argv[i], i_count);
            fclose(text);
            exit(0);
        }
    }
    for(int j = 1; j < argc; j++)
    {
        wait(NULL);
    }
    return 0;
}