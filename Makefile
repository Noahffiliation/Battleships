CXXFLAGS = -g -Wall -Og -std=c++11
CXX = g++

# ADD NEW PLAYER.o HERE
CONTESTOBJECTS = AIContest.o Board.o Message.o Player.o conio.o contest.o SmarterPlayer.o DumbPlayer.o

BONUSOBJECTS = CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o

HOST_NAME := $(shell hostname)
HOST_OK := no

instructions:
	@echo "Make options: contest, clean"

contest: $(CONTESTOBJECTS)
	g++ -o contest $(CONTESTOBJECTS)
	@echo "Contest binary is in 'contest'. Run as './contest'"

clean:
	rm -f contest $(CONTESTOBJECTS)

contest.o: contest.cpp
contest.cpp: defines.h Message.cpp

Message.o: Message.cpp defines.h
Message.cpp: Message.h defines.h

conio.o: conio.cpp
conio.cpp: conio.h

AIContest.o: AIContest.cpp
AIContest.cpp: AIContest.h Message.h defines.h

SmarterPlayer.o: SmarterPlayer.cpp Message.cpp
SmarterPlayer.cpp: SmarterPlayer.h defines.h Player.h conio.cpp

DumbPlayer.o: DumbPlayer.cpp Message.cpp
DumbPlayer.cpp: DumbPlayer.h defines.h Player.h conio.cpp

Board.o: Board.cpp
Board.cpp: board.h

# ADD NEW PLAYER DEPENDENCIES HERE
