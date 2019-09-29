/**
 * @author Noah Lindsey
 * @date September 2019
 */

#ifndef DUMBPLAYER_H
#define DUMBPLAYER_H

#include "Player.h"
#include "Message.h"
#include "defines.h"

using namespace std;

class DumbPlayer: public Player {
    public:
        DumbPlayer(int boardSize);
        ~DumbPlayer();
        void newRound();
        Message placeShip(int length);
        Message getMove();
        void update(Message msg);

    private:
	    void initializeBoard();
        int lastRow;
        int lastCol;
	    int numShipsPlaced;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
