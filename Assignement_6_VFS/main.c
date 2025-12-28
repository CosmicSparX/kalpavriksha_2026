#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define MIN_BLOCKS 1
#define MAX_NUM_BLOCKS 5000
#define MAX_FILENAME_LEN 50
#define MAX_WRITE_BUFFER 2048
#define MAX_BLOCKS_PER_FILE 100
#define MAX_INPUT_LEN 128

char **virtualDisk;

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct File {
    char name[MAX_FILENAME_LEN];
    int isDirectory;
    struct File *parent;
    struct File *next;
    struct File *prev;
    struct File *children;
    int blockCount;
    int blocks[MAX_BLOCKS_PER_FILE];
    int size;
} File;

File *root = NULL;
File *cwd = NULL;
FreeBlock *freeBlockHead = NULL;
int usedBlocks = 0;

void initDisk();
void initFreeBlocks();
void initFileSystem();
void readWriteBuffer(char *dataInput);
File* findChild(char *name);
int getFreeBlock();
void releaseBlock(int index);

void mkdirCommand(char *name);
void rmdirCommand(char *name);
void lsCommand();
void cdCommand(char *name);
void printPath(File *dir);
void createCommand(char *name);
void writeCommand(char *name, char *data);
void readCommand(char *name);
void deleteCommand(char *name);
void dfCommand();

void freeAll(File *dir);
void cleanup();

void processCommand(char *cmd, char *arg1, char *arg2);
void readUserInput(char *cmd, char *arg1, char *arg2);


int main() {
    initFileSystem();
    char cmd[MAX_INPUT_LEN], nameArg[MAX_INPUT_LEN], writeBuf[MAX_WRITE_BUFFER];
    while (1) {
        printf("%s > ", (cwd == root) ? "/" : cwd->name);
        readUserInput(cmd, nameArg, writeBuf);
        if (strcmp(cmd, "exit") == 0) {
            cleanup();
            printf("Memory released. Exiting program...\n");
            break;
        }
        processCommand(cmd, nameArg, writeBuf);
    }
    return 0;
}


void initDisk() {
    virtualDisk = malloc(MAX_NUM_BLOCKS * sizeof(char *));
    for (int i = 0; i < MAX_NUM_BLOCKS; i++)
        virtualDisk[i] = calloc(BLOCK_SIZE, sizeof(char));
}

void initFreeBlocks() {
    FreeBlock *prev = NULL;
    for (int i = 0; i < MAX_NUM_BLOCKS; i++) {
        FreeBlock *node = malloc(sizeof(FreeBlock));
        node->index = i;
        node->next = NULL;
        node->prev = prev;
        if (prev) prev->next = node;
        else freeBlockHead = node;
        prev = node;
    }
}

void initFileSystem() {
    initDisk();
    initFreeBlocks();
    root = malloc(sizeof(File));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = root->prev = root;
    root->children = NULL;
    root->blockCount = 0;
    root->size = 0;
    cwd = root;
    printf("Virtual File System initialized successfully.\n");
}

void readWriteBuffer(char *dataInput) {
    char *ptr = dataInput;
    int ch = getchar();
    while (ch == ' ' || ch == '\t') ch = getchar();
    if (ch == '\n' || ch == EOF) { *dataInput = '\0'; return; }
    if (ch == '"') {
        while ((ch = getchar()) != '"' && ch != EOF && (ptr - dataInput) < MAX_WRITE_BUFFER - 1)
            *ptr++ = (char)ch;
    } else {
        *ptr++ = (char)ch;
        while ((ch = getchar()) != '\n' && ch != EOF && (ptr - dataInput) < MAX_WRITE_BUFFER - 1)
            *ptr++ = (char)ch;
    }
    *ptr = '\0';
}

File* findChild(char *name) {
    if (!cwd->children) return NULL;
    File *cur = cwd->children;
    do {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    } while (cur != cwd->children);
    return NULL;
}

