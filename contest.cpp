/**
 * @author Noah Lindsey
 * @date September 2019
 * Main driver for BattleShips implementations
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#include "Board.h"
#include "AIContest.h"
#include "Player.h"
#include "conio.h"

Player* getPlayer(int playerId, int boardSize);
void playMatch(int player1Id, int player2Id, bool showMoves);
int comparePlayers(const void * a, const void * b);

using namespace std;
using namespace conio;

#include "SmarterPlayer.h"
#include "DumbPlayer.h"

/**
 * UPDATE NEW PLAYER INFORMATION HERE
 */
const int NumPlayers = 2;
string playerNames[NumPlayers] = {
    "Smarter Player",
    "Dumb Player",
};

Player* getPlayer(int playerId, int boardSize) {
    switch(playerId) {
        default:
        case 0: return new SmarterPlayer(boardSize);
        case 1: return new DumbPlayer(boardSize);
        // NEW PLAYER CASE HERE
    }
}

float secondsPerMove = 1;
int boardSize;
int totalGames = 0;
int totalCountedMoves = 0;
int wins[NumPlayers][NumPlayers];
int playerIds[NumPlayers];
int lives[NumPlayers];
int winCount[NumPlayers];
int statsShotsTaken[NumPlayers];
int statsGamesCounted[NumPlayers];



int main() {
    // Adjust based on the number of players!
    // Initialize various win statistics
    for (int i = 0; i < NumPlayers; i++) {
        statsShotsTaken[i] = 0;
        statsGamesCounted[i] = 0;
        winCount[i] = 0;
        lives[i] = NumPlayers / 2;
        playerIds[i] = i;
        for (int j = 0; j < NumPlayers; j++) {
            wins[i][j] = 0;
        }
    }

    // Seed (setup) the random number generator.
    // This only needs to happen once per program run.
    srand(time(NULL));

    // Now to get the board size.
    cout << "Welcome to the AI Bot contest." << endl << endl;
    cout << "What size board would you like? [Anything other than numbers 3-10 exits.] ";
    cin >> boardSize;
    // If have invalid board size input (non-number, or 0-2, or > 10).
    if (!cin || boardSize < 3 || boardSize > 10) {
        cout << "Exiting" << endl;
        return 1;
    }

    // Find out how many times to test the AI.
    cout << "How many times should I test the game AI? ";
    cin >> totalGames;

    cout << "The first game of each AI match is played at the specified speed," << endl << "all subsequent games are done without visual display." << endl << "How many seconds per move? (E.g., 1, 0.5, 1.3) : ";
    cin >> secondsPerMove;

    // And now it's show time!
    int offset = 1;
    while (offset < NumPlayers / 2) {
        for (int player = 0; player + offset < NumPlayers; player += offset + 1) {
            playMatch(player, player + offset, true);
            usleep(3000000);
        }
        ++offset;
    }
    for (int player1Id = 0; player1Id < NumPlayers; player1Id++) {
        for (int player2Id = player1Id + 1; player2Id < NumPlayers; player2Id++) {
            if (lives[player1Id] == 0 || lives[player2Id] == 0) continue;

            playMatch(player1Id, player2Id, true);
            usleep(3000000);	// Pause 3 seconds to let viewers see stats
        }
    }
    cout << endl << endl;

    // Now calculate contest results
    qsort(playerIds, NumPlayers, sizeof(int), comparePlayers);

    // Add up the total wins per player
    for (int i = 0; i < NumPlayers; i++) {
        for (int j = 0; j < NumPlayers; j++)
            winCount[i]+= wins[i][j];
    }

    int tiesInARow = 0;
    for (int i = 0; i < NumPlayers; ++i) {
        // If one of two or more that are tied for first place, switch on BOLD
        if (lives[playerIds[i]] == lives[playerIds[0]] && winCount[playerIds[i]] == winCount[playerIds[0]]) cout << setTextStyle(BOLD);
        if (i > 0 && lives[playerIds[i]] == lives[playerIds[i-1]] && winCount[playerIds[i]] == winCount[playerIds[i - 1]]) ++tiesInARow;
        else tiesInARow = 0;

        cout << setw(2) << i + 1 - tiesInARow << ": " << playerNames[playerIds[i]] << " (Lives=" << lives[playerIds[i]] << ", Wins=" << winCount[playerIds[i]] << ")";
        if ((i < NumPlayers - 1 && lives[playerIds[1]] == lives[playerIds[1 + 1]] && winCount[playerIds[i]] == winCount[playerIds[i + 1]])) cout << " -- tied ";
        else if (tiesInARow > 0 || (i < NumPlayers - 1 && lives[playerIds[i]] == lives[playerIds[i - 1]] && winCount[playerIds[i]] == winCount[playerIds[i - 1]])) cout << " -- tied ";
        cout << resetAll () << endl;
    }

    return 0;
}

