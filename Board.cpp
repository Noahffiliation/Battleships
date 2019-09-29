/**
 * @author Noah Lindsey
 * @date September 2019
 * Shell for the Board implementations
 */

#include "Board.h"

Board::Board(int width) {
    shipMark = 'a';
    boardSize = width;

    initialize(shotBoard);
    initialize(shipBoard);
}

Board::Board(const Board& other) {
    operator=(other);
}

void Board::operator=(const Board& other) {
    // Avoid setting yourself to yourself
    if (this == &other) return;

    shipMark = other.shipMark;
    boardSize = other.boardSize;

    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            shipBoard[row][col] = other.shipBoard[row][col];
            shotBoard[row][col] = other.shotBoard[row][col];
        }
    }
}

bool Board::placeShip(int row, int col, int length, Direction dir) {
    if (positionOk(row, col, length, dir == Horizontal)) {
        markShip(row, col, length, dir == Horizontal);
        return true;
    } else {
        return false;
    }
}

bool Board::positionOk(int row, int col, int length, bool horiz) {
    if (horiz) {
        if (row < 0 || row > boardSize || col < 0 || col + length - 1 >= boardSize) {
            cerr << "positionOk: horizontal position invalid" << endl;
            return false;
        }

        for (int c = col; c < col + length; c++) {
            if (shotBoard[row][c] == SHIP) {
                cerr << "psotionOk: horizontal collision at row = " << row << ", col = " << c << endl;
                return false;
            }
        }
    } else {
        if (col < 0 || col >= boardSize || row < 0 || row + length - 1 > boardSize) {
            cerr << "positionOk: vertical position invalid" << endl;
            return false;
        }

        for (int r = row; r < row + length; r++) {
            if (shotBoard[r][col] == SHIP) {
                cerr << "positionOk: horizontal collision at row = " << r << ", col = " << col << endl;
                return false;
            }
        }
    }

    return true;
}

void Board::markShip(int row, int col, int length, bool horiz) {
    if (horiz) {
        for (int c = col; c < col + length; c++) {
            shipBoard[row][c] = shipMark;
            shotBoard[row][c] = SHIP;
        }
    } else {
        for (int r = row; r < row + length; r++) {
            shipBoard[r][col] = shipMark;
            shotBoard[r][col] = SHIP;
        }
    }

    // Increment shipMark to the next value to denote different ships
    shipMark++;
}

char Board::getOpponentView(int row, int col) {
    char value = shotBoard[row][col];
    switch (value) {
        case HIT:
        case MISS:
        case KILL:
            return value;
        default:
            return WATER;
    }
}

char Board::getOwnerView(int row, int col) {
    if (shotBoard[row][col] == SHIP) return shipBoard[row][col];
    else return shotBoard[row][col];
}

char Board::getShipMark(int row, int col) {
    return shipBoard[row][col];
}

char Board::processShot(int row, int col) {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) return INVALID_SHOT;

    switch (shotBoard[row][col]) {
        case WATER:
            shotBoard[row][col] = MISS;
            return MISS;
        case MISS:
        case HIT:
        case KILL:
            return DUPLICATE_SHOT;
        case SHIP:
            shotBoard[row][col] = HIT;
            if (isSunk(row, col)) {
                markSunk(row, col);
                return KILL;
            }
            else return HIT;
        default:
            cerr << "This didn't happen!!!!" << endl << shotBoard << "[" << row << "][" << col <<"] had value " << shotBoard[row][col] << endl;
            break;
    }

    return MISS;
}

bool Board::isSunk(int row, int col) {
    char mark = shipBoard[row][col];
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (shipBoard[row][col] == mark && shotBoard[row][col] == SHIP) return false;
        }
    }

    return true;
}

void Board::markSunk(int row, int col) {
    char mark = shipBoard[row][col];
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (shipBoard[row][col] == mark) shotBoard[row][col] = KILL;
        }
    }
}

bool Board::hasWon() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (shotBoard[row][col] == SHIP) return false;
        }
    }

    return true;
}

void Board::initialize(char board[MaxBoardSize][MaxBoardSize]) {
    for (int row = 0; row < MaxBoardSize; row++)
        for (int col = 0; col < MaxBoardSize; col++)
            board[row][col] = WATER;
}
