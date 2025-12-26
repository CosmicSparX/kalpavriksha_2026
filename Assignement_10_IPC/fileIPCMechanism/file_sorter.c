#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define IPC_FILENAME "ipc_data_file.txt"
#define MAX_ELEMENTS 100

void read_integer_array(int *data, int size);
void display_array(int *data, int size, const char *label);
void bubble_sort(int *data, int size);
void read_data_from_file(char *file_path, int *data, int *size);
void write_data_to_file(char *file_path, int *data, int size);

int main()
{
    int size;
    printf("Enter the number of elements: ");
    scanf("%d", &size);

    int data[size];
    read_integer_array(data, size);
    display_array(data, size, "Before sorting");

    write_data_to_file(IPC_FILENAME, data, size);

    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        int child_data[MAX_ELEMENTS];
        int read_size;
        read_data_from_file(IPC_FILENAME, child_data, &read_size);
        bubble_sort(child_data, read_size);
        write_data_to_file(IPC_FILENAME, child_data, read_size);
    }
    else
    {
        wait(NULL);
        int parent_data[MAX_ELEMENTS];
        int read_size;
        read_data_from_file(IPC_FILENAME, parent_data, &read_size);
        display_array(parent_data, read_size, "After sorting");
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

void read_data_from_file(char *file_path, int *data, int *size)
{
    FILE *file_handle = fopen(file_path, "r+");
    if (!file_handle)
    {
        perror("fopen error (read)");
        return;
    }
    fscanf(file_handle, "%d", size);
    for (int i = 0; i < *size; i++)
    {
        fscanf(file_handle, "%d", &data[i]);
    }
    fclose(file_handle);
}

void write_data_to_file(char *file_path, int *data, int size)
{
    FILE *file_handle = fopen(file_path, "w");
    if (!file_handle)
    {
        perror("fopen error (write)");
        return;
    }
    fprintf(file_handle, "%d\n", size);
    for (int i = 0; i < size; i++)
    {
        fprintf(file_handle, "%d ", data[i]);
    }
    fprintf(file_handle, "\n");
    fclose(file_handle);
}
