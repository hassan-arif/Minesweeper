#pragma once
#include <iostream>
using namespace std;

#include "game.h"
#include "cell.h"
#include <Windows.h>	//used for Sleep and SetColorAndBackground
#include <conio.h>		//used for _kbhit and _getch to take input without printing
#include <fstream>		//used to maintain recently saved game
#include <ctime>		//srand and rand for generating random coordinate for mines

void SetColorAndBackground(int ForgC, int BackC);						//coloring

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void gotoXY(int x, int y);												//to restructure messages

class minesweeper : public game {
private:
	cell keyboard[5][5], playboard[5][5];
	bool play, once;
	bool lost, victory;

	void introduction() const;											//MINESWEEPER
	void move() const;													//in between instructions to enter coordinates, save, load, exit
	void saveGame() const;												//saves both 2d arrays in memory.txt
	void loadGame();													//loads memory.txt in both 2d arrays
	void revealBoard(int x, int y);										//reveals playboard after each entry
	void revealNeighbour(int y, int x);									//spreads 360 degree in case entered coordinate was '0' (from revealBoard function)
	bool scanboard() const;												//scans whether user won the game or not

public:			//all overrided functions are inherited from game.h
	minesweeper();														//sets null in every cell and assigns value to boolean variables
	void decision() override;											//asks whether to start new game, load prev. game, exit
	void getInput() override;											//asks whether to enter coordinates, save/load/exit game
	bool proceed() const override;										//game runs as long as it returns true
	void conclude() const;												//concludes game with final message
	void printboard() const override;									//prints playboard with current value of cells
};

inline void minesweeper::introduction() const {
	gotoXY(53, 0);

	SetColorAndBackground(5, 0);
	cout << "MINE"; Sleep(500);

	SetColorAndBackground(8, 0); cout << '-';

	SetColorAndBackground(5, 0);
	cout << "SWEEP"; Sleep(500);

	SetColorAndBackground(2, 0); cout << '-';

	SetColorAndBackground(5, 0);
	cout << "ER"; Sleep(500);// system("cls");

	SetColorAndBackground(15, 0);
}

inline void minesweeper::move() const {	//displayed once only when the board is displayed
	cout << endl;
	for (int i = 1; i <= 4; i++) {
		SetColorAndBackground(8, 0);
		cout << i << ". ";
		SetColorAndBackground(15, 0);

		if (i == 1) cout << "Enter \'m\' to enter Move\n";
		else if (i == 2) cout << "Enter \'s\' to Save current game\n";
		else if (i == 3) cout << "Enter \'l\' to Load previous savegame\n";
		else if (i == 4) cout << "Enter \'e\' to Exit current game (without saving)\n";
	}
}

void minesweeper::saveGame() const {
	ofstream fout;
	fout.open("memory.txt");
	for (int i = 0; i < 5; i++) { //stores all entries of keyboard by using overloaded ostream operator
		for (int j = 0; j < 5; j++) {
			if (j == 4)
				fout << keyboard[i][j];
			else fout << keyboard[i][j] << " ";
		}
		fout << endl;
	}
	fout << endl;
	for (int i = 0; i < 5; i++) { //stores all entries of playboard (semi revealed) by using overload ostream operator
		for (int j = 0; j < 5; j++) {
			if (j == 4)
				fout << playboard[i][j];
			else
				fout << playboard[i][j] << " ";
		}
		if (i < 4) fout << endl;
	}
	fout.close();
}

void minesweeper::loadGame() {
	int i = 0, j = 0, k = 0, l = 0;
	ifstream fin;
	fin.open("memory.txt");
	while (!fin.eof()) {
		char c;
		fin >> c;
		if (fin.eof()) break;
		if (i < 5) { //loads keyboard in 2d keyboard array
			keyboard[i][j++].setCell(c);
			if (j == 5) {
				i++;
				j = 0;
			}
			if (i == 5) fin >> c; //to skip additional \n between both boards in memory.txt
		}
		if (i >= 5) { //loads playboard in 2d playboard array
			playboard[k][l++].setCell(c);
			if (l == 5) {
				k++;
				l = 0;
			}
		}
	}
	fin.close();
}

void minesweeper::revealBoard(int x, int y) { //takes user entered coordinates and compares with keyboard
	int r = y, c = x;
	if (keyboard[r][c].getCell() == 'M') { //mine detected
		playboard[r][c].setCell(keyboard[r][c].getCell());
		lost = true;
		play = false;
	}
	else {
		if (keyboard[r][c].getCell() == '0') {
			int count = 0;
			playboard[r][c].setCell(keyboard[r][c].getCell());
			revealNeighbour(r, c); //called to clear all space around 0
		}
		else {
			playboard[r][c].setCell(keyboard[r][c].getCell()); //nonzero number detected
		}
	}
}