void playMatch(int player1Id, int player2Id, bool showMoves) {
    Player *player1, *player2;
    AIContest *game;
    int matchWins[2] = {0, 0};
    bool player1Won = false, player2Won = false;
    int player1Ties = 0, player2Ties = 0;

    player1 = getPlayer(player1Id, boardSize);
    player2 = getPlayer(player2Id, boardSize);

    bool silent = true;
    for (int count = 0; count < totalGames; count++) {
        player1Won = false; player2Won = false;
        player1->newRound();
        player2->newRound();

        if (count == 0) {
            silent = false;
            game = new AIContest(player1, playerNames[player1Id], player2, playerNames[player2Id], boardSize, silent);
            game->play(secondsPerMove, totalCountedMoves, player1Won, player2Won);
        } else {
            silent = true;
            game = new AIContest(player1, playerNames[player1Id], player2, playerNames[player2Id], boardSize, silent);
            game->play(0, totalCountedMoves, player1Won, player2Won);
        }
        if ((player1Won && player2Won) || !(player1Won || player2Won)) {
            player1Ties++;
            player2Ties++;
            statsShotsTaken[player1Id] += totalCountedMoves;
            statsGamesCounted[player1Id]++;
            statsShotsTaken[player2Id] += totalCountedMoves;
            statsGamesCounted[player2Id]++;
        } else if (player1Won) {
            matchWins[0]++;
            wins[player1Id][player2Id]++;
            statsShotsTaken[player1Id] += totalCountedMoves;
            statsGamesCounted[player1Id]++;
        } else if (player2Won) {
            matchWins[1]++;
            wins[player2Id][player1Id]++;
            statsShotsTaken[player2Id] += totalCountedMoves;
            statsGamesCounted[player2Id]++;
        }
        delete game;
    }
    delete player1;
    delete player2;

    cout << endl << "********************" << endl;
    // file deepcode ignore IntegerOverflow: Old school project, non-issue
    cout << playerNames[player1Id] << ": " << setTextStyle(NEGATIVE_IMAGE) << "wins=" << matchWins[0] << resetAll() << " losses=" << totalGames-matchWins[0]-player1Ties << " ties=" << player1Ties << " (cumulative avg. shots/game = " << (statsGamesCounted[player1Id]==0 ? 0.0 : (float)statsShotsTaken[player1Id]/(float)statsGamesCounted[player1Id]) << ")" << endl;
    cout << playerNames[player2Id] << ": " << setTextStyle(NEGATIVE_IMAGE) << "wins=" << matchWins[1] << resetAll() << " losses=" << totalGames-matchWins[1]-player2Ties << " ties=" << player2Ties << " (cumulative avg. shots/game = " << (statsGamesCounted[player2Id]==0 ? 0.0 : (float)statsShotsTaken[player2Id]/(float)statsGamesCounted[player2Id]) << ")" << endl;
    cout << "********************" << endl;

    cout << setTextStyle(NEGATIVE_IMAGE);
    if (wins[player1Id][player2Id] > wins[player2Id][player1Id]) {
        // Player 2 lost the match
        lives[player2Id]--;
        cout << playerNames[player2Id] << " lost one life.";
        if (lives[player2Id] == 0) {
            cout << fgColor(RED);
        }
        cout << " Lives left: " << lives[player2Id] << resetAll() << endl;
    } else if (wins[player1Id][player2Id] < wins[player2Id][player1Id]) {
        // Player 1 lost the match
        lives[player1Id]--;
        cout << playerNames[player1Id] << " lost one life.";
        if (lives[player1Id] == 0) {
            cout << fgColor(RED);
        }
        cout << " Lives left: " << lives[player1Id] << resetAll() << endl;
    } else {
        // Tied -- both players lose a life: the only time this likely happens is when both
        // players are unable to do anythig worthwhile, so loosing a life is appropriate.
        lives[player1Id]--;
        lives[player2Id]--;
        cout << setTextStyle(NEGATIVE_IMAGE) << "A tie. Both players lose a life." << endl;
        cout << playerNames[player2Id] << " Lives left: " << lives[player2Id] << endl;
        cout << playerNames[player1Id] << " Lives left: " << lives[player1Id] << endl;
    }
    cout << resetAll() << "********************" << endl;
}

int comparePlayers (const void* a, const void* b) {
    int p1 = *(int*)a;
    int p2 = *(int*)b;
    if (lives[p1] > lives[p2]) return -1;
    else if (lives[p1] < lives[p2]) return 1;
    else {
        if (winCount[p1] > winCount[p2]) return -1;
        else if (winCount[p1] < winCount[p2]) return 1;
        else return 0;
    }
}
