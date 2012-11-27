// Credits: http://www.linuxgazette.net/issue65/padala.html

#include <stdio.h>
#include "termcolor.h"

void termcolor(int attr, int fg) {
    char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, 40);
	printf("%s", command);
}

void termcolor3(int attr, int fg, int bg) {
    char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

void reset_termcolor() {
    printf("%s", RESETDEFAULT);
}