void minesweeper::revealNeighbour(int y, int x) { //recursive function
	int r, c;
	for (int i = 0; i < 8; i++) { //looks in all 8 directions
		switch (i) {
		case 0:
			r = y - 1;
			c = x;
			break;
		case 1:
			r = y - 1;
			c = x + 1;
			break;
		case 2:
			r = y;
			c = x + 1;
			break;
		case 3:
			r = y + 1;
			c = x + 1;
			break;
		case 4:
			r = y + 1;
			c = x;
			break;
		case 5:
			r = y + 1;
			c = x - 1;
			break;
		case 6:
			r = y;
			c = x - 1;
			break;
		case 7:
			r = y - 1;
			c = x - 1;
		}
		if (r >= 0 && r <= 4 && c >= 0 && c <= 4) { //checks whether that cell exists or not
			if (keyboard[r][c].getCell() == '0' && playboard[r][c].getCell() != '0') { //checks whether new '0' cell revealed was already revealed in playboard or not
				playboard[r][c].setCell(keyboard[r][c].getCell()); //reveals new cell in playboard
				revealNeighbour(r, c); //recursion called until new revealed cell is not zero
			}
			if (keyboard[r][c].getCell() != 'M') //recursion stops when new revealed cell is nonzero number, this block reveals that nonzero area
				playboard[r][c].setCell(keyboard[r][c].getCell());
		}
	}
}

bool minesweeper::scanboard() const { //decides victory by counting unrevealed cells
	int count = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++)
			if (playboard[i][j].getCell() == 'X') count++;
	}
	if (count == 5) return true;
	return false;
}

minesweeper::minesweeper() {
	introduction();
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++) {
			keyboard[i][j] = '\0';
			playboard[i][j] = '\0';
		}
	play = false;
	once = true;
	lost = false;
	victory = false;
}

void minesweeper::decision() { //decides whether to start/load/exit game
	gotoXY(54, 1);
	cout << "Start Menu\n\n";

	for (int i = 1; i <= 3; i++) {
		SetColorAndBackground(8, 0);
		cout << i << ". ";

		SetColorAndBackground(15, 0);
		if (i == 1) cout << "Press \'s\' to start new game\n";
		else if (i == 2) cout << "Press \'l\' to load previous game\n";
		else if (i == 3) cout << "Press \'e\' to exit";
	}

	char d = '\0';
	while (d != 's' && d != 'l' && d != 'e') {
		if (_kbhit())
			d = _getch();
	}
	if (d == 'l') {
		play = true;
		loadGame();
		system("cls");

		SetColorAndBackground(14, 0);
		cout << "Loading...";
		Sleep(500);
		system("cls");

		gotoXY(53, 0);

		SetColorAndBackground(5, 0);
		cout << "MINESWEEPER";

		SetColorAndBackground(2, 0);
		cout << "\n\nGame loaded successfully!\n\n";

		SetColorAndBackground(15, 0);
	}
	else if (d == 's') {
		play = true;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++) {
				playboard[i][j].setCell('X');
				keyboard[i][j].setCell('0');
			}
		srand(time_t(0));
		for (int i = 0; i < 5; i++) {
			int x = 0, y = 0;
			x = rand() % 5;
			y = rand() % 5;
			while (x == 0 && y == 0 || keyboard[y][x].getCell() == 'M') {
				x = rand() % 5;
				y = rand() % 5;
			}
			keyboard[y][x].setCell('M');
			int r, c;
			for (int i = 0; i < 8; i++) { //adjusts cells' value which are besides a mine
				switch (i) {
				case 0:
					r = y - 1;
					c = x;
					break;
				case 1:
					r = y - 1;
					c = x + 1;
					break;
				case 2:
					r = y;
					c = x + 1;
					break;
				case 3:
					r = y + 1;
					c = x + 1;
					break;
				case 4:
					r = y + 1;
					c = x;
					break;
				case 5:
					r = y + 1;
					c = x - 1;
					break;
				case 6:
					r = y;
					c = x - 1;
					break;
				case 7:
					r = y - 1;
					c = x - 1;
				}
				if (r >= 0 && r <= 4 && c >= 0 && c <= 4)
					if (keyboard[r][c].getCell() != 'M') keyboard[r][c]++; //overloaded post increment for cell used here
			}
		}
		system("cls");

		SetColorAndBackground(14, 0);
		cout << "Loading...";
		Sleep(500);
		system("cls");

		gotoXY(53, 0);

		SetColorAndBackground(5, 0);
		cout << "MINESWEEPER";

		SetColorAndBackground(2, 0);
		cout << "\n\nNew Game Loaded successfully!\n\n";
		SetColorAndBackground(15, 0);
	}
	else if (d == 'e') {
		system("cls");

		SetColorAndBackground(14, 0);
		cout << "Leaving...";
		Sleep(500);
		system("cls");

		SetColorAndBackground(15, 0);
	}
}

