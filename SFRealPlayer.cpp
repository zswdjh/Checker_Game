//
//  SFRealPlayer.cpp
//  assignment4
//
//  Created by Jing Liang on 3/11/18.
//  Copyright © 2018 Jing Liang. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include "SFGame.hpp"
#include "SFPlayer.hpp"
#include <stdexcept>
class SFRealPlayer: public SFPlayer{
public:
    SFRealPlayer(PieceColor color):SFPlayer(color){};
    
    void takeTurn(SFGame &aGame){
        getPieceLocation(aGame);
        getMyMoves(aGame);
        char type = chooseMyBestMove(aGame);
        if(type=='m'){
            aGame.move(this,SFLocation(srcRow,srcCol),SFLocation(dstRow1,dstCol1));
        }else if(type=='1'){
            aGame.jump(this, SFLocation(srcRow,srcCol), SFLocation(dstRow1,dstCol1));
        }else if(type=='2'){
            aGame.jump(this, SFLocation(srcRow,srcCol), SFLocation(dstRow1,dstCol1), SFLocation(dstRow2,dstCol2));
        }else if(type=='3'){
            aGame.jump(this, SFLocation(srcRow,srcCol), SFLocation(dstRow1,dstCol1), SFLocation(dstRow2,dstCol2),SFLocation(dstRow3,dstCol3));
        }else if(type=='4'){
            aGame.jump(this, SFLocation(srcRow,srcCol), SFLocation(dstRow1,dstCol1), SFLocation(dstRow2,dstCol2),SFLocation(dstRow3,dstCol3),SFLocation(dstRow4,dstCol4));
        }else if(type=='d'){
            throw std::invalid_argument("You have no where to go");
        }
        myPieceLocation.erase(myPieceLocation.begin(),myPieceLocation.end());
        myOppoLocation.erase(myOppoLocation.begin(),myOppoLocation.end());
        moveDst.erase(moveDst.begin(),moveDst.end());
        jumpDst.erase(jumpDst.begin(),jumpDst.end());
        srcRow = 0;
        srcCol = 0;
    }
    
    
    void getPieceLocation(SFGame &aGame){
        auto& tiles = aGame.getBoard().getTiles();
        PieceColor compareColor= PieceColor::red;
        for(int row= 0; row<8; row++){
            for(int col = 0; col<8; col++){
                auto singleTile = tiles[row][col];
                if( singleTile.isEmpty()==false ){   //current tile is not empty
                    if(this->getColor()==compareColor){
                        if(singleTile.getPiece().getColor() == compareColor){  //color on current tile eqauls the color of player
                            myPieceLocation.push_back(std::make_pair(row,col));
                        }
                        else{
                            myOppoLocation.push_back(std::make_pair(row,col));
                        }
                    }
                    else{        //player is black
                        if(singleTile.getPiece().getColor() != compareColor){  //color on current tile eqauls the color of my color
                            myPieceLocation.push_back(std::make_pair(row,col));
                        }
                        else{
                            myOppoLocation.push_back(std::make_pair(row,col));
                        }
                    }
                }
                else{
                    continue;
                }
            }
        }
    }
    
    
    void getMyMoves(SFGame &aGame){
        for(auto const& locpair: myPieceLocation){
            int row = locpair.first;
            int col = locpair.second;
            bool canJump = getAllJumpForSinglePiece(aGame,row, col);  //vector is not null
            if(canJump==false)
               getAllMoveForSinglePiece(aGame,row, col);//vector is not null
        }
    }
    
