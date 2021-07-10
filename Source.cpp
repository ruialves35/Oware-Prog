#include <iostream>
#include <string>
#include <sstream> 
#include <Windows.h>  //colors on output only work on windows os
using namespace std;

enum state {
	INIT,
	P1,
	P2,
	FINISH,
	EXIT,
};


//struct for the board of the game. Seeds represent each house with the respective number of seeds
//the owner represents who is the owner of the house
struct board {
	const int boardSize = 12;
	int currPlayer;	//if = 0, player 1 is playing, if 1, player 2 is playing
	int seeds[12];
	int owner[12];
};


void finish(string player1, string player2, int score1, int score2);
void changeColor(int desiredColor);
void drawBoard(struct board board, string player1, string player2, int score1, int score2);
void GetMove(struct board board, int& move, bool& validMove);
void INIT_Board(enum state& pos, string& player1, string& player2, struct board& board);
void stMachine(bool& exit);
void endInput(struct board board, int& score1, int& score2);
void getSeeds(struct board &board, struct board &temporary, int& move, bool& capture, int numberSeeds, bool& can_play, int& score1, int& score2, enum state& pos);

/**
displays who is the winner. 
@param1 player 1 name
@param2 player 2 name
@param3 player 1 score
@param4 player 2 score
*/
void finish(string player1, string player2, int score1, int score2) {
	if (score1 > 24) {
		cout << "The winner is " << player1 << " with " << score1 << " points. Congratulations " << player1 << endl;;
	}
	else if (score1 == 24 && score2 == 24) {
		cout << "It was a draw!! Very well played by both of you." << endl;
	}
	else {
		cout << "The winner is " << player2 << " with " << score2 << " points. Congratulations " << player2 << endl;;
	}
}

/**
draws the board (houses, seeds, current player, each player score)
@param1 board: struct to build the board
@param2  player1 name
@param3  player2 name
@param4  player1 score
@param5  player2 score
*/
void drawBoard(struct board board, string player1, string player2, int score1, int score2) {
	cout << endl;
	changeColor(10);//score a vermelho
	cout << player1 << " score: " << score1 << endl << endl;
	changeColor(15);//linhas a branco
	cout << "\t1\t2\t3\t4\t5\t6" << endl;
	cout << "\t------------------------------------------" << endl;
	changeColor(11); //P1 houses blue
	//shows  P1 houses
	for (int i = 0; i < board.boardSize / 2; i++) {
		cout << "\t" << board.seeds[i];
	};
	cout << endl;
	changeColor(14); //P2 houses yellow
	//show P2 houses
	for (int i = 6; i < board.boardSize; i++) {
		cout << "\t" << board.seeds[i];
	};
	changeColor(15);
	cout << endl;
	cout << "\t------------------------------------------" << endl;
	cout << "\t7\t8\t9\t10\t11\t12" << endl << endl;
	changeColor(10); //score red
	cout << player2 << " score: " << score2 << endl << endl;
	changeColor(15);
}

/*
the input was 0 and they want to finish, get all seeds in each player houses to his score
@param1 board: struct to build the board
@param2 score of player 1
@param3 score of player 2
*/
void endInput(struct board board, int& score1, int& score2) {
	//get all seeds in player1 houses to his score
	for (int i = 0; i < board.boardSize / 2; i++) {
		score1 += board.seeds[i];
	}
	//get all seeds in player2 houses to his score
	for (int i = 6; i < board.boardSize; i++) {
		score2 += board.seeds[i];
	}
}

/**
gets the move from input
@param1 gets the board to check if the house has 0 seeds, if it has can't move
@param2 changes the value of move to play latter
@param3 changes validMove to if it can move the seeds from that house
*/
void GetMove(struct board board,int &move, bool &validMove) {
	string str_num; //receive input to be checked
	validMove = true; //assume is true
	cout << "Enter move: ";
	getline(cin, str_num);
	for (int i = 0; i < str_num.length(); i++) {
		if (!isdigit(str_num[i])) {		//if the input has a char that is not number, its an invalid input
			validMove = false;
			break;
		}
	}
	if (validMove) {
		stringstream geek(str_num); //get the string to integer
		geek >> move;	//pass to the variable move, in integer
		if ((move > (board.currPlayer*6+6)|| move <= board.currPlayer*6 || board.seeds[move - 1] == 0) && move != 0) { 	//invalid move
			cout << "Invalid move. Do it again.";
			validMove = false;
		}
	}
	else {
		cout << "Invalid move.";
	}
}

