#include<bits/stdc++.h>
using namespace std;
char board[9]={0};

void visualize_board()
{
	int pos=0;
	int t;
	cout<<"----++---++----"<<endl;
	for(int i=0;i<3;i++)
    {
		for(int j=0;j<3;j++)
		{
			cout<<"| ";
			if(board[pos] == 0){
				cout<<pos+1<<" |";
			}
			else
			{
				if(board[pos]==-1)  //for the user its cross
				{
					cout<<"X |";
				}
				else
				{
					cout<<"O |"; //for the computer its circle
				}
			}
			pos++;
		}
        cout<<"\n----++---++----"<<endl;
	}
}


int ifwin() {
    //determines if a player has won, returns 0 otherwise.
    int prob_wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    //index for the rows ,columns and the diagonals for checking purpose
    for(int i = 0; i < 8; ++i)
    {
        if(board[prob_wins[i][0]] != 0)
        {
  			if(board[prob_wins[i][0]] == board[prob_wins[i][1]] && board[prob_wins[i][1]] == board[prob_wins[i][2]])
                return board[prob_wins[i][2]];
        }
    }
    return 0;  //if no one has won
}

int minimax(int player) {
    int winner = ifwin();

    if(winner != 0)
    	return winner*player;

    int bestmove = -1;
    int bestscore = -2;

    for(int i = 0; i < 9; i++) {

        if(board[i] == 0) {

            board[i] = player; //setting as the player's move

            int currentScore = -minimax(player*-1);
            if(currentScore > bestscore) //finding the best score for each player
            {
                bestscore = currentScore;
                bestmove = i;
            }
            board[i] = 0;//Reset board after try
        }
    }
    if(bestmove == -1)
     return 0;

    return bestscore;
}

int computerMove() {
    int bestmove = -1;
    int score = INT_MIN;
    int k=0;
    int tempScore;
    for(int i = 0; i < 9; ++i) {

        if(board[i] == 0) //empty cells
        {
            board[i] = 1; //setting as 1
            tempScore = -minimax( -1);
            board[i] = 0; //resetting to unoccupied

            if(tempScore > score)
            {
                score = tempScore;
                bestmove = i;
            }
        }
    }
    //returns a score based on minimax at a given node.
    return bestmove;
}


int main()
{

	//Player = -1 ; Computer = 1
	while(true)
    {

        cout <<"~~~~~~~~~~~~~Welcome To Tic Tac Toe~~~~~~~~~~~~~"<<endl;

        cout <<"      BOARD:"<<endl;

        cout << "----++---++----" << endl;
        cout << "| 1 || 2 || 3 |" << endl;
        cout << "----++---++----" << endl;
        cout << "| 4 || 5 || 6 |" << endl;
        cout << "----++---++----" << endl;
        cout << "| 7 || 8 || 9 |" << endl;
        cout << "----++---++----" << endl << endl << endl;

        cout<<"Only legal moves are the numbers you see on the board"<<endl;
        cout<<endl;
        cout<<"User uses X and computer uses O"<<endl<<endl;

        board[9]={0};

        int moves=0,k;

        while(moves < 9)
        {
            int mv;
            cout<<"Enter Player 1's Move"<<endl;
            cin>>mv;
            if(board[mv-1]==0)
            {
                board[mv-1]=-1; // -1 means user's move

                moves++;
                cout<<"Board after your move:"<<endl<<endl;
                visualize_board();

                if(ifwin()==0)  //still no winner
                {
                    k=computerMove();  //computer move
                    board[k]=1; // 1 means computer's move , check in draw_board function

                    cout<<"Board after computer's move:"<<endl;

                    visualize_board(); //drawing the board after computers move
                    moves++;

                    if (ifwin() != 0) //this means either user or computer has won
                    {
                        break;
                    }
                }
                else
                    break;
            }
            else
            {
                cout<<"Move not allowed"<<endl;
                continue;
            }
        }
        int ans=ifwin();
        if(ans==0)
            cout<<"Its a draw.Try harder next time"<<endl;
        else if(ans==1)
            cout<<"You lose, better luck next time"<<endl;
        else
            cout<<"You have beaten the computer"<<endl;


        cout<<"press 1 to play again, 0 to quit"<<endl;

        int inp;
        cin>> inp;

        if(inp==0){
            cout<<"quitting"<<endl;
            break;
        }

    }
	return 0;
}
