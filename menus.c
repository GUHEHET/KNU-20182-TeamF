#include <stdio.h>
#include <string.h>
#include <ncursesw/curses.h>
#include <locale.h>

#define BLANK   "                                                     "
#define NAME    "SONG  NAME : "
#define DIFF    "DIFFICULTY : "

typedef struct song {
	char name[20];
	int diff;
}song;

int pos = 0;

void select_screen(int sel, int size, song *list)
{   
	char buf[50];

	if (sel == 1)
		pos = (pos+1) % size;
	else if (sel == -1)
		pos = (pos-1 + size) % size;

	clear(); 
	move(10, 10);
	addstr(BLANK);
	strcpy (buf, NAME);
	strcat (buf, list[pos].name);
	move(10, 10);
	addstr(buf);
	buf[0] = '\0';

	move(12, 10);
	addstr(BLANK);
	strcpy (buf, DIFF);
	buf[strlen(buf) + 1] = '\0';
	buf[strlen(buf)] = list[pos].diff + '0';
	move(12, 10);
	addstr(buf);
	move(0, 0);
	refresh();
}
