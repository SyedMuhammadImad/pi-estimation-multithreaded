// Pi Estimation — Multithreaded Implementation
// Computes Pi using the Maclaurin series for arctan(1) = Pi/4, splitting the
// n terms evenly across a user-specified number of POSIX threads. Each thread
// accumulates its own partial sum locally, then merges it into a shared
// global_sum under a mutex to avoid race conditions.
//
// Usage: ./pi_mutex
//   (prompts for number of threads and number of terms n, where n > 100000)

#include <iostream>
#include <pthread.h>
using namespace std;

double global_sum = 0.0;
pthread_mutex_t mutex;

struct ThreadData {
    long start;
    long end;
};

void* calculate_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double local_sum = 0.0;

    for (long i = data->start; i < data->end; i++) {
        double term;
        if (i % 2 == 0)
            term = 1.0 / (2 * i + 1);
        else
            term = -1.0 / (2 * i + 1);
        local_sum += term;
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    int num_threads;
    long n;

    cout << "Enter number of threads: ";
    cin >> num_threads;
    cout << "Enter number of terms (greater than 100000): ";
    cin >> n;

    if (n <= 100000) {
        cout << "Error: Number of terms must be greater than 100000" << endl;
        return 1;
    }

    pthread_t threads[num_threads];
    ThreadData data[num_threads];
    pthread_mutex_init(&mutex, NULL);

    long terms_per_thread = n / num_threads;

    for (int i = 0; i < num_threads; i++) {
        data[i].start = i * terms_per_thread;
        data[i].end = (i == num_threads - 1) ? n : (i + 1) * terms_per_thread;
        pthread_create(&threads[i], NULL, calculate_pi, &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    double pi = 4 * global_sum;
    cout << "Estimated Pi = " << pi << endl;
    return 0;
}
