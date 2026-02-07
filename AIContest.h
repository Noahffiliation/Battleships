/**
 * @author Noah Lindsey
 * @date September 2019
 * Main diver for Battleships implementation
 */

#ifndef AICONTEST_H
#define AICONTEST_H

#include <iomanip>
#include <iostream>
#include <unistd.h>

#include "Board.h"
#include "Message.h"
#include "Player.h"
#include "conio.h"
#include "defines.h"
#include <memory>

using namespace std;
using namespace conio;

class AIContest {
public:
  AIContest(Player *player1, const string &player1Name, Player *player2,
            const string &player2Name, int boardSize, bool silent);
  ~AIContest() = default;
  void play(float secondsDelay, int &totalMoves, bool &player1Won,
            bool &player2Won);

private:
  enum Side { Left = 1, Right = 2 };
  bool placeShips(Player *player, Board *board) const;
  struct HighlightOptions {
    bool highlightRecent;
    int row;
    int col;
  };
  void showBoard(Board *board, bool ownerVIew, const string &playerName,
                 bool fullRedraw, Side side,
                 HighlightOptions highlightOptions) const;
  void clearScreen();
  void updateAI(Player *player, Board *board, int hitRow, int hitCol);
  void snooze(float seconds);
  bool processShot(const string &playerName, Player *player, Board *board,
                   Side side, int row, int col, Player *otherPlayer);

  Player *player1;
  Player *player2;
  std::unique_ptr<Board> player1Board;
  std::unique_ptr<Board> player2Board;
  string player1Name;
  string player2Name;
  int boardSize;
  bool silent;
  bool player1Won;
  bool player2Won;
  int NumShips;
  static const int MAX_SHIPS = 6;
  string shipNames[MAX_SHIPS];
  int shipLengths[MAX_SHIPS];
  int numShips;
};

#endif
