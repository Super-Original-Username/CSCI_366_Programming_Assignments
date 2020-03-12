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


/* Extra prototypes. Full definitions below*/
bool boardSizeVerifier(ifstream f, int expectedSize);

bool checkFileExistenceS(string s);




void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board)
{
   string new_p1b = "../" + p1_setup_board;
   string new_p2b = "../" + p2_setup_board;
   Server::board_size = board_size;
   if (checkFileExistenceS(new_p1b) && checkFileExistenceS(new_p2b))
   {
      //cout << "both boards exist";
      Server::p1_setup_board = ifstream(new_p1b);
      Server::p2_setup_board = ifstream(new_p2b);
      Server::p1_setup_board.close();
      Server::p2_setup_board.close();

      vector<vector<string>> p1b(board_size, vector<string>(board_size));
      vector<vector<string>> p2b(board_size, vector<string>(board_size));

      if (!boardSizeVerifier(ifstream(new_p1b), board_size) || !boardSizeVerifier(ifstream(new_p2b), board_size))
      {
         throw ServerException("one of these boards is the wrong size");
      }
   }
   else
   {
      throw ServerException("One of those files doesn't exist");
   }
}


/**
   I didn't want to change the contents of common, so I just added an s to the name of this.
   It is otherwise identical to the similarly named function in client.
   Uses stat.h to verify existence of files. Supposedly this is the fastest method
   for checking, according the the linked stackoverflow page.
   @param s - the name of the file to be checked
*/
bool checkFileExistenceS(string s)
{
   //solution from https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
   // supposedly a very fast way to verify the existence of a file on posix compliant systems
   struct stat buffer;
   return (stat(s.c_str(), &buffer) == 0);
}


/**
   Checks if the board file has the correct dimensions
   @param f - the ifstream for the file to be checked
   @param expectedSize - the size of the board
*/
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

   return false;
}

/**
 * @param shipList - ensures that SHIPS is a string. Probably unnecessary
 * @param c - the character to check
*/
bool shipTypeExists(const string & shipList, char c)
{
   cout<<shipList<<"\n";
   cout<<c<<"\n";
   bool res = (shipList.find(c) != std::string::npos);
   cout<<res<<"\n";
   return res;
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y)
{
   // checks that the player number is valid
   if (player < 1)
   {
      throw ServerException("You attempted to enter a player number less than 1");
   }
   else if (player > MAX_PLAYERS)
   {
      string ex = "You attempted to enter a player number greater than " + MAX_PLAYERS;
      throw ServerException(ex);
   }

   //checks that the coordinates are valid
   if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0)
   {
      return OUT_OF_BOUNDS;
   }
   // the following assumes that the input string is ALWAYS called  "player_#.setup_board.txt"
   // I have yet to find a way to make use of the ifstream class members outside of the initialize function.
   // as far as I can tell, these should have been defined as strings. I would make the change on my own, since I don't
   // make use of the ifstreams, though I'm concerned that there may be some conflict with the tests
   string boardName = "../player_" + to_string(player) + ".setup_board.txt";
   ifstream txtBoard(boardName);
   vector<vector<char>> board(board_size, vector<char>(board_size));
   string line;

   int row = 0; 
   //builds the board from file
   while (getline(txtBoard, line))
   {
      for (int col = 0; col < line.length(); col++)
      {
         board[row][col] = line[col];
         //cout<<line[col];
      }
      ++row;
      //cout<<"\n";
   }

   char coord = board[y][x];// this was for debugging. probably unnecessary now

   if (shipTypeExists(SHIPS,coord))
   {
      return HIT;
   }

   return MISS;
}

int Server::process_shot(unsigned int player)
{

   //same block as used in evaluate_shot, because I am lazy and it works
   if (player < 1)
   {
      throw ServerException("You attempted to enter a player number less than 1");
   }
   else if (player > MAX_PLAYERS)
   {
      string ex = "You attempted to enter a player number greater than " + MAX_PLAYERS;
      throw ServerException(ex);
   }

   int x, y;
   string shot = "player_" + to_string(player) + ".shot.json";
   if (!checkFileExistenceS(shot))
   {
      return NO_SHOT_FILE;
   }
   else
   {
      ifstream f(shot);
      cereal::JSONInputArchive inputArchive(f);
      inputArchive(x, y);

      f.close();

      remove(shot.c_str());

      string thisResult = "player_" + to_string(player) + ".result.json";
      ofstream res(thisResult);
      cereal::JSONOutputArchive outArch(res);
      int result = evaluate_shot(player, x, y);
      outArch(cereal::make_nvp("result", result));
      res.flush();
      
      

      return result;
   }
   return NO_SHOT_FILE;
}