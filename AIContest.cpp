/**
 * @author Noah Lindsey
 * @date September 2019
 * Main driver for Battleships implementation
 */

#include "AIContest.h"
#include "Random.h"
#include <iomanip>
#include <iostream>
#include <unistd.h>

// Set up the game for a contest between two players
AIContest::AIContest(Player *player1, const string &player1Name,
                     Player *player2, const string &player2Name, int boardSize,
                     bool silent)
    : player1(player1), player2(player2),
      player1Board(std::make_unique<Board>(boardSize)),
      player2Board(std::make_unique<Board>(boardSize)),
      player1Name(player1Name), player2Name(player2Name), boardSize(boardSize),
      silent(silent), player1Won(false), player2Won(false),
      numShips((boardSize - 2 > MAX_SHIPS) ? MAX_SHIPS : (boardSize - 2)) {

  // Ship stuff
  shipNames[0] = "Submaraine";
  shipNames[1] = "Destroyer";
  shipNames[2] = "Aircraft Carrier";
  shipNames[3] = "Destroyer 2";
  shipNames[4] = "Submarine 2";
  shipNames[5] = "Aircraft Carrier 2";

  for (int i = 0; i < numShips; i++) {
    shipLengths[i] = Random::get_int(3, MIN_SHIP_SIZE + 2);
  }
}

// Places the ships
bool AIContest::placeShips(Player *player, Board *board) const {
  for (int i = 0; i < numShips; i++) {
    Message loc = player->placeShip(shipLengths[i]);
    bool placedOk = board->placeShip(loc.getRow(), loc.getCol(), shipLengths[i],
                                     loc.getDirection());
    if (!placedOk) {
      cerr << "Error: couldn't place " << shipNames[i] << " (length "
           << shipLengths[i] << ")" << endl;
      return false;
    }
  }

  // All ships placed ok
  return true;
}

// Show the player's boards with shot data
void AIContest::showBoard(Board *board, bool ownerView,
                          const string &playerName, bool fullRedraw, Side side,
                          HighlightOptions highlightOptions) const {
  if (silent) {
    return;
  }

  // 'fullRedraw indicates fill board draw or obly an update
  // 'side' (Left / Right) indicates where to draw the board

  char ch;
  int boardColOffset = side == Left ? 1 : 50;

  cout << gotoRowCol(1, boardColOffset) << playerName << flush;
  // Top row
  cout << gotoRowCol(3, boardColOffset) << " |";
  for (int count = 0; count < boardSize; count++) {
    cout << count;
  }
  cout << flush;

  cout << gotoRowCol(4, boardColOffset) << flush;
  // Put out horizontal header line
  for (int col = 0; col < boardSize; col++) {
    cout << '-';
  }
  cout << flush;

  for (int row = 0; row < boardSize; row++) {
    cout << gotoRowCol(5 + row, boardColOffset) << (char)(row + 'A') << "|";
    for (int col = 0; col < boardSize; col++) {
      if (ownerView) {
        ch = board->getOwnerView(row, col);
      } else {
        ch = board->getOpponentView(row, col);
      }

      switch (ch) {
      case SUNK:
        cout << fgColor(BLACK) << bgColor(LIGHT_RED);
        break;
      case HIT:
        cout << fgColor(BLACK) << bgColor(LIGHT_MAGENTA);
        break;
      case MISS:
        cout << fgColor(BLACK) << bgColor(GRAY);
        break;
      case WATER:
        cout << fgColor(BLACK) << bgColor(LIGHT_CYAN);
        break;
      }

      if (ch >= 'a' && ch <= 'k') {
        cout << setTextStyle(NEGATIVE_IMAGE) << ch << resetAll() << flush;
      } else if (highlightOptions.highlightRecent &&
                 highlightOptions.row == row && highlightOptions.col == col) {
        cout << setTextStyle(NEGATIVE_IMAGE) << ch
             << setTextStyle(NEGATIVE_IMAGE) << resetAll() << flush;
      } else {
        cout << ch << resetAll() << flush;
      }
    }
    cout << resetAll() << flush;
  }
  cout << resetAll() << flush;
}

// Clears the screen
void AIContest::clearScreen() {
  if (silent) {
    return;
  }
  cout << clrscr() << endl;
}

// Snooze updating
void AIContest::snooze(float seconds) {
  // usleep() takes argument in microseconds, so need to convert seconds to
  // microseconds
  long sleepTime = long(1000000 * seconds);
  usleep(sleepTime);
}

// Mark all sections of a ship the same
void AIContest::updateAI(Player *player, Board *board, int hitRow, int hitCol) {
  Message killMsg(SUNK, -1, -1, "");
  char shipMark = board->getShipMark(hitRow, hitCol);

  for (int row = 0; row < boardSize; row++) {
    for (int col = 0; col < boardSize; col++) {
      if (board->getShipMark(row, col) == shipMark) {
        killMsg.setRow(row);
        killMsg.setCol(col);
        player->update(killMsg);
      }
    }
  }
}

