// Pi Estimation — Sequential Implementation
// Computes Pi using the Maclaurin series for arctan(1) = Pi/4:
//   Pi = 4 * [1 - 1/3 + 1/5 - 1/7 + 1/9 - ...]
// Single-threaded baseline for comparison against the multithreaded version.

#include <iostream>
using namespace std;

int main() {
    long n = 200000;
    double sum = 0.0;

    for (long i = 0; i < n; i++) {
        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        sum += term;
    }

    double pi = 4 * sum;
    cout << "Estimated Pi = " << pi << endl;
    return 0;
}
