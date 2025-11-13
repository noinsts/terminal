#include <ncurses.h>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <fstream>

#define all(v) (v).begin(), (v).end()

namespace Keys
{
    constexpr int EXIT = 24; // Ctrl + X
    constexpr int SAVE = 19; // Ctrl + S
    constexpr std::array<int, 2> ENTER_KEYS = { 13, 10 };
    constexpr std::array<int, 4> BACKSPACE_KEYS = { KEY_BACKSPACE, 8, 127, 7 };
}

class Editor
{

public:
    Editor()
    {
        if (lines.empty())
            lines.emplace_back("");
        draw();
    }

    void update(int c)
    {
        handleInput(c);
        draw();
    }

private:
    int cursorX{ 0 };                       // X-координата курсору
    int cursorY{ 0 };                       // Y-координата курсору
    std::vector<std::string> lines;         // Надрукований текст

    // === Відображення ===

    void draw() const
    {
        clear();
        drawLines();
        drawStatusBar();
        positionCursor();
        refresh();
    }

    void drawLines() const
    {
        for (size_t i = 0; i < lines.size(); ++i)
        {
            mvprintw(static_cast<int>(i), 0, "%s", lines[i].c_str());
        }
    }

    void drawStatusBar() const
    {
        int height, width;
        getmaxyx(stdscr, height, width);

        attron(A_REVERSE);
        mvprintw(
            height - 1, 0,
            " Ln %d, Col %d | Lines %zu | Ctrl + X: Exit | Ctrl + S: Save ",
            cursorY + 1, cursorX + 1, lines.size()
        );

        for (size_t i = getcurx(stdscr); i < width; i++)
        {
            addch(' ');
        }

        attroff(A_REVERSE);
    }

    void positionCursor() const
    {
        move(cursorY, cursorX);
    }

    // === Обробка введення ===

    void handleInput(int c) {
        if (isBackspace(c))
        {
            handleBackspace();
        }
        else if (isEnter(c))
        {
            handleEnter();
        }
        else if (isArrowKey(c))
        {
            handleArrow(c);
        }
        else if (isSave(c))
        {
            saveContent();
        }
        else if (isPrintable(c))
        {
            handleChar(c);
        }
    }

    // === Перевірки типу клавіші ===

    static bool isBackspace(int key)
    {
        return std::find(all(Keys::BACKSPACE_KEYS), key) != Keys::BACKSPACE_KEYS.end();
    }

    static bool isEnter(int key)
    {
        return std::find(all(Keys::ENTER_KEYS), key) != Keys::ENTER_KEYS.end();
    }

    static bool isPrintable(int key)
    {
        return key >= 32 && key <= 126;
    }

    static bool isArrowKey(int key)
    {
        return key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT;
    }

    static bool isSave(int key)
    {
        return key == Keys::SAVE;
    }

    // === Дії з текстом ===

    void handleEnter()
    {
        std::string remainingText = lines[cursorY].substr(cursorX);
        lines[cursorY] = lines[cursorY].substr(0, cursorX);

        cursorY++;
        lines.insert(lines.begin() + cursorY, remainingText);

        cursorX = 0;
    }

    void handleBackspace()
    {
        if (cursorX > 0)
        {
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
        }
        else if (cursorY > 0)
        {
            cursorX = static_cast<int>(lines[cursorY - 1].length());
            lines[cursorY - 1] += lines[cursorY];
            lines.erase(lines.begin() + cursorY);
            cursorY--;
        }
    }

    void handleChar(int c)
    {
        lines[cursorY].insert(cursorX, 1, static_cast<char>(c));
        cursorX++;
    }

    // === Зберігання тексту ===

    void saveContent() const
    {
        std::ofstream file("output.txt");
        for (auto& line : lines)
        {
            file << line << std::endl;
        }
    }

    // === Переміщення курсора ===

    void handleArrow(int key)
    {
        switch (key)
        {
            case KEY_UP:    moveCursorUp(); break;
            case KEY_DOWN:  moveCursorDown(); break;
            case KEY_LEFT:  moveCursorLeft(); break;
            case KEY_RIGHT: moveCursorRight(); break;
            default: break;
        }
    }

    void moveCursorUp()
    {
        if (cursorY > 0)
        {
            cursorY--;
            cursorX = std::min(cursorX, static_cast<int>(lines[cursorY].length()));
        }
    }

    void moveCursorDown()
    {
        if (cursorY < static_cast<int>(lines.size()) - 1)
        {
            cursorY++;
            cursorX = std::min(cursorX, static_cast<int>(lines[cursorY].length()));
        }
        else
        {
            cursorX = static_cast<int>(lines[cursorY].length());
        }
    }

    void moveCursorLeft()
    {
        if (cursorX > 0)
        {
            cursorX--;
        }
        else if (cursorY > 0)
        {
            cursorY--;
            cursorX = static_cast<int>(lines[cursorY].length());
        }
    }

    void moveCursorRight()
    {
        if (cursorX < static_cast<int>(lines[cursorY].length()))
        {
            cursorX++;
        }
        else if (cursorY < static_cast<int>(lines.size()) - 1)
        {
            cursorY++;
            cursorX = std::min(cursorX, static_cast<int>(lines[cursorY].length()));
        }
    }
};

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