/**
 * @author Noah Lindsey
 * @date September 2019
 * Main diver for Battleships implementation
 */

#ifndef AICONTEST_H
#define AICONTEST_H

#include <iostream>
#include <unistd.h>
#include <iomanip>

#include "Message.h"
#include "Board.h"
#include "Player.h"
#include "defines.h"
#include "conio.h"

using namespace std;
using namespace conio;

class AIContest {
    public:
        AIContest(Player* player1, string player1Name, Player* player2, string player2Name, int boardSize, bool silent);
        ~AIContest();
        void play(float secondsDelay, int& totalMoves, bool& player1Won, bool& player2Won);

    private:
        enum Side {Left = 1, Right = 2};
        bool placeShips(Player* player, Board* board);
        void showBoard(Board* board, bool ownerVIew, string playerName, bool fullRedraw, Side side, bool hLMostRecentShot, int hLRow, int hLCol);
        void clearScreen();
        void updateAI(Player* player, Board* board, int hitRow, int hitCol);
        void snooze(float seconds);
        bool processShot(string playerName, Player* player, Board* board, Side side, int row, int col, Player* otherPlayer);
        
        Player* player1;
        Player* player2;
        Board* player1Board;
        Board* player2Board;
        string player1Name;
        string player2Name;
        int boardSize;
        bool silent;
        bool player1Won;
        bool player2Won;
        int NumShips;
        static const int MAX_SHIPS = 6;
        string shipNames[MAX_SHIPS];
        int shipLengths[MAX_SHIPS];
        int numShips;
};

#endif
