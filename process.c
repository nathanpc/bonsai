/**
 *  process.c
 *  Process the request and respond.
 *  
 *  @author Nathan Campos
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

#include "misc.h"
#include "mime.h"
#include "termcolor.h"

#define MAX_HEADERS 25
#define HEADER_SIZE 1025

char output[16385];  // Maximum size of the output.

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
    DIR *dir;
    bool redir = false;
    bool invalid_url = false;

    strcpy(file_location, "htdocs");
    strcat(file_location, file_name);

    if (file_name[strlen(file_name) - 1] == '/') {
        strcat(file_location, "index.html");
    } else {
        char tmp_absolute_location[PATH_MAX + 1];

        realpath(file_location, tmp_absolute_location);
        strcat(tmp_absolute_location, "/");
        dir = opendir(tmp_absolute_location);

        if (dir != NULL) {
            strcat(file_location, "/index.html");
            redir = true;
        }
    }

    // TODO: Prevent the use of .. in the URL. Security
    get_mime(mime, file_location);
    realpath(file_location, absolute_location);
    file = fopen(absolute_location, "r");
    
    if (redir) {
        read_buffer = NULL;

        strcpy(response_headers[0], "HTTP/1.1 301 Moved Permanently");
        strcpy(response_headers[2], "Location: ");
        strcat(response_headers[2], file_name);
        strcat(response_headers[2], "/");
    } else if (invalid_url) {
        read_buffer = NULL;
    } else if (file != NULL) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        read_buffer = calloc((sizeof * read_buffer * file_size) + 1, sizeof(char));
        fread(read_buffer, 1, file_size, file);

        strcpy(response_headers[0], "HTTP/1.1 200 OK");
        strcpy(response_headers[2], "Content-Type: ");
        strcat(response_headers[2], mime);

        fclose(file);
    } else {
        read_buffer = NULL;
        // TODO: Get the 404 page from the template dir.
        strcpy(response_headers[0], "HTTP/1.1 404 Not Found");
        strcpy(response_headers[2], "Content-Type: text/html");
    }

    strcpy(response_headers[1], "Server: bamboo v0.0.0a");
    send_headers(connection, response_headers, sizeof(response_headers) / sizeof(*response_headers));

    if (read_buffer) {
        snprintf(output, sizeof(output), "%s", read_buffer);
        write(connection, output, strlen(output));
        free(read_buffer);
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
void print_request_headers(char headers[MAX_HEADERS][HEADER_SIZE]) {
    for (int i = 0; i < MAX_HEADERS; i++) {
        if (strcmp(headers[i], "") != 0) {
            printf("%s\n", headers[i]);
        }
    }

    printf("\n");
}

/**
 *  What actually process the request.
 *
 *  @param connection Connection descriptor.
 *  @param request Request file description.
 */
void process_request(int connection, FILE *request) {
    char request_type[8];
    char file_requested[501];
    char request_headers[MAX_HEADERS][HEADER_SIZE] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };

    memset(output, 0, sizeof(output));

    parse_request_headers(request_headers, request);
    request_type_and_file(request_type, file_requested, request_headers[0]);

    termcolor(BOLD, WHITE);
    printf("%s %s\n", request_type, file_requested);
    reset_termcolor();
    print_request_headers(request_headers);

    if (strcmp(request_type, "GET") == 0) {
        send_file(connection, file_requested);
    }
}
