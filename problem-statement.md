[← Back to overview](../README.md)

# Problem Statement

**Design and implementation:** Estimate Pi using the Maclaurin series for arctan(x).

Since `arctan(1) = π/4`, Pi can be computed as:

```
π = 4 × [1 - 1/3 + 1/5 - 1/7 + 1/9 - ...]
```

The program is invoked as:

```
pi_mutex <number of threads> <n>
```

Where `n` is the number of terms of the Maclaurin series to use.

## Constraints

**Range of conflicting requirements**

1. `n` should be greater than 100,000.
2. The final result must be a global value, with all submodules (threads) updating it accordingly.
3. The program should correctly handle a number of terms not evenly divisible by the number of threads, while still ensuring load balancing.

**Depth of knowledge required**

The solution should account for workload balance, data dependencies, and communication overhead, with the implementation fine-tuned for efficient resource utilization and minimal idle time.

**Interdependence**

Sub-modules (threads) work independently on their own term ranges and only interact when merging results into the shared total, via synchronization that avoids race conditions.

---

[← Back to overview](../README.md)
