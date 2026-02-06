#ifndef SmarterPlayer_H
#define SmarterPlayer_H

#include "Message.h"
#include "Player.h"
#include "defines.h"


class SmarterPlayer : public Player {
public:
  SmarterPlayer(int boardSize);
  ~SmarterPlayer();
  void newRound();
  int getNeighbors(int row, int col);
  bool checkShot(int row, int col);
  bool validShot(int row, int col);
  Message placeShip(int length);
  Message getMove();
  int *searchAndDestroy(int lastRow, int lastCol);

private:
  void initializeBoard();
  bool canPlaceShip(int length, Direction dir, int row, int col);
  void placeOnBoard(int length, Direction dir, int row, int col);
  char shipBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  int shotMatrix[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
