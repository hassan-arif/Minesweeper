#pragma once
#include <iostream>
using namespace std;

class cell {
private:
	char ch;
public:
	cell();
	cell(char ch);

	char getCell() const;
	void setCell(char ch);

	void operator ++(int);											//helped to update surrounding cells after randomnly placing mines

	friend ostream& operator << (ostream& cout, const cell& c);		//was used to print 25 separate cells which make a board
};

cell::cell() : ch('X') {}

cell::cell(char ch) : ch(ch) {}

char cell::getCell() const {
	return ch;
}

void cell::setCell(char ch) {
	this->ch = ch;
}

void cell::operator ++(int) {
	ch++;
}

ostream& operator << (ostream& cout, const cell& c) {
	cout << c.ch;
	return cout;
}