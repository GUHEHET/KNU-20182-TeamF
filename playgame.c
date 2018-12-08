#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

int** load_note(char*);
int linecount = 0;
void free_note_arr(int**);
void draw_notes(int**);
void print_note(int[][4]);

int main()
{
    int **notes;
    
    notes = load_note("notes/black_swan.txt");
    
    draw_notes(notes);
    
    free_note_arr(notes);

    endwin();
    
    return 0;
}

int** load_note(char *fname)
{
    FILE *fp;
    char temp[BUFSIZ];
    int **notes;
    int i = 0;

    fp = fopen(fname, "r");

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

    fp = fopen(fname, "r");

    if (!fp)
    {
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

    return notes;
}

void free_note_arr(int** notes)
{
    for (int i=0; i<linecount; i++)
	free(notes[i]);

    free(notes);
}

void draw_notes(int** notes)
{
    int ingame_note[35][4];
    int i = 0, ingame_index = 0;
    
    for (i=0; i<35; i++)
    {
        for (int j=0; j<4; j++)
        {
            ingame_note[i][j] = 0;
        }
    }
    
    initscr();
    clear();
    
    i = 0;
    ingame_index = 0;
    
    for (i=0; i<linecount; i++)
    {
        //reload note
        for (int k=33; k>=0; k--)
        {
            for (int l=0; l<4; l++)
                ingame_note[k+1][l] = ingame_note[k][l];
        }
        
        for (int j=0; j<4; j++)
            ingame_note[0][j] = notes[i][j];
        
        print_note(ingame_note);
        usleep(100000);
    }
}

void print_note(int note[][4])
{
    for (int i=0; i<35; i++)
    {
        move(i+3, 5);
        if (note[i][0] == 1) addstr("@@@@ ");
        else addstr("     ");
        
        move(i+3, 14);
        if (note[i][1] == 1) addstr("@@@@ ");
        else addstr("     ");
        
        move(i+3, 23);
        if (note[i][2] == 1) addstr("@@@@ ");
        else addstr("     ");
        
        move(i+3, 32);
        if (note[i][3] == 1) addstr("@@@@");
        else addstr("    ");
        
        move(38, 5);
        addstr("---------------------------");
        
        if (i == 34) addstr("                   ");
    }
    
    refresh();
    
}
