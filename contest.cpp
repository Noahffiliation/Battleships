/**
 * @author Noah Lindsey
 * @date September 2019
 * Main driver for BattleShips implementations
 */

#include <iomanip>
#include <iostream>
#include <unistd.h>

#include "AIContest.h"
#include "Player.h"
#include "conio.h"
#include <algorithm>
#include <memory>

using namespace std;
using namespace conio;

// INCLUDE NEW PLAYER HEADERS HERE
#include "DumbPlayer.h"
#include "SmarterPlayer.h"

/**
 * UPDATE NEW PLAYER INFORMATION HERE
 */
const int NumPlayers = 2;
const string playerNames[NumPlayers] = {
    "Smarter Player",
    "Dumb Player",
};

struct ContestState {
  float secondsPerMove = 1;
  int boardSize = 0;
  int totalGames = 0;
  int wins[NumPlayers][NumPlayers] = {{0}};
  int playerIds[NumPlayers] = {0};
  int lives[NumPlayers] = {0};
  int winCount[NumPlayers] = {0};
  int statsShotsTaken[NumPlayers] = {0};
  int statsGamesCounted[NumPlayers] = {0};
};

std::unique_ptr<Player> getPlayer(int playerId, int boardSize);
void playMatch(ContestState &state, int player1Id, int player2Id,
               bool showMoves);

std::unique_ptr<Player> getPlayer(int playerId, int boardSize) {
  switch (playerId) {
  default:
  case 0:
    return std::make_unique<SmarterPlayer>(boardSize);
  case 1:
    return std::make_unique<DumbPlayer>(boardSize);
    // NEW PLAYER CASE HERE
  }
}

int main() {
  ContestState state;
  // Adjust based on the number of players!
  // Initialize various win statistics
  for (int i = 0; i < NumPlayers; i++) {
    state.statsShotsTaken[i] = 0;
    state.statsGamesCounted[i] = 0;
    state.winCount[i] = 0;
    state.lives[i] = NumPlayers / 2;
    state.playerIds[i] = i;
    for (int j = 0; j < NumPlayers; j++) {
      state.wins[i][j] = 0;
    }
  }

  // Seed (setup) the random number generator.
  // This only needs to happen once per program run.
  // Now to get the board size.
  cout << "Welcome to the AI Bot contest." << endl << endl;
  cout << "What size board would you like? [Anything other than numbers 3-10 "
          "exits.] ";
  cin >> state.boardSize;
  // If have invalid board size input (non-number, or 0-2, or > 10).
  if (!cin || state.boardSize < 3 || state.boardSize > 10) {
    cout << "Exiting" << endl;
    return 1;
  }

  // Find out how many times to test the AI.
  cout << "How many times should I test the game AI? ";
  cin >> state.totalGames;

  cout << "The first game of each AI match is played at the specified speed,"
       << endl
       << "all subsequent games are done without visual display." << endl
       << "How many seconds per move? (E.g., 1, 0.5, 1.3) : ";
  cin >> state.secondsPerMove;

  // And now it's show time!
  int offset = 1;
  while (offset < NumPlayers / 2) {
    for (int player = 0; player + offset < NumPlayers; player += offset + 1) {
      playMatch(state, player, player + offset, true);
      usleep(3000000);
    }
    ++offset;
  }
  for (int player1Id = 0; player1Id < NumPlayers; player1Id++) {
    for (int player2Id = player1Id + 1; player2Id < NumPlayers; player2Id++) {
      if (state.lives[player1Id] == 0 || state.lives[player2Id] == 0)
        continue;

      playMatch(state, player1Id, player2Id, true);
      usleep(3000000); // Pause 3 seconds to let viewers see stats
    }
  }
  cout << endl << endl;

  // Now calculate contest results
  std::sort(state.playerIds, state.playerIds + NumPlayers,
            [&state](int p1, int p2) {
              if (state.lives[p1] > state.lives[p2])
                return true;
              else if (state.lives[p1] < state.lives[p2])
                return false;
              else {
                if (state.winCount[p1] > state.winCount[p2])
                  return true;
                else if (state.winCount[p1] < state.winCount[p2])
                  return false;
                else
                  return false; // Return false for equal to maintain strict
                                // weak ordering
              }
            });

  // Add up the total wins per player
  for (int i = 0; i < NumPlayers; i++) {
    for (int j = 0; j < NumPlayers; j++)
      state.winCount[i] += state.wins[i][j];
  }

  int tiesInARow = 0;
  for (int i = 0; i < NumPlayers; ++i) {
    // If one of two or more that are tied for first place, switch on BOLD
    if (state.lives[state.playerIds[i]] == state.lives[state.playerIds[0]] &&
        state.winCount[state.playerIds[i]] ==
            state.winCount[state.playerIds[0]])
      cout << setTextStyle(BOLD);
    if (i > 0 &&
        state.lives[state.playerIds[i]] ==
            state.lives[state.playerIds[i - 1]] &&
        state.winCount[state.playerIds[i]] ==
            state.winCount[state.playerIds[i - 1]])
      ++tiesInARow;
    else
      tiesInARow = 0;

    cout << setw(2) << i + 1 - tiesInARow << ": "
         << playerNames[state.playerIds[i]]
         << " (Lives=" << state.lives[state.playerIds[i]]
         << ", Wins=" << state.winCount[state.playerIds[i]] << ")";
    if ((i < NumPlayers - 1 &&
         state.lives[state.playerIds[i]] ==
             state.lives[state.playerIds[i + 1]] &&
         state.winCount[state.playerIds[i]] ==
             state.winCount[state.playerIds[i + 1]]))
      cout << " -- tied ";
    else if (tiesInARow > 0 || (i < NumPlayers - 1 &&
                                state.lives[state.playerIds[i]] ==
                                    state.lives[state.playerIds[i - 1]] &&
                                state.winCount[state.playerIds[i]] ==
                                    state.winCount[state.playerIds[i - 1]]))
      cout << " -- tied ";
    cout << resetAll() << endl;
  }

  return 0;
}

