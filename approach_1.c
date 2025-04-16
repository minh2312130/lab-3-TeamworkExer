#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_POINTS 100000000
#define NUM_RUNS 1000

int main() {
    FILE* file = fopen("pi_single_runs.txt", "w");
    if (file == NULL) {
        perror("Failed to open output file");
        return 1;
    }

    fprintf(file, "Run\tEstimated_Pi\tExecution_Time(s)\n");

    for (int run = 1; run <= NUM_RUNS; run++) {
        clock_t start, end;
        double cpu_time_used;
        long points_in_circle = 0;
        double x, y;

        start = clock();

        unsigned int seed = time(NULL) ^ run; // unique seed per run

        for (long i = 0; i < NUM_POINTS; i++) {
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;

            if ((x * x + y * y) <= 1.0) {
                points_in_circle++;
            }
        }

        double pi_estimate = 4.0 * points_in_circle / NUM_POINTS;

        end = clock();
        cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

        // Print to console (optional)
        printf("Run %d: Pi = %f, Time = %f sec\n", run, pi_estimate, cpu_time_used);

        // Write to file
        fprintf(file, "%d\t%f\t%f\n", run, pi_estimate, cpu_time_used);
    }

    fclose(file);
    printf("All runs complete! Results saved in pi_single_runs.txt\n");

    return 0;
}

