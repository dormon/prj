#include<ncurses.h>

int main()
{
int ch;

/* Curses Initialisations */
initscr();
raw();
keypad(stdscr, TRUE);
noecho();

printw("Press E to Exit\n");

while((ch = getch()) != 'E')
{
    switch(ch)
    {
    case KEY_UP:         printw("\nUp Arrow");
                break;
    case KEY_DOWN:      printw("\nDown Arrow");
                break;
    case KEY_LEFT:      printw("\nLeft Arrow");
                break;
    case KEY_RIGHT:     printw("\nRight Arrow");
                break;
    default:    
                printw("\nThe pressed key is %c",ch);

    }
}

printw("\n\Exiting Now\n");
endwin();

return 0;
}
