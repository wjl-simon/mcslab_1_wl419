#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << '\n';
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << '\n';
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << '\n';
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << '\n';
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r) << "   ";
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */
/*=======================================================================*/

/* Check if the board is fully filed  */
bool is_complete(char board[][9])
{  // flag indicates if there is an entry unfilled by a digit in the board
  bool flag = 1;
  
  for(int i = 0; i < 9; i++)
    for(int j = 0; j < 9; j++)
      if(board[i][j]<'0' || board[i][j]>'9')
      {
        flag = 0; // If one isn't occupied by a digit: quit
        break;
      }

  return flag;
}

/* Check if a move is valid and place a digit onto a Sudoku board  */
bool make_move(const char position[], char digit, char board[][9])
{ //---Translate the coordinates into indices of an array---
  int row_id = static_cast<int>(position[0]) - 65; // row index
  int col_id = static_cast<int>(position[1]) - 49; // column index

  //------Check if this move is valid----
  // 1. Check if the digit is valid
    if(digit < '0' || digit > '9') return false;
  
  // 2. check if out of range:
  if(row_id < 0 || row_id > 8 || col_id < 0 || col_id > 8) return false;
    
  // 3. Check if two copies of the same digit are in the same col
  for(int i = 0; i < 9; i++)
    if(i == row_id) continue;
    else if(board[i][col_id] == digit) return false;

  // 4. Check if two copies of the same digit are in the same row
  for(int j = 0; j < 9; j++)
    if(j == col_id) continue;
    else if(board[row_id][j] == digit) return false;

  // 5. Check if two copies of the same digit are in the same subboard
  // Indices of the centre entry of the corresonping subborad that the position belongs to
  int row_ctr = centre_index(row_id), col_ctr = centre_index(col_id);
  for(int i = row_ctr-1; i <= row_ctr+1; i++)
    for(int j = col_ctr-1; j <= col_ctr+1; j++)
      if(i==row_id && j==col_id) continue;
      else if(board[i][j] == digit) return false;

  //---If this move is valid the program should manage to run to here---
  board[row_id][col_id] = digit; // Place the desirable digit
  return true;
}

/* 
   Given the row/col index of an entry in the board, it returns the row/col index 
   of the centre entry of the corresponding subboard that this position belongs to.
*/
int centre_index(int index)
{
  switch(index)
  {
    case 0:
    case 1:
    case 2: return 1; break;
    
    case 3:
    case 4:
    case 5: return 4; break;

    case 6:
    case 7:
    case 8: return 7; break;
    default: return 0;
  }
}


/* Save the board to a file. If sucess then returns 1 otherwise returns 0 */
bool save_board(const char* filename, const char board[][9])
{ // Create the output file
  ofstream outfile; outfile.open(filename);

  // Output the content of the board into the file
  if(!outfile.fail())
  {
    for(int i = 0; i < 9; i++)
      for(int j = 0; j < 9; j++)
      {
        outfile << board[i][j];
        if(j == 8)
          outfile << endl;
      }

    outfile.close(); // close the file  
    return true;
  }
  else return false;
}


/* SudokuBoard: a board with pointers to next and previous board */
struct SudokuBoard
{
  char board[9][9]; // a copy of sudoku board
  char successful_digit; // The digit of this board's successful move
  SudokuBoard *previous; // Pointers to the previous/next board
  //SudokuBoard *next;
  SudokuBoard *GetAddress(){ return this; } // Return the address of itself
  void suicide() { delete this; } // delete itself
  void CopyBoard(const char board_src[][9]) //  Copy the board from others
  {
    for(int i = 0; i < 9; i++)
      for(int j = 0; j < 9; j++)
        this->board[i][j] = board_src[i][j];
  }
};


/* Solve the sudoku puzzle */
bool solve_board(char board[][9])
{ // Encapsulate the board into of SudokuBoard struct
  //SudokuBoard *front;
  SudokuBoard game;
  //game.previous = front;
  game.previous = NULL; game.CopyBoard(board); game.successful_digit = '0';
  // Solve it!
  SudokuBoard solution = solve_one_step(game);
  // Write the solution into board
  if(is_complete(solution.board))
  {
    for(int i = 0; i < 9; i++)
      for(int j = 0; j < 9; j++)
        board[i][j] = solution.board[i][j];    
    return true;
  }
  else return false;
}

/* Algorithm for recursively solve the puzzle */
SudokuBoard solve_one_step(SudokuBoard &game)
{ // The index of the current entry in which we attempt to fill
  static int cur_row = 0; static int cur_col = 0;

  // Only solve if the board isn't completed
  if(!is_complete(game.board))
  {  //----1. Search for the next position of an empty entry----
    bool found_new_pos = 0; // flag indicates if new position is found
    for(int i = cur_row; i < 9; i++)
    {
      if(!found_new_pos)
      {
        for(int j = 0; j < 9; j++)
          if(game.board[i][j] == '.')
          { // Update the current position in which we fill
            found_new_pos = 1;
            cur_row = i; cur_col = j;
            cout<<"curent pos " << char(i+65) << "," << j+1 << endl;
            cin.ignore();
            break;
          }
      }
      else break;
    }

    //----2. Create a copy of the board----
    SudokuBoard *game_copy = new SudokuBoard;
    game_copy->CopyBoard(game.board);
    game_copy->previous = game.GetAddress();
    //game_copy->next = NULL;
    //game.next = game_copy->GetAddress();
   
    //----3. Attempt to solve----
    // Convert the position  back to a string
    char position[3] = {(char)(cur_row+65), (char)(cur_col+49), '\0'};
    // At current position, this digit is either invalid or will lead to a dead end
    static char fail_digit = '0';

    for(char ch = fail_digit + 1; ch <= '9'; ch++)
    {
      if(make_move(position, ch, game_copy->board))
      {// If new move succeeds, that move is stored and game_copy becomes a new board
        cout << "Successful new move in " << (char)(cur_row+65) << ","
             << cur_col+1 << " : " << ch << endl;
        display_board(game_copy->board);
        cin.ignore();
        game_copy->successful_digit = ch;
        fail_digit = '0';
        return solve_one_step(*game_copy);
      }
      else fail_digit = ch;
    }

    //---- 4. Capture the exceptions----
    // if the program manages to run here: the previous step(s) is(are) wrong
    // so we delete the newed board
    //game.next = NULL;
    cout << "Failed attempt! delete the current board!" << endl;
    cin.ignore();
    delete game_copy;
    // Tell the program that the previous step is wrong and don't do that again
    if(game.successful_digit < '9') // Very likly the lastes previous step is wrong
    {
      fail_digit = game.previous->successful_digit;
      game.previous->successful_digit = '0';
      //game.previous->next = NULL;
    // Resume to the lastest previous board
    SudokuBoard *last_game = game.previous->GetAddress();
    game.suicide(); // Kill the current board
    return solve_one_step(*last_game);
    }
    else // Very likely the second previous is wrong!
    {
      fail_digit = game.previous->previous->successful_digit;
      game.previous->previous->successful_digit = '0';
      // Resume to the second last board
      SudokuBoard *last_last_game = game.previous->previous->GetAddress();
      game.previous->suicide(); game.suicide();
      return solve_one_step(*last_last_game);
    }
  }
  else return game; // The final solution has already found
}
