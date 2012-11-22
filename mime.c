/**
 *  mime.c
 *  Handles the MIME Type operation.
 *
 *  @author Nathan Campos
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/**
 *  Get the MIME Type from a file name.
 *
 *  @param mime String that will get the MIME Type stored.
 *  @param file_name The file name.
 */
void get_mime(char mime[60], char *file_name) {
    char command[PATH_MAX + 1] = "./scripts/get-mime.rb ";
    strcat(command, file_name);

    FILE *exec = popen(command, "r");

    char output[60];
    fgets(output, sizeof(output), exec);
    strcpy(mime, output);

    pclose(exec);
}
