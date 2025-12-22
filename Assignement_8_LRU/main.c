#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_KEY 1000
#define MAX_VAL_LEN 100

typedef struct Node
{
     int key;
     char value[MAX_VAL_LEN];
     struct Node *next;
     struct Node *prev;
} Node;

typedef struct LRUCache
{
     int capacity;
     int size;
     Node *head;
     Node *tail;
     Node *map[MAX_KEY];
} LRUCache;

Node *createNode(int key, char *value);
LRUCache *createCache(int capacity);
void removeNode(Node *node);
void addNode(LRUCache *cache, Node *node);
char *get(LRUCache *cache, int key);
void put(LRUCache *cache, int key, char *value);
void freeCache(LRUCache *cache);
LRUCache *runCacheCommands();

int main()
{
     LRUCache *cache = runCacheCommands();
     freeCache(cache);
     return 0;
}

Node *createNode(int key, char *value)
{
     Node *newnode = (Node *)malloc(sizeof(Node));
     newnode->key = key;
     strncpy(newnode->value, value, MAX_VAL_LEN);
     newnode->next = NULL;
     newnode->prev = NULL;
     return newnode;
}

LRUCache *createCache(int capacity)
{
     if (capacity < 1 || capacity > 1000)
     {
          return NULL;
     }
     LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
     cache->capacity = capacity;
     cache->size = 0;

     cache->head = createNode(-1, "");
     cache->tail = createNode(-1, "");
     cache->head->next = cache->tail;
     cache->tail->prev = cache->head;

     for (int i = 0; i < MAX_KEY; i++)
     {

          cache->map[i] = NULL;
     }

     return cache;
}

void removeNode(Node *node)
{
     node->prev->next = node->next;
     node->next->prev = node->prev;
}

void addNode(LRUCache *cache, Node *node)
{
     node->next = cache->head->next;
     node->prev = cache->head;
     cache->head->next->prev = node;
     cache->head->next = node;
}

char *get(LRUCache *cache, int key)
{
     if (!cache || key < 0 || key >= MAX_KEY || cache->map[key] == NULL)
          return "NULL";

     Node *node = cache->map[key];
     removeNode(node);
     addNode(cache, node);
     return node->value;
}

void put(LRUCache *cache, int key, char *value)
{
     if (!cache || key < 0 || key >= MAX_KEY)
          return;

     Node *node = cache->map[key];

     if (node != NULL)
     {
          strncpy(node->value, value, MAX_VAL_LEN);
          removeNode(node);
          addNode(cache, node);
     }
     else
     {
          if (cache->size == cache->capacity)
          {
               Node *lru = cache->tail->prev;
               removeNode(lru);
               cache->map[lru->key] = NULL;
               free(lru);
               cache->size--;
          }

          Node *newNode = createNode(key, value);
          addNode(cache, newNode);
          cache->map[key] = newNode;
          cache->size++;
     }
}

void freeCache(LRUCache *cache)
{
     if (!cache)
          return;

     Node *curr = cache->head;
     while (curr != NULL)
     {
          Node *next = curr->next;
          free(curr);
          curr = next;
     }
     free(cache);
}

LRUCache *runCacheCommands()
{
     LRUCache *cache = NULL;
     char command[20];

     while (1)
     {
          scanf("%s", command);

          if (strcmp(command, "exit") == 0)
          {
               break;
          }
          else if (strcmp(command, "createCache") == 0)
          {
               int capacity;
               scanf("%d", &capacity);
               cache = createCache(capacity);
          }
          else if (strcmp(command, "put") == 0)
          {
               int key;
               char value[MAX_VAL_LEN];
               scanf("%d %s", &key, value);

               if (!cache)
               {
                    printf("NULL\n");
                    continue;
               }
               put(cache, key, value);
          }
          else if (strcmp(command, "get") == 0)
          {
               int key;
               scanf("%d", &key);

               if (!cache)
               {
                    printf("NULL\n");
                    continue;
               }
               printf("%s\n", get(cache, key));
          }
          else
          {
               printf("Unknown command\n");
          }
     }

     return cache;
}
