//
//  main.cpp
//  assignment4
//
//  Created by Jing Liang on 3/7/18.
//  Copyright © 2018 Jing Liang. All rights reserved.
//

#include <iostream>
#include "SFGame.hpp"
#include "SFPlayer.hpp"
#include "SFRealPlayer.cpp"
int main(int argc, const char* argv[]){
    SFRealPlayer yourPlayer(PieceColor::red);
    SFRealPlayer opponentPlayer(PieceColor::black);
    SFGame *theGame = SFGame::create();
    theGame->run(yourPlayer,opponentPlayer);
    delete theGame;
    return 0;
}
