#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>

#define LENGTH 100

int *repet;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_Data {
    char filename[100];
    char needle[100];
    int jump_pos;
    int start_pos;
    int current_pos;
};

char input[LENGTH];

int word_counter_function(const char *filename, const char *needle, int start_pos, int size) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1; // Error opening file
    }

    if (fseek(file, start_pos, SEEK_SET) != 0) {
        perror("Error seek not possible");
        fclose(file);
        return 1;
    }

    char *buff = malloc(size + 1);
    if (buff == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(buff, 1, size, file);
    fclose(file);

    if (bytesRead < size) {
        perror("Failed to read file");
        free(buff);
        return 1;
    }

    buff[size] = '\0';
    int count = 0;
    char *position = buff;
    while ((position = strstr(position, needle)) != NULL) {
        count++;
        position += strlen(needle);
    }

    free(buff);
    return count;
}

void *Threads_Function(void *var) {
    struct thread_Data *args = (struct thread_Data *)var;

    pthread_mutex_lock(&mutex);
    repet[args->current_pos] = word_counter_function(args->filename, args->needle, args->start_pos, args->jump_pos);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int number_threads = atoi(argv[1]);

    printf("Enter a word: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0'; // Remove the newline character
    } else {
        perror("Error reading input");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < (argc - 2); i++) {
        pid_t fo = fork();
        if (fo == 0) {
            FILE *file = fopen(argv[i + 2], "r");
            if (file == NULL) {
                perror("Error opening file");
                return EXIT_FAILURE;
            }

            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            int jump_local = ceil(file_size / number_threads);

            pthread_t threads[number_threads];
            repet = (int *)malloc(sizeof(int) * number_threads);
            struct thread_Data args[number_threads];
            args[0].start_pos = 0;
            int jump_2 = jump_local;

            for (int z = 0; z < number_threads; z++) {
                char character = fgetc(file);
                while (character != ' ' && character != '\n' && character != '\r' && character != '\t') {
                    if (z == number_threads - 1) {
                        jump_2 = file_size - args[z].start_pos;
                        break;
                    }
                    jump_2++;
                    fseek(file, args[z].start_pos + jump_2, SEEK_SET);
                    character = fgetc(file);
                }
                strcpy(args[z].needle, input);
                args[z].jump_pos = jump_2;
                args[z].current_pos = z;
                strcpy(args[z].filename, argv[i + 2]);

                int attempt = pthread_create(&threads[z], NULL, Threads_Function, (void *)&(args[z]));
                if (attempt) {
                    exit(EXIT_FAILURE);
                }

                args[z + 1].start_pos = jump_2 + args[z].start_pos;
                jump_2 = jump_local;
            }

            int total = 0;
            for (int j = 0; j < number_threads; ++j) {
                pthread_join(threads[j], NULL);
                total += repet[j];
            }

            printf("%s - %d\n", argv[i + 2], total);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < argc - 2; i++) {
        wait(NULL);
    }

    return 0;
}
