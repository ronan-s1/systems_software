#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char file_name[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *file_to_send;
    int bytes_read;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Get file name from user
    printf("Enter file name to send: ");
    scanf("%s", file_name);

    // Open file
    if ((file_to_send = fopen(file_name, "rb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Send file name to server
    char message[BUFFER_SIZE * 2]; // Increase buffer size for message

    // Send only file name to server
    char *file_name_only = strrchr(file_name, '/');
    if (file_name_only == NULL) {
        file_name_only = file_name;
    } else {
        file_name_only++;
    }

    sprintf(message, "%s>", file_name_only);
    if (send(client_socket, message, strlen(message), 0) < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Send file data to server
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file_to_send)) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    printf("File sent successfully\n");

    // Close file and socket
    fclose(file_to_send);
    close(client_socket);

    return 0;
}
