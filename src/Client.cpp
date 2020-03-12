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
#include <sys/stat.h> // this is for checking the existence of files. As far as I understand, this __only__ works on posix compliant systems

bool checkFileExistence(string s);

Client::~Client()
{
}

void Client::initialize(unsigned int player, unsigned int board_size)
{
    try
    {
        Client::player = player;
        if (player > 2)
        {
            throw ClientWrongPlayerNumberException();
        }
        Client::board_size = board_size;
        board_name = "player_" + to_string(player) + ".action_board.json";
        if (checkFileExistence(board_name))
        {
            throw ClientException("It looks like player " + to_string(player) + " already has a board.");
            //cout << "That player already has a board. Delete and remake?\n (Y/n)";
            
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
            archive(cereal::make_nvp("board",board));
            initialized = true;
            f.flush();
        }
    }
    catch (ClientException &e)
    {
        cout << e.what() << '\n';
    }
    catch (ClientWrongPlayerNumberException &e)
    {
        cout << e.what() << '\n';
    }
    //cout << "init done\n";
}

bool checkFileExistence(string s)
{
    //solution from https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    // supposedly a very fast way to verify the existence of a file on posix compliant systems
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
    /* ifstream f(s);
    //return f.good();
    return false;*/
}

void Client::fire(unsigned int x, unsigned int y)
{
    string shot = "player_" + to_string(player) + ".shot.json";
    //cout << "fire called\n";
    std::ifstream inf(board_name);

     
    //cout<<"ifstram\n";
    std::vector<std::vector<int>> trackedBoard(
        board_size,
        std::vector<int>(board_size));
    cereal::JSONInputArchive inputArchive(inf);
    inputArchive(trackedBoard);
    inf.close();


    //cout << trackedBoard.size();
    for (int i = 0; i < trackedBoard.size(); i++)
    {
        for (int j = 0; j < trackedBoard[i].size(); j++)
        {
            //cout << trackedBoard[i][j];
        }
        //cout << '\n';
    }
    //cout << "read archive\n";
    int sendX = x;
    int sendY = y;

    std::ofstream outf(shot);
    cereal::JSONOutputArchive outArch(outf);
    outArch(cereal::make_nvp("x",sendX),cereal::make_nvp("y",sendY));
    outf.flush();
    //outputArchive(trackedBoard);
}

bool Client::result_available()
{
    string thisResult = "player_" + to_string(player) + ".result.json";
    return checkFileExistence(thisResult);
}

int Client::get_result()
{

    int res;
    string thisResult = "player_" + to_string(player) + ".result.json";
    std::ifstream inf(thisResult);
    cereal::JSONInputArchive inputArchive(inf);
    inputArchive(res);
    inf.close();
    remove(thisResult.c_str());
    if(res != HIT && res != MISS && res != OUT_OF_BOUNDS){
        throw exception();
    }
    return res;
   
}

void Client::update_action_board(int result, unsigned int x, unsigned int y)
{
    if(result == OUT_OF_BOUNDS){

    }else{
    int inX = x;
    int inY = y;

    std::ifstream inf(board_name);     

    std::vector<std::vector<int>> trackedBoard(
        board_size,
        std::vector<int>(board_size));

    cereal::JSONInputArchive inputArchive(inf);
    inputArchive(trackedBoard);
    inf.close();

    trackedBoard[inY][inX] = result;
    //cout << trackedBoard[inX][inY];

    std::ofstream outf(board_name);

    cereal::JSONOutputArchive outarch(outf);
    outarch(cereal::make_nvp("board",trackedBoard));
    outf.flush();
    }

}

string Client::render_action_board()
{
    string toReturn = "";

    std::ifstream inf(board_name);
    //cout<<"ifstram\n";
    std::vector<std::vector<int>> trackedBoard(
        board_size,
        std::vector<int>(board_size));
    cereal::JSONInputArchive inputArchive(inf);
    inputArchive(trackedBoard);
    inf.close();

    for (int i = 0; i < trackedBoard.size(); i++)
    {
        for (int j = 0; j < trackedBoard[i].size(); j++)
        {
            toReturn += trackedBoard[i][j];
        }
        toReturn += '\n';
    }
    return toReturn;
}