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

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    Client::player = player;
    Client::board_name = "player_" + player + ".result.json";
    Client::board_size = board_size;
    int [board_size][board_size] trackingBoard;
}


void Client::fire(unsigned int x, unsigned int y) {
    int sendX = x - 1;
    int sendY = y - 1;
    bool newShot = false;
    while(!newShot){
    if (trackingBoard[sendY][sendX] == 1){
        cout << "Looks like you've already fired there, try again\n";
        cout << "Enter fire x position: ";
        cin >> x;
        cout << "Enter fire y position: ";
        cin >> y;
    }
    else{
        trackingBoard[sendY][sendX] = 1;
        newShot = true;
    }
    }
    cereal::JSONOutputArchive archive( std::board_name );
    bool arr[] = {true, false};
    std::vector<int> vec = {1, 2, 3, 4, 5};
    archive(trackingBoard);

}


bool Client::result_available() {
}


int Client::get_result() {
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {
}


string Client::render_action_board(){
}