    bool getAllJumpForSinglePiece(SFGame &aGame, int aRow, int aCol){ //now JumpDst contains all the possible jump destinations from (aRow,aCol) and return true if there is jump available, false otherwise
        //at most 4 jumps
        size_t initialLen = jumpDst.size();
        if(MySingleJump(aGame, aRow, aCol))
        {
            size_t curLenAfterOneJump = jumpDst.size();
            size_t NewAddedAfterOneJump = curLenAfterOneJump - initialLen;
            auto it = jumpDst.begin();
            if(initialLen!=0){
                for(int i=0;i<initialLen;i++)
                {
                    it++;
                }
            }
            if(NewAddedAfterOneJump!=0)
            {
                for (int count=0;count<NewAddedAfterOneJump;count++)
                {
                    //it++;
                    int curRow = it->second.first;
                    int curCol = it->second.second;
                    MySingleJump(aGame,curRow, curCol);
                }
                size_t curLenAfterTwoJump = jumpDst.size();
                size_t NewAddedAfterTwoJump = curLenAfterTwoJump-curLenAfterOneJump;
                if(NewAddedAfterTwoJump!=0){
                    for(int count=0;count<NewAddedAfterTwoJump;count++)
                    {
                        it++;
                        int curRow = it->second.first;
                        int curCol = it->second.second;
                        MySingleJump(aGame, curRow, curCol);
                    }
                    size_t curLenAfterThreeJump = jumpDst.size();
                    size_t NewAddedAfterThreeJump = curLenAfterThreeJump-curLenAfterTwoJump;
                    if(NewAddedAfterThreeJump!=0)
                    {
                        for(int count=0;count<NewAddedAfterThreeJump;count++)
                        {
                            it++;
                            int curRow = it->second.first;
                            int curCol = it->second.second;
                            MySingleJump(aGame, curRow, curCol);
                        }
                    }
                }
            }
            return true;
        }
        else
            return false;
    }
    bool MySingleJump(SFGame &aGame,int aRow,int aCol){ //by default, suppose the color of my piece is red, and check is there any possibility for me to have a forward jump. Return true if I have a possibility to jump one step, and the new destination location has been already pushed in JumpDst.
        PieceColor compareColor= PieceColor::red;
        auto tiles = aGame.getBoard().getTiles();
        int oppoRow = 0;
        int oppoRow1 = 0;
        if(this->getColor()==compareColor){
            if(!tiles[aRow][aCol].isEmpty() && tiles[aRow][aCol].getPiece().isKing()==true){  //red king
                oppoRow1 = aRow-1;
            }
            oppoRow = aRow+1;  //red pawn
        }
        else{
            if( !tiles[aRow][aCol].isEmpty() && tiles[aRow][aCol].getPiece().isKing()==true){  //black king
                oppoRow1 = aRow+1;
            }
            oppoRow = aRow-1;
        }
        int oppoCol1 = aCol -1;
        int oppoCol2 = aCol +1;
        bool exist1 = false;
        bool exist2 = false;
        exist1 = givenOppoCheckMy(aGame, oppoRow, oppoCol1,aRow,aCol);
        exist2 = givenOppoCheckMy(aGame, oppoRow, oppoCol2,aRow,aCol);
        if(!tiles[aRow][aCol].isEmpty() && tiles[aRow][aCol].getPiece().isKing()==false){
            if(exist1||exist2)
                return true;
        }
        else{
            bool exist3 = false;
            bool exist4 = false;
            exist3 = givenOppoCheckMy(aGame, oppoRow1, oppoCol1,aRow,aCol);
            exist4 = givenOppoCheckMy(aGame, oppoRow1, oppoCol2,aRow,aCol);
            if(exist1||exist2||exist3||exist4)
                return true;
        }
        return false;
    }
    bool givenOppoCheckMy(SFGame &aGame, int aRow, int aCol,int aOriginRow,int aOriginCol){  //given(aRow,aCol) for opponent's color piece, judge is there any possibility that my piece can jump ahead of it and take off it
        if(checkLocValid(aRow, aCol) && checkOppoValid(aRow, aCol)){//if it is valid tile and has opponent piece
            int potenMyRow;
            int potenMyCol1;
            if(aRow-aOriginRow==1 && aCol-aOriginCol==1){
                potenMyRow = aRow+1;
                potenMyCol1 = aCol+1;
            }
            else if(aRow-aOriginRow==1 && aCol-aOriginCol==-1){
                potenMyRow = aRow+1;
                potenMyCol1 = aCol-1;
            }
            else if(aRow-aOriginRow==-1 && aCol-aOriginCol==1){
                potenMyRow = aRow-1;
                potenMyCol1 = aCol+1;
            }
            else{
                potenMyRow = aRow-1;
                potenMyCol1 = aCol-1;
            }
            bool dstvalid= checkLocValid(potenMyRow, potenMyCol1);
            std::pair<int,int> srcLoc;
            srcLoc = std::make_pair(aOriginRow, aOriginCol);
            if(checkEmptyLoc(aGame,potenMyRow,potenMyCol1)&& dstvalid){
                std::pair<int,int> potenLoc;
                potenLoc = std::make_pair(potenMyRow, potenMyCol1);
                if(!checkJumpRd(srcLoc,potenLoc)){
                    srcRow = aOriginRow;
                    srcCol = aOriginCol;
                    jumpDst.push_back(std::make_pair(srcLoc,potenLoc));
                    return true;
                }
            }
        }
        return false;
    }
    bool checkJumpRd(std::pair<int,int> startpair, std::pair<int,int> checkpair){  //true: have redundancy   false: no redundancy
        for(auto pos = jumpDst.begin();pos!=jumpDst.end();pos++){
            if(pos->first.first==startpair.first && pos->first.second==startpair.second && pos->second.first ==checkpair.first && pos->second.second == checkpair.second)
                return true;
            else
                continue;
        }
        return false;
    }
    
