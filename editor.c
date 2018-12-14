#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fmod.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#define gotoxy(x, y) wmove(stdscr, y-1, x-1)

#define SONG	"Dream"
#define DELAY	30000
#define SIZE	8000

typedef struct song {
    char name[20];
    int diff;
	int delay;
}song;

FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[4];
FMOD_CHANNEL *g_Channel[2];
FMOD_BOOL IsPlaying;
int notes[SIZE][4];
int i = 0, done = 0;

void save_note();
void game_screen();
void* on_input(void *);
int set_ticker(int);

int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	initscr();
	noecho();
	crmode();
	clear();
	game_screen();
	endwin();

	save_note();

	return 0;
}

void save_note()
{
    FILE *fp;
	char name[50];

	strcpy(name, "notes/");
	strcat(name, SONG);
	strcat(name, ".txt");

	fp = fopen(name, "w");

	for (i = 0; i < SIZE; i++)
		fprintf(fp, "%d%d%d%d\n", notes[i][0], notes[i][1], notes[i][2], notes[i][3]);

	fclose(fp);
	printf("Saved.\n");
}

void game_screen()
{
	char songpath[50];
	pthread_t keythread;

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
	move(35, 20);
	addstr("     D        F        J        K    \n");
	refresh();

	pthread_create(&keythread, NULL, on_input, (void *)NULL);

	strcpy(songpath, "sound/song/");
	strcat(songpath, SONG);
	strcat(songpath, ".mp3");

	for (i = 0; i < 32; i++)
		usleep(DELAY);

	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 4, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(g_System, songpath, FMOD_LOOP_OFF, 0, &g_Sound[0]);
	FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel[0]);

	for (i = 0; i < SIZE; i++)
	{
		if (done == 1)
			break;

		usleep(DELAY);
	}
	pthread_join(keythread, (void**)NULL);
}

void *on_input(void *a) {
	char c;

	
	while(1)
	{
		c = getch();
		if (c == 'd' || c == 'D')
			notes[i][0] = 1;

		else if (c == 'f' || c == 'F')
			notes[i][1] = 1;

		else if (c == 'j' || c == 'J')
			notes[i][2] = 1;

		else if (c == 'k' || c == 'K')
			notes[i][3] = 1;

		else if (c == 'q')
		{
			done = 1;
			return NULL;
		}
	}
}
