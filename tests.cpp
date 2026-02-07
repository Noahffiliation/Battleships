#include "AIContest.h"
#include "Board.h"
#include "DumbPlayer.h"
#include "defines.h"
#include <cassert>
#include <cstdio>
#include <iostream>

void testBoard() {
  std::cout << "Testing Board..." << std::endl;
  Board b1(10);

  // Test placement
  assert(b1.placeShip(0, 0, 3, Horizontal));
  assert(!b1.placeShip(0, 0, 3, Horizontal)); // Collision
  assert(b1.placeShip(5, 5, 2, Vertical));

  // Test Copy Constructor
  Board b2(b1);
  assert(b2.getOwnerView(0, 0) == 'a');

  // Test Assignment
  Board b3(5);
  b3 = b1;
  assert(b3.getOwnerView(5, 5) == 'b');

  // Test shooting
  char r1 = b1.processShot(0, 0);
  if (r1 != HIT) {
    std::cerr << "FAILED processShot(0,0): returned '" << r1 << "'(" << (int)r1
              << ") expected '" << HIT << "'(" << (int)HIT << ")" << std::endl;
    std::cerr << "shotBoard[0][0] was: '" << b1.getOpponentView(0, 0) << "'"
              << std::endl;
  }
  assert(r1 == HIT);
  assert(b1.processShot(0, 1) == HIT);
  assert(b1.processShot(0, 2) == SUNK);
  assert(b1.processShot(0, 0) == DUPLICATE_SHOT);
  assert(b1.processShot(-1, 0) == INVALID_SHOT);
  assert(b1.processShot(9, 9) == MISS);

  // Test Views
  assert(b1.getOpponentView(0, 0) == SUNK);
  assert(b1.getOpponentView(9, 9) == MISS);
  assert(b1.getOpponentView(5, 5) == WATER);

  assert(b1.getOwnerView(5, 5) == 'b');
  assert(b1.getOwnerView(9, 9) == MISS);

  assert(b1.getShipMark(5, 5) == 'b');

  // Test Win Condition
  assert(!b1.hasWon());
  b1.processShot(5, 5);
  b1.processShot(6, 5);
  assert(b1.hasWon());

  std::cout << "Board tests passed!" << std::endl;
}

void testAIContest() {
  std::cout << "Testing AIContest..." << std::endl;
  DumbPlayer p1(10);
  DumbPlayer p2(10);

  AIContest contest(&p1, "P1", &p2, "P2", 10, true);
  int totalMoves = 0;
  bool p1Won = false, p2Won = false;

  contest.play(0, totalMoves, p1Won, p2Won);
  assert(totalMoves > 0);

  std::cout << "AIContest tests passed!" << std::endl;
}

#include "SmarterPlayer.h"

void testSmarterPlayer() {
  std::cout << "Testing SmarterPlayer..." << std::endl;
  SmarterPlayer p(10);

  // Test checkShot bounds
  assert(!p.checkShot(-1, 0));
  assert(!p.checkShot(0, -1));
  assert(!p.checkShot(10, 0));
  assert(!p.checkShot(0, 10));
  assert(!p.checkShot(MAX_BOARD_SIZE, 0));
  assert(!p.checkShot(0, MAX_BOARD_SIZE));

  // Test inside bounds (assuming empty board is water, but checkShot filters
  // hits/misses etc) Initially board is water, so checkShot should return true
  // (valid target) if it checks for non-shot spots? Let's check logic:
  // checkShot returns false if board[row][col] is HIT, SUNK, MISS,
  // DUPLICATE_SHOT. It returns true otherwise (WATER, SHIP).
  // p.initializeBoard() sets everything to WATER.
  // Warning: Player constructor calls newRound check calls initializeBoard.
  // But Player::Player calls initializeBoard? No, it calls it in newRound.
  // SmarterPlayer constructor calls Player constructor.
  // Let's force initialization just in case.
  p.newRound();

  assert(p.checkShot(0, 0));
  assert(p.checkShot(9, 9));

  std::cout << "SmarterPlayer tests passed!" << std::endl;
}

int main() {
  testBoard();
  testAIContest();
  testSmarterPlayer();
  std::cout << "All tests passed!" << std::endl;
  return 0;
}
