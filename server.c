#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7575

int vulnerable_function(char *str){
    char buffer[100];

    /* The following statement has a buffer overflow problem */
    strcpy(buffer, str);

    /* The following statement returns the address of str*/
    printf("The base address of str is 0x%x \n", (unsigned int) &str);
    
    return 1;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char str[400];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen error");
        exit(1);
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        // Accept the connection
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accept error");
            continue;
        }

        // Receive data from the client
        ssize_t bytes_received = recv(client_socket, str, sizeof(str), 0);
        if (bytes_received == -1) {
            perror("Receive error");
            close(client_socket);
            continue;
        }

        
        vulnerable_function(str);
        //system("your_command_here");
    

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (will not be reached in this example)
    close(server_socket);

    return 0;
}