int getFreeBlock() {
    if (!freeBlockHead) return -1;
    int idx = freeBlockHead->index;
    FreeBlock *tmp = freeBlockHead;
    freeBlockHead = freeBlockHead->next;
    if (freeBlockHead) freeBlockHead->prev = NULL;
    free(tmp);
    usedBlocks++;
    return idx;
}

void releaseBlock(int index) {
    FreeBlock *node = malloc(sizeof(FreeBlock));
    node->index = index;
    node->next = freeBlockHead;
    node->prev = NULL;
    if (freeBlockHead) freeBlockHead->prev = node;
    freeBlockHead = node;
    usedBlocks--;
}

void printPath(File *dir) {
    if (!dir) return;
    if (dir->parent) {
        printPath(dir->parent);
        if (strcmp(dir->name, "/") != 0)
            printf("/%s", dir->name);
    } else printf("/");
}

void mkdirCommand(char *name) {
    if (strlen(name) > MAX_FILENAME_LEN) { printf("Error: Name too long.\n"); return; }
    if (findChild(name)) { printf("Directory already exists.\n"); return; }
    File *newDir = malloc(sizeof(File));
    strcpy(newDir->name, name);
    newDir->isDirectory = 1;
    newDir->parent = cwd;
    newDir->children = NULL;
    newDir->blockCount = 0;
    newDir->size = 0;
    if (!cwd->children) {
        cwd->children = newDir;
        newDir->next = newDir->prev = newDir;
    } else {
        File *first = cwd->children;
        File *last = first->prev;
        newDir->next = first;
        newDir->prev = last;
        last->next = newDir;
        first->prev = newDir;
    }
    printf("Directory '%s' created.\n", name);
}

void rmdirCommand(char *name) {
    File *target = findChild(name);
    if (!target) { printf("Directory not found.\n"); return; }
    if (!target->isDirectory) { printf("'%s' is a file.\n", name); return; }
    if (target->children) { printf("Directory not empty.\n"); return; }
    if (target == cwd->children && target->next == target) cwd->children = NULL;
    else {
        if (target == cwd->children) cwd->children = target->next;
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }
    free(target);
    printf("Directory removed.\n");
}

void lsCommand() {
    if (!cwd->children) { printf("(empty)\n"); return; }
    File *cur = cwd->children;
    do {
        printf("%s%s  ", cur->name, cur->isDirectory ? "/" : "");
        cur = cur->next;
    } while (cur != cwd->children);
    printf("\n");
}

void cdCommand(char *name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent) cwd = cwd->parent;
        else printf("Already at root.\n");
        return;
    }
    File *target = findChild(name);
    if (!target || !target->isDirectory) { printf("Directory not found.\n"); return; }
    cwd = target;
}

void createCommand(char *name) {
    if (strlen(name) > MAX_FILENAME_LEN) { printf("Error: Name too long.\n"); return; }
    if (findChild(name)) { printf("File already exists.\n"); return; }
    File *newFile = malloc(sizeof(File));
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->blockCount = 0;
    newFile->size = 0;
    newFile->children = NULL;
    if (!cwd->children) {
        cwd->children = newFile;
        newFile->next = newFile->prev = newFile;
    } else {
        File *first = cwd->children;
        File *last = first->prev;
        newFile->next = first;
        newFile->prev = last;
        last->next = newFile;
        first->prev = newFile;
    }
    printf("File '%s' created.\n", name);
}

void writeCommand(char *name, char *data) {
    File *target = findChild(name);
    if (!target || target->isDirectory) { printf("File not found.\n"); return; }
    for (int i = 0; i < target->blockCount; i++)
        releaseBlock(target->blocks[i]);
    target->blockCount = 0;
    target->size = 0;
    int bytes = strlen(data);
    if (bytes == 0) { printf("Empty data.\n"); return; }
    int blocksNeeded = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocksNeeded > MAX_BLOCKS_PER_FILE) {
        printf("File too large.\n");
        return;
    }
    for (int i = 0; i < blocksNeeded; i++) {
        int blk = getFreeBlock();
        if (blk == -1) { printf("Disk full.\n"); return; }
        target->blocks[target->blockCount++] = blk;
        int remaining = bytes - i * BLOCK_SIZE;
        int toCopy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(virtualDisk[blk], data + i * BLOCK_SIZE, toCopy);
    }
    target->size = bytes;
    printf("Data written successfully (%d bytes).\n", bytes);
}

