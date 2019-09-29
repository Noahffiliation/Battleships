/**
 * @author Noah Lindsey
 * @date September 2019
 *
 * This Battleships AI is very simple and does nothing beyond playing
 * a legal game. However, that makes it a good starting point for writing
 * a more sophisticated AI.
 */

#include <iostream>
#include <cstdio>

#include "conio.h"
#include "DumbPlayer.h"

using namespace conio;

DumbPlayer::DumbPlayer(int boardSize):Player(boardSize) {}

DumbPlayer::~DumbPlayer() {}

void DumbPlayer::initializeBoard() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            this->board[row][col] = WATER;
        }
    }
}

Message DumbPlayer::getMove() {
    lastCol++;
    if (lastCol >= boardSize) {
        lastCol = 0;
        lastRow++;
    }
    if (lastRow >= boardSize) {
        lastCol = 0;
        lastRow = 0;
    }

    Message result(SHOT, lastRow, lastCol, "Bang", None, 1);
    return result;
}

void DumbPlayer::newRound() {
    this->lastRow = 0;
    this->lastCol = -1;
    this->numShipsPlaced = 0;

    this->initializeBoard();
}

Message DumbPlayer::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
    Message response(PLACE_SHIP, numShipsPlaced, 0, shipName, Horizontal, length);
    numShipsPlaced++;

    return response;
}

void DumbPlayer::update(Message msg) {
    switch (msg.getMessageType()) {
        case HIT:
        case KILL:
        case MISS:
            board[msg.getRow()][msg.getCol()] = msg.getMessageType();
            break;
        case WIN:
        case LOSE:
        case TIE:
        case OPPONENT_SHOT:
            break;
    }
}