    bool checkOppoValid(int aRow,int aCol){    //check whether (aRow,aCol) has opponents' piece
        std::pair<int,int> checkpair;
        checkpair = std::make_pair(aRow,aCol);
        auto pos = find(myOppoLocation.begin(),myOppoLocation.end(),checkpair);
        if(pos!=myOppoLocation.end())
            return true;
        else
            return false;
    }
    bool checkEmptyLoc(SFGame &aGame,int aRow, int aCol){  //check whether this tile is empty
        auto& tiles = aGame.getBoard().getTiles();
        auto singleTile = tiles[aRow][aCol];
        if(singleTile.isEmpty())
            return true;
        else
            return false;
    }
    
    bool checkLocValid(int aRow,int aCol){   //generally, check whether (aRow,aCol) is a valid tile
        if(aRow<=7 && aRow>=0 && aCol<=7 && aCol>=0)
            return true;
        else
            return false;
    }
    bool checkEachMove(SFGame &aGame,int aRow,int aCol,int aOriginRow,int aOriginCol){  //check whether each move is valid and store them in moveDST
        if(checkEmptyLoc(aGame,aRow,aCol)){
            std::pair<int, int> newpair;
            newpair = std::make_pair(aRow,aCol);
            if(checkLocValid(aRow, aCol)){
                std::pair<int,int> srcLoc;
                srcLoc = std::make_pair(aOriginRow, aOriginCol);
                moveDst.push_back(std::make_pair(srcLoc,newpair));
                return true;
            }
            
        }
        return false;
    }
    
    bool getAllMoveForSinglePiece(SFGame &aGame, int aRow, int aCol)//now MoveDst contains all the possible move destinations from (aRow,aCol) and return true if there is jump available, false otherwise
    {
            auto tiles = aGame.getBoard().getTiles();
            PieceColor compareColor= PieceColor::red;
            int possibleRowOne = aRow+1;
            int possibleRowTwo = aRow-1;
            int possibleColOne = aCol+1;
            int possibleColTwo = aCol-1;
        if(tiles[aRow][aCol].getPiece().isKing()==false)  //pawn
        {
            if(this->getColor()==compareColor){   //red pawn
                if(checkEachMove(aGame, possibleRowOne, possibleColOne,aRow,aCol)||checkEachMove(aGame, possibleRowOne, possibleColTwo,aRow,aCol))
                    return true;
            }
            else{     //black pawn
                if(checkEachMove(aGame, possibleRowTwo, possibleColOne,aRow,aCol)||checkEachMove(aGame, possibleRowTwo, possibleColTwo,aRow,aCol))
                    return true;
            }
        }
        else{  //red black king
            if(checkEachMove(aGame, possibleRowOne, possibleColOne,aRow,aCol)||checkEachMove(aGame, possibleRowOne, possibleColTwo,aRow,aCol)||checkEachMove(aGame, possibleRowTwo, possibleColOne,aRow,aCol)||checkEachMove(aGame, possibleRowTwo, possibleColTwo,aRow,aCol)){
                return true;
            }
        }
        return false;
    }
    
