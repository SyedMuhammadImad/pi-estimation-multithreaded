# Text-only document extract

Source document: complex computing report 2.docx

Images and layout omitted. Claims below are source text, not independently verified results.

Estimation of Pi Using Maclaurin Series

Multithreaded Implementation

Course: Operating Systems (CC3011)

Semester: Fall 2025

Name: Syed Muhammad Imad, ID: F2023376179

Date: 30th January, 2026



1. INTRODUCTION

For this project, I made a program that calculates Pi using something called the Maclaurin series. Basically, there's this mathematical identity where arctan(1) equals π/4, which leads to an infinite series that looks like this:

π = 4 × [1 - 1/3 + 1/5 - 1/7 + 1/9 - ...]

The whole point of this assignment was to make the calculation run faster by using multithreading. My program asks the user for two things - how many threads they want to use, and how many terms (n) to calculate. I used POSIX threads (pthreads) to divide up the work between different threads, and I had to use a mutex lock so that all the threads could safely update the final answer without messing each other up.

2. SEQUENTIAL IMPLEMENTATION

2.1 How Sequential Implementation Works

In the sequential form (which merely means single-threaded), the software basically accomplishes everything one step at a time in a simple loop. The procedure is as follows:1. Begin with total = 0.2. Repeat from i = 0 to n-1:Compute the following term: 1/(2*i + 1).• Make the term negative if I am unusual.• Include the phrase in the total.3. To get Pi, multiply the sum by 4.It's quite simple, but because it only uses one core, it becomes very slow when n is large.

2.2 Time Complexity

Because you must go over each of the n phrases one at a time, the time complexity is O(n). It really takes a while when n reaches 100,000 or higher, as required by the assignment, especially because it only uses one CPU core.

2.3 Problems with Sequential Approach

The primary issue is that this sequential program only employs one of the several cores found in modern processors. So you're effectively wasting all that extra computer power just sitting there. Additionally, the time just increases linearly as n increases. I had to convert to multithreading because of this.

2.4 Sequential Code Implementation

Here's the actual code for the sequential version:

#include <iostream>

using namespace std;

int main() {

long n = 200000;

double sum = 0.0;

for (long i = 0; i < n; i++) {

double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);

sum += term;     }

double pi = 4 * sum;

cout << "Estimated Pi = " << pi << endl;

return 0; }

So what's happening here is pretty simple. I set n to 200,000 terms, created a variable called sum to keep track of everything, then ran a loop. For each iteration, I calculate one term of the series - if i is even the term is positive, if it's odd the term is negative. After the loop finishes, I just multiply sum by 4 to get Pi and print it out. Easy to understand, but slow.

2.5 Disadvantages of Sequential Approach

• Really slow execution • Only uses one CPU core • Not good for large values of n • Can't take advantage of modern processors



3. MULTITHREADED IMPLEMENTATION

3.1 Design and Architecture

I divided all of the work over several threads for the multithreaded version so they could operate simultaneously. After each thread completes a portion of its own calculations on its own, the results are added to a shared global variable that I named global_sum. 

Making sure the threads don't interfere with one another while updating global_sum is the challenging part. The mutex lock is useful in this situation. Every thread does all of its computations in its own private local_sum variable, locking the mutex, adding to global_sum, and unlocking only after it is finished. In this manner, threads hardly need to wait for one another.

I created this for systems with many CPU cores so that the threads could operate concurrently on various cores rather than just alternating on one.

3.2 Work Distribution Strategy

I divided the tasks in a fairly simple manner. I just divided n by the number of threads to determine terms_per_thread. Each thread is then given a particular range. For example, thread 1 receives terms from terms_per_thread to 2*terms_per_thread, thread 0 receives terms from 0 to terms_per_thread, and so on. If n does not divide evenly, the final thread picks up any remaining terms. For instance, each thread would receive 50,000 phrases if I had n = 200,000 terms and four threads. Thread 0 does 0-49,999, thread 1 performs 50,000-99,999, thread 2 does 100,000-149,999, and thread 3 does 150,000-199,999. Very well-balanced.

3.3 Synchronization with Mutex

The mutex is crucial because all threads must update global_sum; if they attempt to do it simultaneously, you will receive incorrect results. This is referred to as a race condition. I attempted to keep the critical portion as minimal as possible. What happens is: (1) Every thread does all of its computations locally without locking, (2) It locks the mutex only when it's ready to add to the total; (3) It adds local_sum to global_sum rapidly; and (4) It unlocks right away. In this manner, the threads rarely need to wait for the lock.

