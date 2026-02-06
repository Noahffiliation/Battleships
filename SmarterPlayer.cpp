#include "SmarterPlayer.h"
#include "Random.h"
#include <cstdio>
#include <cstdlib>

SmarterPlayer::SmarterPlayer(int boardSize) : Player(boardSize) {}

SmarterPlayer::~SmarterPlayer() {}

void SmarterPlayer::initializeBoard() {
  Player::initializeBoard();
  for (int row = 0; row < MAX_BOARD_SIZE; row++) {
    for (int col = 0; col < MAX_BOARD_SIZE; col++) {
      this->shipBoard[row][col] = WATER;
      this->shotMatrix[row][col] = 0;
    }
  }
}

int SmarterPlayer::getNeighbors(int row, int col) {
  int sum = 0;
  if (validShot(row - 1, col) && (shipBoard[row - 1][col] != WATER))
    sum++;
  if (validShot(row + 1, col) && (shipBoard[row + 1][col] != WATER))
    sum++;
  if (validShot(row, col - 1) && (shipBoard[row][col - 1] != WATER))
    sum++;
  if (validShot(row, col + 1) && (shipBoard[row][col + 1] != WATER))
    sum++;
  if (validShot(row - 1, col + 1) && (shipBoard[row - 1][col + 1] != WATER))
    sum++;
  if (validShot(row + 1, col - 1) && (shipBoard[row + 1][col - 1] != WATER))
    sum++;
  if (validShot(row - 1, col - 1) && (shipBoard[row - 1][col - 1] != WATER))
    sum++;
  if (validShot(row + 1, col + 1) && (shipBoard[row + 1][col + 1] != WATER))
    sum++;

  return sum;
}

bool SmarterPlayer::canPlaceShip(int shipSize, Direction dir, int row,
                                 int col) {
  if (dir == Horizontal) {
    for (int i = 0; i < shipSize; ++i)
      if (shipBoard[row][col + i] != WATER || getNeighbors(row, col + i) > 1)
        return false;
    return true;
  } else {
    for (int i = 0; i < shipSize; ++i)
      if (shipBoard[row + i][col] != WATER || getNeighbors(row + i, col) > 1)
        return false;
    return true;
  }
}

void SmarterPlayer::placeOnBoard(int length, Direction dir, int row, int col) {
  if (dir == Horizontal)
    for (int i = 0; i < length; ++i)
      shipBoard[row][col + i] = SHIP;
  else
    for (int i = 0; i < length; ++i)
      shipBoard[row + i][col] = SHIP;
}

bool SmarterPlayer::checkShot(int row, int col) {
  if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
    return false;
  switch (board[row][col]) {
  case DUPLICATE_SHOT:
  case HIT:
  case SUNK:
  case MISS:
    return false;
  }
  return true;
}

bool SmarterPlayer::validShot(int row, int col) {
  if (row < 0 || row >= boardSize)
    return false;
  if (col < 0 || col >= boardSize)
    return false;
  if (!checkShot(row, col))
    return false;
  return true;
}

int *SmarterPlayer::searchAndDestroy(int row, int col) {
  int targetRow = -1;
  int targetCol = -1;

  // Search in four directions for a valid WATER spot
  int dr[] = {-1, 1, 0, 0};
  int dc[] = {0, 0, -1, 1};

  for (int i = 0; i < 4; i++) {
    int nextRow = row + dr[i];
    int nextCol = col + dc[i];

    if (validShot(nextRow, nextCol)) {
      if (board[nextRow][nextCol] == WATER) {
        targetRow = nextRow;
        targetCol = nextCol;
        break;
      }
    }
  }

  if (targetRow != -1) {
    int *loc = new int[2];
    loc[0] = targetRow;
    loc[1] = targetCol;
    return loc;
  }

  return nullptr;
}

Message SmarterPlayer::getMove() {
  int *loc;
  for (int row = 0; row < boardSize; row++) {
    for (int col = 0; col < boardSize; col++) {
      if (board[row][col] == HIT) {
        loc = searchAndDestroy(row, col);
        if (loc != nullptr) {
          Message outbound(SHOT, loc[0], loc[1], "Bang!", None, 1);
          delete[] loc;
          return outbound;
        }
      }
    }
  }

  int row = 0;
  int col = 0;
  int startCol = 0;

  while (row < boardSize && col < boardSize) {
    if (board[row][col] == WATER) {
      if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
        Message result(SHOT, row, col, "Bang", None, 1);
        return result;
      }
    }
    col += 3;
    if (col >= boardSize) {
      row += 2;
      startCol++;
      if (startCol > 2)
        startCol = 0;
      col = startCol;
    }
    if (row > boardSize)
      break;
  }

  for (int r = 0; r < boardSize; r++) {
    for (int c = 0; c < boardSize; c++) {
      if (checkShot(r, c)) {
        Message result(SHOT, r, c, "Bang", None, 1);
        return result;
      }
    }
  }

  Message result(SHOT, -1, -1, "Bang", None, 1);
  return result;
}

void SmarterPlayer::newRound() {
  Player::newRound();
  this->lastRow = 0;
  this->lastCol = 0;
}

Message SmarterPlayer::placeShip(int length) {
  char shipName[10];
  std::snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

  while (true) { // random ship placement
    int row;
    int col;
    Direction dir = Direction(Random::get_int(1, 2));

    if (dir == Horizontal) {
      row = Random::get_int(0, boardSize - 1);
      col = Random::get_int(0, boardSize - length);
    } else {
      row = Random::get_int(0, boardSize - length);
      col = Random::get_int(0, boardSize - 1);
    }

    if (canPlaceShip(length, dir, row, col)) {
      placeOnBoard(length, dir, row, col);
      Message response(PLACE_SHIP, row, col, shipName, dir, length);
      numShipsPlaced++;
      return response;
    }
  }
}
