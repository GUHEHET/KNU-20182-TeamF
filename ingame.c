#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <signal.h>
#include <locale.h>

#define BLANK	"                                                     "
#define NAME	"SONG  NAME : "
#define DIFF	"DIFFICULTY : "

typedef struct song {
	char name[20];
	int diff;
}song;

void game_screen();
int create_note(int [][4]);

void main()
{
	int note[1000][4], size;

	setlocale(LC_CTYPE, "ko_KR.utf-8");
	initscr();
	crmode();
	noecho();
	clear();

	size = create_note(note);
	
	game_screen();
	getchar();
	endwin();
}

void game_screen()
{
	int i;

	clear();
	move(1, 20);
	addstr("┏━━━━━━━━┯━━━━━━━━┯━━━━━━━━┯━━━━━━━━┓\n");
	
	for (i = 0; i < 30; i++) {
		move(i+2, 20);
		addstr("┃        │        │        │        ┃\n");
	}
	move(32, 20);
	addstr("┠────────┼────────┼────────┼────────┨\n");
	move(33, 20);
	addstr("┃        │        │        │        ┃\n");
	move(34, 20);
	addstr("┗━━━━━━━━┷━━━━━━━━┷━━━━━━━━┷━━━━━━━━┛\n");
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
