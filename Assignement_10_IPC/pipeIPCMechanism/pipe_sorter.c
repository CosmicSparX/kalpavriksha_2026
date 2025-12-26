#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ELEMENTS 100

void read_integer_array(int *data, int size);
void display_array(int *data, int size, const char *label);
void bubble_sort(int *data, int size);
void write_array_to_pipe(int *data, int size, int pipe_write_end);
void read_array_from_pipe(int *data, int *size, int pipe_read_end);

int main()
{
     int size;
     printf("Enter number of elements: ");
     scanf("%d", &size);

     int data[MAX_ELEMENTS];
     read_integer_array(data, size);
     display_array(data, size, "Before sorting");

     int p2c_pipe[2];
     int c2p_pipe[2];

     if (pipe(p2c_pipe) == -1 || pipe(c2p_pipe) == -1)
     {
          perror("pipe failed");
          exit(1);
     }

     pid_t child_pid = fork();

     if (child_pid == 0)
     {
          close(p2c_pipe[1]);
          close(c2p_pipe[0]);

          int child_data[MAX_ELEMENTS];
          int received_size;

          read_array_from_pipe(child_data, &received_size, p2c_pipe[0]);
          bubble_sort(child_data, received_size);
          write_array_to_pipe(child_data, received_size, c2p_pipe[1]);

          close(p2c_pipe[0]);
          close(c2p_pipe[1]);
          _exit(0);
     }
     else
     {
          close(p2c_pipe[0]);
          close(c2p_pipe[1]);

          write_array_to_pipe(data, size, p2c_pipe[1]);
          wait(NULL);

          int sorted_data[MAX_ELEMENTS];
          int received_size;
          read_array_from_pipe(sorted_data, &received_size, c2p_pipe[0]);
          display_array(sorted_data, received_size, "After sorting");

          close(p2c_pipe[1]);
          close(c2p_pipe[0]);
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

void write_array_to_pipe(int *data, int size, int pipe_write_end)
{
     write(pipe_write_end, &size, sizeof(int));
     write(pipe_write_end, data, sizeof(int) * size);
}

void read_array_from_pipe(int *data, int *size, int pipe_read_end)
{
     read(pipe_read_end, size, sizeof(int));
     read(pipe_read_end, data, sizeof(int) * (*size));
}
