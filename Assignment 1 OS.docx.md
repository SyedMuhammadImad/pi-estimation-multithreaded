# Text-only document extract

Source document: Assignment 1 OS.docx

Images and layout omitted. Claims below are source text, not independently verified results.

Assignment 1: Analysis and Engineering of Process Synchronization Protocols      (CLO 1,2,3)

Objective

To conduct a comprehensive technical study of software, hardware, and OS-level solutions to the Critical Section (CS) Problem. Students will evaluate the evolution of synchronization logic and design a custom protocol to understand the inherent trade-offs in concurrent programming.



Task 1: Comprehensive Comparative Analysis

Discuss and evaluate the full spectrum of existing synchronization mechanisms. Your analysis must cover:

Software-Based Solutions: Peterson’s Algorithm, Dekker’s Algorithm, and Lamport’s Bakery Algorithm.

Hardware-Level Primitives: Disabling Interrupts, Atomic Instructions (Test-and-Set, Compare-and-Swap).

High-Level OS/Language Abstractions: Mutex Locks, Counting and Binary Semaphores, and Monitors.



Task 2: Evaluation Framework

For every mechanism listed above, provide a rigorous analysis based on the following four pillars of synchronization:

Mutual Exclusion

Progress

Bounded Waiting

Architectural Neutrality



Task 3: Protocol Design & Implementation

Propose an original (or hybrid) synchronization solution designed to solve the CS problem.

Logic & Pseudocode: Provide clear pseudocode for your implementation.

Primary Criteria Proof: Demonstrate mathematically or logically how your solution ensures all the necessary criteria.



Task 4: Critical Limitation Review

No software level solution is perfect. Analyze your proposed protocol and identify its specific weaknesses.

Failure Analysis: Explain why your solution may fail to satisfy Bounded Waiting or Architectural Neutrality (if it does).

Technical Root Cause: Discuss the specific technical reasons (e.g., race conditions, memory reordering, or starvation) that lead to these limitations.

Submission Requirements

Format: Entirely handwritten on paper.

Final File: Scanned and submitted as a single PDF document.

Legibility: Ensure all code blocks and analysis tables are clearly readable. 

DO NOT COPY AND PASTE. PROPOSE YOUR OWN SOLUTION.

