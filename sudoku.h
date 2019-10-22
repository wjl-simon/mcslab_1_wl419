void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);


/* Check if the board is fully filed  */
bool is_complete(char board[][9]);

/* Check if a move is valid and place a digit onto a Sudoku board  */
bool make_move(const char position[], char digit, char board[][9]);

/* Save the board to a file. If sucess then returns 1 otherwise returns 0 */
bool save_board(const char* filename, const char board[][9]);

/* Solve the sudoku puzzle */
bool solve_board(char board[][9]);

/* Copy the board from others */
void copy_board(char board_des[][9], const char board_src[][9]);

/* Recursive Solver for the Sudoku puzzle */
bool sudoku_solver(char board[][9]);
