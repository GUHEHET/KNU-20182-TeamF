#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <aio.h>
#include <string.h>
#include <termios.h>

int delay = 10;
int key = -1;

struct aiocb kbcbuf;

int set_ticker(int);

void main()
{
	void on_alarm(int);
	void on_input(int);
	void setup_aio_buffer();

	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	setup_aio_buffer();
	aio_read(&kbcbuf);

	signal(SIGALRM, on_alarm);
	set_ticker(delay);

	while(1)
		pause();
	endwin();
}

void on_input(int snum)
{
	int c;
	char *cp = (char*) kbcbuf.aio_buf;

	if (aio_error(&kbcbuf) != 0)
		perror("reading failed");
	else if (aio_return(&kbcbuf) == 1)
	{
		c = *cp;
		if (c == 'd' || c == 'D')
			key = 0;
		else if (c == 'f' || c == 'F')
			key = 1;
		else if (c == 'j' || c == 'J')
			key = 2;
		else if (c == 'k' || c == 'K')
			key = 3;
	}
	aio_read(&kbcbuf);
}

void on_alarm(int snum)
{
	signal(SIGALRM, on_alarm);
	if (key == 0)
		addstr("D");
	else if (key == 1)
		addstr("F");
	else if (key == 2)
		addstr("J");
	else if (key == 3)
		addstr("K");
	key = -1;
	refresh();
}

void setup_aio_buffer()
{
	static char input[1];

	kbcbuf.aio_fildes = 0;
	kbcbuf.aio_buf = input;
	kbcbuf.aio_nbytes = 1;
	kbcbuf.aio_offset = 0;

	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}
