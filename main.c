#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <curses.h>
#include <termios.h>
#include <unistd.h>

static const int maxpos = 20;
static int curpos = 0;
static const int pitfall_sz = 2;
static int pitfall;


static void getcmd(void)
{
    int c;

    c = getch();

    if (c == KEY_LEFT && curpos)
    	curpos--;
    else if (c == KEY_RIGHT && curpos < maxpos - 1)
    	curpos++;
}


static void draw(int position)
{
    int pos;

    putchar(' ');
    putchar('|');
    for (pos = 0; pos < maxpos; pos++) {
    	if (pos == position)
    	    putchar('%');
    	else if (pos >= pitfall && pos < pitfall + pitfall_sz)
    	    putchar(' ');
    	else
    	    putchar('_');
    }
    putchar('|');
    putchar('\r');
    refresh();
}


static void fall(void)
{
    const char seq[] = "_,-=\"^";
    int ii, pos;

    for (ii = 0; ii < sizeof(seq); ii++) {
	putchar(' ');
	putchar('|');
	for (pos = 0; pos < maxpos; pos++) {
    	    if (pos == curpos)
    		putchar('%');
    	    else
    		putchar(seq[ii]);
	}
	putchar('|');
	putchar('\r');
	refresh();
	usleep(100000);
    }

    /* draw without the cursor */
    draw(-1);

    printf("\n");
    printf("\r");
}


int main(void)
{
    int st;

    initscr();
    cbreak(); noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();

    pitfall = maxpos - pitfall_sz;

    while (1) {
    	draw(curpos);

    	if (curpos >= pitfall && curpos < pitfall + pitfall_sz) {
    	    fall();
    	    pitfall = rand() % (maxpos - pitfall_sz);
    	} else {
    	    getcmd();
    	}
    }

    return 0;
}
