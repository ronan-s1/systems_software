#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 2048

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *socket_desc)
{
    int client_socket = *(int *)socket_desc;
    char buffer[BUFFER_SIZE] = {0};
    char file_name[BUFFER_SIZE];
    char username[BUFFER_SIZE];
    int user_id;
    int group_id;
    FILE *received_file;
    char *file_content = NULL; // Initialize to NULL
    int bytes_received;
    int user_in_manufacturing = 0;
    int user_in_distribution = 0;
    struct group *grp;

    // Receive message from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0)
    {
        perror("recv");
        close(client_socket);
        pthread_exit(NULL);
    }
    buffer[bytes_received] = '\0'; // Add null terminator to received message

    // Parse received message to extract username and file name
    char *token = strtok(buffer, ">");
    if (token == NULL)
    {
        fprintf(stderr, "Invalid message format\n");
        close(client_socket);
        pthread_exit(NULL);
    }
    strcpy(file_name, token);

    token = strtok(NULL, ">");
    if (token == NULL)
    {
        fprintf(stderr, "Invalid message format\n");
        close(client_socket);
        pthread_exit(NULL);
    }
    strcpy(username, token);

    token = strtok(NULL, ">");
    if (token == NULL)
    {
        fprintf(stderr, "Invalid message format\n");
        close(client_socket);
        pthread_exit(NULL);
    }
    user_id = atoi(token);

    token = strtok(NULL, ">");
    if (token == NULL)
    {
        fprintf(stderr, "Invalid message format\n");
        close(client_socket);
        pthread_exit(NULL);
    }
    group_id = atoi(token);

    file_content = strtok(NULL, ">"); // strtok to get file content
    if (file_content == NULL)
    {
        fprintf(stderr, "Invalid message format\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    printf("\nUsername: %s\nFile: %s\nUserID: %d\nGroupID: %d\nContent: %s", username, file_name, user_id, group_id, file_content);

    // Get user's group
    if ((grp = getgrgid(group_id)) != NULL)
    {
        printf("\nGroup name: %s\n", grp->gr_name);
        if (strcmp(grp->gr_name, "manufacturing") == 0)
        {
            user_in_manufacturing = 1;
        }
        else if (strcmp(grp->gr_name, "distribution") == 0)
        {
            user_in_distribution = 1;
        }
    }
    else
    {
        perror("getgrgid");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Open file
    char path[BUFFER_SIZE * 2];
    if (user_in_manufacturing)
    {
        sprintf(path, "./manufacturing/%s", file_name);
    }
    else if (user_in_distribution)
    {
        sprintf(path, "./distribution/%s", file_name);
    }
    else
    {
        printf("Invalid user group\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Create file
    if ((received_file = fopen(path, "wb")) == NULL)
    {
        perror("fopen");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Receive file data from client
    fwrite(file_content, 1, strlen(file_content), received_file);

    fclose(received_file);

    // Set ownership of the file to the transfer user
    if (chown(path, user_id, -1) == -1)
    {
        perror("chown");
    }

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
