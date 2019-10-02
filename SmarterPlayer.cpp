/**
 * @author Noah Lindsey
 * @date September 2019
 */

#include "SmarterPlayer.h"

SmarterPlayer::SmarterPlayer(int boardSize):Player(boardSize) {}

SmarterPlayer::~SmarterPlayer() {}

void SmarterPlayer::initializeBoard() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            this->board[row][col] = WATER;
            this->shipBoard[row][col] = WATER;
            this->shotMatrix[row][col] = 0;
        }
    }
}

int SmarterPlayer::getNeighbors(int row, int col){
    int sum = 0;
    if (validShot(row - 1, col) && (shipBoard[row - 1][col] != WATER)) sum++;
    if (validShot(row + 1, col) && (shipBoard[row + 1][col] != WATER)) sum++;
    if (validShot(row, col - 1) && (shipBoard[row][col - 1] != WATER)) sum++;
    if (validShot(row, col + 1) && (shipBoard[row][col + 1] != WATER)) sum++;
    if (validShot(row - 1, col + 1) && (shipBoard[row - 1][col + 1] != WATER)) sum++;
    if (validShot(row + 1, col - 1) && (shipBoard[row + 1][col - 1] != WATER)) sum++;
    if (validShot(row - 1, col - 1) && (shipBoard[row - 1][col - 1] != WATER)) sum++;
    if (validShot(row + 1, col + 1) && (shipBoard[row + 1][col + 1] != WATER)) sum++;
    
    return sum;
}

bool SmarterPlayer::canPlaceShip(int shipSize, Direction dir, int row, int col) {
    if (dir == Horizontal) {
	    for (int i = 0; i < shipSize; ++i) if (shipBoard[row][col+i] != WATER || getNeighbors(row, col + i) > 1) return false;
	    return true;
    } else {
        for (int i = 0; i < shipSize; ++i) if (shipBoard[row + i][col] != WATER || getNeighbors(row + i, col) > 1) return false;
        return true;
    }
}

void SmarterPlayer::placeOnBoard(int length, Direction dir, int row, int col) {
    if (dir == Horizontal) for (int i = 0; i < length; ++i) shipBoard[row][col+i] = SHIP;
    else for (int i = 0; i < length; ++i) shipBoard[row+i][col] = SHIP;
}

bool SmarterPlayer::checkShot(int row, int col) {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) return false;
    switch (board[row][col]) {
        case DUPLICATE_SHOT:
        case HIT:
        case KILL:
        case MISS:
            return false;
    }
    return true;
}

bool SmarterPlayer::validShot(int row, int col) {
    if (row < 0 || row >= boardSize) return false;
    if (col < 0 || col >= boardSize) return false;
    if (!checkShot(row, col)) return false;
    return true;
}

int* SmarterPlayer::searchAndDestroy(int row, int col) {
    int *loc = new int[2];
    for (int i = 1; i <= 1; i++) {
        if (validShot(row - i, col)) {
            char spot = board[row-i][col];
            if (spot == MISS || spot == DUPLICATE_SHOT || spot == KILL || spot == HIT) break;
            if (spot == WATER) {
                loc[0] = row - i;
                loc[1] = col;
                return loc;
            }
        }
    }

    for (int i = 1; i <= 1; i++) {
        if (validShot(row+i, col)) {
            char spot = board[row + i][col];
            if (spot == MISS || spot == DUPLICATE_SHOT || spot == KILL || spot == HIT) break;
            if (spot == WATER) {
                loc[0] = row + i;
                loc[1] = col;
                return loc;
            }
        }
    }

    for (int i = 1; i <= 1; i++) {
        if (validShot(row, col-i)) {
            char spot = board[row][col - i];
            if (spot == MISS || spot == DUPLICATE_SHOT || spot == KILL || spot == HIT) break;
            if (spot == WATER) {
                loc[0] = row;
                loc[1] = col - i;
                return loc;
            }
        }
    }

    for (int i = 1; i <= 1; i++) {
        if (validShot(row, col + i)) {
            char spot = board[row][col + i];
            if (spot == MISS || spot == DUPLICATE_SHOT || spot == KILL || spot == HIT) break;
            if (spot == WATER) {
                loc[0] = row;
                loc[1] = col + i;
                return loc;
            }
        }
    }

    loc[0] = -1;
    loc[1] = -1;

    return loc;
}

Message SmarterPlayer::getMove() {
    int *loc;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (board[row][col] == HIT) {
                loc = searchAndDestroy(row, col);
                if (loc[0] != -1) {
                    Message outbound(SHOT, loc[0], loc[1], "Bang!", None, 1);
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
            if (startCol > 2) startCol = 0;
            col = startCol;
        }
        if (row > boardSize) break;
    }

    for (int r = 0; r < boardSize; r++ ) {
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
    this->lastRow = 0;
    this->lastCol = 0;
    this->numShipsPlaced = 0;
    this->initializeBoard();
}

Message SmarterPlayer::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);
    
    while (true) { //random ship placement
        int row;
        int col;
        Direction dir = Direction(rand() % 2 + 1);
        
        if (dir == Horizontal) {
            row = rand() % boardSize;
            col = rand() % ((boardSize - length) + 1);
        } else {
                row = rand() % ((boardSize - length) + 1);
                col = rand() % boardSize;
        }

        if (canPlaceShip(length, dir, row, col)) {
            placeOnBoard(length, dir, row, col);
            Message response(PLACE_SHIP, row, col, shipName, dir, length);
            numShipsPlaced++;
            return response;
        }
    }
}

void SmarterPlayer::update(Message msg) {
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
