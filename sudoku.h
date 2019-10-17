void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);


/* Check if the board is fully filed  */
bool is_complete(char board[][9]);

/* Check if a move is valid and place a digit onto a Sudoku board  */
bool make_move(const char position[], char digit, char board[][9]);

/* Given the row/col index of an entry in the board, it returns the row/col index 
   of the centre entry of the corresponding subboard that this position belongs to.
*/
int centre_index(int index);

/* Save the board to a file. If sucess then returns 1 otherwise returns 0 */
bool save_board(const char* filename, const char board[][9]);

/* SudokuBoard: a board with pointers to next and previous board */
struct SudokuBoard;

/* Solve the sudoku puzzle */
bool solve_board(char board[][9]);

/* Algorithm for recursively solve the puzzle */
SudokuBoard solve_one_step(SudokuBoard &game);
