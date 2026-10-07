# Text-only document extract

Source document: os final project (2) mohib.docx

Images and layout omitted. Claims below are source text, not independently verified results.

Title:

Estimation of Pi Using Parallel Programming in Operating Systems



reference: Operating Systems


University: University of Management and Technology, Lahore

Department: Artificial Intelligence Student Name: Muhammad Mohib Bhatti Roll No: F2023376137



ABSTRACT



This project focuses on estimating the value of π (Pi) using the Maclaurin series for arctangent.

The implementation is carried out using both sequential and multithreaded approaches in C++. The multithreaded solution utilizes POSIX threads and mutex locks to ensure proper synchronization while multiple threads update a shared global variable. The objective of this project is to analyze performance improvement, workload distribution, and synchronization challenges in parallel computing. The results demonstrate that multithreaded execution significantly improves performance compared to the single-threaded approach.



PROBLEM ANALYSIS



The objective of this project is to compute the value of π using the Maclaurin series:

1143000178336



This series converges slowly, therefore a large number of terms (n > 100,000) is required to achieve reasonable accuracy. Computing such a large number of terms sequentially is time-consuming. To improve performance and CPU utilization, parallel programming techniques are used.

Load must be evenly distributed among threads

Synchronization must be ensured using mutex locks



SEQUENTIAL (SINGLE FLOW) IMPLEMENTATION



Explanation



In the sequential method, only one thread performs all the calculations. The program runs a loop from the first term to the last term and adds each value to a variable. This method is simple but slow because it uses only one CPU core.







Sequential Code Used



#include <iostream> using namespace std; int main() {

long n = 200000; double sum = 0.0;



for (long i = 0; i < n; i++) {

double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 *

i + 1);

sum += term;

}



double pi = 4 * sum;

cout << "Estimated Pi = " << pi << endl;

return 0;



Constraints:



Number of terms (n) must be greater than 100,000

The final value of π must be stored in a global variable

Multiple threads must update the global result safely

}



Disadvantages



Slow execution

Uses only one CPU core

Not efficient for large values of n



Sequential Code Explanation



In the sequential approach, the program uses only one thread to calculate the value of Pi. First, a variable n is set to 200000, which represents the number of terms used in the calculation. A variable sum is used to store the total value of the series.

MULTITHREADED CODE USED



#include <iostream>#include <pthread.h> using namespace std;

double global_sum = 0.0;pthread_mutex_t mutex;

struct ThreadData { long start;

long end;



A for loop runs from 0 to n. In each loop, one term of the series is calculated. If the loop index is even, the term is positive, and if it is odd, the term is negative. Each term is added to the sum variable.



After the loop finishes, the value of Pi is calculated by multiplying the sum by 4. Finally, the estimated value of Pi is displayed on the screen.



This method is easy to understand, but it is slow because all calculations are done by only one thread and only one CPU core is used.



MULTITHREADED IMPLEMENTATION



Explanation

};

void* calculate_pi(void* arg) { ThreadData* data = (ThreadData*)arg; double local_sum = 0.0;



for (long i = data->start; i < data->end; i++) { double term;

if (i % 2 == 0)

term = 1.0 / (2 * i + 1); else

term = -1.0 / (2 * i + 1);

local_sum += term;

}



pthread_mutex_lock(&mutex); global_sum += local_sum; pthread_mutex_unlock(&mutex);

pthread_exit(NULL);



In the multithreaded approach, the total number of terms is divided among multiple threads. Each thread calculates a small portion of the series and stores the result in a local variable. After finishing its work, the thread safely adds its result to a global variable.



SYNCHRONIZATION USING MUTEX



All threads share a global variable called global_sum. If multiple threads update it at the same time, incorrect results may occur. To avoid this problem, a mutex lock is used. The mutex ensures that only one thread can update the global variable at a time.

}