void minesweeper::getInput() {
	if (once) {
		move();
		once = false;
	}
jump:	//goto function sent here after saving game while playing
	cout << "\nEnter Character: ";
	char sel = '\0';
	SetColorAndBackground(11, 0);
	while (sel != 'm' && sel != 's' && sel != 'l' && sel != 'e') {
		if (_kbhit())
			sel = _getch();
	}
	cout << sel << endl;
	SetColorAndBackground(15, 0);
	/*cin >> sel;
	SetColorAndBackground(15, 0);
	while (sel != 'm' && sel != 's' && sel != 'l' && sel != 'e') {
		SetColorAndBackground(12, 0);
		cout << "Invalid Entry Detected! Enter Character Again: ";

		SetColorAndBackground(11, 0);
		cin >> sel;
		SetColorAndBackground(15, 0);
	}*/
	if (sel == 'm') {
		cout << "Enter coordinates x, y [column (0-4), row (0-4)]: ";
		int x = 0, y = 0;
		SetColorAndBackground(11, 0);
		cin >> x >> y;

		while (x < 0 || x>4) {
			SetColorAndBackground(12, 0);
			cout << "Enter x again: ";
			SetColorAndBackground(11, 0);
			cin >> x;
		}
		while (y < 0 || y>4) {
			SetColorAndBackground(12, 0);
			cout << "Enter y again: ";
			SetColorAndBackground(11, 0);
			cin >> y;
		}
		cout << endl;

		SetColorAndBackground(15, 0);

		revealBoard(x, y); //reveals board after taking coordinates
		if (!lost && play) {
			victory = scanboard(); //scans unrevealed entries and returns boolean value in victory
		}
		if (victory) { //called when you win the game
			play = false;
			printboard();
			Sleep(500);
			SetColorAndBackground(2, 0);
			cout << "\nYOU WON!\n\n";
			SetColorAndBackground(15, 0);
			Sleep(500);
		}
	}
	else if (sel == 's') {
		saveGame();
		SetColorAndBackground(14, 0);
		cout << "Saving...";
		Sleep(500);
		SetColorAndBackground(2, 0);
		cout << "\nGame saved successfully!\n";
		SetColorAndBackground(15, 0);
		goto jump;
	}
	else if (sel == 'l') {
		loadGame();
		SetColorAndBackground(14, 0);
		cout << "Loading...";
		Sleep(500);
		SetColorAndBackground(2, 0);
		cout << "\nGame loaded successfully!\n\n";
		SetColorAndBackground(15, 0);
	}
	else if (sel == 'e') {
		play = false;
		SetColorAndBackground(8, 0);
		cout << "\nLeaving...";
		Sleep(500);
		cout << endl;
		SetColorAndBackground(15, 0);
		return;
	}
}

bool minesweeper::proceed() const {
	return play;
}

void minesweeper::conclude() const {
	if (lost) {
		printboard();
		SetColorAndBackground(12, 0);
		Sleep(500);
		cout << "\nGAME OVER!\n\n";
	}
	else {
		if (victory) {
			SetColorAndBackground(10, 0);
			cout << "Game has ended automatically.\n\n";
		}
		else { //lost=false, victory=false, play=false, i.e., you left in the middle 
			SetColorAndBackground(8, 0);
			cout << "Game Exited Successfully!\n\n";
		}
	}
	SetColorAndBackground(15, 0);
}

inline void minesweeper::printboard() const { //print the board after each move

	SetColorAndBackground(8, 0); //grey
	for (int i = 0; i < 13; i++)
		cout << '-';
	cout << endl;

	for (int i = 0; i < 5; i++) {
		SetColorAndBackground(8, 0); //grey
		cout << "| ";
		for (int j = 0; j < 5; j++) {
			if (playboard[i][j].getCell() == 'M') {
				SetColorAndBackground(12, 0); //light red
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '0') {
				SetColorAndBackground(0, 0); //black, hide
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '1') {
				SetColorAndBackground(11, 0); //light aqua
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '2') {
				SetColorAndBackground(10, 0); //light green
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '3') {
				SetColorAndBackground(5, 0); //purple
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '4') {
				SetColorAndBackground(14, 0); //light yellow
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == '5') {
				SetColorAndBackground(1, 0); //blue
				cout << playboard[i][j] << " ";
			}
			else if (playboard[i][j].getCell() == 'X') {
				SetColorAndBackground(8, 0); //grey
				cout << playboard[i][j] << " ";
			}
		}
		SetColorAndBackground(8, 0); //grey
		cout << '|';
		cout << endl;
	}

	for (int i = 0; i < 13; i++)
		cout << '-';
	cout << endl;

	SetColorAndBackground(15, 0);
}

void SetColorAndBackground(int ForgC, int BackC) //coloring
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

void gotoXY(int x, int y)
{
	CursorPosition.X = x; // Locates column
	CursorPosition.Y = y; // Locates Row
	SetConsoleCursorPosition(console, CursorPosition); // Sets position for next thing to be printed 
}