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
#define BLANK    "                                                     "
#define NAME    "SONG  NAME : "
#define DIFF    "DIFFICULTY : "

typedef struct song {
    char name[20];
    int diff;
    int delay;
}song;

typedef struct judge {
    int perfect;
    int good;
    int bad;
    int miss;
	int maxcombo;
    int combo;
} judge;

FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[4];
FMOD_CHANNEL *g_Channel[2];
FMOD_BOOL IsPlaying;
int pos = 0;
int key = -1;
int linecount = 0;
int **notes;
song list[3];
int ingame_note[35][4];
judge total;
int thread_end_flag = 0;

void load_note(char*);
void free_note_arr(int**);
void titleScreen();
void playBGM();
void* on_input(void *);
void game_screen();
void select_music();
void select_screen(int,int,song*);
void print_note(int[][4]);
void draw_notes(int**);
void resultScreen();
void songinit(int, char*, int, int);

int main()
{
    setlocale(LC_CTYPE, "ko_KR.utf-8");
    
    songinit(0, "Magnolia", 2, 1);
    songinit(1, "black_swan", 7, 90000);
    songinit(2, "first_kiss", 4, 100580);
    
    titleScreen();
    endwin();
    
    return 0;
}

void titleScreen()
{
    int s = 0;
    char c;
    
    playBGM();
    initscr();
    noecho();
    clear();
    crmode();
    
    refresh();
    
    //signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    gotoxy(69, 10);
    printw("%s", "UBUNJAM_PROJECT");
    gotoxy(69, 11);
    printw("%s", "Ver. 0.0.4");
    
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
                    usleep(100983);        printw("%c", ' ');
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
            clear();
            if(!s)
            {
                while (1)
                {
                    select_music();
                }
            }
            break;
        }
    }
}

void songinit(int index, char* name, int diff, int delay)
{
    strcpy(list[index].name, name);
    list[index].diff = diff;
    list[index].delay = delay;
}

void select_music()
{
    pthread_t keythread;
    int key, sel=0;
    char somnailPath[100];
    
    pos = 0;
    total.perfect = 0;
    total.good = 0;
    total.bad = 0;
    total.miss = 0;
	total.maxcombo = 0;
    total.combo = 0;
    
    FMOD_Channel_Stop(g_Channel[0]);
    FMOD_Sound_Release(g_Sound[0]);
    FMOD_Sound_Release(g_Sound[1]);
    
    while(1) {
        select_screen(sel, 3, list);
        sel = 0;
        move(1,1);
        printw("%d %c", key, key);
        refresh();
        strcpy(somnailPath, "sound/somnail/");
        strcat(somnailPath, list[pos].name);
        strcat(somnailPath, "_somnail.mp3");
        refresh();
		FMOD_Channel_Stop(g_Channel[0]);
        FMOD_Sound_Release(g_Sound[0]);
        FMOD_System_CreateSound(g_System, somnailPath, FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
        FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel[0]);
        
        if ((key = getchar()) == 27 && (key = getchar()) == 91) {
            if ((key = getchar()) == 67)
                sel = 1;
            else if (key == 68)
                sel = -1;
        }
        else if ((key == 13) || (key == 10)) {
            sel = 0;
            break;
        }
        else if (key == 'Q') {
            endwin();
            exit(1);
        }
    }
    FMOD_Channel_Stop(g_Channel[0]);
    load_note(list[pos].name);
    thread_end_flag = 0;
    pthread_create(&keythread, NULL, on_input, (void *)NULL);
    game_screen();
    thread_end_flag = 1;
    pthread_join(keythread, NULL);
    return;
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
    FMOD_System_Init(g_System, 4, FMOD_INIT_NORMAL, NULL);
    FMOD_System_CreateSound(g_System, "sound/bgm/Menu_sound.mp3",
                            FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
    FMOD_System_CreateSound(g_System, "sound/effect/Menu_select.wav",
                            FMOD_LOOP_OFF, 0, &g_Sound[1]);
    FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel[0]);
}

void load_note(char *fname)
{
    FILE *fp;
    char temp[BUFSIZ];
    char path[BUFSIZ];
    int i = 0;
    strcpy(path, "notes/");
    strcat(path, fname);
    strcat(path, ".txt");
    
    fp = fopen(path, "r");
    
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }
    
    while (!feof(fp))
    {
        fscanf(fp, "%s", temp);
        linecount++;
    }
    
    fclose(fp);
    
    notes = (int**)malloc(sizeof(int*) * linecount);
    for (i=0;  i<linecount; i++)
        notes[i] = (int*)malloc(sizeof(int) * 4);
    
    fp = fopen(path, "r");
    
    if (!fp)
    {
        endwin();
        perror("fopen");
        exit(1);
    }
    
    i = 0;
    
    while (!feof(fp))
    {
        fscanf(fp, "%s", temp);
        
        notes[i][0] = (int)(temp[0]-48);
        notes[i][1] = (int)(temp[1]-48);
        notes[i][2] = (int)(temp[2]-48);
        notes[i][3] = (int)(temp[3]-48);
        
        i++;
    }
    fclose(fp);
    
    //free_note_arr(notes);
}

void free_note_arr(int** notes)
{
    for (int i=0; i<linecount; i++)
        free(notes[i]);
    
    free(notes);
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
    move(35, 20);
    addstr("     D        F        J        K    \n");
	gotoxy(60, 27);
    printw("Combo : %03d", total.combo);
    refresh();
    draw_notes(notes);
    resultScreen();
}

