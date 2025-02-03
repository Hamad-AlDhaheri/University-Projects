#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

struct th_info {
    int count;
    char word[50];
    char file_name[1024];
    int start;
    int end;
};

void* search(void* args) {
    struct th_info* th_info = (struct th_info*)args;

    FILE* file = fopen(th_info->file_name, "r");
    fseek(file, th_info->start, SEEK_SET);

    char chunk[1024];
    size_t word_len = strlen(th_info->word);
    size_t count = 0;

    while (ftell(file) < th_info->end && fgets(chunk, sizeof(chunk), file) != NULL) {
        char* pos = chunk;
        while ((pos = strstr(pos, th_info->word)) != NULL) {
            count++;
            pos += word_len;
        }
    }

    th_info->count = count;
    fclose(file);
    pthread_exit(NULL);
    }

int main(int argc, char* argv[]) {
    int num_th = atoi(argv[1]);
    pid_t pid;
    char word[50];

    printf("Enter a word: ");
    scanf("%s", word);

    for (int i = 2; i < argc; i++) {
        pid = fork();

        if (pid == 0) {
            pthread_t th[num_th];
            struct th_info info[num_th];

            FILE* file = fopen(argv[i], "r");

            fseek(file, 0, SEEK_END);
            int file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            int chunk_size = file_size / num_th;

            for (int j = 0; j < num_th; j++) {
                strncpy(info[j].word, word, 50);
                strncpy(info[j].file_name, argv[i], 1024);
                info[j].start= j * chunk_size;
                info[j].end = (j + 1) * chunk_size;
                pthread_create(&th[j], NULL, search, (void*)&info[j]);
            }
            int count = 0;
            for (int j = 0; j < num_th; j++) {
                pthread_join(th[j], NULL);
                count += info[j].count;
            }

            printf("%s - %d\n", argv[i], count);
            fclose(file);
            exit(0);
        }
    }

    for (int i = 2; i < argc; i++) {
        wait(NULL);
    }

    return 0;
}
