/**
 * @author Noah Linsey
 * @date September 2019
 * Declaration source file for Board
 */

#ifndef BOARD_H
#define BOARD_H

#include <iostream>

#include "defines.h"

using namespace std;

class Board {
    public:
        Board(int width);
        Board(const Board& other);
        void operator=(const Board& other);
        bool placeShip(int row, int col, int length, Direction dir);
        char getOpponentView(int row, int col);
        char getOwnerView(int row, int col);
        char getShipMark(int row, int col);
        char processShot(int row, int col);
        bool hasWon();

        const static int MaxBoardSize = 10;
    
    private:
        void initialize(char board[MaxBoardSize][MaxBoardSize]);
        bool positionOk(int row, int col, int length, bool horiz);
        void markShip(int row, int col, int length, bool horiz);
        bool isSunk(int row, int col);
        void markSunk(int row, int col);

        // Tracks the placed ships and shots
        char shipBoard[MaxBoardSize][MaxBoardSize];
        // Tracks where ships are through whole game
        char shotBoard[MaxBoardSize][MaxBoardSize];
        // Tracks how many rows/cols are actually being used
        int boardSize;

        // Used to mark the ships on shipBoard
        char shipMark;
};

#endif
