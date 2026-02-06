#include <cstdio>
#include <iostream>


#include "DumbPlayer.h"
#include "conio.h"


using namespace conio;

DumbPlayer::DumbPlayer(int boardSize) : Player(boardSize) {}

DumbPlayer::~DumbPlayer() {}

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

Message DumbPlayer::placeShip(int length) {
  char shipName[10];
  snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

  Message response(PLACE_SHIP, numShipsPlaced, 0, shipName, Horizontal, length);
  numShipsPlaced++;

  return response;
}
