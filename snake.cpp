////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// purpose:  to create a version of the classic game snake with      ///////////
//           with elements of tron and the main goal is to move the  ///////////
//           move the snake as far as possible                       ///////////
//       written by: Michael Edegware                                ///////////
//               Date: 3/9/2016                                      ///////////
//               Joke: Q. How can a man go eight days without sleep? ///////////
//                     A. He sleeps at night                          //////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
using namespace std;
#include "termfuncs.h"
const int ROWS = 23; // the extra two array space is for the walls at both end
const int COLS = 63; // the extra two array space is for the walls at both end
const char SNAKE_HEAD_UP = '^';
const char SNAKE_HEAD_DOWN = 'v';
const char SNAKE_HEAD_LEFT = '<';
const char SNAKE_HEAD_RIGHT = '>';
const char SNAKE_TAIL = '#';
const char EGG = 'o';
const char BUG = 'X';
const char EMPTY = ' ';
int EU;
bool gameOver;
////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION DECLEARTION       /////////////////////////
////////////////////////////////////////////////////////////////////////////////
void setup_board(char board[ROWS][COLS]);
int play_game(char board[ROWS][COLS]);
void initialize_array(char array[ROWS][COLS]);
void populate_array(char board[ROWS][COLS]);
void display_game(char board[ROWS][COLS], int score);
void start_snakeHead(char array[ROWS][COLS]);
void set_foodX(char array[ROWS][COLS]);
void set_foodo(char array[ROWS][COLS]);
void set_EU();
void EU_remaining();
void read_input(char array[ROWS][COLS]);
void rotate_counterclock(char array[ROWS][COLS]);
void rotate_clock(char array[ROWS][COLS]);
void move_forward(char array[ROWS][COLS]);
void gain_EUX(char board[ROWS][COLS]);
void gain_EUo(char board[ROWS][COLS]);
int track_snakeLength(char array[ROWS][COLS]);
void move_left(char board[ROWS][COLS], int r, int c);
void move_right(char board[ROWS][COLS], int r, int c);
void move_up(char board[ROWS][COLS], int r, int c);
void move_down(char board[ROWS][COLS], int r, int c);
void runs_intoItSElf(char board[ROWS][COLS], int r, int c);
void report(int score);
int main() {
        char board[ROWS][COLS];
        int score;
        setup_board(board);
        score = play_game(board);
        report(score);
        return 0;
}
void setup_board(char board[ROWS][COLS]) {
        initialize_array(board);
        populate_array(board);
        gameOver = false;
        start_snakeHead(board);
        set_foodX(board); // initilizes foodX location
        set_foodo(board); // initialize foodo location
        set_EU();         // initialize the enrgy unit
}
////////////////////////////////////////////////////////////////////////////////
///                       POPULATE ARRAY                       /////////////////
/// purpose:      function to populate the array with strings  /////////////////
///               and give the key structure of the board.      ////////////////
/// argument:     2D strings of arrays                         /////////////////
/// return value: none                                         /////////////////
////////////////////////////////////////////////////////////////////////////////
//
void populate_array(char board[ROWS][COLS]) {
        // Top boundary
        for (int i = 1; i < COLS - 1; i++) {
                board[0][i] = '-';
        }
        board[0][0] = '+';
        board[0][COLS - 1] = '+';

        // Bottom boundary
        for (int i = 1; i < COLS - 1; i++) {
                board[ROWS - 1][i] = '-';
        }
        board[ROWS - 1][0] = '+';
        board[ROWS - 1][COLS - 1] = '+';

        // Left boundary
        for (int i = 1; i < ROWS - 1; i++) {
                board[i][0] = '|';
        }

        // Right boundary
        for (int i = 1; i < ROWS - 1; i++) {
                board[i][COLS - 1] = '|';
        }
}
////////////////////////////////////////////////////////////////////////////////
///                       INITIAIZE ARRAY                               ////////
/// purpose:      to fill all empty space in the array with a sting ' ' ////////
/// argument:     2D strings of arrays                                  ////////
/// return value: none                                                  ////////
////////////////////////////////////////////////////////////////////////////////
void initialize_array(char array[ROWS][COLS]) {
        for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                        array[r][c] = EMPTY;
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
///                      SET ENERGY                                /////////////
/// purpose:      to initialize energy unit                        /////////////
/// argument:     no                                               /////////////
/// return value: none                                             /////////////
////////////////////////////////////////////////////////////////////////////////
void set_EU() {  // energy level
        EU = 60; // startup energy unit
}
////////////////////////////////////////////////////////////////////////////////
///                   PRINT BOARD                                 //////////////
/// purpose:      function to print the element of a 2D           //////////////
///               array with strings                              //////////////
///               i.e draw the board on screen                    //////////////
/// argument:     2D strings of arrays and an integer score       //////////////
/// return value: none                                            //////////////
////////////////////////////////////////////////////////////////////////////////
void display_game(char board[ROWS][COLS], int score) {
        for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                        cout << board[r][c];
                }
                cout << endl;
        }
        cout << "EU: " << EU << " "
             << " "
             << "Score: " << score << endl;
}
////////////////////////////////////////////////////////////////////////////////
///                     PLAY GAME                                    ///////////
/// purpose:      function to play the classical snakegame while     ///////////
//                the boolen gameOver is false                       ///////////
/// argument:     2D strings of arrays                               ///////////
/// return value: score                                              ///////////
////////////////////////////////////////////////////////////////////////////////
int play_game(char board[ROWS][COLS]) {
        int score = 0;
        screen_clear();
        display_game(board, score);
        do {
                read_input(board); // takes input from user, without 'enter' key
                score = track_snakeLength(board);
                screen_clear();
                display_game(
                    board, score); // display screen after each move every move
        } while (!gameOver);       // Loop ends when energy is finished.
        return score;
}
////////////////////////////////////////////////////////////////////////////////
///                   SET SBNAKE HEAD                         //////////////////
/// purpose:      set the initial snake head location to      //////////////////
///               the center of the board                     //////////////////
///               and draw the boatd on screen                //////////////////
/// argument:     2D strings of arrays                        //////////////////
/// return value: none                                        //////////////////
////////////////////////////////////////////////////////////////////////////////
void start_snakeHead(char board[ROWS][COLS]) {
        // Starts snake in center of board
        board[(ROWS - 1) / 2][(COLS - 1) / 2] = SNAKE_HEAD_UP;
}
////////////////////////////////////////////////////////////////////////////////
///                    SET FOODX                               /////////////////
/// purpose:      set the food X at random places and          /////////////////
///               replaces it when eaten such that it's never  /////////////////
///               in on the snake or same place as food X      /////////////////
/// argument:     2D strings of arrays                         /////////////////
/// return value: none                                         /////////////////
////////////////////////////////////////////////////////////////////////////////
void set_foodX(char board[ROWS][COLS]) {
        // generates location of food at random
        int foodX_r = random_int(1, (ROWS - 2));
        int foodX_c = random_int(1, (COLS - 2));

        // sets food only if index contains "' '" or practically empty
        if (board[foodX_r][foodX_c] == EMPTY) {
                board[foodX_r][foodX_c] = BUG;
        } else {
                set_foodX(board); // relocate food if index is filled
        }
}
////////////////////////////////////////////////////////////////////////////////
///                         SET FOODO                           ////////////////
/// purpose:      set the food X at ranidom places and          ////////////////
///               replaces it when eaten such that it's never   ////////////////
///               in on the snake or same place as food X       ////////////////
/// argument:     2D strings of arrays                          ////////////////
/// return value: none                                          ////////////////
////////////////////////////////////////////////////////////////////////////////
void set_foodo(char board[ROWS][COLS]) {
        // generates location of food at random
        int foodo_r = random_int(1, (ROWS - 2));
        int foodo_c = random_int(1, (COLS - 2));

        // sets food only if index contains "' '" or practically empty
        if (board[foodo_r][foodo_c] == EMPTY) {
                board[foodo_r][foodo_c] = EGG;
        } else {
                set_foodo(board); // relocate food if index is filled
        }
}
////////////////////////////////////////////////////////////////////////////////
///////////////////   CHECK ENERGY LEVEL                      //////////////////
/// purpose:      function to reduce energy level by one      //////////////////
///               anytime the snake moves forward and give a  //////////////////
///               true value to thebool gameOver if EU        //////////////////
///               becomes zero.for energy level               //////////////////
/// argument:     no                                          //////////////////
/// return value: none                                        //////////////////
////////////////////////////////////////////////////////////////////////////////
void EU_remaining() {
        EU--;
        if (EU == 0) {
                gameOver = true;
        }
}
////////////////////////////////////////////////////////////////////////////////
///                      READ INPUT                           //////////////////
/// purpose:      function to read in input from users        //////////////////
///               without requiring them to press the enter   //////////////////
///               key and act on the input if valid.          //////////////////
///               and draw the boatd on screen                //////////////////
/// argument:     2D strings of arrays                        //////////////////
/// return value: none                                        //////////////////
////////////////////////////////////////////////////////////////////////////////
void read_input(char board[ROWS][COLS]) {
        char input = getachar(); // gets input without asking user to press
                                 // enter
        switch (input) {
        case 'l':
                rotate_counterclock(board);
                break;

        case 'r':
                rotate_clock(board);
                break;

        case 'f':
                move_forward(board);
                EU_remaining(); // reduce the energy level after move
                break;
        }
}
////////////////////////////////////////////////////////////////////////////////
///                  ROTATE HEAD COUNTER CLOCKWISE               ///////////////
/// purpose:      function to roate snake head counterclockwise  ///////////////
///               when the "L" key is pressed                    ///////////////
/// argument:     2D strings of arrays                           ///////////////
/// return value: none                                           ///////////////
////////////////////////////////////////////////////////////////////////////////
void rotate_counterclock(char board[ROWS][COLS]) {
        for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                        if (board[r][c] == SNAKE_HEAD_UP)
                                board[r][c] = SNAKE_HEAD_LEFT;
                        else if (board[r][c] == SNAKE_HEAD_LEFT)
                                board[r][c] = SNAKE_HEAD_DOWN;
                        else if (board[r][c] == SNAKE_HEAD_DOWN)
                                board[r][c] = SNAKE_HEAD_RIGHT;
                        else if (board[r][c] == SNAKE_HEAD_RIGHT)
                                board[r][c] = SNAKE_HEAD_UP;
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
///                    ROTATE HEAD CLOCKWISE                  //////////////////
/// purpose:      function to rotate the snake head clockwise //////////////////
///               when the "R" key is pressed                 //////////////////
/// argument:     2D strings of arrays                        //////////////////
/// return value: none                                        //////////////////
////////////////////////////////////////////////////////////////////////////////
void rotate_clock(char board[ROWS][COLS]) {
        for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                        if ((board[r][c] == SNAKE_HEAD_UP) ||
                            (board[r][c] == SNAKE_HEAD_LEFT) ||
                            (board[r][c] == SNAKE_HEAD_RIGHT) ||
                            (board[r][c] == SNAKE_HEAD_DOWN)) {
                                if (board[r][c] == SNAKE_HEAD_UP) {
                                        board[r][c] = SNAKE_HEAD_RIGHT;
                                } else if (board[r][c] == SNAKE_HEAD_LEFT) {
                                        board[r][c] = SNAKE_HEAD_UP;
                                } else if (board[r][c] == SNAKE_HEAD_DOWN) {
                                        board[r][c] = SNAKE_HEAD_LEFT;
                                } else if (board[r][c] == SNAKE_HEAD_RIGHT) {
                                        board[r][c] = SNAKE_HEAD_DOWN;
                                }
                        }
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
///                 MOVE SNAKE FORWARD                        //////////////////
/// purpose:      function to move snake forward and calls a  //////////////////
///               function that leave trails of "#" behind    //////////////////
/// argument:     2D strings of arrays                        //////////////////
/// return value: none                                        //////////////////
////////////////////////////////////////////////////////////////////////////////
void move_forward(char board[ROWS][COLS]) {
        for (int r = 1; r < ROWS; r++) {
                for (int c = 1; c < COLS; c++) {
                        if (board[r][c] == SNAKE_HEAD_UP) {
                                runs_intoItSElf(board, r, c);
                                move_up(board, r, c);
                                return;
                        } else if (board[r][c] == SNAKE_HEAD_LEFT) {
                                runs_intoItSElf(board, r, c);
                                move_left(board, r, c);
                                return;
                        } else if (board[r][c] == SNAKE_HEAD_RIGHT) {
                                runs_intoItSElf(board, r, c);
                                move_right(board, r, c);
                                return;
                        } else if (board[r][c] == SNAKE_HEAD_DOWN) {
                                runs_intoItSElf(board, r, c);
                                move_down(board, r, c);
                                return;
                        }
                }
        }
}

////////////////////////////////////////////////////////////////////////////////
///                      GAIN ENERGY                                 ///////////
/// purpose:      function to gain 25 EU when snake eats food 'X'    ///////////
/// argument:     2D strings of arrays                               ///////////
/// return value: none                                               ///////////
////////////////////////////////////////////////////////////////////////////////
void gain_EUX(char board[ROWS][COLS]) {
        EU = EU + 25;     // gains 25 EU
        set_foodX(board); // relocates food
}
////////////////////////////////////////////////////////////////////////////////
///                         GAIN ENERGY                              ///////////
/// purpose:      function to gain 15 EU when snake eats food 'o'    ///////////
/// argument:     2D strings of arrays                               ///////////
/// return value: none                                               ///////////
////////////////////////////////////////////////////////////////////////////////
void gain_EUo(char board[ROWS][COLS]) {
        EU = EU + 15;     // gains 15   EU
        set_foodo(board); // relocates food 'o'
}
////////////////////////////////////////////////////////////////////////////////
///               track snake length and keep score             ////////////////
/// purpose:      function to move snake forward                ////////////////
/// argument:     2D strings of arrays                          ////////////////
/// return value: count                                         ////////////////
////////////////////////////////////////////////////////////////////////////////
int track_snakeLength(char board[ROWS][COLS]) {
        int num_score = 0;
        for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                        if (board[r][c] == SNAKE_TAIL) {
                                num_score++;
                        }
                }
        }
        return num_score;
}
////////////////////////////////////////////////////////////////////////////////
///                              MOVE SNAKE UP                      ////////////
/// purpose:      to move the snake up the screen                   ////////////
/// argument:     2D strings of arrays and two integers c and r     ////////////
/// return value: none                                              ////////////
////////////////////////////////////////////////////////////////////////////////
void move_up(char board[ROWS][COLS], int r, int c) {
        if (r == 1) { // wrap snake when it reaches top end
                r = ROWS - 2;
                board[r][c] = SNAKE_HEAD_UP;
                board[1][c] = SNAKE_TAIL;
        } else if (board[r - 1][c] == BUG) {
                gain_EUX(board);
                board[r - 1][c] = SNAKE_HEAD_UP;
                board[r][c] = SNAKE_TAIL;
        } else if (board[r - 1][c] == EGG) {
                gain_EUo(board);
                board[r - 1][c] = SNAKE_HEAD_UP;
                board[r][c] = SNAKE_TAIL;
        } else {
                board[r - 1][c] = SNAKE_HEAD_UP;
                board[r][c] = SNAKE_TAIL;
        }
}
////////////////////////////////////////////////////////////////////////////////
///                            MOVE SNAKE LEFT                      ////////////
/// purpose:      to move the snake left the screen if snake's head ////////////
///               faces up                                          ////////////
/// argument:     2D strings of arrays and two integers c and r     ////////////
/// return value: none                                              ////////////
////////////////////////////////////////////////////////////////////////////////
void move_left(char board[ROWS][COLS], int r, int c) {
        if (c == 1) { // wrap snake when it reaches left end
                c = COLS - 2;
                board[r][c] = SNAKE_HEAD_LEFT;
                board[r][1] = SNAKE_TAIL;
        } else if (board[r][c - 1] == BUG) {
                gain_EUX(board);
                board[r][c - 1] = SNAKE_HEAD_LEFT;
                board[r][c] = SNAKE_TAIL;
        } else if (board[r][c - 1] == EGG) {
                gain_EUo(board);
                board[r][c - 1] = SNAKE_HEAD_LEFT;
                board[r][c] = SNAKE_TAIL;
        } else {
                board[r][c - 1] = SNAKE_HEAD_LEFT;
                board[r][c] = SNAKE_TAIL;
        }
}

