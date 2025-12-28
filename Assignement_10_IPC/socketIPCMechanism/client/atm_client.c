#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080

int establish_server_connection();
void handle_user_transactions(int server_socket_fd);

int main() {
    int server_socket_fd = establish_server_connection();
    if (server_socket_fd < 0) {
        return -1;
    }

    handle_user_transactions(server_socket_fd);

    close(server_socket_fd);
    return 0;
}

int establish_server_connection() {
    int server_socket_fd = 0;
    struct sockaddr_in server_address;

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return server_socket_fd;
}

void handle_user_transactions(int server_socket_fd) {
    int choice;
    double amount;
    char buffer[1024] = {0};
    char request[50];

    while (1) {
        printf("\n--- ATM Menu ---\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) break; 

        amount = 0;
        if (choice == 1 || choice == 2) {
            printf("Enter amount: ");
            scanf("%lf", &amount);
        }

        snprintf(request, sizeof(request), "%d %.2lf", choice, amount);
        send(server_socket_fd, request, strlen(request), 0);

        memset(buffer, 0, 1024);
        read(server_socket_fd, buffer, 1024);
        printf("Server Response: %s\n", buffer);
    }
}