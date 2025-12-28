# FCFS - Based OS Process Scheduling Simulator

## 1. Project Title

**Design and Implementation of a FCFS-Based Operating System Process Scheduler using Queues and HashMap in C**

## 2. Objective

The goal of this project is to simulate a simplified CPU process scheduler similar to that of a modern operating system. The simulator will maintain processes using a **Process Control Block (PCB)** stored in a HashMap and manage process execution through **Ready**, **Waiting**, and **Terminated** queues.

The scheduler must support:

- FCFS based scheduling
- I/O blocking and I/O completion
- Process creation, termination, and inspection at runtime
- Execution timeline and logging
- Simulation of CPU time and I/O time

This project demonstrates concepts of OS scheduling, process state transitions, data structures, and CPU scheduling mechanics.

## 3. Key Concepts

- **CPU Burst**
  - Time spent actively executing on the CPU.
  - This includes: arithmetic, logic operations, memory accesses (but not I/O and system calls).
  - This is what CPU schedulers consider as actual computation.

- **I/O Burst**
  - Time waiting for I/O operations: disk read/write, network I/O, keyboard/mouse input, OS system call waiting.
  - The process does not use CPU during this period.

## 4. System Architecture Overview

The simulator consists of:

1.  **PCB Hashmap**
    - **Key:** PID
    - **Value:** PCB structure containing process name, burst time, I/O timing, state, etc.

2.  **Queues (Linked List based)**
    - **Ready Queue:** runnable processes in the Ready state
    - **Waiting Queue:** processes performing I/O
    - **Terminated Queue:** processes that have finished execution

3.  **Scheduler**
    - Picks the process to be executed based on FCFS
    - Simulates CPU burst and I/O events

4.  **System Clock**
    - Drives CPU burst progression
    - Updates waiting (I/O) times
    - Triggers scheduling decisions

## 5. Input Specification

**Input Process details and kill events:**

```text
<process_name> <pid> <burst_time> <io_start_time> <io_duration>
KILL <PID> <kill_time>
```
