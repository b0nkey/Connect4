#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <ctype.h>
#include "connect4.h"


// TODO FEATURE undo
// TODO FEATURE flip tokens every 5 turns
// FEATURE colour tokens
// FEATURE colour selection
// FEATURE added error sounds to error messages
char p1Colour, p2Colour,gLevel;

int main ( void ){

    int option ;
	Game g ;

	// intitialise random seed
	srand(time(NULL));

    while (( option = main_menu()) != -1 ){
        if ( option == 1 ){
            // setup a new game
            setup_game ( &g ) ;
            // now play this game
            play_game ( &g ) ;
        } else if ( option == 2 ){
            // attempt to load the game from the save file
            if ( load_game ( &g, "game.txt" ) == 0 ){
                // if the load went well, resume this game
                play_game ( &g ) ;
            } else {
                printf ( "Loading game failed.\n") ;
            }
        } else if ( option == -1 ){
            printf ( "Exiting game, goodbye!\n") ;
        }
    }
}

int main_menu ( void ){
	// will loop until something is returned
	while(1){
		char menuIn;
		printf("Welcome to connect 4\nn: New game\nl: Load game\nq: Quit Game\n");
		gets(&menuIn);

		if(menuIn=='n'){
			return 1;
		}else if(menuIn=='l'){
			return 2;
		}else if(menuIn=='q'){
			return -1;
		}else{
			printf("\nEnter a valid option.\n\a");
		}
	}
}

int column_full ( int board[COLS][ROWS], int col ){
    if(board[col-1][0]!=0){
        return 1;
    }else{
        return 0;
    } 
}

int get_move ( int board[COLS][ROWS] ){
	// will loop until something is returned
	while(1){
		char input=0;
		int a;
		printf("Enter your move: \n"); 
		gets(&input);    //reads a line of text from the user

		if(input=='s' || input=='S'){   //checks if the user has entered s or q for saving for quitting
			return -1;
		}else if(input=='q' || input=='Q'){
			return -2;
		}

		a=atoi(&input); //converts a to an int
		
		if(input=='\0'){ //null input check
			puts("Enter a move before pressing enter.\a");
			continue; //continue stops current loop only
		}
		
		if(column_full(board, a)==1){ //full column check
			puts("Column full. Try again.\a");
			continue;
		}
		
		if(a>0 && a<8){     //checks that the int is in the column range
			return a;
		}else{
			printf("%d is Invalid Input\n\a", input);
		}
		system("beep1.mp3");
	}
} 

int add_move ( int board[COLS][ROWS], int col, int colour ){
    for(int i=6;i>0;i--){
        if(board[col-1][i-1]==0){
            board[col-1][i-1]=colour;
            return 0;
        }
    } 
	return -1;
}

int board_full ( int board[COLS][ROWS] ){
	// adds one to a counter if there is something in the column. if it reaches 7, the column is full.
    int i, count=0;
    for(i=0;i<7;i++){
      if(board[i][0]!=0){
        count++;
      }
    }

    if(count==7){
        return 1;
    }else{
        return 0;
    }
}

int display_board ( int board[COLS][ROWS] ){
	// loops through and displays the appropriate token if there is one there from the board[][] data. uses colour codes.
    int i, j;
    for(i=0;i<6;i++){
        printf(" |");
        for(j=0;j<7;j++){
            if(board[j][i]==1){
				if(p1Colour=='r' || p1Colour=='R'){
					printf("\x1B[31m" "o" "\033[0m");
					printf("|");
				}else if(p1Colour=='g' || p1Colour=='G'){
					printf("\x1B[32m" "o" "\033[0m");
					printf("|");
				}else if(p1Colour=='b' || p1Colour=='B'){
					printf("\x1B[34m" "o" "\033[0m");
					printf("|");
				}
				// printf("o|");

            }else if(board[j][i]==2){
				if(p2Colour=='r' || p2Colour=='R'){
					printf("\x1B[31m" "x" "\033[0m");
					printf("|");
				}else if(p2Colour=='g' || p2Colour=='G'){
					printf("\x1B[32m" "x" "\033[0m");
					printf("|");
				}else if(p2Colour=='b' || p2Colour=='B'){
					printf("\x1B[34m" "x" "\033[0m");
					printf("|");
				}
				// printf("x|"); 
            }else{
                printf(" |");
            }
        }
        printf("\n");
    }
    printf(" |=|=|=|=|=|=|=|\n");
    printf(" |1|2|3|4|5|6|7|\n");
    printf("                \n");

}

