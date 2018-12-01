#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>

#define BLANK	"                                                     "
#define NAME	"SONG  NAME : "
#define DIFF	"DIFFICULTY : "

typedef struct song {
	char name[20];
	int diff;
}song;

int pos = 0;

void main()
{
	struct song list[3];
	int key, sel = 0;
	int note[1000][4];
	int i, j, size;
	void select_screen(int, int, song *);
	void game_screen();
	int create_note(int [][4]);

	strcpy(list[0].name, "song1");
	list[0].diff = 2;
	strcpy(list[1].name, "song2");
	list[1].diff = 7;
	strcpy(list[2].name, "song3");
	list[2].diff = 4;

	initscr();
	crmode();
	noecho();
	clear();

	select_screen(sel, 2, list);
	
	while(1)
	{
		sel = 0;
		if ((key = getchar()) == 27 && (key = getchar()) == 91) {
			if ((key = getchar()) == 67)
				sel = 1;
			else if (key == 68)
				sel = -1;
		}
		else if (key == 13) {
			sel = 0;
			game_screen();
		}

		move(0,0);
		printw("%2d %2d", key, sel);
		refresh();

		select_screen(sel, 3, list);
	}
	endwin();
	
	size = create_note(note);
}

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

void game_screen()
{
	int i;

	clear();
	move(1, 20);
	addstr("==================================\n");
	
	for (i = 0; i < 30; i++) {
		move(i+2, 20);
		addstr("|       |       ||       |       |\n");
	}
	refresh();
	getch();
}

int create_note(int note[][4])
{
	FILE *fp;
	int buf, size;
	int temp, i = 0, j = 0;

	fp = fopen("song1.txt", "r");

	fseek(fp, 0, SEEK_END);
	size = ftell(fp) / 5;

	fseek(fp, 0, SEEK_SET);
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < 4; j++) {
			temp = fgetc(fp) - '0';
			note[i][j] = temp;
		}
		fgetc(fp);
	}
	fclose(fp);
	return size;
}
