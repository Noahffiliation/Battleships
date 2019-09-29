/**
 * @author Noah Lindsey
 * @date September 2019
 */

#ifndef SmarterPlayer_H
#define SmarterPlayer_H

using namespace std;

#include "Player.h"
#include "Message.h"
#include "defines.h"

class SmarterPlayer: public Player {
    public:
        SmarterPlayer( int boardSize );
        ~SmarterPlayer();
        void newRound();
        int getNeighbors(int row, int col);
        bool checkShot(int row, int col);
        bool validShot(int row, int col);
        Message placeShip(int length);
        Message getMove();
        int* searchAndDestroy(int lastRow, int lastCol);
	    void update(Message msg);
	
    private:
        void initializeBoard();
        bool canPlaceShip(int length, Direction dir, int row, int col);
        void placeOnBoard(int length, Direction dir, int row, int col);
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        char shipBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        int shotMatrix[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        int lastRow;
        int lastCol;
	    int numShipsPlaced;
};

#endif
