/** 
 * @author Noah Lindsey
 * @date September 2019
 */ 

#ifndef CONIO_CPP
#define CONIO_CPP

#include "conio.h"

namespace conio {
    // Positions cursor to the specified row, col location
    string gotoRowCol(const int row, const int col) {
        std::ostringstream strm;
        strm << CSI << row << ';' << col << 'H';
        return strm.str();
    }

    const int Foreground = 1;
    const int Background = 2;

    string getColorSequence(Color c, int fgOrBg) {
        int BGOFFSET = 10;
        int offset = 0;
        if (fgOrBg == Background) offset += BGOFFSET; 

        std::ostringstream strm;
        switch (c) {
            case BLACK:
            case RED:
            case GREEN:
            case YELLOW:
            case BLUE:
            case MAGENTA:
            case CYAN:
            case LIGHT_GRAY:
            case RESET:
            case GRAY:
            case LIGHT_RED:
            case LIGHT_GREEN:
            case LIGHT_YELLOW:
            case LIGHT_BLUE:
            case LIGHT_MAGENTA:
            case LIGHT_CYAN:
            case WHITE:
                strm << CSI << Color( c+offset ) << 'm';
                break;
            default:
                strm << "conio: invalid color: " << int(c) << endl;
        }

        return strm.str();
    }

    // Returns a string that contains the escape sequence to set the foreground color to the specified color
    string fgColor(Color c) {
	    return getColorSequence(c, Foreground);
    }

    // Returns a string that contains the escape sequence to set the
    string bgColor(Color c) {
	    return getColorSequence(c, Background);
    }

    // Returns a string that contains the escape sequence to set the text style to the specified TextStyle
    string setTextStyle(TextStyle ts) {
        std::ostringstream strm;
        strm << CSI << ts << 'm';
        return strm.str();
    }


    // Returns a string that contains the escape sequence to reset all the text attributes to default.
    string resetAll() {
        std::ostringstream strm;
        strm << CSI << "0m";
        return strm.str();
    }

    // Returns a string that contains the escape sequence to clear the screen
    string clrscr() {
        std::ostringstream strm;
        strm << CSI << "H" << CSI << "2J";
        return strm.str();
    }

}

#endif
