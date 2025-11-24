#include "Editor.hpp"

#include "Keys.hpp"

#include <algorithm>
#include <fstream>
#include <ncurses.h>

#define all(arr) (arr).begin(), (arr).end()

Editor::Editor()
{
    if (lines.empty())
    {
        lines.emplace_back("");
    }
    draw();
}

void Editor::update(int c)
{
    handleInput(c);
    draw();
}

// ================ DRAW ================

void Editor::draw() const
{
    clear();
    drawLines();
    drawStatusBar();
    positionCursor();
    refresh();
}

void Editor::drawLines() const
{
    for (size_t i = 0; i < lines.size(); ++i)
    {
        mvprintw(static_cast<int>(i), 0, "%s", lines[i].c_str());
    }
}

void Editor::drawStatusBar() const
{
    int height, width;
    getmaxyx(stdscr, height, width);

    attron(A_REVERSE);

    mvprintw(
        height - 1, 0,
        " Ln %d, Col %d | Lines %zu | Ctrl + X: Exit | Ctrl + S: Save ",
        cursorY + 1, cursorX + 1, lines.size()
    );

    hline(' ', width - getcurx(stdscr));

    attroff(A_REVERSE);
}

void Editor::positionCursor() const
{
    move(cursorY, cursorX);
}

// ================ INPUT CHECKS ================

bool Editor::isBackspace(int key)
{
    return std::find(all(Keys::BACKSPACE_KEYS), key) != Keys::BACKSPACE_KEYS.end();
}

bool Editor::isEnter(int key)
{
    return std::find(all(Keys::ENTER_KEYS), key) != Keys::ENTER_KEYS.end();
}

bool Editor::isArrowKey(int key)
{
    return key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT;
}

bool Editor::isPrintable(int key)
{
    return key >= 32 && key <= 126;
}

bool Editor::isSave(int key)
{
    return key == Keys::SAVE;
}

// ================ TODO ================

void Editor::handleInput(int key)
{
    if (isBackspace(key))
    {
        handleBackspace();
    }
    else if (isEnter(key))
    {
        handleEnter();
    }
    else if (isArrowKey(key))
    {
        handleArrow(key);
    }
    else if (isSave(key))
    {
        saveContent();
    }
    else if (isPrintable(key))
    {
        handleChar(key);
    }
}

void Editor::handleEnter()
{
    std::string remainingText = lines[cursorY].substr(cursorX);
    lines[cursorY] = lines[cursorY].substr(0, cursorX);

    cursorY++;
    lines.insert(lines.begin() + cursorY, remainingText);

    cursorX = 0;
}

void Editor::handleBackspace()
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

void Editor::handleChar(int c)
{
    lines[cursorY].insert(cursorX, 1, static_cast<char>(c));
    cursorX++;
}

void Editor::saveContent() const
{
    std::ofstream file("output.txt");
    if (!file.is_open())
    {
        return;
    }
    for (auto& line : lines)
    {
        file << line << std::endl;
    }
}


void Editor::handleArrow(int key)
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

void Editor::moveCursorUp()
{
    if (cursorY > 0)
    {
        cursorY--;
        cursorX = std::min(cursorX, static_cast<int>(lines[cursorY].length()));
    }
}

void Editor::moveCursorDown()
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

void Editor::moveCursorLeft()
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

void Editor::moveCursorRight()
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