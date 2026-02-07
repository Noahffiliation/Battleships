CXXFLAGS = -g -Wall -Og -std=c++14
CXX = g++

# ADD NEW PLAYER.o HERE
CONTESTOBJECTS = AIContest.o Board.o Message.o Player.o conio.o contest.o SmarterPlayer.o DumbPlayer.o

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

AIContest.o: AIContest.cpp Random.h
AIContest.cpp: AIContest.h Message.h defines.h Random.h

SmarterPlayer.o: SmarterPlayer.cpp Message.cpp Random.h
SmarterPlayer.cpp: SmarterPlayer.h defines.h Player.h conio.cpp Random.h

DumbPlayer.o: DumbPlayer.cpp Message.cpp
DumbPlayer.cpp: DumbPlayer.h defines.h Player.h conio.cpp

Board.o: Board.cpp
Board.cpp: Board.h

# ADD NEW PLAYER DEPENDENCIES HERE

