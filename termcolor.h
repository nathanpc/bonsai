// Credits: http://www.linuxgazette.net/issue65/padala.html

#define RESETDEFAULT  "\033[0m"

#define RESET		0
#define BOLD 		1
#define DIM	    	2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED	    	1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void termcolor(int attr, int fg);
void termcolor3(int attr, int fg, int bg);
void reset_termcolor();
