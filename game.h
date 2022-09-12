#pragma once

class game {								//INTERFACE
public:
	virtual void decision() = 0;
	virtual void getInput() = 0;
	virtual bool proceed() const = 0;
	virtual void printboard() const = 0;
};