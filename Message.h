/**
 * @author Noah Lindsey
 * @date September 2019
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "defines.h"

using namespace std;

class Message {
    public:
        Message(char messageType);
        Message(char messageType, int row, int col, string str);
        Message(char messageType, int row, int col, string str, Direction direction, int length);

        void setMessage(char messageType, int row, int col, string str, Direction dir, int length);

        void setMessageType(char messageType);
        char getMessageType();

        void setRow(int row);
        int getRow();

        void setCol(int col);
        int getCol();

        void setString(string str);
        string getString();

        void setDirection(Direction dir);
        Direction getDirection();

        void setLength(int length);
        int getLength();

    private:
        char messageType;
        int row;
        int col;
        int length;
        string str;
        Direction dir;
};

#endif
