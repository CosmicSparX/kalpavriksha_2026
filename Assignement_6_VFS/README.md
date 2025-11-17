# Virtual File System (VFS) in C

This project is a C-based simulation of an in-memory Virtual File System (VFS). It demonstrates core file system concepts like directory management, file operations, and disk block allocation using circular and doubly linked lists.

## Core Design

The VFS simulates a disk environment entirely in memory.

### 1. Virtual Disk & Free Block Management

- **Virtual Disk:** Represented as a 2D byte array (`virtualDisk[NUM_BLOCKS][BLOCK_SIZE]`).
- **Free Blocks:** All unallocated blocks are managed using a **doubly linked list** of `FreeBlock` nodes.
  - **Allocation:** When a file needs space, blocks are removed from the `head` of the free list.
  - **Deallocation:** When a file is deleted, its blocks are returned to the `tail` of the free list.

### 2. Directory & File Structure (FileNode)

A single `FileNode` structure is used to represent both files and directories, distinguished by a type flag.

- **Directories:**
  - Maintain a **circular linked list** of their child entries (other `FileNode`s).
  - Do not consume any blocks on the virtual disk; they exist only as in-memory linked lists.
- **Files:**
  - Use an array (`blockPointers[]`) to store the indices of the disk blocks that hold their data.
  - Consume disk space based on their content size.

### 3. Current Working Directory (CWD)

A global `cwd` pointer tracks the user's current location in the file system hierarchy.

- On startup, `cwd` points to the root (`/`) directory.
- `mkdir` and `create` add new nodes to the `cwd`'s child list, but do not change the `cwd` itself.
- The `cwd` pointer is only updated by the `cd` command.

---

## 🚀 Supported Operations

- **`mkdir <name>`**: Creates a new directory under the current working directory.
- **`create <name>`**: Creates a new, empty file under the current working directory.
- **`write <file> "<data>"`**: Writes string data to a file. Allocates blocks from the free list as needed.
- **`read <file>`**: Reads and prints the content of a file by traversing its allocated blocks.
- **`delete <file>`**: Deletes a file, removes it from its parent's list, and returns its blocks to the free list.
- **`rmdir <name>`**: Removes an _empty_ directory. Fails if the directory contains any entries.
- **`ls`**: Lists all files and subdirectories within the current working directory.
- **`cd <path>`**: Changes the current working directory. Supports `..` for the parent directory.
- **`pwd`**: Displays the full path of the current working directory by traversing parent pointers up to the root.
- **`df`**: Displays disk usage statistics (total blocks, used blocks, and free blocks).
- **`exit`**: Releases all allocated memory (for nodes and the virtual disk) and terminates the program.

---

## 🛠️ How to Build and Run

1.  **Clone the repository:**
    ```bash
    git clone <your-repo-url>
    cd <your-repo-directory>
    ```
2.  **Compile the source code:**
    ```bash
    gcc -o vfs vfs.c
    ```
3.  **Run the executable:**
    ```bash
    ./vfs
    ```

---

## 🖥️ Console Usage Example

```bash
🏁 System Startup
$ ./vfs
Compact VFS - ready. Type 'exit' to quit.
/ >

📁 mkdir (Create Directory)
/ > mkdir docs
Directory 'docs' created successfully.
/ > ls
docs/

📂 cd (Change Directory)
/ > cd docs
Moved to /docs
docs >

📄 create (Create File)
docs > create notes.txt
File 'notes.txt' created successfully.
docs > ls
notes.txt

✍️ write (Write to File)
docs > write notes.txt "This is my first note."
Data written successfully (size=24 bytes).
docs > df
Total Blocks: 1024
Used Blocks: 1
Free Blocks: 1023
Disk Usage: 0.10%

📖 read (Read File)
docs > read notes.txt
This is my first note.

🗑️ delete (Delete File)
docs > delete notes.txt
File deleted successfully.
docs > ls
(empty)
docs > df
Total Blocks: 1024
Used Blocks: 0
Free Blocks: 1024
Disk Usage: 0.00%

📂 rmdir (Remove Directory)
docs > cd ..
Moved to /
/ > rmdir docs
Directory removed successfully.
/ > ls
(empty)

🧭 pwd (Show Current Path)
/ > mkdir projects
/ > cd projects
Moved to /projects
projects > pwd
/projects

📊 df (Show Disk Usage)
/ > df
Total Blocks: 1024
Used Blocks: 0
Free Blocks: 1024
Disk Usage: 0.00%

🚪 exit (Terminate Program)
/ > exit
Memory released. Exiting program...
```
