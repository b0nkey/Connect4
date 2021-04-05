#ifndef CONNECT4_H
#define CONNECT4_H 1

#define ROWS 6      // height of the board
#define COLS 7      // width of the board (values of 9 are going to display poorly!!)

// These lines detect what sort of compiler you are using. This is used to handle the time delay
// function wait() in various operating systems. Most OS will use sleep(), however for windows it is
// Sleep() instead.
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


typedef struct {

    int player1, player2 ;      // variables for each player - 1 for human, 0 for computer player
    int board[COLS][ROWS] ;     // the game board. 0 for empty space, 1 for player 1, 2 for player 2
                                // Note that row 0 is the TOP row of the board, not the bottom!
                                // column 0 is on the left of the board
    int turn ;                  // whose turn it is, 1 or 2
    int winner ;                // who has won - 0 for nobody, 1 for player 1, 2 for player 2
	int level;
	int turnCount;
} Game ;

// displays the welcome screen and main menu of the game, and prompts the user to enter an option until
// a valid option is entered.
// Returns 1 for new game, 2 for load game, -1 for quit
int main_menu ( void ) ;

// displays the board to the screen
int display_board ( int[COLS][ROWS] ) ;


// sets up the game to a new state
// prompts the user if each player should be a human or computer, and initialises the relevant fields
// of the game structure accordingly
int setup_game ( Game *g ) ;


// Returns TRUE if the specified column in the board is completely full
// FALSE otherwise
// col should be between 1 and COLS
int column_full ( int[COLS][ROWS], int col ) ;


// plays a game until it is over
int play_game ( Game *g ) ;


// prompts the user to enter a move, and checks that it is valid
// for the supplied board and board size
// Returns the column that the user has entered, once it is valid (1-COLS)
// note that this value is betweeen 1 and COLS (7), NOT between 0 and 6!!
// If the user enters 'S' or 's' the value -1 should be returned, indicating that the game should be saved
// If the user enters 'Q' or 'q' the value -2 should be returned, indicating that the game should be abandoned
int get_move ( int[COLS][ROWS] ) ;

// calcualtes a column for the computer to move to, using artificial "intelligence"
// The 'level' argument describes how good the computer is, with higher numbers indicating better play
// 0 indicates very stupid (random) play, 1 is a bit smarter, 2 smarter still, etc..
int computer_move ( int[COLS][ROWS], int colour, int level ) ;

// adds a token of the given value (1 or 2) to the board at the
// given column (col between 1 and COLS inclusive)
// Returns 0 if successful, -1 otherwise
int add_move ( int b[COLS][ROWS], int col, int colour ) ;

// determines who (if anybody) has won.  Returns the player id of the
// winner, otherwise 0
int winner ( int[COLS][ROWS] ) ;

// determines if the board is completely full or not
int board_full ( int[COLS][ROWS] ) ;


// saves the game to the specified file. The file is text, with the following format
// player1 player2 turn winner
// board matrix, each row on a separate line
// Example:
//
//1 0 1 0        player 1 human, player 2 computer, player 1's turn, nobody has won
//0 0 0 0 0 0 0  board state - 1 for player 1's moves, 2 for player 2's moves, 0 for empty squares
//0 0 0 0 0 0 0
//0 0 0 2 0 0 0
//0 0 0 2 0 0 0
//0 2 1 1 1 0 0
//0 2 2 1 1 2 1
int save_game ( Game g, char filename[] ) ;


// loads a saved game into the supplied Game structure. Returns 0 if successfully loaded, -1 otherwise.
int load_game ( Game *g, char filename[] ) ;


// waits for s seconds - platform specific! THIS FUNCTION IS INCLUDED IN THE LIBRARY, NO NEED TO WRITE IT!
void wait_seconds ( int s ) ;


// library versions of functions. Exactly the same behaviour done by course staff. Please just call these if you have not completed your version as yet.
int display_board_lib ( int[COLS][ROWS] ) ;
int setup_game_lib ( Game *g ) ;
int column_full_lib ( int[COLS][ROWS], int col ) ;
int play_game_lib ( Game *g ) ;
int get_move_lib ( int[COLS][ROWS] ) ;
int add_move_lib ( int b[COLS][ROWS], int col, int colour ) ;
int winner_lib ( int[COLS][ROWS] ) ;
int board_full_lib ( int[COLS][ROWS] ) ;
int computer_move_lib ( int[COLS][ROWS], int colour, int level ) ;
int save_game_lib ( Game g, char filename[] ) ;
int load_game_lib ( Game *g, char filename[] ) ;
int main_menu_lib ( void ) ;


#endif
