# Pi estimation with POSIX threads

Sequential and multithreaded implementations of the alternating arctangent series. The multithreaded version splits all terms across workers, merges partial sums under a mutex, and validates input and thread creation.

```sh
g++ -std=c++17 -Wall -Wextra -pedantic sequential.cpp -o pi_sequential
g++ -std=c++17 -Wall -Wextra -pedantic -pthread multithreaded.cpp -o pi_threads
./pi_sequential
./pi_threads
```

The threaded program accepts 1–256 threads and 100001–1000000000 terms. MSYS2 UCRT64 provides the Windows compiler and POSIX threading support used for verification. More threads do not imply faster execution. Academic project; see VERIFICATION.json for actual checks. No credentials or picture/video assets are included.
