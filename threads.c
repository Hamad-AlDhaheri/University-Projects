#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_THREADS 100
#define SIZE 4064

struct TData {
    char *filename;
    char *needle;
    char *haystack;
    int count;
};

void *tsearch(void *arg)
{
    struct TData *data = (struct TData *)arg;
    int cot=0;
    FILE* file = fopen(thread_info->file_name,"r");
    char *position = thread_info->haystack;

    while((pos = memmem(position, size_t hlen = strlen(position), thread_info->needle, size_t nlen=strlen(thread_info->needle))))
    {
        cot++;
        thread_info->count++;
        pos += nlen;
    }

    fclose(file);
    pthread_exit(NULL);
}
int main (int argc, char *argv[])
{
    int num= atoi(argv[1]);
    pid_t fo;
    char needle[SIZE];
    printf("Enter a word: ");
    scanf("%s", needle);
    
    for(int i=1;i<argc;i++)
    {
        fo=fork();
        if(fo==0)
        {
            int s = SIZE/MAX_THREADS;
            char haypack[s];
            int count=0;
            pthread_t TData[MAX_THREADS];
            struct TData info[MAX_THREADS];
            int packsize= strlen(haypack);
            int packlocal = packsize/MAX_THREADS;

            for(int j=0; j <MAX_THREADS; j++)
            {
                info[j].haystack= (chunk+j*packlocal)-(strlen(needle)+1);
                info[j].needle = needle;
                info[j].file_name = argv[i];
                pthread_create(&TData[j],NULL, search, (void*)&info[j]);
            }
            for(int z=0; z< MAX_THREADS; z++)
            {
                pthread_join(TData[z], NULL);
                count +=info[z].count;
            }
            printf("%s - %d\n", argv[i], count);
        }
    }
    for (int h=2; h<argc; h++)
    {
        wait(NUll);
    }
    return 0;
}