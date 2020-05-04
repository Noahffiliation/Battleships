<h3 align="center">Battleships</h3>

<div align="center">

  [![Status](https://img.shields.io/badge/status-active-success.svg)]() 
  [![GitHub Issues](https://img.shields.io/github/issues/Noahffiliation/Battleships.svg)](https://github.com/Noahffiliation/Battleships/issues)
  [![GitHub Pull Requests](https://img.shields.io/github/issues-pr/Noahffiliation/Battleships.svg)](https://github.com/Noahffiliation/Battleships/pulls)
  
</div>

---

<p align="center"> This project sets up different Battleship AI plays in a contest to determine which AI is best at playing Battleships.
    <br> 
</p>

## Table of Contents
- [About](#about)
- [Usage](#usage)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## About <a name = "about"></a>
This is a Battleships simulator that puts different AI against each other in a contest to determine who has the better Battleships AI. The dafault player is the Dumb Player, that shoots from top to bottom across each row, and other AI players can be created to add more to the contest.

## Usage <a name="usage"></a>
### Adding More AI Players
To make more players, you can add to the DumbPlayer code to do any extra necessary logic.

You will have to update `contest.cpp` with the total number of players and a name for each player.

You will also have to update the `Makefile` to include the new player AI objects and its dependices.

### Running the contest
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

## Authors <a name = "authors"></a>
- [@Noahffiliation](https://github.com/Noahffiliation) - Initial work

## Acknowledgements <a name = "acknowledgement"></a>
- Thanks to Taylor University class COS 121 Foundations of Computer Science for the inspiration for this project
