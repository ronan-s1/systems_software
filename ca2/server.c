#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 2048

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *handle_client(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[BUFFER_SIZE] = {0};
    char file_name[BUFFER_SIZE];
    FILE *received_file;
    char *file_content;
    int bytes_received;

    // Receive file name from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Parse received message to extract file name
    buffer[bytes_received] = '\0';
    sscanf(buffer, "%[^>]", file_name);
    printf("buffer: %s\n", buffer);

    // Get current username
    char *current_user = getlogin();
    if (current_user == NULL) {
        perror("getlogin");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Open file
    char path[BUFFER_SIZE * 2];
    if (strcmp(current_user, "Manufacturing") == 0) {
        sprintf(path, "./manufacturing/%s", file_name);
    } else if (strcmp(current_user, "Distribution") == 0) {
        sprintf(path, "./distribution/%s", file_name);
    } else {
        printf("Unknown user: %s\n", current_user);
        close(client_socket);
        pthread_exit(NULL);
    }
    printf("Path: %s\n", path);
    if ((received_file = fopen(path, "wb")) == NULL) {
        perror("fopen");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Receive file data from client
    file_content = strchr(buffer, '>') + 1; // Move pointer to file content
    fwrite(file_content, 1, bytes_received - (strlen(file_name) + 1), received_file);

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, received_file);
    }

    // Close file and client socket
    fclose(received_file);
    close(client_socket);

    pthread_exit(NULL);
}


int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(server_addr);
    pthread_t thread_id;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Waiting for incoming connections...\n");

        // Accept connection
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

        // Create thread to handle client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) < 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    close(server_socket);

    return 0;
}
