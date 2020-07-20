#pragma once
#include "Model.h"


struct PieceAtributes {

	float health;
	float attack;


};


struct Piece {

	const char* pieceName;
	PieceAtributes atributs;
	Model * mesh;

};

class Gamer
{

	const char* name;
	std::array<Piece, 5> pieces;

};

