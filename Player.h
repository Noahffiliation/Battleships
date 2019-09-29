/**
 * @author Noah Lindsey
 * @date September 2019
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "Message.h"

class Player {
    public:
        Player(int boardSize);
        // Tells the player to prepare for a new round in a game against the same opponent
        virtual void newRound() = 0;
        // Gets the player's move choice, then returned to the caller
        virtual Message getMove() = 0;
        // Inform the player of the results of the moves by player or opponent
        virtual void update(Message msg) = 0;
        // Inform the game controller of where the player wishes to place a ship
        virtual Message placeShip(int length) = 0;
        virtual ~Player() {};

    protected:
        // Standard data that all players have access to
        int boardSize;

};

#endif