// Determine shot types against each player
bool AIContest::processShot(const string &playerName, Player *player,
                            Board *board, Side side, int row, int col,
                            Player *otherPlayer) {
  bool won = false;
  int resultsRow = 16;
  int shotColOffset = side == Right ? 1 : 50;
  int boardColOffset = side == Left ? 1 : 50;

  // Wipe any previous contents clean first
  if (!silent)
    cout << gotoRowCol(resultsRow, shotColOffset) << playerName << "'s shot: ["
         << row << ", " << col << "]" << endl;
  Message msg = board->processShot(row, col);
  // Board doesn't set these properly
  msg.setRow(row);
  msg.setCol(col);

  switch (msg.getMessageType()) {
  case MISS:
    if (!silent) {
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << "Miss" << flush;
    }
    player->update(msg);
    break;
  case HIT:
    if (!silent) {
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << "Hit" << flush;
    }
    player->update(msg);
    break;
  case SUNK:
    if (!silent) {
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << "It's a SUNK!"
           << msg.getString() << flush;
    }
    // Notify that it's a hit
    msg.setMessageType(HIT);
    player->update(msg);
    msg.setMessageType(SUNK);
    // Notify that all segments of ship are now a SUNK
    updateAI(player, board, row, col);

    // Chance to win after every kill
    won = board->hasWon();
    break;
  case DUPLICATE_SHOT:
    if (!silent) {
      cout << fgColor(RED) << bgColor(WHITE);
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset)
           << "You already shot there.";
      cout << resetAll() << flush;
    }
    player->update(msg);
    break;
  case INVALID_SHOT:
    if (!silent) {
      cout << fgColor(RED) << bgColor(WHITE);
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << playerName
           << "Invalid coordinates: [row = " << row << ", col = " << col << "]"
           << flush;
      cout << resetAll() << flush;
    }
    player->update(msg);
    break;
  default:
    if (!silent) {
      cout << fgColor(RED) << bgColor(WHITE);
      cout << gotoRowCol(resultsRow + 1, boardColOffset) << setw(30) << "";
      cout << gotoRowCol(resultsRow + 1, boardColOffset)
           << "Invalid return from processShot: " << msg.getMessageType() << "("
           << msg.getString() << ")";
      cout << resetAll() << flush;
    }
    player->update(msg);
    break;
  }

  // Notify the other player of the shot
  msg.setMessageType(OPPONENT_SHOT);
  otherPlayer->update(msg);

  return won;
}

// Play the game and determine winner
void AIContest::play(float secondsDelay, int &totalMoves, bool &player1Won,
                     bool &player2Won) {
  int maxShots = boardSize * boardSize * 2;
  totalMoves = 0;
  clearScreen();

  // Validate ship placements
  if (!placeShips(player1, player1Board.get())) {
    cout << endl;
    cout << player1Name << " placed ship in invalid location and forfeits game."
         << endl;
    cout << endl;
    snooze(secondsDelay * 4);
    player2Won = true;
  }
  if (!placeShips(player2, player2Board.get())) {
    cout << endl;
    cout << player2Name << " placed ship in invalid location and forfeits game."
         << endl;
    cout << endl;
    snooze(secondsDelay * 4);
    player1Won = true;
  }

  // Keep processing shots until a player wins or run out of moves
  while (!(player1Won || player2Won) && totalMoves < maxShots) {
    Message shot1 = player1->getMove();
    player1Won = processShot(player1Name, player1, player2Board.get(), Left,
                             shot1.getRow(), shot1.getCol(), player2);
    Message shot2 = player2->getMove();
    player2Won = processShot(player2Name, player2, player1Board.get(), Right,
                             shot2.getRow(), shot2.getCol(), player1);

    if (!silent) {
      showBoard(player1Board.get(), false, player1Name + "'s Board", false,
                Left, {true, shot2.getRow(), shot2.getCol()});
      showBoard(player2Board.get(), false, player2Name + "'s Board", false,
                Right, {true, shot1.getRow(), shot1.getCol()});
    }

    totalMoves++;

    if (!silent && secondsDelay > 0)
      snooze(secondsDelay);
  }

  // Show final board results
  if (!silent) {
    clearScreen();
    showBoard(player1Board.get(), true,
              "Final status of " + player1Name + "'s board", true, Left,
              {false, -1, -1});
    cout << endl;
    showBoard(player2Board.get(), true,
              "Final status of " + player2Name + "'s board", true, Right,
              {false, -1, -1});
    cout << endl;
  }

  // Determine win/loss/tie
  if (player1Won && player2Won) {
    cout << "The game was a tie. Both players sunk all ships." << endl;
    Message msg(TIE);
    player1->update(msg);
    player2->update(msg);
  } else if (player1Won) {
    cout << player1Name << " won." << endl;
    Message msg(WIN);
    player1->update(msg);
    msg.setMessageType(LOSE);
    player2->update(msg);
  } else if (player2Won) {
    cout << player2Name << " won." << endl;
    Message msg(WIN);
    player2->update(msg);
    msg.setMessageType(LOSE);
    player1->update(msg);
  } else {
    cout << "The game was a tie. Neither player sunk all ships." << endl;
    Message msg(LOSE);
    player1->update(msg);
    player2->update(msg);
  }

  // Show shot stats
  cout << "--- (Moves = " << totalMoves << ", percentage of board shot at = "
       << (100.0 * (float)totalMoves) / (boardSize * boardSize) << "%.)"
       << endl;
  cout << endl;
  if (!silent)
    snooze(5);
}
