#include <ncursesw/curses.h>
#include <locale.h>
#include <unistd.h>

void main()
{
	setlocale(LC_CTYPE, "ko_KR.utf-8");
	initscr();
	
	while(1) {
		move(18, 0);
		printw("■ ■ ■ ■ ■ 　");
		move(19, 0);
		printw("■ ■ ■ 　　　");
		move(20, 0);
		printw("■ ■ ■ ■ ■ ■ ");
		move(21, 0);
		printw("■ ■ ■ ■ 　　");


		move(18, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(19, 108);
		printw("　　　■ ■ ■ ");
		move(20, 108);
		printw("■ ■ ■ ■ ■ ■ ");
		move(21, 108);
		printw("　　■ ■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ ■ ■ ■ ■ ");
        move(19, 0);
        printw("■ ■ 　　　　");
        move(20, 0);
        printw("■ ■ ■ ■ ■ 　");
        move(21, 0);
        printw("■ ■ ■ ■ ■ 　");

                move(18, 108);
		printw("■ ■ ■ ■ ■ ■ ");
		move(19, 108);
		printw("　　　　■ ■ ");
		move(20, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(21, 108);
		printw("　■ ■ ■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ ■ ■ ■ 　");
        move(19, 0);
        printw("■ ■ ■ 　　　");
        move(20, 0);
        printw("■ ■ ■ ■ 　　");
        move(21, 0);
        printw("■ ■ ■ ■ ■ ■ ");

                move(18, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(19, 108);
		printw("　　　■ ■ ■ ");
		move(20, 108);
		printw("　　■ ■ ■ ■ ");
		move(21, 108);
		printw("■ ■ ■ ■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ ■ ■ 　　");
        move(19, 0);
        printw("■ ■ ■ ■ 　　");
        move(20, 0);
        printw("■ ■ ■ 　　　");
        move(21, 0);
        printw("■ ■ ■ ■ ■ 　");

                move(18, 108);
		printw("　　■ ■ ■ ■ ");
		move(19, 108);
		printw("　　■ ■ ■ ■ ");
		move(20, 108);
		printw("　　　■ ■ ■ ");
		move(21, 108);
		printw("　■ ■ ■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ ■ 　　　");
        move(19, 0);
        printw("■ ■ ■ ■ ■ 　");
        move(20, 0);
        printw("■ ■ 　　　　");
        move(21, 0);
        printw("■ ■ ■ ■ 　　");

                move(18, 108);
		printw("　　　■ ■ ■ ");
		move(19, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(20, 108);
		printw("　　　　■ ■ ");
		move(21, 108);
		printw("　　■ ■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ 　　　　");
        move(19, 0);
        printw("■ ■ ■ ■ ■ ■ ");
        move(20, 0);
        printw("■ ■ ■ 　　　");
        move(21, 0);
        printw("■ ■ ■ 　　　");

                move(18, 108);
		printw("　　　　■ ■ ");
		move(19, 108);
		printw("■ ■ ■ ■ ■ ■ ");
		move(20, 108);
		printw("　　　■ ■ ■ ");
		move(21, 108);
		printw("　　　■ ■ ■ ");
		refresh();
		usleep(60000);

        move(18, 0);
        printw("■ ■ ■ 　　　");
        move(19, 0);
        printw("■ ■ ■ ■ ■ 　");
        move(20, 0);
        printw("■ ■ ■ ■ 　　");
        move(21, 0);
        printw("■ ■ 　　　　");

                move(18, 108);
		printw("　　　■ ■ ■ ");
		move(19, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(20, 108);
		printw("　　■ ■ ■ ■ ");
		move(21, 108);
		printw("　　　　■ ■ ");
		refresh();
		usleep(60000);
		
		move(18, 0);
        printw("■ ■ ■ ■ 　　");
        move(19, 0);
        printw("■ ■ ■ ■ 　　");
        move(20, 0);
        printw("■ ■ ■ ■ ■ 　");
        move(21, 0);
        printw("■ ■ ■ 　　　");

                move(18, 108);
		printw("　　■ ■ ■ ■ ");
		move(19, 108);
		printw("　　■ ■ ■ ■ ");
		move(20, 108);
		printw("　■ ■ ■ ■ ■ ");
		move(21, 108);
		printw("　　　■ ■ ■ ");
		refresh();
		usleep(60000);
	}

	refresh();
	getch();
	endwin();
}

