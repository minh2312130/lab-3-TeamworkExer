#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_POINTS 100000000
#define MAX_THREADS 1000

int *counts;
int total_threads;

void* count_inside_circle(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);

    unsigned int seed = time(NULL) ^ thread_id;
    int local_count = 0;
    int points_per_thread = TOTAL_POINTS / total_threads;

    for (int i = 0; i < points_per_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0)
            local_count++;
    }

    counts[thread_id] = local_count;
    pthread_exit(NULL);
}

int main() {
    FILE* file = fopen("results.txt", "w");
    if (file == NULL) {
        perror("Failed to open results.txt");
        return 1;
    }

    fprintf(file, "Threads\tEstimated_Pi\tExecution_Time(s)\n");

    for (total_threads = 1; total_threads <= MAX_THREADS; total_threads++) {
        pthread_t* threads = malloc(sizeof(pthread_t) * total_threads);
        counts = malloc(sizeof(int) * total_threads);

        clock_t start = clock();

        for (int i = 0; i < total_threads; i++) {
            int* tid = malloc(sizeof(int));
            *tid = i;
            pthread_create(&threads[i], NULL, count_inside_circle, tid);
        }

        for (int i = 0; i < total_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        int total_inside = 0;
        for (int i = 0; i < total_threads; i++) {
            total_inside += counts[i];
        }

        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        double pi = 4.0 * total_inside / TOTAL_POINTS;

        fprintf(file, "%d\t%f\t%f\n", total_threads, pi, time_spent);

        free(counts);
        free(threads);

        // Optional: progress log
        if (total_threads % 100 == 0) {
            printf("Completed %d threads...\n", total_threads);
        }
    }

    fclose(file);
    printf("All done! Results saved in results.txt\n");

    return 0;
}

