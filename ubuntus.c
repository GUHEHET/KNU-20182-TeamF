#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fmod.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <signal.h>

#define gotoxy(x, y) wmove(stdscr, y-1, x-1)
#define BLANK    "                                                     "
#define NAME    "SONG  NAME : "
#define DIFF    "DIFFICULTY : "

typedef struct song {
    char name[20];
    int diff;
}song;

FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[2];
FMOD_CHANNEL *g_Channel[2];
FMOD_BOOL IsPlaying;
int pos = 0;


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
                select_music();
			}
			FMOD_Channel_Stop(g_Channel[0]);
			FMOD_Sound_Release(g_Sound[0]);
			FMOD_Sound_Release(g_Sound[1]);
			break;
		}
	}
}

void select_music()
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
