#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 8080
#define ACCOUNT_DB_PATH "../resource/accountDB.txt" 

pthread_mutex_t db_access_mutex; 

double read_account_balance();
void write_account_balance(double new_balance);
void *service_client_requests(void *client_socket_ptr);

int main() {
    int server_socket_fd, client_socket_fd, *new_client_sock_ptr;
    struct sockaddr_in server_address;
    int address_length = sizeof(server_address);


    pthread_mutex_init(&db_access_mutex, NULL);

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

  
    if (bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("ATM Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        if ((client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&server_address, (socklen_t *)&address_length)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected.\n");

     
        pthread_t thread_id;
        new_client_sock_ptr = malloc(1);
        *new_client_sock_ptr = client_socket_fd;
        
        if (pthread_create(&thread_id, NULL, service_client_requests, (void *)new_client_sock_ptr) < 0) {
            perror("Could not create thread");
            return 1;
        }
        pthread_detach(thread_id); 
    }

    pthread_mutex_destroy(&db_access_mutex);
    return 0;
}


double read_account_balance() {
    FILE *file = fopen(ACCOUNT_DB_PATH, "r");
    double balance;
    fscanf(file, "%lf", &balance);
    fclose(file);
    return balance;
}

void write_account_balance(double new_balance) {
    FILE *file = fopen(ACCOUNT_DB_PATH, "w");
    if (file) {
        fprintf(file, "%.2lf", new_balance);
        fclose(file);
    }
}

void *service_client_requests(void *client_socket_ptr) {
    int client_fd = *(int *)client_socket_ptr;
    free(client_socket_ptr);
    char buffer[1024] = {0};
    int choice;
    double amount;

    while (1) {
        int bytes_read = read(client_fd, buffer, 1024);
        if (bytes_read <= 0){
               break;
        }  

        sscanf(buffer, "%d %lf", &choice, &amount);
        char response[1024];

        pthread_mutex_lock(&db_access_mutex);
        
        double account_balance = read_account_balance();
        
        if (choice == 1) { 
            if (amount > account_balance) {
                snprintf(response, sizeof(response), "Error: Insufficient funds. Balance: %.2lf", account_balance);
            } else {
                account_balance -= amount;
                write_account_balance(account_balance);
                snprintf(response, sizeof(response), "Withdrawal successful. New Balance: %.2lf", account_balance);
            }
        } else if (choice == 2) { 
            account_balance += amount;
            write_account_balance(account_balance);
            snprintf(response, sizeof(response), "Deposit successful. New Balance: %.2lf", account_balance);
        } else if (choice == 3) { 
            snprintf(response, sizeof(response), "Current Balance: %.2lf", account_balance);
        } else {
            strcpy(response, "Invalid option");
        }

        pthread_mutex_unlock(&db_access_mutex);

        send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
    return NULL;
}