void draw_notes(int** notes)
{
    int i = 0, ingame_index = 0;
    char songPath[100];
    
    for (i=0; i<35; i++)
    {
        for (int j=0; j<4; j++)
        {
            ingame_note[i][j] = 0;
        }
    }
    
    i = 0;
    ingame_index = 0;
    
    strcpy(songPath, "sound/song/");
    strcat(songPath, list[pos].name);
    strcat(songPath, ".mp3");
    
    FMOD_Channel_Stop(g_Channel[0]);
    FMOD_Sound_Release(g_Sound[0]);
    FMOD_System_CreateSound(g_System, songPath, FMOD_LOOP_OFF, 0, &g_Sound[0]);
    
    for (i=0; i<linecount; i++)
    {
        //reload note
        if(i == 31)
            FMOD_System_PlaySound(g_System, g_Sound[0], 0, 0, &g_Channel[0]);
        for (int k=33; k>=0; k--)
        {
            for (int l=0; l<4; l++)
                ingame_note[k+1][l] = ingame_note[k][l];
        }
        
        for (int j=0; j<4; j++)
            ingame_note[0][j] = notes[i][j];
        
        if(ingame_note[34][0] == 1 || ingame_note[34][1] == 1 || ingame_note[34][2] == 1 || ingame_note[34][3]) {
            gotoxy(60, 20);
            printw("        ");
            gotoxy(60, 20);
            printw("Miss");
            total.miss++;
			if(total.combo > total.maxcombo)
				total.maxcombo = total.combo;
            total.combo = 0;
            gotoxy(68, 27);
			printw("%03d", total.combo);
            refresh();
        }
        
        print_note(ingame_note);
        usleep(list[pos].delay);
    }
}

void print_note(int note[][4])
{
    for (int i=0; i<34; i++)
    {
        move(i+1, 21);
        if (note[i][0] == 1) printw("● ● ● ● ");
		else if(i == 31)
			addstr("────────");
		else if(i == 33)
			addstr("━━━━━━━━");
        else addstr("　　　　");
        
        move(i+1, 30);
        if (note[i][1] == 1) printw("● ● ● ● ");
		else if(i == 31)
            addstr("────────");
        else if(i == 33)
            addstr("━━━━━━━━");
        else addstr("　　　　");
        
        move(i+1, 39);
        if (note[i][2] == 1) printw("● ● ● ● ");
		else if(i == 31)
            addstr("────────");
        else if(i == 33)
            addstr("━━━━━━━━");
        else addstr("　　　　");
        
        move(i+1, 48);
        if (note[i][3] == 1) printw("● ● ● ● ");
		else if(i == 31)
            addstr("────────");
        else if(i == 33)
            addstr("━━━━━━━━");
        else addstr("　　　　"); 
    }

    move(0,0);
    refresh();
    
}

void *on_input(void *a)
{
    int i;
    char c;
    
    while(1)
    {
        if (thread_end_flag == 1)
            return NULL;
        c = getch();
        if (c == 'd' || c == 'D')
        {
            for(i = 34; i >=28; i--) {
                if(ingame_note[i][0] == 1) {
                    ingame_note[i][0] = 0;
                    break;
                }
            }
        }
        else if (c == 'f' || c == 'F') {
            for(i = 34; i >=28; i--) {
                if(ingame_note[i][1] == 1) {
                    ingame_note[i][1] = 0;
                    break;
                }
            }
        }
        else if (c == 'j' || c == 'J') {
            for(i = 34; i >=28; i--) {
                if(ingame_note[i][2] == 1) {
                    ingame_note[i][2] = 0;
                    break;
                }
            }
        }
        else if (c == 'k' || c == 'K') {
            for(i = 34; i >=28; i--) {
                if(ingame_note[i][3] == 1) {
                    ingame_note[i][3] = 0;
                    break;
                }
            }
        }
        else if (c == 'q' || c == 'Q')
        {
            endwin();
            exit(0);
        }
        
        gotoxy(60, 20);
        printw("        ");
        gotoxy(60, 20);
        switch(i-31) {
            case -2:
                printw("Bad");
                total.bad++;
                total.combo++;
                break;
            case -1:
                printw("Good");
                total.good++;
                total.combo++;
                break;
            case 0:
                printw("Perfect!");
                total.perfect++;
                total.combo++;
                break;
            case 1:
                printw("Good");
                total.good++;
                total.combo++;
                break;
            case 2:
                printw("Bad");
                total.bad++;
                total.combo++;
                break;
        }
        gotoxy(68, 27);
		printw("%03d", total.combo);
		refresh();
    }
}

void resultScreen() {
    char c;
    
    clear();
    gotoxy(30, 20);
    printf("RESULT");
    gotoxy(30, 26);
    printw("Combo : %03d", total.maxcombo);
    gotoxy(30, 28);
    printw("Perfect : %03d", total.perfect);
    gotoxy(30, 30);
    printw("Good : %03d", total.good);
    gotoxy(30, 32);
    printw("Bad : %03d", total.bad);
    gotoxy(30, 34);
    printw("Miss : %03d", total.miss);
    gotoxy(30, 38);
    printw("PRESS ENTER");
    
    refresh();
}