int winner ( int board[COLS][ROWS] ){
   int i, j, p1Count=0, p2Count=0;
    //HORIZONTAL
    for(i=0;i<6;i++){       //rows loop
        for(j=0;j<7;j++){   //cols loop
            if(board[j][i]==1){    //if this location = 1, add 1 to player 1 counter and reset player 2 counter
                p1Count++;
                p2Count=0;	// if a counter has been added to, reset the opposite counter
            }
            if(board[j][i]==2){
                p1Count=0;
                p2Count++;
            }
            if(board[j][i]==0){
                p1Count=0;
                p2Count=0;
            }
			
            if(p1Count>=4){
                return 1;
            }
            if(p2Count>=4){
                return 2;
            }
        }
        p1Count=0;	// resets at each row loop
        p2Count=0;
    }
    //VERTICAL
    for(i=0;i<7;i++){       //cols
        for(j=0;j<6;j++){   //rows
            if(board[i][j]==1){
                p1Count++;
                p2Count=0;
            }
            if(board[i][j]==2){
                p1Count=0;
                p2Count++;
            }
            if(board[i][j]==0){
                p1Count=0;
                p2Count=0;
            }
            if(p1Count>=4){
                return 1;
            }
            if(p2Count>=4){
                return 2;
            }
        }
        p1Count=0;	// resets at each col loop
        p2Count=0;
    }
    //Diagonal		the first if statement in the for loop to make sure that it is only checking inside the bounds of the board. then it pluses or minuses the ROWS/COLS accordingly to check each direction
    for(i=0;i<7;i++){       //cols
        for(j=0;j<6;j++){//rows
			// ++
			if(i<=3 && j<=2){
				if(board[i][j]==1 && board[i+1][j+1]==1 && board[i+2][j+2]==1 && board[i+3][j+3]==1){
					return 1;
				}
				if(board[i][j]==2 && board[i+1][j+1]==2 && board[i+2][j+2]==2 && board[i+3][j+3]==2){
					return 2;
				}
			}
			// +-
			if(i<=3 && j<=3){
				if(board[i][j]==1 && board[i+1][j-1]==1 && board[i+2][j-2]==1 && board[i+3][j-3]==1){
					return 1;
				}
				if(board[i][j]==2 && board[i+1][j-1]==2 && board[i+2][j-2]==2 && board[i+3][j-3]==2){
					return 2;
				}
			}
			// -+
			if(i>=3 && j>=2){
				if(board[i][j]==1 && board[i-1][j+1]==1 && board[i-2][j+2]==1 && board[i-3][j+3]==1){
					return 1;
				}
				if(board[i][j]==2 && board[i-1][j+1]==2 && board[i-2][j+2]==2 && board[i-3][j+3]==2){
					return 2;
				}
			}
			// --
			if(i>=3 && j>=3){
				if(board[i][j]==1 && board[i-1][j-1]==1 && board[i-2][j-2]==1 && board[i-3][j-3]==1){
					return 1;
				}
				if(board[i][j]==2 && board[i-1][j-1]==2 && board[i-2][j-2]==2 && board[i-3][j-3]==2){
					return 2;
				}
			}
        }
    }
	return 0;
}

