#ifndef KILOR_EDITOR_HPP
#define KILOR_EDITOR_HPP

#include <string>
#include <vector>

class Editor
{
  public:
    Editor();
    void update(int c);

  private:
    int cursorX{ 0 }; ///< Позиція курсора по горизонталі.
    int cursorY{ 0 }; ///< Позиція курсора по вертикалі.
    std::vector<std::string> lines; ///< Основний буфер редактора.

    void draw() const;
    void drawLines() const;
    void drawStatusBar() const;
    void positionCursor() const;

    void handleInput(int c);

    static bool isBackspace(int key);
    static bool isEnter(int key);
    static bool isPrintable(int key);
    static bool isArrowKey(int key);
    static bool isSave(int key);

    void handleEnter();
    void handleBackspace();
    void handleChar(int c);
    void saveContent() const;

    void handleArrow(int key);
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
};

#endif // KILOR_EDITOR_HPP
