#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_BUFFER_SIZE 4096
#define TIMEOUT 60000

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void server_mode(int *port) {
    *port = randomize_port();

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        handle_error("Error creating socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(*port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        handle_error("Error binding");
    }

    printf("Listening on port %d\n", *port);

    if (listen(server_socket, 1) == -1) {
        handle_error("Error listening");
    }

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        handle_error("Error accepting connection");
    }

    close(server_socket);

    struct pollfd fds[1];
    fds[0].fd = client_socket;
    fds[0].events = POLLIN;

    while (1) {
        int ret = poll(fds, 1, TIMEOUT);
        if (ret == -1) {
            perror("An error occurred with poll");
            break;
        }

        if (ret > 0) {
            if (fds[0].revents & POLLIN) {
                // Handle incoming data from the client
                char buffer[MAX_BUFFER_SIZE];
                ssize_t bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

                if (bytesRead <= 0) {
                    printf("Connection closed by the other party.\n");
                    break;
                }

                buffer[bytesRead] = '\0';
                write(1, buffer, bytesRead);  // Write to stdout

                // Example: Send a response back to the client
                const char *response = "Message received!";
                send(client_socket, response, strlen(response), 0);
            }
        }
    }

    close(client_socket);
}


void client_mode(int client_fd) {
    struct pollfd fds[2];
    fds[0].fd = 0;  // stdin
    fds[0].events = POLLIN;
    fds[1].fd = client_fd;
    fds[1].events = POLLIN;

    while (1) {
        int ret = poll(fds, 2, TIMEOUT);
        if (ret > 0) {
            if (fds[0].revents & POLLIN) {
                char buffer[MAX_BUFFER_SIZE];
                fgets(buffer, sizeof(buffer), stdin);
                write(client_fd, buffer, strlen(buffer));
            }
            if (fds[1].revents & POLLIN) {
                char buffer[MAX_BUFFER_SIZE];
                ssize_t bytesRead = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead <= 0) {
                    printf("Connection closed by the other party.\n");
                    break;
                }
                buffer[bytesRead] = '\0';
                write(1, buffer, bytesRead);  // Write to stdout
            }
        }
    }
    close(client_fd);
}

int main(int argc, char *argv[]) {
    int port;

    if (argc == 1) {
        server_mode(&port);
    } else if (argc == 3) {
        const char *server_ip = argv[1];
        int port = atoi(argv[2]);
        client_mode(server_ip, port);
    } else {
        fprintf(stderr, "Usage: %s [server_ip] [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
