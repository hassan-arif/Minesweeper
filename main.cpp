#include "game.h"			//interface (consists of all pure virtual functions)
#include "minesweeper.h"	//publicly inherits game.h and has 2 2-d arrays of cell.h (composition)
#include "cell.h"

#include <iostream>
using namespace std;

int main() {
	game* ms = new minesweeper;
	ms->decision();										//asks user if he wants to start new game, load prev game or just exit

	while (ms->proceed()) {								//operated by 'play' boolean variable
		ms->printboard();
		ms->getInput();									//can enter coordinates, save game, load prev saved game, exit without saving
	}

	minesweeper* msr = dynamic_cast<minesweeper*>(ms);

	msr->conclude();									//wasn't part of game.h so dynamic casting helped in calling it to finalize game

	delete msr;
	msr = nullptr;

	return 0;
}