void playMatch(ContestState &state, int player1Id, int player2Id,
               bool showMoves) {
  if (player1Id < 0 || player1Id >= NumPlayers || player2Id < 0 ||
      player2Id >= NumPlayers) {
    cerr << "Invalid player IDs passed to playMatch: " << player1Id << ", "
         << player2Id << endl;
    return;
  }
  std::unique_ptr<Player> player1, player2;
  std::unique_ptr<AIContest> game;
  int matchWins[2] = {0, 0};
  bool player1Won = false, player2Won = false;
  int player1Ties = 0, player2Ties = 0;
  int totalCountedMoves = 0;

  player1 = getPlayer(player1Id, state.boardSize);
  player2 = getPlayer(player2Id, state.boardSize);

  bool silent = true;
  for (int count = 0; count < state.totalGames; count++) {
    player1Won = false;
    player2Won = false;
    player1->newRound();
    player2->newRound();

    if (count == 0) {
      silent = false;
      game = std::make_unique<AIContest>(player1.get(), playerNames[player1Id],
                                         player2.get(), playerNames[player2Id],
                                         state.boardSize, silent);
      game->play(state.secondsPerMove, totalCountedMoves, player1Won,
                 player2Won);
    } else {
      silent = true;
      game = std::make_unique<AIContest>(player1.get(), playerNames[player1Id],
                                         player2.get(), playerNames[player2Id],
                                         state.boardSize, silent);
      game->play(0, totalCountedMoves, player1Won, player2Won);
    }
    if ((player1Won && player2Won) || !(player1Won || player2Won)) {
      player1Ties++;
      player2Ties++;
      state.statsShotsTaken[player1Id] += totalCountedMoves;
      state.statsGamesCounted[player1Id]++;
      state.statsShotsTaken[player2Id] += totalCountedMoves;
      state.statsGamesCounted[player2Id]++;
    } else if (player1Won) {
      matchWins[0]++;
      state.wins[player1Id][player2Id]++;
      state.statsShotsTaken[player1Id] += totalCountedMoves;
      state.statsGamesCounted[player1Id]++;
    } else if (player2Won) {
      matchWins[1]++;
      state.wins[player2Id][player1Id]++;
      state.statsShotsTaken[player2Id] += totalCountedMoves;
      state.statsGamesCounted[player2Id]++;
    }
    // game is automatically deleted when reassigned or out of scope
  }
  // players are automatically deleted when out of scope

  cout << endl << "********************" << endl;
  // file deepcode ignore IntegerOverflow: Old school project, non-issue
  cout << playerNames[player1Id] << ": " << setTextStyle(NEGATIVE_IMAGE)
       << "wins=" << matchWins[0] << resetAll()
       << " losses=" << state.totalGames - matchWins[0] - player1Ties
       << " ties=" << player1Ties << " (cumulative avg. shots/game = "
       << (state.statsGamesCounted[player1Id] == 0
               ? 0.0
               : (float)state.statsShotsTaken[player1Id] /
                     (float)state.statsGamesCounted[player1Id])
       << ")" << endl;
  cout << playerNames[player2Id] << ": " << setTextStyle(NEGATIVE_IMAGE)
       << "wins=" << matchWins[1] << resetAll()
       << " losses=" << state.totalGames - matchWins[1] - player2Ties
       << " ties=" << player2Ties << " (cumulative avg. shots/game = "
       << (state.statsGamesCounted[player2Id] == 0
               ? 0.0
               : (float)state.statsShotsTaken[player2Id] /
                     (float)state.statsGamesCounted[player2Id])
       << ")" << endl;
  cout << "********************" << endl;

  cout << setTextStyle(NEGATIVE_IMAGE);
  if (state.wins[player1Id][player2Id] > state.wins[player2Id][player1Id]) {
    // Player 2 lost the match
    state.lives[player2Id]--;
    cout << playerNames[player2Id] << " lost one life.";
    if (state.lives[player2Id] == 0) {
      cout << fgColor(RED);
    }
    cout << " Lives left: " << state.lives[player2Id] << resetAll() << endl;
  } else if (state.wins[player1Id][player2Id] <
             state.wins[player2Id][player1Id]) {
    // Player 1 lost the match
    state.lives[player1Id]--;
    cout << playerNames[player1Id] << " lost one life.";
    if (state.lives[player1Id] == 0) {
      cout << fgColor(RED);
    }
    cout << " Lives left: " << state.lives[player1Id] << resetAll() << endl;
  } else {
    // Tied -- both players lose a life: the only time this likely happens is
    // when both players are unable to do anythig worthwhile, so loosing a life
    // is appropriate.
    state.lives[player1Id]--;
    state.lives[player2Id]--;
    cout << setTextStyle(NEGATIVE_IMAGE) << "A tie. Both players lose a life."
         << endl;
    cout << playerNames[player2Id] << " Lives left: " << state.lives[player2Id]
         << endl;
    cout << playerNames[player1Id] << " Lives left: " << state.lives[player1Id]
         << endl;
  }
  cout << resetAll() << "********************" << endl;
}
