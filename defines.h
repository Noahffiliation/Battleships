/**
 * @author Noah Lindsey
 * @date September 2019
 * Lists globally defined values
 */

#ifndef DEFINES_H
#define DEFINES_H

// Shot related
const char SHOT = '@';
const char MISS = '*';
const char DUPLICATE_SHOT = '!';
const char HIT = 'X';
const char KILL = 'K';
const char OPPONENT_SHOT = 'O';
const char SHIP = 'S';
const char WATER = '~';
const char INVALID_SHOT = char(0);

// Meta information - win/lose/quit
const char WIN = 'W';
const char LOSE = 'L';
const char TIE = 'T';
const char QUIT = 'Q';
const char PEEK = 'P';
const char PLACE_SHIP = 'Z';

const int MAX_BOARD_SIZE = 10;
const int MAX_SHIP_SIZE = 5;
const int MIN_SHIP_SIZE = 3;

enum Direction {None = 0, Horizontal = 1, Vertical = 2};

#endif