////////////////////////////////////////////////////////////////////////////////
///                       MOVE SNAKE RIGHT                         /////////////
/// purpose:      function to move snake right the board if        /////////////
///               the head is facing the right                     /////////////
/// argument:     2D strings of arrays and two integers c and r    /////////////
/// return value: none                                             /////////////
////////////////////////////////////////////////////////////////////////////////
void move_right(char board[ROWS][COLS], int r, int c) {
        if (c == COLS - 2) { // wrap snake when it reaches right end
                c = 1;
                board[r][c] = SNAKE_HEAD_RIGHT;
                board[r][COLS - 2] = SNAKE_TAIL;
        } else if (board[r][c + 1] == BUG) {
                gain_EUX(board);
                board[r][c + 1] = SNAKE_HEAD_RIGHT;
                board[r][c] = SNAKE_TAIL;
        } else if (board[r][c + 1] == EGG) {
                gain_EUo(board);
                board[r][c + 1] = SNAKE_HEAD_RIGHT;
                board[r][c] = SNAKE_TAIL;
        } else {
                board[r][c + 1] = SNAKE_HEAD_RIGHT;
                board[r][c] = SNAKE_TAIL;
        }
}
////////////////////////////////////////////////////////////////////////////////
///                          MOVE SNAKE DOWN                       /////////////
/// purpose:      function to move snake right the board if        /////////////
///               the head is facing the right                     /////////////
/// argument:     2D strings of arrays and two integers c and r    /////////////
/// return value: none                                             /////////////
////////////////////////////////////////////////////////////////////////////////
void move_down(char board[ROWS][COLS], int r, int c) {
        if (r == ROWS - 2) { // wrap snake when it reaches botton end
                r = 1;
                board[r][c] = SNAKE_HEAD_DOWN;
                board[ROWS - 2][c] = SNAKE_TAIL;
        } else if (board[r + 1][c] == BUG) {
                gain_EUX(board);
                board[r + 1][c] = SNAKE_HEAD_DOWN;
                board[r][c] = SNAKE_TAIL;
        } else if (board[r + 1][c] == EGG) {
                gain_EUo(board);
                board[r + 1][c] = SNAKE_HEAD_DOWN;
                board[r][c] = SNAKE_TAIL;
        } else {
                board[r + 1][c] = SNAKE_HEAD_DOWN;
                board[r][c] = SNAKE_TAIL;
        }
}

