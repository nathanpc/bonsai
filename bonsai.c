/**
 *  bonsai Server
 *  bonsai.c
 *  The awesome tiny server.
 *
 *  @author Nathan Campos
 */


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    int binded;
    
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    
    binded = bind(socket_descriptor, (struct sockaddr*)&address, sizeof(address));
    listen(socket_descriptor, MAX_CONNECTIONS);

    // TODO: Handle the ECONNREFUSED error <http://www.gnu.org/software/libc/manual/html_node/Accepting-Connections.html>
    if (binded == 0) {
        printf("Server listening on port %d\n", PORT);
    } else {
        perror("ERROR");
        return 1;
    }
    
    while (true) {
        connection = accept(socket_descriptor, (struct sockaddr*)NULL, NULL);
        FILE *request_headers = fdopen(connection, "a+");

        process_request(connection, request_headers);
        
        close(connection);
        fclose(request_headers);
        sleep(1);
    }
    
    return 0;
}