void readCommand(char *name) {
    File *target = findChild(name);
    if (!target || target->isDirectory) { printf("File not found.\n"); return; }
    if (target->blockCount == 0) { printf("(empty)\n"); return; }
    int remaining = target->size;
    for (int i = 0; i < target->blockCount; i++) {
        int toRead = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite(virtualDisk[target->blocks[i]], 1, toRead, stdout);
        remaining -= toRead;
    }
    printf("\n");
}

void deleteCommand(char *name) {
    File *target = findChild(name);
    if (!target) { printf("File not found.\n"); return; }
    if (target->isDirectory) { printf("'%s' is a directory.\n", name); return; }
    for (int i = 0; i < target->blockCount; i++)
        releaseBlock(target->blocks[i]);
    if (target == cwd->children && target->next == target) cwd->children = NULL;
    else {
        if (target == cwd->children) cwd->children = target->next;
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }
    free(target);
    printf("File deleted.\n");
}

void dfCommand() {
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n",
           MAX_NUM_BLOCKS, usedBlocks, MAX_NUM_BLOCKS - usedBlocks,
           (usedBlocks * 100.0) / MAX_NUM_BLOCKS);
}

void freeAll(File *dir) {
    if (dir->isDirectory && dir->children) {
        File *first = dir->children;
        first->prev->next = NULL;
        File *cur = first;
        while (cur) {
            File *next = cur->next;
            freeAll(cur);
            cur = next;
        }
    }
    free(dir);
}

void cleanup() {
    if (!root) return;
    freeAll(root);
    FreeBlock *cur = freeBlockHead;
    while (cur) {
        FreeBlock *next = cur->next;
        free(cur);
        cur = next;
    }
    for (int i = 0; i < MAX_NUM_BLOCKS; i++) free(virtualDisk[i]);
    free(virtualDisk);
}

void readUserInput(char *cmd, char *arg1, char *arg2) {
    if (scanf("%s", cmd) != 1) return;
    if (strcmp(cmd, "write") == 0) {
        scanf("%s", arg1);
        readWriteBuffer(arg2);
    } else if (strcmp(cmd, "mkdir") == 0 ||
               strcmp(cmd, "rmdir") == 0 ||
               strcmp(cmd, "cd") == 0 ||
               strcmp(cmd, "create") == 0 ||
               strcmp(cmd, "read") == 0 ||
               strcmp(cmd, "delete") == 0) {
        scanf("%s", arg1);
        arg2[0] = '\0';
    } else {
        arg1[0] = arg2[0] = '\0';
    }
}

void processCommand(char *cmd, char *arg1, char *arg2) {
    if (strcmp(cmd, "mkdir") == 0) mkdirCommand(arg1);
    else if (strcmp(cmd, "rmdir") == 0) rmdirCommand(arg1);
    else if (strcmp(cmd, "ls") == 0) lsCommand();
    else if (strcmp(cmd, "cd") == 0) cdCommand(arg1);
    else if (strcmp(cmd, "pwd") == 0) { printPath(cwd); printf("\n"); }
    else if (strcmp(cmd, "create") == 0) createCommand(arg1);
    else if (strcmp(cmd, "write") == 0) writeCommand(arg1, arg2);
    else if (strcmp(cmd, "read") == 0) readCommand(arg1);
    else if (strcmp(cmd, "delete") == 0) deleteCommand(arg1);
    else if (strcmp(cmd, "df") == 0) dfCommand();
    else if (strcmp(cmd, "exit") == 0) return;
    else {
        printf("Unknown command.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }
}

