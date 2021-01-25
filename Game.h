#ifndef GAME_H
#define GAME_H
	
#include <vector>

class Tic_Tac_Toe
{
	//Variable to store the curent round number
	int i_rounds;
	std::vector<std::vector<int>> Matrix;

public:
	//Constructor
	Tic_Tac_Toe();
	//Function that adds elements in matrix
	void Add_Element(int i_Button, int i_Player);
	//Function that returns the winner if there is one
	int Winner();
	//Function that resets the number of round and Matrix values
	void Reset();
};

#endif