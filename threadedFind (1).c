#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>


int *ret;
struct ThreadArgs {
        char filename[100];
        char word[100];
        int jump;
        int index;
        int start;
};

char *line = NULL;
int countWordInFile(const char *filename, const char *word, int start, int length) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1; // Error opening file
    }

    if (fseek(file, start, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(file);
        return -1; // Error seeking in file
    }

    char *buffer = malloc(length + 1); // +1 for null terminator
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return -1; // Memory allocation error
    }

    size_t bytesRead = fread(buffer, 1, length, file);
    fclose(file);

    if (bytesRead < length) {
        perror("Error reading from file");
        free(buffer);
        return -1; // Error reading from file
    }

    buffer[length] = '\0'; // Null-terminate the string

    int count = 0;
    char *pos = buffer;

    while ((pos = strstr(pos, word)) != NULL) {
        count++;
        pos += strlen(word);
    }

    free(buffer);

    return count;
}

void *threadFunc(void *var) {
        struct ThreadArgs *args = (struct ThreadArgs *)var;


        ret[args->index] = countWordInFile(args->filename, args->word, args->start, args->jump);
        pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
        pid_t pid = 0;
        int p[2], jump, sum;
        char prompt[45], character;
        int numThreads = atoi(argv[1]);

        if (pipe(p) < 0) {
                perror("pipe Failed");
                exit(EXIT_FAILURE);
        }

        printf("Enter a word: ");
        scanf("%s", prompt);

        for (int i = 0; i < (argc - 2); i++) {
                pid = fork();
                FILE *file = fopen(argv[i+2], "r");
                if (file == NULL) {
                        perror("Error opening file");
                        exit(EXIT_FAILURE);
                }

                if (fseek(file, 0, SEEK_END) != 0) {
                        perror("Error seeking to end of file");
                        fclose(file);
                        return EXIT_FAILURE;
                }

                long size = ftell(file);
                if (size == -1) {
                        perror("Error getting file size");
                        fclose(file);
                        return EXIT_FAILURE;
                }

                int jumpAmount = ceil(size / numThreads);

                if (pid < 0) {
                        perror("fork failed");
                        exit(EXIT_FAILURE);
                }

                if (pid == 0) {
                        pthread_t threads[numThreads];
                        ret = (int *)malloc(sizeof(int)*numThreads);
                        struct ThreadArgs args[numThreads];
                        args[0].start = 0;
                        jump = jumpAmount;
                        for (int j = 0; j < numThreads; j++) {
                                if (fseek(file, args[j].start + jumpAmount, SEEK_SET) != 0) {
                                        perror("Error seeking in file");
                                        fclose(file);
                                        return 1;
                                }
                                character = fgetc(file);
                                while(character != ' ' &&
                                character != '\n' &&
                                character != '\r' &&
                                character != '\t'){

                                        if(j == numThreads - 1){
                                                jump = size - args[j].start;
                                                break;
                                        }

                                        jump++;
                                        fseek(file, args[j].start + jump, SEEK_SET);
                                        character = fgetc(file);
                                }
                                strcpy(args[j].word, prompt);
                                args[j].jump = jump;
                                args[j].index = j;
                                strcpy(args[j].filename, argv[i+2]);

                                int result = pthread_create(&threads[j], NULL, threadFunc, (void *)&(args[j]));
                                if (result) {
                                        exit(EXIT_FAILURE);
                                }
                                args[j+1].start = jump + args[j].start;
                                jump = jumpAmount;

                        }
                        sum = 0;
                        for (int k = 0; k < numThreads; ++k) {
                                pthread_join(threads[k], NULL);
                                sum += ret[k];

                        }

                        printf("%s - %d\n", argv[i+2], sum);

                        exit(EXIT_SUCCESS);
                }

        }

        for (int l = 0; l < argc - 2; l++){
                wait(NULL);
        }
}