    char chooseMyBestMove(SFGame &aGame){
        char result;
        if(jumpDst.size()!=0){   //can jump
            result = jumpStrategy(aGame);
        }
        else
        {
            if(moveDst.size()!=0)   //can move
            {
                result = MoveStrategy(aGame);
            }
            else{
                result = 'd';   //can not jump and move,return d;
            }
        }
        return result;
    }
    
    struct decision{
        bool exist;
        int nextRow;
        int nextCol;
    };
    decision findInJumpDst(int aRow,int aCol){  //find whether (aRow,aCol) shows up in JumpDst(source pair, destination pair) as a source pair
        for(auto it = jumpDst.begin();it!=jumpDst.end();it++){
            if(it->first.first==aRow && it->first.second==aCol){
                decision returnValue = {true,it->second.first,it->second.second};
                return returnValue;
            }
            else{
                continue;
            }
        }
        decision returnValue = {false,0,0};
        return returnValue;
    }
    
    bool findInMyPieceLoc(int aRow,int aCol){ //find whether(aRow,aCol) shows up in myPieceLocation
        for(auto it=myPieceLocation.begin();it!=myPieceLocation.end();it++){
            if(it->first==aRow && it->second==aCol){
                return true;
            }
            continue;
        }
        return false;
    }
    
    
    char jumpStrategy(SFGame &aGame){  //choose the maximum jump times
        int NumJump = 0;
        int CurNumJump = 0;
        int tempDstRow1;int tempDstRow2;int tempDstRow3;int tempDstRow4;
        int tempDstCol1;int tempDstCol2;int tempDstCol3;int tempDstCol4;
        for(auto iter = jumpDst.begin();iter!=jumpDst.end();iter++)
        {
            tempDstRow1 = 0;tempDstRow2 = 0;tempDstRow3 = 0;tempDstRow4 = 0;
            tempDstCol1 = 0;tempDstCol2 = 0;tempDstCol3 = 0;tempDstCol4 = 0;
            CurNumJump = 0;
            if(findInMyPieceLoc(iter->first.first, iter->first.second))
            {
                srcRow = iter->first.first;
                srcCol = iter->first.second;
                decision next = findInJumpDst(srcRow, srcCol);
                tempDstRow1 = next.nextRow;
                tempDstCol1 = next.nextCol;
                CurNumJump++;
                next = findInJumpDst(tempDstRow1, tempDstCol1);
                if(next.exist==true)
                {
                    CurNumJump++;
                    tempDstRow2 = next.nextRow;
                    tempDstCol2 = next.nextCol;
                    next = findInJumpDst(tempDstRow2, tempDstCol2);
                    if(next.exist==true)
                    {
                        CurNumJump++;
                        tempDstRow3 = next.nextRow;
                        tempDstCol3 = next.nextCol;
                        next = findInJumpDst(tempDstRow3, tempDstCol3);
                        if(next.exist==true)
                        {
                            CurNumJump++;
                            tempDstRow4 = next.nextRow;
                            tempDstCol4 = next.nextCol;
                        }
                    }
                }
                if(CurNumJump>=NumJump)
                {
                    dstRow1 = tempDstRow1; dstRow2 = tempDstRow2; dstRow3 = tempDstRow3; dstRow4 = tempDstRow4;
                    dstCol1 = tempDstCol1; dstCol2 = tempDstCol2; dstCol3 = tempDstCol3; dstCol4 = tempDstCol4;
                    NumJump = CurNumJump;
                }
            }
            else{
                continue;
            }
        }
        char result = NumJump+'0';
        return result;
    }
    
    
    