int main() {



int num_threads; long n;

cout << "Enter number of threads: "; cin >> num_threads;



cout << "Enter number of terms (greater than 100000): ";

cin >> n;



if (n <= 100000) {

cout << "Error: Number of terms must be greater than 100000" << endl;

return 1;



1143000-5024}



pthread_t threads[num_threads]; ThreadData data[num_threads];

pthread_mutex_init(&mutex, NULL);

long terms_per_thread = n / num_threads; for (int i = 0; i < num_threads; i++) {

data[i].start = i * terms_per_thread; data[i].end = (i == num_threads - 1) ? n : (i

+ 1) * terms_per_thread;



pthread_create(&threads[i], NULL, calculate_pi, &data[i]);

}

for (int i = 0; i < num_threads; i++) { pthread_join(threads[i], NULL);

}

pthread_mutex_destroy(&mutex); double pi = 4 * global_sum;

cout << "Estimated Pi = " << pi << endl;



return 0;

divides the work among multiple threads and uses mutex locks to ensure correct results. This comparison helps in understanding the importance of parallel programming and synchronization in operating systems.



FLOWCHART DESCRIPTION



The flowchart explains how the multithreaded program calculates the value of Pi step by step. It shows the complete flow of the program from start to end and helps in understanding how threads and mutex work together.



}



Multithreaded Code Explanation



In the multithreaded approach, the same Pi calculation is done using multiple threads. This allows the program to run faster by using more than one CPU core.



A global variable called global_sum is used to store the final result. Since this variable is shared by all threads, a mutex is used to protect it and avoid incorrect results.



The program first asks the user to enter the number of threads and the number of terms. The number of terms must be greater than 100000 to ensure better accuracy. The total number of terms is then divided equally among all threads.



Each thread receives a starting and ending index and calculates only its assigned part of the series. The result of each thread is stored in a local variable called local_sum. After finishing its calculation, the thread locks the mutex, adds its local result to the global sum, and then unlocks the mutex.



Once all threads complete their work, the main program waits for them using pthread_join.

After that, the mutex is destroyed, and the final value of Pi is calculated by multiplying the global sum by 4. The estimated value of Pi is then displayed.



This method is faster and more efficient because the workload is shared among multiple threads, and the mutex ensures safe synchronization.



Comparison of Both Approaches



The sequential approach is simple but slow because it uses only one thread. The multithreaded approach is faster because it

The flowchart starts with the Start symbol, which represents the beginning of the program. After that, the program asks the user to enter the number of threads and enter the number of terms (n). These inputs are required to control how much work the program will do and how many threads will be used.



Next, a decision box checks whether the entered value of n is greater than 100,000 or not. If the value is not greater than 100,000, an error message is displayed and the program ends. If the value is valid, the program continues.



After validation, the program initializes the global variable global_sum and initializes the mutex. The mutex is required to safely manage shared data between threads.



The program then creates multiple threads. After creating the threads, the total number of terms is divided equally among all threads so that each thread gets a fair amount of work.



Each thread then starts executing in parallel. Inside each thread, a local sum is calculated for the assigned range of terms. Once a thread finishes its local calculation, it locks the mutex, adds its local result to the global sum, and then unlocks the mutex. This ensures that only one thread updates the global value at a time.



After all threads finish their work, the main program waits for all threads to join. This ensures that no thread is still running before the final result is calculated.



Finally, the program calculates the value of Pi using the formula

Pi = 4 × global_sum, displays the estimated value of Pi, and then reaches the End of the program.



This flowchart clearly shows the use of multithreading, synchronization using mutex, and the complete execution flow of the program.







1965516169411DIAGRAM :



INTERDEPENDENCE



In this program, each thread works on its own part of the calculation. Every thread calculates different terms of the Pi series, so they can work independently without disturbing each other.

However, all threads share one important variable called global_sum, which stores the final result.



Because many threads want to update this shared variable, a mutex is used. The mutex acts like a key that allows only one thread to update the global value at a time. This creates safe communication between threads. Even if one thread finishes earlier or faces a problem, the other threads can still continue their work. This makes the system more reliable and prevents total failure.

Video Link :



https://drive.google.com/drive/folders/ 1oGqSvu0YD8ltK6sHMzqPt3rKa71qIzzR?usp=drive_link

(also shared at lms comment)











PERFORMANCE ANALYSIS



The multithreaded program runs much faster than the sequential program. This is because multiple threads run at the same time and use different CPU cores. Instead of one thread doing all the work, the workload is divided equally among all threads.



This balanced work distribution reduces execution time and improves overall speed. The mutex is used only when updating the shared variable, so it does not slow down the program much. As a result, the program remains both fast and accurate.

1138237299283



CONCLUSION



This project clearly shows the power of multithreading in operating systems. By using multiple threads, the program completes faster and uses system resources more efficiently. The use of mutex locks ensures proper synchronization and prevents incorrect results.



Overall, the multithreaded approach is much better than the single-thread approach. It improves performance, ensures correctness, and helps us understand important operating system concepts like threads, synchronization, and shared memory.
