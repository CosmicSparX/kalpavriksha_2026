# Interprocess Communication & Socket Programming Assignment

## Assignment Overview

This assignment focuses on implementing various Interprocess Communication (IPC) mechanisms in C and developing a socket-based ATM simulation system. The goal is to strengthen your understanding of IPC techniques, multithreading, file handling, and client–server communication.

---

## Part 1: Interprocess Communication (IPC) Programs

You must implement the following IPC mechanisms in C. Each must be a separate program with clear input/output behavior and comments.

**Scenario:**
There will be two processes.

1. The **first process** will read the integer array data and display it before sorting.
2. The **second process** will sort the data.
3. The **first process** will then display the data after sorting.

**Mechanisms to Implement:**

1. File-Based IPC Mechanism
2. Message Queue IPC Mechanism
3. Pipe IPC Mechanism
4. Shared Memory IPC Mechanism

---

## Part 2: Socket-Based ATM Transaction Simulation

You must implement a TCP client-server system using socket IPC.

**Supported Operations:**

1. Withdraw Amount
2. Deposit Amount
3. Display Current Balance

### Server Requirements

- Use **TCP sockets**.
- Maintain a shared data file (e.g., `account.txt`) to store current balance and user data.
- **For every client request:**
  - Create a dedicated thread using `pthreads`.
  - Process the specific transaction (withdraw, deposit, display).
  - Update/read the shared balance safely.
  - Implement synchronization using **mutex** to protect file access.
  - Return the result back to the client.

### Client Requirements

- Connect to the server using a TCP socket.
- Display menu:
  1.  Withdraw
  2.  Deposit
  3.  Display Balance
  4.  Exit
- Send request to server.
- Receive transaction result and print it.

### Transaction Logic

- **Withdrawal** should fail if `amount > available balance`.
- **Deposit** adds the amount to the balance.
- All balance updates must be written to the file.
- Server must handle **concurrent clients** requests using threads.

---

## Expected Folder Structure

Write the `.c` program file in the respective folder.

```text
IPCAssignment/
├── fileIPCMechanism/
├── pipeIPCMechanism/
├── messageQueueIPCMechanism/
├── sharedMemoryIPCMechanism/
├── socketIPCMechanism/
│   ├── client/
│   ├── server/
│   ├── resource/
│   └── accountDB.txt
```
