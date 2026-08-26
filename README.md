# Pi Estimation — Multithreaded (Maclaurin Series)

Estimates the value of Pi using the Maclaurin series for `arctan(1) = π/4`,
implemented two ways for direct comparison: a single-threaded baseline and
a multithreaded version using POSIX threads (pthreads) with mutex-protected
shared state.

```
π = 4 × [1 - 1/3 + 1/5 - 1/7 + 1/9 - ...]
```

## Why This Exists

This project explores how much parallelism actually helps on a
compute-bound, embarrassingly-parallel problem like series summation —
and just as importantly, where the *limits* of that speedup are on
consumer hardware. The results below aren't idealized: they include the
overhead, the diminishing returns past core count, and the reasoning
behind both.

## How It Works

### Sequential (`src/sequential.cpp`)

A single loop walks through `n` terms of the series one at a time,
alternating the sign of each term and accumulating a running sum. Simple,
but limited to one CPU core — time complexity is `O(n)`.

### Multithreaded (`src/multithreaded.cpp`)

The `n` terms are split evenly across a user-specified number of threads.
Each thread:

1. Computes its assigned range of terms into a private `local_sum` (no locking needed here — it's thread-local).
2. Locks a mutex just long enough to add `local_sum` into the shared `global_sum`.
3. Unlocks and exits.

Keeping the critical section (the mutex-protected addition) as small as
possible means threads rarely have to wait on each other — the lock
overhead measured out to well under 1% of total runtime.

If `n` doesn't divide evenly across threads, the last thread absorbs the
remainder, so no thread finishes early while others are still working.

## Results

### Accuracy

| Terms (n) | Correct Decimal Places |
|---|---|
| 100,000 | ~4 |
| 1,000,000 | ~6 |
| 10,000,000 | ~7 |

(Reference value: `π = 3.141592653589793`)

### Performance (n = 1,000,000)

| Threads | Execution Time (s) | Speedup |
|---|---|---|
| 1 | 0.009288 | 1.00x |
| 2 | 0.008136 | 1.14x |
| 4 | 0.003990 | 2.33x |
| 7 | 0.004397 | 2.11x |

*Speedup = time with 1 thread ÷ time with n threads*

**Key observation:** speedup scales well up to 4 threads, then *drops*
at 7 threads. This points to the test machine having 4 physical cores —
beyond that, additional threads add OS context-switching overhead without
gaining real parallelism. Perfect linear speedup wasn't expected or
achieved; realistic overhead sources include thread creation/teardown
cost, brief mutex contention, and cache effects from shared memory access.

## Project Structure

```
pi-estimation-multithreaded/
├── README.md
├── LICENSE
├── src/
│   ├── sequential.cpp      # single-threaded baseline
│   └── multithreaded.cpp   # pthreads + mutex implementation
└── docs/
    └── problem-statement.md # original assignment requirements
```

## How to Build & Run

**Sequential:**

```bash
g++ src/sequential.cpp -o pi_sequential
./pi_sequential
```

**Multithreaded** (requires linking pthreads):

```bash
g++ src/multithreaded.cpp -o pi_mutex -lpthread
./pi_mutex
```

The multithreaded version will prompt for the number of threads and the
number of terms `n` (must be greater than 100,000).

## Tech

- C++
- POSIX threads (`pthread_create`, `pthread_join`, `pthread_mutex_t`)

## License

MIT — see [LICENSE](./LICENSE).
