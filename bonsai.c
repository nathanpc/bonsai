#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "process.h"


int PORT = 8080;
int MAX_CONNECTIONS = 10;

int main(int argc, char *argv[]) {
    int socket_descriptor = 0;
    int connection = 0;
    struct sockaddr_in address;
    
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    
    bind(socket_descriptor, (struct sockaddr*)&address, sizeof(address));
    listen(socket_descriptor, MAX_CONNECTIONS);
    
    printf("Server listening on port %d\n", PORT);
    
    while (1) {
        connection = accept(socket_descriptor, (struct sockaddr*)NULL, NULL);
        FILE *request_headers = fdopen(connection, "a+");

        process_request(connection, request_headers);
        
        close(connection);
        fclose(request_headers);
        sleep(1);
    }
    
    return 0;
}