/**
 * @author Noah Lindsey
 * @date September 2019
 */

#include "Player.h"

Player::Player(int boardSize) {
  this->boardSize = boardSize;
  if (this->boardSize > MAX_BOARD_SIZE)
    this->boardSize = MAX_BOARD_SIZE;
  if (this->boardSize < 0)
    this->boardSize = 0;

  this->lastRow = 0;
  this->lastCol = -1;
  this->numShipsPlaced = 0;
}

void Player::newRound() {
  this->numShipsPlaced = 0;
  this->initializeBoard();
}

void Player::initializeBoard() {
  for (int row = 0; row < MAX_BOARD_SIZE; row++) {
    for (int col = 0; col < MAX_BOARD_SIZE; col++) {
      this->board[row][col] = WATER;
    }
  }
}

void Player::update(Message msg) {
  switch (msg.getMessageType()) {
  case HIT:
  case SUNK:
  case MISS:
    if (msg.getRow() >= 0 && msg.getRow() < boardSize && msg.getCol() >= 0 &&
        msg.getCol() < boardSize) {
      board[msg.getRow()][msg.getCol()] = msg.getMessageType();
    }
    break;
  default:
    break;
  }
}