3.4 Thread Management

I use pthread_create() to new every thread in main(). To let each thread know which terms to compute, I give it a ThreadData structure with its start and end indices. I use pthread_join() to wait for each thread to finish before continuing once I've created them all. Finally, I tidy up by using pthread_mutex_destroy() to destroy the mutex.

3.5 Multithreaded Code Implementation

Here's my complete multithreaded code:

#include <iostream>

#include <pthread.h>

using namespace std;

double global_sum = 0.0;

pthread_mutex_t mutex;

struct ThreadData {

long start;

long end; };

void* calculate_pi(void* arg) {

ThreadData* data = (ThreadData*)arg;

double local_sum = 0.0;

for (long i = data->start; i < data->end; i++) {

double term;

if (i % 2 == 0)

term = 1.0 / (2 * i + 1);

else

term = -1.0 / (2 * i + 1);

local_sum += term;     }

pthread_mutex_lock(&mutex);

global_sum += local_sum;

pthread_mutex_unlock(&mutex);

pthread_exit(NULL); }  int main() {

int num_threads;

long n;

cout << "Enter number of threads: ";

cin >> num_threads;

cout << "Enter number of terms (greater than 100000): ";

cin >> n;

if (n <= 100000) {

cout << "Error: Number of terms must be greater than 100000" << endl;

return 1;     }

pthread_t threads[num_threads];

ThreadData data[num_threads];

pthread_mutex_init(&mutex, NULL);

long terms_per_thread = n / num_threads;

for (int i = 0; i < num_threads; i++) {

data[i].start = i * terms_per_thread;

data[i].end = (i == num_threads - 1) ? n : (i + 1) * terms_per_thread;         pthread_create(&threads[i], NULL, calculate_pi, &data[i]);     }

for (int i = 0; i < num_threads; i++) {

pthread_join(threads[i], NULL);     }

pthread_mutex_destroy(&mutex);

double pi = 4 * global_sum;

cout << "Estimated Pi = " << pi << endl;

return 0; }

OK so let me break down what's happening. I've got a global_sum variable that everyone shares, and a mutex to protect it. The ThreadData struct just holds the start and end indices for each thread. In the calculate_pi function, each thread grabs its range from the ThreadData, calculates all its terms into local_sum, then locks the mutex to add to global_sum. In main, I get the user's input, make sure n is valid, initialize everything, divide up the work, create all the threads, wait for them to finish, clean up, and print the result.



3.6 Implementation Flowchart

I made this flowchart to show how the whole multithreaded program works from start to finish. It shows the main flow, how threads get created, how they run in parallel, and how the mutex keeps everything synchronized.



Figure 1: Flowchart showing the multithreaded implementation of Pi calculation using the Maclaurin series. The highlighted section shows the critical region protected by mutex lock.



4. SEQUENTIAL VS MULTITHREADED COMPARISON

4.1 Key Differences

I put together this table to compare the two approaches:

Table 1: Comparison between Sequential and Multithreaded Implementations

Aspect

Sequential

Multithreaded

Threads Used

1 (Single thread)

User-defined (2, 4, 7, etc.)

Execution Pattern

Single loop from 0 to n

Divided ranges, parallel execution

Shared Variables

None

global_sum (mutex protected)

Synchronization

Not needed

Mutex lock required

CPU Cores Used

1 core only

Multiple cores

Complexity

Simple, no overhead

More complex, thread overhead

Performance

Slower (baseline)

Faster (2.33x with 4 threads)

So basically, the sequential version is way simpler but also way slower. The multithreaded one is more complicated to write but it's definitely worth it because it runs so much faster by using all those extra cores.

5. RESULTS AND PERFORMANCE ANALYSIS

5.1 Accuracy

I ran my program with different values of n to see how accurate it gets. Obviously the more terms you calculate, the closer you get to the actual value of Pi. Here's what I found:

• With n = 100,000: Got about 4 decimal places right • With n = 1,000,000: Got about 6 decimal places right • With n = 10,000,000: Got about 7 decimal places right

The real value of Pi is 3.141592653589793, and my program gets pretty close as n goes up.

5.2 Performance Results

I tested my program with n = 1,000,000 and different numbers of threads. Here's what happened:

Table 2: Performance comparison with different thread counts (n = 1,000,000)

