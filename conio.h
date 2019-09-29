/**
 * @author Noah Lindsey
 * @date September 2019
 * Declarations for the console I/O functions namespace
 */

#ifndef CONIO_H
#define CONIO_H

#include <iostream>
#include <sstream>

using namespace std;

// The conio namespace is a collection of functions that perform console I/O functions
namespace conio {
    // Lists all the valid foreground and background colors
    enum Color {
        // Normal intesity
        BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35,
        CYAN = 36, LIGHT_GRAY = 37, RESET = 39,
        // High intensity
        GRAY = 90, LIGHT_RED = 91, LIGHT_GREEN = 92, LIGHT_YELLOW = 93, LIGHT_BLUE = 94,
        LIGHT_MAGENTA = 95, LIGHT_CYAN = 96, WHITE = 97
    };

    // Lists all the valid text styles
    enum TextStyle { BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4, SLOW_BLINK = 5, RAPID_BLINK = 6,
                NEGATIVE_IMAGE = 7, CONCEAL = 8, DOUBLE_UNDERLINE = 21, NORMAL_INTENSITY = 22, REVEAL = 28 };

    // CSI C-string is at the start of all the escape code sequences
    const char CSI[] = {'\033', '[', '\0'};

    string gotoRowCol(const int c, const int y);
    string fgColor(Color c);
    string bgColor(Color c);
    string setTextStyle(TextStyle ts);
    string resetAll();
    string clrscr();
}

#endif
