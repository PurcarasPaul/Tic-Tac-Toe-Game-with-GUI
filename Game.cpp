#include <iostream>
#include "Game.h"

//Function that resets the number of round and Matrix values
inline Tic_Tac_Toe::Tic_Tac_Toe()
{
	i_rounds = 0;
	//Rezises the matrix to 5 lines and 4 columns and initializes it with 0
	Matrix.resize(5, std::vector<int>(4, 0));
}

//Function that adds elements in matrix
inline void Tic_Tac_Toe::Add_Element(int i_Button,int i_Player)
{
	/*Algorithm explanation:
	* 
	  Initially the matrix looks like this: 

	  0  0  0  0
	  0  0  0  0
	  0  0  0  0
	  0  0  0  0
	  0  0  0  0
	  
	  After a button is pressed,example button 1,if the it is Player1's turn,the one with "X"
	  the value will be added to the border of the matrix(X = 1,O = -1)and it will look like this:

	  1  1  0  0
	  1  X  0  0
	  0  0  0  0
	  0  0  0  0
	  0  0  0  0

	  After this,if Player2,the one with "O" presses the button 5:

	  0  1 -1  0
	  1  X  0  0
	 -1  0  O  0
	  0  0  0  0
	 -1  0  0  0

	  Now Player1 will choose button 7:

	  0  2 -1  0
	  1  X  0  0
	 -1  0  O  0
	  1  X  0  0
	  0  0  0  0

	  If one of the the value in the border reaches 3,that means the winner is Player1,
	  else if the value in the border reaches -3,the winner is Player2
	  */

	i_rounds++;

	switch (i_Button)
	{
	case 1:
		Matrix[0][0] += i_Player;
		Matrix[0][1] += i_Player;
		Matrix[1][0] += i_Player;

		break;
	case 2:
		Matrix[1][0] += i_Player;
		Matrix[0][2] += i_Player;

		break;
	case 3:
		Matrix[4][0] += i_Player;
		Matrix[1][0] += i_Player;
		Matrix[0][3] += i_Player;

		break;
	case 4:
		Matrix[2][0] += i_Player;
		Matrix[0][1] += i_Player;
		
		break;
	case 5:
		Matrix[0][0] += i_Player;
		Matrix[0][2] += i_Player;
		Matrix[2][0] += i_Player;
		Matrix[4][0] += i_Player;

		break;
	case 6:
		Matrix[2][0] += i_Player;
		Matrix[0][3] += i_Player;

		break;
	case 7:
		Matrix[4][0] += i_Player;
		Matrix[3][0] += i_Player;
		Matrix[0][1] += i_Player;

		break;
	case 8:
		Matrix[3][0] += i_Player;
		Matrix[0][2] += i_Player;

		break;
	case 9:
		Matrix[0][0] += i_Player;
		Matrix[3][0] += i_Player;
		Matrix[0][3] += i_Player;
	}

}

//Function that returns the winner if there is one
inline int Tic_Tac_Toe::Winner()
{
	/*If the number of rounds is 5 or greater(minimum necessary for a player to win),
	* the algorithm will check the border,if it finds a value of 3 or -3,it will return
	* the value,otherwise it won't do anything
	*/
	if (i_rounds >= 5)
	{
		for (int i = 0;i < 4;i++)
		{
			if (Matrix[0][i] == -3 || Matrix[0][i] == 3)
			{
				return Matrix[0][i];
			}
		}

		for (int i = 0;i < 5;i++)
		{
			if (Matrix[i][0] == -3 || Matrix[i][0] == 3)
			{
				return Matrix[i][0];
			}
		}
	}

	//If the number of rounds reaches 9 and there is no winner,the function will return -1
	if (i_rounds == 9)
	{
		return -1;
	}

	return 0;
}

//Function that resets the number of round and Matrix values
inline void Tic_Tac_Toe::Reset()
{
	//Sets the number of rounds to 0
	i_rounds = 0;
	
	//Reinitialises the border of the matrix with 0
	for (int i = 0;i < 4;i++)
	{
		Matrix[0][i] = 0;
	}

	for (int i = 0;i < 5;i++)
	{
		Matrix[i][0] = 0;
	}
}