/*
capture, or not, the seeds in the next houses; with a temporary board check if the opponent can play, make the move
@param1 struct of board
@param2 temporary struct of board
@param3	integer of the move input
@param4 bool variable to check if can capture seeds
@param5 number of seeds in the house that player chose to move
@param6 bool variable to check if the move allows opponent to play
@param7 P1 score
@param8 P2 score
@param9 state of machine, in variable pos
*/
void getSeeds(struct board &board, struct board &temporary, int& move, bool& capture, int numberSeeds, bool &can_play, int &score1, int&score2,enum state &pos) {
	//see if it has the condition to capture (the last house has 1/2 seeds)
	if (board.seeds[(move - 1 + numberSeeds) % 12] == 1 || board.seeds[(move - 1 + numberSeeds) % 12] == 2) {
		capture = true;
	}
	else capture = false;
	//distribute the seeds for the houses
	for (int i = numberSeeds; i >= 1; i--) {
		//it is the same house that he played so it wont get any seed in there
		if ((move - 1 + i) % 12 == (move - 1)) {
			continue;
		}
		else {
			temporary.seeds[(move - 1 + i) % 12] += 1;
		}
	}
	if (capture) {
		//it can capture the seeds, lets capture them for temporary!!!
		for (int i = numberSeeds;; i--) {
			if (temporary.seeds[(move - 1 + i) % 12] == 2 || temporary.seeds[(move - 1 + i) % 12] == 3) {
				temporary.seeds[(move - 1 + i) % 12] = 0;
			}
			else
				break; // out of for, it can't capture more
		}
	}
	can_play = false; //assume it can't play
	//check if the opponent can play, condition of the game that you just can play if your play allows the opponent to play
	//if P1 (= 0), goes check P2 houses(from 7 to 12), if P2(=1), goes check P1 houses (from 0 to 6)
	for (int i = board.boardSize*abs(board.currPlayer-1) / 2; i < board.boardSize*abs(board.currPlayer-1)/2 + 6; i++) { //start in boardSize/2 ends in board size because P2 houses starts in boardSize/2
		if (temporary.seeds[i] != 0) { // he has seeds in one of his houses, so he can play
			can_play = true;
			break;
		}
	}
	//if doesnt enter here, the player who is going to play is player 1 AGAIN, get move again, it was an invalid play
	//because he putted all enemy houses to 0
	if (can_play) {
		board.seeds[move - 1] = 0; //current house comes to 0 seeds
		//distribute the seeds for the houses
		for (int i = numberSeeds; i >= 1; i--) {
			//it is the same house that he played so it wont get any seed in there
			if ((move - 1 + i) % 12 == (move - 1)) {
				continue;
			}
			else {
				board.seeds[(move - 1 + i) % 12] += 1;
			}
		}
		if (capture) {
			//it can capture the seeds, lets capture them!
			for (int i = numberSeeds;; i--) {
				if (board.seeds[(move - 1 + i) % 12] == 2 || board.seeds[(move - 1 + i) % 12] == 3) {
					if (board.currPlayer == 0) {
						score1 += board.seeds[(move - 1 + i) % 12];
					}
					else
						score2 += board.seeds[(move - 1 + i) % 12];
					board.seeds[(move - 1 + i) % 12] = 0;
				}
				else
					break; // out of for, it can't capture more
			}
		}
		if (board.currPlayer == 0) {
			pos = P2; //player 1 already played, go to player 2 move.
		}
		else
			pos = P1;
	}
}

/**
shows the board when player's enter in the game. Creates the board to play (houses and seeds)
@param pos: changes a state pos to say if it is player1 or player 2 that is going to play or if it is to exit the game
@player1: changes player1 variable to the name of the player1 input
@player2: changes player2 variable to the name of the player2 input
*/
void INIT_Board(enum state &pos,string& player1, string& player2, struct board &board) {
	bool validInput;
	int option;
	do {
		validInput = true;
		string str_option; //receive input to be checked for option case
		cout << "Begin game (1)" << endl;
		cout << "Instructions (2)" << endl;
		cout << "Exit (3)" << endl;
		cout << "Enter option (1/2/3): ";
		getline(cin, str_option);
		for (int i = 0; i < str_option.length(); i++) {
			if (!isdigit(str_option[i]))
			{
				validInput=false;
				break;
			}

		}
		if (!validInput)
		{
			cout << "Invalid Option " << endl << endl;
			Sleep(800);
			system("cls");
		}
		else {
			stringstream geek(str_option);
			geek >> option; //string to integer
			if (option < 1 || option >3) {
				cout << "Invalid Option " << endl << endl;
				Sleep(800);
				system("cls");
			}
		}
	} while (!validInput);
	switch (option) {
	case 1: //the player wants to play
		cout << "Enter player 1 name: ";
		getline(cin, player1);
		cout << "Enter player 2 name: ";
		getline(cin, player2);
		pos = P1;
		for (int i = 0; i < board.boardSize; i++) {
			board.seeds[i] = 4;
			if (i < 6)
				board.owner[i] = 1;
			else
				board.owner[i] = 2;
		}
		break;
	case 2: // the player wants to see instructions
		system("cls");
		changeColor(10); //change color to green
		cout << "BEGINNING" << endl;
		changeColor(15); //change color to white
		cout << "The game requires an oware board and 48 seeds. A typical oware board has two straight rows of six pits, called 'houses', and optionally one large 'score' house at either end." << endl << "Each player controls the six houses on their side of the board, and the score house on their end." << endl << "The game begins with four seeds in each of the twelve smaller houses." << endl << " The goal is to get the most seeds possible. You get a seed when the last seed of your house, after distributed, goes to a house with 1 or 2 seeds. You can't make a move from an empty house" << endl;
		cout << endl;
		changeColor(10);
		cout << "LET THE OPPONENT PLAY" << endl;
		changeColor(15);
		cout << "The proscription against capturing all an opponent's seeds is related to a more general idea, that one ought to make a move that allows the opponent to continue playing. If an opponent's houses are all empty, the current player must make a move that gives the opponent seeds. If no such move is possible, the current player captures all seeds in his/her own territory, ending the game." << endl;
		cout << endl;
		changeColor(10);
		cout << "WINNING" << endl;
		changeColor(15);
		cout << "The game is over when one player has captured 25 or more seeds, or each player has taken 24 seeds (draw). If both players agree that the game has been reduced to an endless cycle, the game ends when each player has seeds in his holes and then each player captures the seeds on their side of the board." << endl;
		cout << endl;
		changeColor(10);
		cout << "INFINITE GAME" << endl;
		changeColor(15);
		cout << "If both players think its becomming an infinite game, enter 0 on the move." << endl;
		cout << endl << endl;
		break;
	case 3:
		pos = EXIT;
		break;
	};
}

