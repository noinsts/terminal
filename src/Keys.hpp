//
// Created by andriy on 11/14/25.
//

#ifndef KILOR_KEYS_HPP
#define KILOR_KEYS_HPP

#include <array>
#include <ncurses.h>

namespace Keys
{
constexpr int EXIT = 24; // Ctrl + X
constexpr int SAVE = 19; // Ctrl + S
constexpr std::array<int, 2> ENTER_KEYS = { 13, 10 };
constexpr std::array<int, 4> BACKSPACE_KEYS = { KEY_BACKSPACE, 8, 127, 7 };
} // namespace Keys

#endif // KILOR_KEYS_HPP
