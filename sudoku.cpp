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
bool is_complete(char board[][9]){
  // flag indicates if there is an entry unfilled by a digit in the board
  bool flag = 1;
  
  for(int i = 0; i < 9; i++)
    for(int j = 0; j < 9; j++)
      if(board[i][j]<'0' || board[i][j]>'9'){
        flag = 0; // If one isn't occupied by a digit: quit
        break;
      }

  return flag;
}

/* Check if a move is valid and place a digit onto a Sudoku board  */
bool make_move(const char position[], char digit, char board[][9]){
  // Translate the coordinates into indices of an array
  int row_id = static_cast<int>(position[0]) - 65; // row index
  int col_id = static_cast<int>(position[1]) - 49; // column index

  //=====Check if this move is valid=====
  // Check if the digit is valid
    if(digit < '0' || digit > '9') return false;
  
  // check if out of range:
  if(row_id < 0 || row_id > 8 || col_id < 0 || col_id > 8) return false;
    
  // Check if two copies of the same digit are in the same col
  for(int i = 0; i < 9; i++)
    if(i == row_id) continue;
    else if(board[i][col_id] == digit) return false;

  // Check if two copies of the same digit are in the same row
  for(int j = 0; j < 9; j++)
    if(j == col_id) continue;
    else if(board[row_id][j] == digit) return false;

  // Check if two copies of the same digit are in the same subboard
  // Indices of the centre entry of the corresonping subborad that the position belongs to
  int row_ctr = centre_index(row_id), col_ctr = centre_index(col_id);
  for(int i = row_ctr-1; i <= row_ctr+1; i++)
    for(int j = col_ctr-1; j <= col_ctr+1; j++)
      if(i==row_id && j==col_id) continue;
      else if(board[i][j] == digit) return false;

  // If this move is valid the program should manage to run to here
  board[row_id][col_id] = digit; // Put the desirable digit
  return true;
}

/* Check if one entry is occupied by a digit*/
//bool is_entry_filled(const char board[][9], int row_id, int col_id){
//  if(board[row_id][col_id]>='0' && board[row_id][col_id]<='9')
//    return 1;
//  else
//    return 0;
//}


/* Given the row/col index of an entry in the board, it returns the row/col index 
   of the centre entry of the corresponding subboard that this position belongs to.
*/
int centre_index(int index){
  switch(index){
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
bool save_board(const char* filename, const char board[][9]){
  // Create the output file
  ofstream outfile; outfile.open(filename);

  // Output the content of the board into the file
  if(!outfile.fail()){
    for(int i = 0; i < 9; i++)
      for(int j = 0; j < 9; j++){
        outfile << board[i][j];
        if(j == 8)
          outfile << endl;
      }

    outfile.close(); // close the file  
    return true;
  }
  else
    return false;
}
