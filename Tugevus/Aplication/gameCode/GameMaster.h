#pragma once
#include <string>
#include "Gamer.h"

struct Cell {
	Model * entity;

	bool isOcupied;

	
};


class GameMaster
{
public:

	Gamer player1;
	Gamer player2;

	std::vector< std::vector< Cell> > board;

	//MATCH STATE MACHINE

};