    char MoveStrategy(SFGame &aGame){
        PieceColor compareColor= PieceColor::red;
        auto tiles = aGame.getBoard().getTiles();
        bool flag = 0;
        if(this->getColor()==compareColor)  //color of my piece is red
        {
            for(auto it=moveDst.begin();it!=moveDst.end();it++)
            {
                int aSrcRow = it->first.first;
                int aSrcCol = it->first.second;
                int aDstRow = it->second.first;
                int aDstCol = it->second.second;
                if(aDstRow ==7 && tiles[aSrcRow][aSrcCol].getPiece().isKing()==false)
                {  //if the piece is pawn and can be a king throught one move, do it.
                    srcRow = aSrcRow;
                    srcCol = aSrcCol;
                    dstRow1 = aDstRow;
                    dstCol1 = aDstCol;
                    return 'm';
                }
                if(aSrcRow!=0 && aSrcCol>1 && aSrcCol<7)
                {  //try not to move the piece located on Row 0, prevent my opponent's piece become king
                    flag=true;
                    srcRow = aSrcRow;
                    srcCol = aSrcCol;
                    dstRow1 = aDstRow;
                    dstCol1 = aDstCol;
                    break;
                }
            }
            if(flag==false)
            {
                auto target = moveDst.begin();  //otherwise, randomly choose a step
                srcRow = target->first.first;
                srcCol = target->first.second;
                dstRow1 = target->second.first;
                dstCol1 = target->second.second;
            }
            return 'm';
        }
        else
        {    //color of my piece is black
            for(auto it=moveDst.begin();it!=moveDst.end();it++)
            {
                int aSrcRow = it->first.first;
                int aSrcCol = it->first.second;
                int aDstRow = it->second.first;
                int aDstCol = it->second.second;
                if(aDstRow ==0 && tiles[aSrcRow][aSrcCol].getPiece().isKing()==false)
                {  //if the piece is pawn and can be a king throught one move, do it.
                    srcRow = aSrcRow;
                    srcCol = aSrcCol;
                    dstRow1 = aDstRow;
                    dstCol1 = aDstCol;
                    return 'm';
                }
                if(aSrcRow != 7 && aSrcCol>0 && aSrcCol<7 )
                {  //try not to move the piece located on Row 0, prevent my opponent's piece become king
                    flag=true;
                    srcRow = aSrcRow;
                    srcCol = aSrcCol;
                    dstRow1 = aDstRow;
                    dstCol1 = aDstCol;
                    break;
                }
            }
            if(flag==false)
            {
                auto target = moveDst.begin();  //otherwise, randomly choose a step
                srcRow = target->first.first;
                srcCol = target->first.second;
                dstRow1 = target->second.first;
                dstCol1 = target->second.second;
            }
            return 'm';
            
        }
    }
    
private:
    
    std::vector<std::pair<int,int> > myPieceLocation;
    std::vector<std::pair<int,int> > myOppoLocation;
    std::vector<std::pair<std::pair<int,int>,std::pair<int,int> > > moveDst;   //valid moving source and  destination
    std::vector<std::pair<std::pair<int,int>,std::pair<int,int> > > jumpDst;   //valid jumping source and destination
    int srcRow ;
    int srcCol ;
    int dstRow1 = 0;
    int dstRow2 = 0;
    int dstRow3 = 0;
    int dstCol1 = 0;
    int dstCol2 = 0;
    int dstCol3 = 0;
    int dstRow4 = 0;
    int dstCol4 = 0;
};

