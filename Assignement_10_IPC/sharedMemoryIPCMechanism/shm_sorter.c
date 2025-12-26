#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>

#define MAX_ELEMENTS 100
#define SHARED_MEM_KEY 12345

void read_integer_array(int *data, int size);
void display_array(int *data, int size, const char *label);
void bubble_sort(int *data, int size);

int main()
{
     int size;
     printf("Enter the number of elements: ");
     scanf("%d", &size);

     int shm_id = shmget(SHARED_MEM_KEY, sizeof(int) * (size + 1), IPC_CREAT | 0666);
     if (shm_id == -1)
     {
          perror("shmget failed");
          return 1;
     }

     int *shared_data_ptr = (int *)shmat(shm_id, NULL, 0);
     if (shared_data_ptr == (int *)-1)
     {
          perror("shmat failed");
          return 1;
     }
     shared_data_ptr[0] = size;

     read_integer_array(&shared_data_ptr[1], size);
     display_array(&shared_data_ptr[1], size, "Before sorting");

     pid_t child_pid = fork();
     if (child_pid == 0)
     {
          int *child_data_ptr = (int *)shmat(shm_id, NULL, 0);
          if (child_data_ptr == (int *)-1)
          {
               perror("shmat failed in child");
               return 1;
          }

          bubble_sort(&child_data_ptr[1], child_data_ptr[0]);
          printf("Child sorted the array.\n");
          shmdt(child_data_ptr);
          exit(0);
     }
     else
     {
          wait(NULL);
          display_array(&shared_data_ptr[1], size, "After sorting");
          shmdt(shared_data_ptr);
          shmctl(shm_id, IPC_RMID, NULL);
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
