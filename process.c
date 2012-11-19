/**
 *  process.c
 *  Process the request and respond.
 *  
 *  @author Nathan Campos
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "misc.h"

char output[1025];

/**
 *  Parse the request headers and populate a string array with them.
 *
 *  @param headers Array to be populated with the headers.
 *  @param request Request file description.
 */
void parse_request_headers(char headers[25][1024], FILE *request) {
    int index = 0;
    
    while (index < 25) {
        char line[1024];
        fgets(line, sizeof(line), request);
        
        if (strlen(line) == 2) {
            break;
        }
        
        remove_newline(line);
        strcpy(headers[index], line);
        
        index++;
    }
}

/**
 *  Send the response headers back to the requester.
 *
 *  @param connection Connection descriptor.
 *  @param headers Array of headers to be sent.
 *  @param count Number of readers to be sent back.
 */
void send_headers(int connection, char *headers[], int count) {
    for (int i = 0; i < count; i++) {
        snprintf(output, sizeof(output), "%s%s", headers[i], "\r\n");
        write(connection, output, strlen(output));
    }

    snprintf(output, sizeof(output), "\r\n");
    write(connection, output, strlen(output));
}

/**
 *  Print the request headers (for debugging)
 *
 *  @see parse_request_headers()
 *  @param headers Header array obtained from parse_request_headers()
 */
void print_request_headers(char headers[25][1024]) {
    for (int i = 0; i < sizeof(headers) / sizeof(*headers); i++) {
        if (strcmp(headers[i], "") != 0) {
            printf("%s\n", headers[i]);
        }
    }
}

/**
 *  What actually process the request.
 *
 *  @param connection Connection descriptor.
 *  @param request Request file description.
 */
void process_request(int connection, FILE *request) {
    char request_headers[25][1024] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
    parse_request_headers(request_headers, request);
    
    memset(output, 0, sizeof(output));
    
    char *response_headers[] = { "HTTP/1.0 200 OK",
                                 "Server: bamboo v0.0.0a",
                                 "Content-Type: text/html" };
    send_headers(connection, response_headers, sizeof(response_headers) / sizeof(*response_headers));
    
    snprintf(output, sizeof(output), "<h1>It works!</h1>\r\n");
    write(connection, output, strlen(output));
}