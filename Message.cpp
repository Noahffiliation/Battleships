/**
 * @author Noah Lindsey
 * @date Spetember 2019
 * Implementation of the Message object
 */

#include "Message.h"

Message::Message(char messageType) {
    this->messageType = messageType;
    this->row = -1;
    this->col = -1;
    this->str = "";
    this->dir = Horizontal;
    this->length = -1;
}

Message::Message(char messageType, int row, int col, string str) {
    setMessage(messageType, row, col, str, None, 0);
}

Message::Message(char messageType, int row, int col, string str, Direction dir, int length) {
    this->messageType = messageType;
    this->row = row;
    this->col = col;
    this->str = str;
    this->dir = dir;
    this->length = length;
}

void Message::setMessage(char messageType, int row, int col, string str, Direction dir, int length) {
    this->messageType = messageType;
    this->row = row;
    this->col = col;
    this->str = str;
    this->dir = dir;
    this->length = length;
}

void Message::setMessageType(char messageType) {
    this->messageType = messageType;
}

char Message::getMessageType() {
    return this->messageType;
}

void Message::setRow(int row) {
    this->row = row;
}

int Message::getRow() {
    return this->row;
}

void Message::setCol(int col) {
    this->col = col;
}

int Message::getCol() {
    return this->col;
}

void Message::setString(string str) {
    this->str = str;
}

string Message::getString() {
    return this->str;
}

void Message::setDirection(Direction dir) {
    this->dir = dir;
}

Direction Message::getDirection() {
    return this->dir;
}

void Message::setLength(int length) {
    this->length = length;
}

int Message::getLength() {
    return this->length;
}
