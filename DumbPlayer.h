#ifndef DUMBPLAYER_H
#define DUMBPLAYER_H

#include "Message.h"
#include "Player.h"
#include "defines.h"

class DumbPlayer : public Player {
public:
  DumbPlayer(int boardSize);
  ~DumbPlayer() = default;
  Message placeShip(int length);
  Message getMove();
};

#endif
