# Battleships
This is a Battleships simulator that puts different AI against each other in a contest to determine who has the better Battleships AI. The dafault player is the Dumb Player, that shoots from top to bottom across each row, and other AI players can be created to add more to the contest.

## Adding More AI Players
To make more players, you can add to the DumbPlayer code to do any extra necessary logic.

You will have to update `contest.cpp` with the total number of players and a name for each player, 

You will also have to update the `Makefile` to include the new player AI objects and its dependices.

## Running the contest
Build the contest objects and the `contest` executable
```
$ make contest
```

Run the contest and follow the instructions for board sizes and speed
```
$ ./contest
```

Clean and rebuild the contest
```
$ make clean
```
