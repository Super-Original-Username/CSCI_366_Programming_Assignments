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
#include "Client.hpp"

bool checkFileExistence(string s);

Client::~Client()
{
}

void Client::initialize(unsigned int player, unsigned int board_size)
{
    //bool newPlayer = false;
    //while(!newPlayer){
    try
    {
        player = player;
        if(player > 2){
            throw new ClientWrongPlayerNumberException;
        }
        board_size = board_size;
        board_name = "./boardStates/player_" + to_string(player) + ".result.json";
        if (checkFileExistence(board_name))
        {
            //throw new ClientException("It looks like player " + to_string(player) + " already has a board.");
            cout << "that player already has a board\n";
        }
        else
        {
            //cereal is new and scary and I don't know how to deal with it, so I made a vector
            //vectors are also new and scary and I don't know how to deal with them
            std::vector<std::vector<int>> board(
                board_size,
                std::vector<int>(board_size));
            std::ofstream f(board_name);

            cereal::JSONOutputArchive archive(f);
            archive(board);
            initialized = true;
            f.flush();
            //f.close();
        }
    }catch (ClientException& e) {
        cout << e.what() << '\n';
    }catch (ClientWrongPlayerNumberException& e){
        cout << e.what() << '\n';
    }
    //cout << "init done\n";
}

bool checkFileExistence(string s)
{
    ifstream f(s);
    //return f.good();
    return false;
}

void Client::fire(unsigned int x, unsigned int y)
{
    cout<<"fire called\n";
   //std::ofstream outf;
   //outf.open(board_name);
   //cout<<"ofstram made\n";
   std::ifstream inf(board_name);
   cout<<"ifstram\n";
    std::vector<std::vector<int>> trackedBoard(
            board_size,
            std::vector<int>(board_size));
    cout << "vector made";
    //cereal::JSONOutputArchive outputArchive(outf);

    cereal::JSONInputArchive inputArchive(inf);
    inputArchive(trackedBoard);
    cout<<"read archive\n";
    int sendX = x - 1;
    int sendY = y - 1;
    bool newShot = false;
    do{
        cout<<"in the loop\n";
        if (trackedBoard[sendY][sendX] == 1)
        {
            cout << "Looks like you've already fired there, try again\n";
            cout << "Enter fire x position: ";
            cin >> x;
            cout << "Enter fire y position: ";
            cin >> y;
        }
        else
        {
            trackedBoard[sendY][sendX] = 1;
            newShot = true;
        }
    }while (!newShot);
    //outputArchive(trackedBoard);
}

bool Client::result_available()
{
    cout<<"teehee you dumb";
    return false;
}

int Client::get_result()
{
    cout<< "teehee you dumb";
    return 0;
}

void Client::update_action_board(int result, unsigned int x, unsigned int y)
{
}

string Client::render_action_board()
{
    return "teehee you dumb";
}