#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fmod.h>
#include <ncursesw/curses.h>
#include <locale.h>

#define gotoxy(x, y) wmove(stdscr, y-1, x-1)

FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[2];
FMOD_CHANNEL *g_Channel[2];
FMOD_BOOL IsPlaying;

void titleScreen();
void playBGM();

void main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	puts("실행에 필요한 프로그램을 설치합니다...");
	system("sudo apt-get -y install xterm");
	system("resize -s 40 160");
	titleScreen();
	endwin();
}

void titleScreen()
{
	int s = 0;
	char c;
	
	playBGM();
	initscr();
	noecho();
	crmode();

	refresh();

	gotoxy(69, 10);
	printw("%s", "UBUNTUS_PROJECT");
	gotoxy(69, 11);
	printw("%s", "Ver. 0.0.1");

	gotoxy(68, 25);
	printw("%s", ">1. Start");
	gotoxy(69, 26);
	printw("%s", "2. Exit");
	gotoxy(1, 1);
	refresh();
	while(1) {
		if((c = getch()) == 27 && getch() == 91) {
			FMOD_Channel_IsPlaying(g_Channel[1], &IsPlaying);
			if(IsPlaying == 1)
				FMOD_Channel_Stop(g_Channel[1]);
			FMOD_System_PlaySound(g_System,g_Sound[1], 0, 0, &g_Channel[1]);
			c = getch();
			switch(c) {
				case 65:
					gotoxy(68, 26);
					printw("%c", ' ');
					gotoxy(68, 25);
					printw("%c", '>');
					s = 0;
					break;
				case 66:
					gotoxy(68, 25);
					printw("%c", ' ');
					gotoxy(68, 26);
					printw("%c", '>');
					s = 1;
					break;
			}
			FMOD_System_Update(g_System);
		}
		gotoxy(1, 1);
		refresh();
		if(c == 10) {
			system("clear");
			if(!s) {
				printf("start");
			}
			FMOD_Channel_Stop(g_Channel[0]);
			FMOD_Sound_Release(g_Sound[0]);
			FMOD_Sound_Release(g_Sound[1]);
			break;
		}
	}
}

void playBGM()
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 2, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(g_System, "sound/bgm/Menu_sound.mp3",
			FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	FMOD_System_CreateSound(g_System, "sound/effect/Menu_select.wav",
			FMOD_LOOP_OFF, 0, &g_Sound[1]);
    FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel[0]);
}

