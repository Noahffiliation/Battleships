################################################
# Two changes are needed.
# (1) Add your player's .o file to CONTESTOBJECTS.
# (2) Add your player's dependencies.
################################################

CXXFLAGS = -g -Wall -Og -std=c++11
CXX = g++

################################################
# Change 1:
# Add your player on the line after GamblerPlayer 
################################################
CONTESTOBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o contest.o \
	SmarterPlayer.o DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o

TESTEROBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o tester.o \
	SmarterPlayer.o CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o

HOST_NAME := $(shell hostname)
HOST_OK := no

instructions:
	@echo "Make options: contest, testAI, clean, tar"

contest: $(CONTESTOBJECTS)
	g++ -o contest $(CONTESTOBJECTS)
	@echo "Contest binary is in 'contest'. Run as './contest'"

testAI: $(TESTEROBJECTS) 
	g++ -o testAI $(TESTEROBJECTS)
	@echo "Warning -- testAI is not quite complete. This message will disappear after update."
	@echo "Tester is in 'testAI'. Run as './testAI'"

clean:
	rm -f contest $(CONTESTOBJECTS) $(TESTEROBJECTS)

tar:
	g++ -c -Wall -O2 CleanPlayerV2.cpp
	g++ -c -Wall -O2 SemiSmartPlayerV2.cpp
	g++ -c -Wall -O2 GamblerPlayerV2.cpp
	g++ -c -Wall -O2 BoardV3.cpp
	tar -cvf binaries.tar CleanPlayerV2.o GamblerPlayerV2.o BoardV3.o SemiSmartPlayerV2.o

contest.o: contest.cpp
contest.cpp: defines.h Message.cpp

tester.o: tester.cpp
tester.cpp: defines.h Message.cpp

Message.o: Message.cpp defines.h
Message.cpp: Message.h defines.h

conio.o: conio.cpp
conio.cpp: conio.h

AIContest.o: AIContest.cpp
AIContest.cpp: AIContest.h Message.h defines.h

################################################
# Change 2:
# Add your player dependency information below
################################################
SmarterPlayer.o: SmarterPlayer.cpp Message.cpp
SmarterPlayer.cpp: SmarterPlayer.h defines.h PlayerV2.h conio.cpp

DumbPlayerV2.o: DumbPlayerV2.cpp Message.cpp
DumbPlayerV2.cpp: DumbPlayerV2.h defines.h PlayerV2.h conio.cpp

# CleanPlayerV2.o and other provided binaries are only available as a linkable Linux binary, not as source code.
CleanPlayerV2.o: 
	tar -xvf binaries.tar CleanPlayerV2.o
GamblerPlayerV2.o:
	tar -xvf binaries.tar GamblerPlayerV2.o
LearningGambler.o: 
	tar -xvf binaries.tar LearningGambler.o
SemiSmartPlayerV2.o: 
	tar -xvf binaries.tar SemiSmartPlayerV2.o
BoardV3.o:
	tar -xvf binaries.tar BoardV3.o
