#ifndef PLAYER_H
#define PLAYER_H

#include "Message.h"
#include "defines.h"

class Player {
public:
  Player(int boardSize);
  virtual ~Player() {};

  // Tells the player to prepare for a new round
  virtual void newRound();

  // Gets the player's move choice
  virtual Message getMove() = 0;

  // Inform the player of the results of the moves
  virtual void update(Message msg);

  // Inform the game controller of ship placement
  virtual Message placeShip(int length) = 0;

protected:
  virtual void initializeBoard();

  int boardSize;
  int lastRow;
  int lastCol;
  int numShipsPlaced;
  char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