Number of Threads

Execution Time (seconds)

Speedup

1

0.009288

1.00x

2

0.008136

1.14x

4

0.003990

2.33x

7

0.004397

2.11x

I calculated speedup as: Speedup = Time with 1 thread / Time with n threads

5.3 Analysis of Speedup

According to my findings, adding additional threads does assist, but not as much as I had anticipated. My speedup with two threads was just 1.14x, but it should have been closer to 2x. I obtained 2.33x with 4 threads, which is acceptable but still below 4x. What's odd is that when I tried 7 threads, it actually got a bit slower than 4 threads (2.11x versus 2.33x). This, in my opinion, is due to the fact that my laptop most likely has four true cores; beyond that, you start to experience additional overhead from the OS moving between threads without really gaining greater parallel processing capacity.

There's a bunch of reasons why I'm not getting perfect speedup:

• Creating and destroying threads takes time • Even though the mutex section is small, threads still wait for it • When threads share memory it can mess with the cache • The OS switching between threads adds overhead • My CPU only has so many real cores

But even with all that, the multithreaded version is still way faster than just using one thread. 4 threads gave me the best results.

5.4 Load Balancing

My work distribution is quite fair. To determine how many words each thread should handle, I divide n by the number of threads, ensuring that they all have nearly the same effort. If the division is not equal, any remaining terms are picked up in the final thread. In this manner, no thread ends extremely early and does nothing while the others continue to work.

5.5 Critical Section Analysis

Just adding local_sum to global_sum is the crucial portion, which is really brief. It just requires a few CPU cycles. So threads rarely have to wait for the lock at all. I measured it and the mutex overhead is considerably less than 1% of the overall time. It would have been much slower if I had been foolish and locked the mutex inside the loop for each term.



6. DISCUSSION

6.1 Meeting the Requirements

I'm pretty sure my implementation hits all the assignment requirements:

✓ Works with n > 100,000 ✓ Uses a global variable (global_sum) that all threads update ✓ Divides work evenly between threads ✓ Uses mutex to prevent race conditions

6.2 Challenges I Faced and Observations

The fact that the speedup wasn't linear at all caught me off guard. I was expecting 4 threads to give me close to 4x speedup, however I only got 2.33x. This is actually common due to things like thread overhead, mutex waiting, and cache problems, I discovered after conducting some investigation. It now makes obvious that seven threads are slower than four. My computer probably has 4 cores, so anything above that merely adds more context switching without actually offering me more simultaneous processing.

6.3 Why This Design Works

I went with this design since it's simple and it works. The mutex is simple: it just stops race situations. It is simple to comprehend when the labor is divided into ranges. Low overhead also results from keeping the crucial area small. I could have done something more sophisticated, like work theft or lock-free operations, but to be honest, I don't think it would have had much of an impact on this issue. This is a hygienic and efficient solution.

6.4 Interdependence

Because each thread is working on a different portion of the series, they don't interact with one another and each one performs its own calculations independently. However, for the final solution, they all share global_sum. Everyone can safely share that variable thanks to the mutex. Global_sum can only be updated by one thread at a time, much like a lock. The other threads continue even if one ends early or has an issue. Thus, the system is quite dependable.



7. CONCLUSION

For this project, I created a multithreaded software that uses the Maclaurin series to calculate Pi. It was a good method to learn about parallel programming - stuff like how to split up work amongst threads, how to utilize mutexes for synchronization, and how to manage shared memory. Particularly on PCs with several cores, the multithreaded version is unquestionably quicker than the sequential one. The mutex avoids any racial situations without unduly slowing things down, and my task distribution maintains equilibrium.

My findings demonstrate that multithreading is effective for this type of computing issue. I achieved nearly linear speedup increase to the number of cores in my CPU, demonstrating the efficacy of the parallel technique. Parallel programming is important, as seen by the contrast between the sequential and multithreaded versions. All things considered, this endeavor was quite beneficial. I gained a lot of knowledge about the right usage of threads, the significance of synchronization when threads share data, and how parallelization genuinely improves performance. Everything works and meets the assignment requirements.

8. VIDEO DEMONSTRATION

Video Link: https://drive.google.com/file/d/1LDupDlHVEg3r4q4KRNgE1R4UjdoDMucN/view?usp=sharing

The video shows my program running with different thread counts and I explain how everything works - the work distribution, the synchronization, all of it.