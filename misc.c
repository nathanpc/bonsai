/**
 *  misc.c
 *  Miscellaneous stuff.
 *
 *  @author Nathan Campos
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  Remove newline characters (\n & \r) from strings.
 *
 *  @param string String to get the newline characters removed.
 */
void remove_newline(char *string) {
    string[strcspn(string, "\r")] = '\0';
    string[strcspn(string, "\n")] = '\0';
}