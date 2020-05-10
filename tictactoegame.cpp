
#include<iostream>
int sizeboard=9;
int board[9]={0};
void printboard(int board[9]);
int whowon(int board[9]);
int compmove(int board[9]);
bool emptycell(int i);


int main()
{

	//Player = -1 ; Computer = 1
	while(true){
        std::cout <<"~~~~~~~~~~~~Welcome To Tic Tac Toe~~~~~~~~~~~~~"<<"\n";

        std::cout<<"\n";

        std::cout << "----++---++----" << "\n";
        std::cout << "| 1 || 2 || 3 |" << "\n";
        std::cout << "----++---++----" << "\n";
        std::cout << "| 4 || 5 || 6 |" << "\n";
        std::cout << "----++---++----" << "\n";
        std::cout << "| 7 || 8 || 9 |" << "\n";
        std::cout << "----++---++----" << "\n" << "\n" << "\n";

        std::cout<<"The numbers you see on board are allowed moves"<<"\n"<<"\n";
        std::cout<<"select one of those as input"<<"\n";
        int board[9]={0};
        int moves=0,k=0;
        while(moves < sizeboard){
            int n;
            std::cout<<"\n"<<"Enter your move"<<"\n";
            std::cin>>n;

            if(board[n-1]==0)
            {
                board[n-1]=-1; // -1 means user's move
                moves++;
                std::cout<<"\n"<<"After your move board :: "<<"\n";
                printboard(board);
                if(whowon(board)==0) //still no winner
                {

                    k=compmove(board);
                    board[k]=1;

                    std::cout<<"\n"<<"Board after computer's move"<<"\n";

                    printboard(board);

                    moves++;
                    if (whowon(board) != 0) //this means either user or computer has won
                    {
                        break;
                    }
                }
                else
                    break;
            }
            else{
                std::cout<<"Invalid move"<<"\n";
            }
        }
        int ans=whowon(board);
        if(ans==0)
        {
            std::cout<<"You drew"<<"\n";
        }
        else if(ans==1)
        {
            std::cout<<"You lose"<<"\n";
        }
        else if(ans==-1)
        {
           std::cout<<"You have beaten computer"<<"\n";
        }


        std::cout<<"press 1 to play again, 0 to quit"<<"\n";

        int inp;
        std::cin>> inp;

        if(inp==0){
            std::cout<<"quitting"<<"\n";
            break;
        }
    }
	return 0;
}


bool emptycell(int i)
{
    if(board[i]==0)
        return true;
    else
        return false;
}


void printboard(int board[9]) {
	int pos=0, i=0, j=0;
	std::cout<<"----++---++----"<<"\n";
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			std::cout<<"| ";
			if(board[pos] == 0){
				std::cout<<pos+1<<" |";
			}
			else{
				if(board[pos]==-1) //for the user its cross
                {
					std::cout<<"X |";
				}
				else{
					std::cout<<"O |";  //for the computer its circle
				}
			}
			pos++;
		}
			std::cout<<"\n"<<"----++---++----"<<"\n";
	}
}


int whowon(int board[9]) {
    //determines if a player has won, returns 0 otherwise.
    int rows[3][3] = {{0,1,2},{3,4,5},{6,7,8}};
    int c[3][3]= {{0,3,6},{1,4,7},{2,5,8}};
    int diag[2][3]={{0,4,8},{2,4,6}};  //index for the rows ,columns and the diagonals for checking purpose
    int i,j;
    for(j = 0; j < 3; ++j) {
        if(board[rows[j][0]] != 0 &&
  			board[rows[j][0]] == board[rows[j][1]] && board[rows[j][1]] == board[rows[j][2]])
            return board[rows[j][2]];
    }

    for(j = 0; j < 3; ++j) {
        if(board[c[j][0]] != 0 &&
  			board[c[j][0]] == board[c[j][1]] && board[c[j][1]] == board[c[j][2]])
            return board[c[j][0]];
    }
    for(j = 0; j < 2; ++j) {
        if(board[diag[j][0]] != 0 &&
  			board[diag[j][0]] == board[diag[j][1]] && board[diag[j][1]] == board[diag[j][2]])
            return board[diag[j][0]];
    }
    return 0;  //if no one has won
}

int miniMaxtic(int board[9], int currplayer) {
    int currwinner = whowon(board);

    if(currwinner != 0)
    	return currwinner*currplayer;

    int bestmove = -1;
    int score = -2;
    int i=0,j=0;
    int currentScore=0;
    for(i = 0; i < sizeboard; i++) {

        if(board[i]==0) {

            board[i] = currplayer; //setting as the player's move
            currentScore = -1*miniMaxtic(board, -1*currplayer);

            if(currentScore > score) {
                score = currentScore;   //finding the best score for level in the tree
                bestmove = i;
            }
            board[i] = 0;//Reset board after try
        }
    }
    if(bestmove == -1)
     return 0;

    return score;
}

int compmove(int board[9]) {
    int bestmove = -1;
    int score = -2;
    int currScore;
    int computer=-1;
    int i,j;
    for(i = 0; i < sizeboard; ++i) {

        if(board[i]==0) {
            board[i] = 1;
            currScore = -miniMaxtic(board, computer);
            board[i] = 0; //resetting to unoccupied

            if(currScore > score) {
                score = currScore;
                bestmove = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    return bestmove;
}
