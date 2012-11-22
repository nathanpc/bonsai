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
#include <limits.h>

#include "misc.h"
#include "mime.h"

#define MAX_HEADERS 25
#define HEADER_SIZE 1025

char output[1025];

/**
 *  Parse the request headers and populate a string array with them.
 *
 *  @param headers Array to be populated with the headers.
 *  @param request Request file description.
 */
void parse_request_headers(char headers[MAX_HEADERS][HEADER_SIZE], FILE *request) {
    int index = 0;
    
    while (index < MAX_HEADERS) {
        char line[HEADER_SIZE];
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
 *  Parse the Request Type and Requested File into the respective strings.
 *
 *  @see parse_request_headers()
 *  @param request_type String where the Request Type will be stored.
 *  @param requested_file String where the Requested File will be stored.
 *  @param first_header First request header string.
 */
void request_type_and_file(char request_type[8], char *requested_file, char *first_header) {
    strcpy(request_type, strtok(first_header, " "));
    strcpy(requested_file, strtok(NULL, " "));
}

/**
 *  Send the response headers back to the requester.
 *
 *  @param connection Connection descriptor.
 *  @param headers Array of headers to be sent.
 *  @param count Number of readers to be sent back.
 */
void send_headers(int connection, char headers[3][HEADER_SIZE], int count) {
    for (int i = 0; i < count; i++) {
        snprintf(output, sizeof(output), "%s%s", headers[i], "\r\n");
        write(connection, output, strlen(output));
    }

    snprintf(output, sizeof(output), "\r\n");
    write(connection, output, strlen(output));
}

/**
 *  Parse the requested file location and send back the file contents.
 *
 *  @see request_type_and_file()
 *  @param connection Connection descriptor.
 *  @param file_name Requested file name acquired from request_type_and_file()
 */
void send_file(int connection, char file_name[501]) {
    char file_location[1025];
    char absolute_location[PATH_MAX + 1];
    FILE *file;
    long file_size;
    char *read_buffer;
    char response_headers[3][HEADER_SIZE];
    char mime[60];

    strcat(file_location, "./htdocs");
    strcat(file_location, file_name);

    if (file_name[strlen(file_name) - 1] == '/') {
        strcat(file_location, "index.html");
    }

    // TODO: Prevent the use of .. in the URL. Security
    get_mime(mime, file_location);
    realpath(file_location, absolute_location);
    file = fopen(absolute_location, "r");
    
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        read_buffer = (char*)malloc(sizeof(char) * file_size);
        fread(read_buffer, 1, file_size, file);

        strcpy(response_headers[0], "HTTP/1.0 200 OK");

        fclose(file);
        free(read_buffer);
    } else {
        // TODO: Send a 404 Header. And get the 404 page from the template dir.
        strcpy(response_headers[0], "HTTP/1.0 404 Not Found");
    }

    strcpy(response_headers[1], "Server: bamboo v0.0.0a");
    strcpy(response_headers[2], "Content-Type: ");
    strcat(response_headers[2], mime);
    send_headers(connection, response_headers, sizeof(response_headers) / sizeof(*response_headers));

    snprintf(output, sizeof(output), "%s", read_buffer);
    write(connection, output, strlen(output));

    snprintf(output, sizeof(output), "\r\n");
    write(connection, output, strlen(output));
}

/**
 *  Print the request headers (for debugging)
 *
 *  @see parse_request_headers()
 *  @param headers Header array obtained from parse_request_headers()
 */
void print_request_headers(char headers[MAX_HEADERS][HEADER_SIZE]) {
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
    char request_headers[MAX_HEADERS][HEADER_SIZE] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
    parse_request_headers(request_headers, request);

    char request_type[8];
    char file_requested[501];
    request_type_and_file(request_type, file_requested, request_headers[0]);
    printf("%s %s\n", request_type, file_requested);
    
    memset(output, 0, sizeof(output));

    if (strcmp(request_type, "GET") == 0) {
        send_file(connection, file_requested);
    }
}
