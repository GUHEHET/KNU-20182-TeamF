#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int** load_note(char*);
int linecount = 0;
void free_note_arr(int**);

int main()
{
    int **notes;
    
    notes = load_note("notes/black_swan.txt");

    free_note_arr(notes);

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

    for (i=0; i<linecount; i++)
	printf("%d %d %d %d\n", notes[i][0], notes[i][1], notes[i][2], notes[i][3]);


    return notes;
}

void free_note_arr(int** notes)
{
    for (int i=0; i<linecount; i++)
	free(notes[i]);

    free(notes);
}
