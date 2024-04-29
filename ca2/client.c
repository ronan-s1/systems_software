#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <grp.h>
#include <sys/types.h>



#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char file_name[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *file_to_send;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Get file name from user
    printf("Enter file path to send: ");
    scanf("%s", file_name);

    // Open file
    if ((file_to_send = fopen(file_name, "r")) == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Get the username
    char *current_user = getenv("USER");
    if (current_user == NULL)
    {
        perror("getenv");
        exit(EXIT_FAILURE);
    }

    // get userID and group name
    uid_t uid = getuid();
    gid_t gid = getgid();

    // Send file name and username to server
    char message[BUFFER_SIZE * 2];

    // Send only file name to server
    char *file_name_only = strrchr(file_name, '/');
    if (file_name_only == NULL)
    {
        file_name_only = file_name;
    }
    else
    {
        file_name_only++;
    }

    // Read the file into buffer
    fseek(file_to_send, 0, SEEK_END);
    long file_size = ftell(file_to_send);
    rewind(file_to_send);
    fread(buffer, file_size, 1, file_to_send);
    sprintf(message, "%s>%s>%d>%d>%s", file_name_only, current_user, uid, gid, buffer);
    if (send(client_socket, message, strlen(message), 0) < 0)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }

    printf("File sent successfully\n");

    // Close file and socket
    fclose(file_to_send);
    close(client_socket);

    return 0;
}