/**
changes the color of the text
@param1 number to choose the color of the text
*/
void changeColor(int desiredColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
}

void stMachine(bool &exit) {
	int numberSeeds;	//number of seeds in the house that player is going to move
	bool capture, can_play;//to see if can capture a seed and if opponent can play after your move, if he can't so u can't make the move
	struct board temporary; //it is going to be used to check if the move can be done
	struct board board; //struct with the board size, each house and the seeds in each house
	enum state pos = INIT; //state
	string player1, player2; //player 1 and player 2 names, by input
	int score1 = 0, score2 = 0; // each player score
	bool play = true;
	while (play) {
		switch (pos) {
		case INIT:
			INIT_Board(pos, player1, player2, board);
			break;
		case P1:
			board.currPlayer = 0;
			if (score1 > 24 || (score1 == 24 && score2 == 24) || score2 > 24) {
				pos = FINISH;
				break;
			};
			int move;   //where the player is going to play
			bool validMove; //see if it was a valid move
			do {
				validMove = true;  //initialize variable
				cout << endl;
				cout << "Current player: " << player1 << endl << endl;
				drawBoard(board, player1, player2, score1, score2);
				GetMove(board, move, validMove);
				//if invalid move, goes to beginning (go to end of while and repeat)
				if (move == 0) {
					pos = FINISH;
					endInput(board, score1, score2);
					break;
				}
				if (validMove) {
					numberSeeds = board.seeds[move - 1]; //get the seeds on the house that the player chose
					struct board temporary = board; //temporary board to see if the opponent can play, if they can't so dont make the move
					temporary.seeds[move - 1] = 0; //current house comes to 0 seeds
					getSeeds(board, temporary, move, capture, numberSeeds, can_play, score1, score2, pos);
				}
			} while (!validMove);//while invalid input repeat the question to make a move
			break;
		case P2:
			board.currPlayer = 1;
			do {
				validMove = true;  //initialize variable
				//show the current player and the respective houses index to play.
				cout << endl;
				cout << "Current player: " << player2 << endl << endl;
				drawBoard(board, player1, player2, score1, score2);
				GetMove(board, move, validMove);
				//if invalid move, goes to beginning (go to end of while and repeat)
				if (move == 0) {
					pos = FINISH;
					endInput(board, score1, score2);
					break;
				}
				if (validMove) {
					numberSeeds = board.seeds[move - 1]; //get the seeds on the house that the player chose
					struct board temporary = board; //temporary board to see if the opponent can play, if they can't so dont make the move
					temporary.seeds[move - 1] = 0; //current house comes to 0 seeds
					getSeeds(board, temporary, move, capture, numberSeeds, can_play, score1, score2, pos);
				}
			} while (!validMove);//while invalid input repeat the question to make a move
			break;
		case FINISH:
			finish(player1, player2, score1, score2);
			play = false; //stop playing
			break;
		case EXIT:
			play = false;
			exit = true;
			break;
		}//end of switch
		
	}//end of while
}


int main() {
	string answer;
	bool again = true, validAnswer, exit = false;
	do {
		stMachine(exit);
		if (exit)
			break;
		cout << "Do you want to play again? (y/n) ";
		do {
			getline(cin, answer);
			if (answer == "Y" || answer == "y" || answer == "yes" || answer == "YES" || answer == "Yes") {
				again = true;
				validAnswer = true;
			}
			else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO") {
				again = false;
				validAnswer = true;
			}
			else
				validAnswer = false;
		} while (!validAnswer);
	} while (again);
	return 0;
}