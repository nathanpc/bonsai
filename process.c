// process.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


char output[1025];

void parse_request_headers(char *headers[], FILE *request) {
    int index = 0;
    
    while (index < 25) {
        char line[1024];
        fgets(line, sizeof(line), request);
        
        if (strlen(line) == 2) {
            break;
        }
        
        sprintf(headers[index], "%s", line);
        index++;
    }
}

void send_headers(int connection, char *headers[], int count) {
    for (int i = 0; i < count; i++) {
        snprintf(output, sizeof(output), "%s%s", headers[i], "\r\n");
        write(connection, output, strlen(output));
    }

    snprintf(output, sizeof(output), "\r\n");
    write(connection, output, strlen(output));
}

void process_request(int connection, FILE *request) {
    char *request_headers[24];
    parse_request_headers(request_headers, request);
    
    for (int i = 0; i < sizeof(request_headers) / sizeof(*request_headers); i++) {
        // Why the hell is this seg faulting?!
        printf("%s\n", request_headers[i]);
    }
    
    memset(output, 0, sizeof(output));
    
    char *response_headers[] = { "HTTP/1.0 200 OK",
                                 "Server: bamboo v0.0.0a",
                                 "Content-Type: text/html" };
    send_headers(connection, response_headers, sizeof(response_headers) / sizeof(*response_headers));
    
    snprintf(output, sizeof(output), "<h1>It works!</h1>\r\n");
    write(connection, output, strlen(output));
}