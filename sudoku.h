void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);

bool is_complete(char board[][9]);

/* Check if a move is valid and place a digit onto a Sudoku board  */
bool make_move(const char position[], char digit, char board[][9]);

/* Given the row/col index of an entry in the board, it returns the row/col index 
   of the centre entry of the corresponding subboard that this position belongs to.
*/
int centre_index(int index);