////////////////////////////////////////////////////////////////////////////////
///                              KILL SNAKE                         ////////////
/// purpose:      function to to  check if snake hit itself and     ////////////
///               set the bool gameOver to true if snake hit itself ////////////
/// argument:     2D strings of arrays    and two integers c and r  ////////////
/// return value: none                                              ////////////
////////////////////////////////////////////////////////////////////////////////
void runs_intoItSElf(char board[ROWS][COLS], int r, int c) {
        if (board[r][c] == SNAKE_HEAD_UP) {
                if (r != 1 && board[r - 1][c] == SNAKE_TAIL) {
                        gameOver = true;
                } else if (r == 1 && board[ROWS - 2][c] == SNAKE_TAIL) {
                        gameOver = true;
                }
        } else if (board[r][c] == SNAKE_HEAD_DOWN) {
                if (r != ROWS - 2 && board[r + 1][c] == SNAKE_TAIL) {
                        gameOver = true;
                } else if (r == ROWS - 2 && board[1][c] == SNAKE_TAIL)
                        gameOver = true;
        } else if (board[r][c] == SNAKE_HEAD_LEFT) {
                if (c != 1 && board[r][c - 1] == SNAKE_TAIL) {
                        gameOver = true;
                } else if (c == 1 && board[r][COLS - 2] == SNAKE_TAIL) {
                        gameOver = true;
                }
        } else if (board[r][c] == SNAKE_HEAD_RIGHT) {
                if (c != COLS - 2 && board[r][c + 1] == SNAKE_TAIL) {
                        gameOver = true;
                } else if (c == COLS - 2 && board[r][1] == SNAKE_TAIL) {
                        gameOver = true;
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
///                         REPORT SCORE                           /////////////
/// purpose:      function to report game score at the end of the  /////////////
///               game                                             /////////////
/// argument:     an intger score                                  /////////////
/// return value: none                                             /////////////
////////////////////////////////////////////////////////////////////////////////
void report(int score) {
        cout << "Game Over.  Final Score:"
             << " " << score << endl;
}