int  save_game ( Game g, char filename[] ){
	int i, j;

    //file opening
    FILE *fp;
    fp=fopen("save.txt", "w");
    if(fp==NULL){
        perror("Cannot open file\n");   //Check if file is actually open
    }

    //player1 h/c save
    if(g.player1==1){
        fprintf(fp, "1");
    }else{
        fprintf(fp, "0");
    }


    //player2 h/c save
    if(g.player2==1){
        fprintf(fp, "1");
    }else{
        fprintf(fp, "0");
    }

    //turn save
    if(g.turn==1){
        fprintf(fp, "1");
    }else{
        fprintf(fp, "0");
    }

    //winner save
    switch(g.winner){
        case 0:
            fprintf(fp, "0");
            break;
        case 1:
            fprintf(fp, "1");
            break;
        case 2:
            fprintf(fp, "2");
            break;
    }
    fprintf(fp, "\n"); //newline for board state

    //saving board state
    for(i=0;i<6;i++){ //rows
        for(j=0;j<7;j++){ //cols

            switch(g.board[j][i]){
                case 0:
                    fprintf(fp, "0");
                    break;

                case 1:
                    fprintf(fp, "1");
                    break;

                case 2:
                    fprintf(fp, "2");
                    break;
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);  
   // return save_game_lib ( g, filename ) ;
}

int load_game ( Game *g, char filename[] ){
	// I couldn't get this to work
	int i=0, j=0, k=0;
    
    FILE *fp;
    fp=fopen("save.txt", "r");
    char gameData[53];


    while(!feof(fp)){
        fgets(gameData, 53, fp);    //populate 1d array
    }
	
	for(i=0;i<53;i++){
		printf("%c.", gameData[i]);
	}
	printf("\n");
	/* for(i=0;i<53;i++){
		// player1 h/c
		if(i==0){
			if(gameData[i]=='0'){
                g->player1=0;
				// printf("Player 1: %s / Computer", gameData[i]);
				puts("Player 1: Computer");
            }else if(gameData[i]=='1'){
                g->player1=1;
				// printf("Player 1: %s / Human", gameData[i]);
				puts("Player 1: Human");
            }else{
				puts("Error loading player 1 h/c");
			}
			continue;
		}
		// player2 h/c
		if(i==1){
			if(gameData[i]=='0'){
                g->player2=0;
				puts("Player 2: Computer");
            }else if(gameData[i]=='1'){
                g->player2=1;
				puts("Player 2: Human");
            }else{
				puts("Error loading player 2 h/c");
			}
			continue;
		}
		// player turn
		if(i==2){
			if(gameData[i]=='0'){
                g->turn=0;
            }else if(gameData[i]=='1'){
                g->turn=1;
            }else{
				puts("Error loading player turn");
			}
			continue;
		}
		// winner
		if(i==3){
            if(gameData[i]=='0'){
                g->winner=0;
            }else if(gameData[i]=='1'){
                g->winner=1;
            }else if(gameData[i]=='2'){
                g->winner=2;
            }else{
				puts("Error loading win state");
			}
			continue;
        }
		// game data
		// j=cols  k=rows
		if(i>=5){
			if(gameData[i]=='\n'){
				k++;
				continue;
			}else if(gameData[i]=='0'){
				g->board[j][k]=0;
				j++;
			}else if(gameData[i]=='1'){
				g->board[j][k]=1;
				j++;
			}else if(gameData[i]=='2'){
				g->board[j][k]=2;
				j++;
			}
			
		}
	} */
	
	for(i=0;i<53;i++){
		// player1 h/c
		if(i==0){
			if(gameData[i]==0){
                g->player1=0;
				// printf("Player 1: %s / Computer", gameData[i]);
				puts("Player 1: Computer");
            }else if(gameData[i]==1){
                g->player1=1;
				// printf("Player 1: %s / Human", gameData[i]);
				puts("Player 1: Human");
            }else{
				puts("Error loading player 1 h/c");
			}
			continue;
		}
		// player2 h/c
		if(i==1){
			if(gameData[i]==0){
                g->player2=0;
				puts("Player 2: Computer");
            }else if(gameData[i]==1){
                g->player2=1;
				puts("Player 2: Human");
            }else{
				puts("Error loading player 2 h/c");
			}
			continue;
		}
		// player turn
		if(i==2){
			if(gameData[i]==0){
                g->turn=0;
            }else if(gameData[i]==1){
                g->turn=1;
            }else{
				puts("Error loading player turn");
			}
			continue;
		}
		// winner
		if(i==3){
            if(gameData[i]==0){
                g->winner=0;
            }else if(gameData[i]==1){
                g->winner=1;
            }else if(gameData[i]==2){
                g->winner=2;
            }else{
				puts("Error loading win state");
			}
			continue;
        }
		// game data
		// j=cols  k=rows
		if(i>=5){
			if(gameData[i]=='\n'){
				k++;
				continue;
			}else if(gameData[i]==0){
				g->board[j][k]=0;
				j++;
			}else if(gameData[i]==1){
				g->board[j][k]=1;
				j++;
			}else if(gameData[i]==2){
				g->board[j][k]=2;
				j++;
			}
			
		}
	}
	
	
	/*
	
    for(i=0;i<53;i++){

        if(gameData[i]=='\n'){
			
            continue;
        }

        if(i==0){       //player 1 h/c
            if(gameData[i]=='0'){
                g->player1=0;
            }else if(gameData[i]=='1'){
                g->player1=1;
            }
        }

        if(i==1){       //player 2 h/c
            if(gameData[i]=='0'){
                g->player2=0;
            }else if(gameData[i]=='1'){
                g->player2=1;
            }
        }

        if(i==2){       //whos turn
            if(gameData[i]=='0'){
                g->turn=0;
            }else if(gameData[i]=='1'){
                g->turn=1;
            }
        }

        if(i==3){       //winner
            if(gameData[i]=='0'){
                g->winner=0;
            }else if(gameData[i]=='1'){
                g->winner=1;
            }else if(gameData[i]=='2'){
                g->winner=2;
            }
        }


        if(i>3){
            if(gameData[i]=='0'){
                g->board[j][k]=0;
                j++;            //moves along the columns
                if(j==6){       //if cols hits 6 (7), reset it and move down a row
                    j=0;
                    k++;
                }

            }

            if(gameData[i]=='1'){
                g->board[j][k]=1;
                j++;            //moves along the columns
                if(j==6){       //if cols hits 6 (7), reset it and move down a row
                    j=0;
                    k++;
                }

            }


            if(gameData[i]=='2'){
                g->board[j][k]=2;
                j++;            //moves along the columns
                if(j==6){       //if cols hits 6 (7), reset it and move down a row
                    j=0;
                    k++;
                }

            }
        }
    }
	*/
    fclose(fp); 
    // return load_game_lib ( g, filename ) ;
}

int computer_move ( int board[COLS][ROWS], int colour, int level ){
    time_t t;
	int a,b,i,j,opponent,tempBoard[7][6];
	level=gLevel;
    srand((unsigned) time(&t));
    int selection=0; 
	
	//setting variables for h/c
	if(colour==1){
		opponent=2;
	}else{
		opponent=1;
	}
	
	// level 0: will just return a random column on ever turn
	// level 1: will choose a winning move if there is one available, otherwise the move will be random.  win>rand
	// Level 3: will block opponents win move if it can, second prioroty will be to win, third will be random. block>win>rand
	
    if(level==0){
        selection = (rand()%7)+1; // returns a random num
        return selection;
    }else if(level==1){
		for(a=0;a<7;a++){	//columns
		
			//tempBoard population loop
			for(i=0;i<7;i++){
				for(j=0;j<6;j++){
					tempBoard[i][j]=board[i][j];
				}
			}
			
			for(b=5;b>=0;b--){	//moves up rows
				//tempBoard population loop
				for(i=0;i<7;i++){
					for(j=0;j<6;j++){
						tempBoard[i][j]=board[i][j];
					}
				}
				
				if(tempBoard[a][b]==0){		//if this location is free
					tempBoard[a][b]=colour;	//make it not free
					//win check
					if(winner(tempBoard)==colour){
						return a+1;
					}
				}
			}
			//rand loop
			if(a==6){
				selection = (rand()%7)+1;
				return selection;
			}
		}
	}else if(level==2){
		for(a=0;a<7;a++){	//columns
		
			// win move loop
			for(b=5;b>=0;b--){	//moves up rows
				//tempBoard population loop
				for(i=0;i<7;i++){
					for(j=0;j<6;j++){
						tempBoard[i][j]=board[i][j];
					}
				}
				if(tempBoard[a][b]==0){		//if this location is free
					tempBoard[a][b]=colour;	//make it not free
					//win check
					if(winner(tempBoard)==colour){
						return a+1;
					}
				}
			}
			
			//opponent block loop
			for(b=5;b>=0;b--){	//moves up rows
				//tempBoard population loop
				for(i=0;i<7;i++){
					for(j=0;j<6;j++){
						tempBoard[i][j]=board[i][j];
					}
				}
				if(tempBoard[a][b]==0){		//if this location is free
					tempBoard[a][b]=opponent;	//make it not free
					//win check
					if(winner(tempBoard)==opponent){
						return a+1;
					}
				}
			}
			
			//rand loop
			if(a==6){
				selection = (rand()%7)+1;
				return selection;
			}
		}	
	}
	return 0;
}

int setup_game ( Game *g ){
	char p1, p2, level;
	time_t t;
	int i, j, rnd;
    srand((unsigned) time(&t));
	
	//player 1 h/c
	printf("Player 1 human/computer (h/c)");
	while(1){
		gets(&p1);
		if(p1=='h'){
			g->player1=1;
			break;
		}else if(p1=='c'){
			g->player1=0;
			break;
		}else{
			printf("Invalid Input. Please enter \"h\" or \"c\"\n");
		}
	}
	// p1 colour
	printf("Pick a colour Red/Green/Blue (r/g/b): ");
	while(1){
		gets(&p1Colour);
		if(p1Colour=='r' || p1Colour=='R' || p1Colour=='g' || p1Colour=='G' || p1Colour=='b' || p1Colour=='B'){
			break;
		}else{
			puts("Invalid Input");
		}
	}
	
	//player 2 h/c
	printf("Player 2 human/computer (h/c)");
	while(1){
		gets(&p2);
		if(p2=='h'){
			g->player2=1;
			break;
		}else if(p2=='c'){
			g->player2=0;
			break;
		}else{
			printf("Invalid Input. Please enter \"h\" or \"c\"\n");
		}
	}
	
	// p2 colour
	if(p1Colour=='r' || p1Colour=='R'){
		printf("Pick a colour Green/Blue (g/b): ");
		while(1){
			gets(&p2Colour);
			if(p2Colour=='g' || p2Colour=='G' || p2Colour=='b' || p2Colour=='B'){
				break;
			}else{
				puts("Invalid Input");
			}
		}
	}else if(p1Colour=='g' || p1Colour=='G'){
		printf("Pick a colour Green/Blue (r/b): ");
		while(1){
			gets(&p2Colour);
			if(p2Colour=='r' || p2Colour=='R' || p2Colour=='b' || p2Colour=='B'){
				break;
			}else{
				puts("Invalid Input");
			}
		}
	}else if(p1Colour=='b' || p1Colour=='B'){
		printf("Pick a colour Green/Blue (r/g): ");
		while(1){
			gets(&p2Colour);
			if(p2Colour=='r' || p2Colour=='R' || p2Colour=='g' || p2Colour=='G'){
				break;
			}else{
				puts("Invalid Input");
			}
		}
	}

	
	
	// level
	if(p1=='c' || p2=='c'){
		printf("AI level(1/2/3): ");
		while(1){
			gets(&level);
			if(level=='1'){
				gLevel=0;
				puts("Level 1 selected");
				break;
			}else if(level=='2'){
				gLevel=1;
				puts("Level 2 selected");
				break;
			}else if(level=='3'){
				gLevel=2;
				puts("Level 3 selected");
				break;
			}else{
				puts("Invalid input. Enter 1/2/3");
			}
		}
	}
	
	//board initialisation
	for(i=0;i<7;i++){
		for(j=0;j<6;j++){
			g->board[i][j]=0;
		}
	}
	
	//set winner to 0
	g->winner=0;
	
	//randomises turn
	rnd=(rand()%2)+1;
	g->turn=rnd;
	return 0;
}

int play_game ( Game *g ){
    return play_game_lib ( g ) ;
}
