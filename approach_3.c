#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


long long global_count = 0;
pthread_mutex_t lock;

typedef struct {
    long long nPoints;
} ThreadArgs;

void* count_points( void* args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    long long n = targs->nPoints;
    int thread_id = *(int*)args;
    unsigned int seed = time(NULL) ^ thread_id;
    for (long long i = 0; i < n; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if ((x * x + y * y) <= 1.0){
            pthread_mutex_lock(&lock);
            global_count ++;
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

double get_time_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void run(long long numPoint , int numThreads,FILE* file) {
    double start = get_time_sec();
    global_count =0;
    long long totalPoints = numPoint;
    int nThreads = numThreads;

    pthread_t threads[nThreads]; 
    ThreadArgs targs[nThreads]; 

    pthread_mutex_init(&lock, NULL);

    long long pointsPerThread = totalPoints / nThreads;
    for (int i = 0; i < nThreads; i++) {
        targs[i].nPoints = pointsPerThread;
        pthread_create(&threads[i], NULL, count_points, &targs[i]);

    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = get_time_sec();

    pthread_mutex_destroy(&lock);
    
    double pi = 4.0 * global_count / totalPoints;
    printf("Total Points = %lld\n", totalPoints);
    printf("Estimated Pi = %.10f\n", pi);
    printf("Execution Time = %.6f seconds\n", end - start);
    printf("---------------------------------------------------------------\n");
    fprintf(file, "%10f %.6f\n",pi ,end - start);

}

int main() {
    FILE* file = fopen("aproach_3.txt", "w");

    int numThreads = 1000;
    run (100000000,100,file); // 10 trieu 100 thraeds -> so sanh voi aproach 2
    run (100000000,numThreads,file); // 10 trieu 1000 thraeds -> so sanh voi aproach 2
    
    run (20000000,numThreads,file); // 20 trieu
    run (30000000,numThreads,file); // 30 trieu
    run (40000000,numThreads,file); // 40 trieu
    run (50000000,numThreads,file); // 50 trieu
    run (60000000,numThreads,file); // 60 trieu
    run (70000000,numThreads,file);// 70 trieu
    run (80000000,numThreads,file); // 80 trieu
    run (90000000,numThreads,file); // 90 trieu
    run (100000000,numThreads,file); // 100 trieu

    fclose(file);
    return 0;
}
