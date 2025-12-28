#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_ELEMENTS 100
#define QUEUE_KEY 12345

struct queue_message
{
     long mtype;
     int data[MAX_ELEMENTS];
     int size;
};
void read_integer_array(int *data, int size);
void display_array(int *data, int size, const char *label);
void bubble_sort(int *data, int size);
void send_data_to_queue(int queue_id, int *data, int size);
void receive_data_from_queue(int queue_id, int *data, int *size);

int main()
{
     int size;
     printf("Enter the number of elements: ");
     scanf("%d", &size);

     int data[size];
     read_integer_array(data, size);
     display_array(data, size, "Before sorting");
     int queue_id = msgget(QUEUE_KEY, IPC_CREAT | 0666);
     if (queue_id == -1)
     {
          perror("msgget failed");
          return 1;
     }

     send_data_to_queue(queue_id, data, size);

     pid_t child_pid = fork();
     if (child_pid == 0)
     {
          int child_data[MAX_ELEMENTS];
          int received_size;
          receive_data_from_queue(queue_id, child_data, &received_size);
          bubble_sort(child_data, received_size);
          send_data_to_queue(queue_id, child_data, received_size);
     }
     else
     {
          wait(NULL);
          int parent_data[MAX_ELEMENTS];
          int received_size;
          receive_data_from_queue(queue_id, parent_data, &received_size);
          display_array(parent_data, received_size, "After sorting");
          msgctl(queue_id, IPC_RMID, NULL);
     }

     return 0;
}

void read_integer_array(int *data, int size)
{
     for (int i = 0; i < size; i++)
     {
          scanf("%d", &data[i]);
     }
}

void display_array(int *data, int size, const char *label)
{
     printf("%s: ", label);
     for (int i = 0; i < size; i++)
     {
          printf("%d ", data[i]);
     }
     printf("\n");
}

void bubble_sort(int *data, int size)
{
     for (int i = 0; i < size; i++)
     {
          for (int j = i + 1; j < size; j++)
          {
               if (data[j] < data[i])
               {
                    int temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
               }
          }
     }
}

void send_data_to_queue(int queue_id, int *data, int size)
{
     struct queue_message msg;
     msg.mtype = 1;
     msg.size = size;
     for (int i = 0; i < size; i++)
     {
          msg.data[i] = data[i];
     }

     if (msgsnd(queue_id, &msg, sizeof(msg) - sizeof(long), 0) == -1)
     {
          perror("msgsnd failed");
          exit(1);
     }
}

void receive_data_from_queue(int queue_id, int *data, int *size)
{
     struct queue_message msg;

     if (msgrcv(queue_id, &msg, sizeof(msg) - sizeof(long), 1, 0) == -1)
     {
          perror("msgrcv failed");
          exit(1);
     }

     *size = msg.size;
     for (int i = 0; i < *size; i++)
     {
          data[i] = msg.data[i];
     }
}
