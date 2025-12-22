# LRU Cache Implementation using HashMap and Queue

Modern operating systems and databases frequently use caching mechanisms to improve performance. One popular strategy is the Least Recently Used (LRU) cache replacement policy, where the cache evicts the item that has not been accessed for the longest time.

Your task is to design and implement an LRU Cache using the following data structures:

- **HashMap** (for O(1) access to nodes)
- **Queue** implemented using your own linked list (to maintain order of usage — most recently used at one end, least recently used at the other)

You must build the queue using struct-based nodes (no library queues).

## Objective

Create an LRU Cache that supports the following operations in O(1) time:

1.  `get(key)` → returns value if key exists in cache; otherwise returns null
2.  `put(key, value)` → inserts/updates the value; if the cache exceeds capacity, evict the least recently used item

## Requirements

### 1. Data Structures

**a. HashMap**

- Maps key to corresponding queue node
- Enables O(1) lookup of a node in the queue

**b. Queue (Doubly Linked List)**
Each node must store:

- Integer `key`
- String `data`

The queue maintains usage:

- **Front (Head)** → Most Recently Used (MRU)
- **Rear (Tail)** → Least Recently Used (LRU)

### 2. Tasks

Implement the following:

- `createCache(int capacity)`: Initialize the LRU cache with positive size `capacity`.
- `string get(int key)`: Return the value of the key if the key exists, otherwise return `null`.
- `void put(int key, string value)`: Update the value of the key if the key exists. Otherwise, add the key-value pair to the cache. If the number of keys exceeds the capacity from this operation, evict the least recently used key.

The functions `get` and `put` must each run in O(1) average time complexity.

### 3. Constraints

- $1 \le capacity \le 1000$
- Keys are integers and values are strings
- All operations must run in O(1)

### 4. Input Format

```text
createCache <size>
put <key> <data>
get <key>
```
