//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Server.hpp"
#include <sys/stat.h>

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file)
{
}

bool boardSizeVerifier(ifstream f, int expectedSize);

bool checkFileExistenceS(string s);


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board)
{
   Server::board_size = board_size;
   if (checkFileExistenceS(p1_setup_board) && checkFileExistenceS(p2_setup_board))
   {

      Server::p1_setup_board = ifstream(p1_setup_board);
      Server::p2_setup_board = ifstream(p2_setup_board);
      Server::p1_setup_board.close();
      Server::p2_setup_board.close();

      vector<vector<string>> p1b(board_size, vector<string>(board_size));
      vector<vector<string>> p2b(board_size, vector<string>(board_size));

      if (!boardSizeVerifier(ifstream(p1_setup_board), board_size) || !boardSizeVerifier(ifstream(p2_setup_board), board_size))
      {
         throw ServerException("one of these boards is the wrong size");
      }
   }
   else
   {
      throw ServerException("One of those files doesn't exist");
   }
}

bool checkFileExistenceS(string s)
{
   //solution from https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
   // supposedly a very fast way to verify the existence of a file on posix compliant systems
   struct stat buffer;
   return (stat(s.c_str(), &buffer) == 0);
   /* ifstream f(s);
    //return f.good();
    return false;*/
}

bool boardSizeVerifier(ifstream f, int expectedSize)
{
   int numCols = 0;
   int numRows = 0;
   string l;
   getline(f, l);
   numCols = l.length();
   ++numRows;
   while (getline(f, l))
   {
      ++numRows;
   }
   if (numRows == numCols)
   {
      if (numRows == expectedSize)
      {
         return true;
      }
   }
   else
      return false;
}


bool shipTypeExists(char c){
   string ships = SHIPS;
   size_t ind = ships.find(c);
   if(ind < ships.length()){
      return true;
   }else{
      return false;
   }
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y)
{
   if(x>=BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0){
      return OUT_OF_BOUNDS;
   }
   string thisResult = "./outputs/player_" + to_string(player) + ".result.json";
   // the following assumes that the input string is ALWAYS called  "player_#.setup_board.txt"
   // I have yet to find a way to make use of the ifstream class members outside of the initialize function.
   // as far as I can tell, these should have been defined as strings. I would make the change on my own, since I don't
   // make use of the ifstreams, though I'm concerned that there may be some conflict with the tests
   string boardName = "./outputs/player_" + to_string(player) + ".setup_board.txt"; 
   vector<vector<string>> board(board_size, vector<string>(board_size));
   string line;
   int row = 0;
   while(getline(Server::p1_setup_board,line)){
      for(int col = 0; col < line.length(); col++){
         board[row][col] = line[col];
      }
      ++row;
   }




}

int Server::process_shot(unsigned int player)
{
   int x, y;
   string shot = "./outputs/player_" + to_string(player) + ".shot.json";
   if (!checkFileExistenceS(shot))
   {
      return NO_SHOT_FILE;
   }
   else
   {
      ifstream f(shot);
      cereal::JSONInputArchive inputArchive(f);
      inputArchive(x,y);
      f.close();
      remove(shot.c_str());
      return evaluate_shot(player,x,y);
   }
}