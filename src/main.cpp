#include "Editor.hpp"
#include "Keys.hpp"

#include <ncurses.h>

int main()
{
    initscr();                  // Ініціалізація ncurses.
    raw();                      // Вхід в режим RAW.
    keypad(stdscr, TRUE);       // Ввімкнення стрілок та спец. клавіш.
    noecho();                   // Не виводить натиснуті клавіші.

    Editor editor;

    int c;
    while ((c = getch()) != Keys::EXIT)
        editor.update(c);

    endwin();                   // Вийти з ncurses режиму.
    return 0;
}