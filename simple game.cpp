/*
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    string choices[] = {"Rock", "Paper", "Scissors"};
    int playerChoice, computerChoice;

    cout << "Rock(0), Paper(1), Scissors(2): ";
    cin >> playerChoice;

    computerChoice = rand() % 3;

    cout << "You chose: " << choices[playerChoice] << endl;
    cout << "Computer chose: " << choices[computerChoice] << endl;

    if (playerChoice == computerChoice) {
        cout << "It's a draw!\n";
    } else if ((playerChoice == 0 && computerChoice == 2) ||
               (playerChoice == 1 && computerChoice == 0) ||
               (playerChoice == 2 && computerChoice == 1)) {
        cout << "You win!\n";
    } else {
        cout << "You lose!\n";
    }

    return 0;
}
*/

#include <iostream>

using namespace std;

void displayBoard(char board[3][3]) 
{
    cout << "\n";
    
    for (int i = 0; i < 3; i++)
	{
        cout << " ";
        for (int j = 0; j < 3; j++) 
		{
            cout << board[i][j];
            
            if (j < 2) 
            {
				cout << " | ";
			}
        }
        
        cout << "\n";
        
        if (i < 2)
		{
			cout << "---|---|---\n";
		}
    }
    
    cout << "\n";
}

void playerMove(char board[3][3], char mark, int choice)
{
    int row = (choice - 1) / 3;
    int col = (choice - 1) % 3;
    
    if (board[row][col] != 'X' && board[row][col] != 'O') 
	{
        board[row][col] = mark;
    } 
	
	else 
	{
        cout << "Spot already taken! Try again.\n";
    }
}

bool checkWin(char board[3][3], char mark) 
{
    for (int i = 0; i < 3; i++) 
	{
        if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) || (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark)) 
		{
            return true;
        }
    }
    
    if ((board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) || (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark)) 
	{
        return true;
    }
    
    return false;
}

bool checkDraw(char board[3][3]) 
{
    for (int i = 0; i < 3; i++) 
	{
        for (int j = 0; j < 3; j++) 
		{
            if (board[i][j] != 'X' && board[i][j] != 'O')
            {
                return false;
            }
        }
    }
    
    return true;
}

int main() {
    char board[3][3] = { {'1','2','3'},
                         {'4','5','6'},
                         {'7','8','9'} };

    char currentPlayer = 'X';
    int choice;

    while (true) 
	{
        displayBoard(board);
        cout << "Player " << currentPlayer << ", enter your move (1-9): ";
        cin >> choice;

        playerMove(board, currentPlayer, choice);

        if (checkWin(board, currentPlayer)) 
		{
            displayBoard(board);
            cout << "Player " << currentPlayer << " wins!\n";
            break;
        } 
		
		else if (checkDraw(board)) 
		{
            displayBoard(board);
            cout << "It's a draw!\n";
            break;
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;
}



