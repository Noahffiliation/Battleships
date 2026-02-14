################################################
# Two changes are needed.
# (1) Add your player's .o file to CONTESTOBJECTS
# (2) Add your player's dependencies right after SmarterPlayer's.
################################################

CXXFLAGS = -g -Wall -Og -std=c++11
CXX = g++


################################################
# Change 1:
# Add your player on the line after GamblerPlayer
################################################
CONTESTOBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o contest.o \
	SmarterPlayer.o DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o

BONUSOBJECTS = CleanPlayerV2.o GamblerPlayerV2.o LearningGambler.o SemiSmartPlayerV2.o

HOST_NAME := $(shell hostname)
HOST_OK := no

instructions:
	@echo "Make options: contest, clean"

contest: $(CONTESTOBJECTS)
	$(CXX) -o contest $(CONTESTOBJECTS) $(LDFLAGS)
	@echo "Contest binary is in 'contest'. Run as './contest'"

coverage:
	$(MAKE) clean
	$(MAKE) contest CXXFLAGS="$(CXXFLAGS) --coverage" LDFLAGS="--coverage"
	$(MAKE) tests CXXFLAGS="$(CXXFLAGS) --coverage" LDFLAGS="--coverage"
	echo "10 10 0" | ./contest
	./tests
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	@echo "Coverage report generated in 'coverage_report/index.html'"

tests: tests.o AIContest.o Board.o Message.o Player.o conio.o SmarterPlayer.o DumbPlayer.o
	$(CXX) -o tests tests.o AIContest.o Board.o Message.o Player.o conio.o SmarterPlayer.o DumbPlayer.o $(LDFLAGS)

tests.o: tests.cpp

duplication:
	cppcheck --enable=all --inconclusive --xml . 2> cppcheck-report.xml
	@echo "Cppcheck report generated in 'cppcheck-report.xml'"

clean:
	rm -f contest tests tests.o $(CONTESTOBJECTS) *.gcda *.gcno coverage.info
	rm -rf coverage_report

contest.o: contest.cpp
contest.cpp: defines.h Message.cpp

Message.o: Message.cpp defines.h
Message.cpp: Message.h defines.h

conio.o: conio.cpp
conio.cpp: conio.h

AIContest.o: AIContest.cpp
AIContest.cpp: AIContest.h Message.h defines.h

# Players here
SmarterPlayer.o: SmarterPlayer.cpp Message.cpp
SmarterPlayer.cpp: SmarterPlayer.h defines.h PlayerV2.h conio.cpp

DumbPlayerV2.o: DumbPlayerV2.cpp Message.cpp
DumbPlayerV2.cpp: DumbPlayerV2.h defines.h PlayerV2.h conio.cpp
################################################
# Change 2:
# Add your player dependency information below
################################################


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

