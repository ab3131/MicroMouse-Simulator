
#include "micromouseserver.h"
using namespace std;
#include <iostream>
bool amIFinished = false;



list<list<int>> neighbors(list<int> coords, int direction, bool iwf, bool iwl, bool iwr){
    list<list<int>> returnList = {};
    if(!iwf){
        if(direction==0){
            returnList.push_back({coords.front(), coords.back()+1});
        }
        else if(direction==1){
            returnList.push_back({coords.front()+1, coords.back()});
        }
        else if(direction==2){
            returnList.push_back({coords.front(), coords.back()-1});
        }
        else{
            returnList.push_back({coords.front()-1, coords.back()});
        }

    }
    if(!iwr){
        if(direction==3){
            returnList.push_back({coords.front(), coords.back()+1});
        }
        else if(direction==0){
            returnList.push_back({coords.front()+1, coords.back()});
        }
        else if(direction==1){
            returnList.push_back({coords.front(), coords.back()-1});
        }
        else{
            returnList.push_back({coords.front()-1, coords.back()});
        }

    }
    if(!iwl){
        if(direction==1){
            returnList.push_back({coords.front(), coords.back()+1});
        }
        else if(direction==2){
            returnList.push_back({coords.front()+1, coords.back()});
        }
        else if(direction==3){
            returnList.push_back({coords.front(), coords.back()-1});
        }
        else{
            returnList.push_back({coords.front()-1, coords.back()});
        }

    }
    return returnList;
}

void microMouseServer::studentAI()
{

    int count = 0;
    bool atFinish=false;
    bool iwf;
    bool iwl;
    bool iwr;
    list coordinates;
    list<list<int>> myNeighbors;
    while(count<400){
        /*
         atFinish = false;
        /*atFinish part
         if(isWallForward() || (isWallRight() && isWallLeft())){
             atFinish=false;
         }
         else{
             moveForward();
             if(isWallRight() && !isWallLeft() && isWallForward()){
                 turnLeft();
                 moveForward();
                 if(!isWallLeft()){
                     atFinish=true;
                 }
             }
             else if(!isWallRight() && isWallLeft() && isWallForward()){
                 turnRight();
                 moveForward();
                 if(!isWallRight()){
                     atFinish=true;
                 }
             }
             else{
                 atFinish=false;
                 turnRight();
                 turnRight();
                 moveForward();
                 turnRight();
                 turnRight();
             }
         }
        /*atFinish part*/

        /*
        if(!atFinish){
            if(isWallRight()){
                if(isWallForward()){
                    if(isWallLeft()){
                        /*Reached deadend, turn around
                        turnRight();
                        turnRight();
                        moveForward();
                    }
                    else{
                        /*Wall on right and forward but not left, so you can only move left
                        turnLeft();
                        moveForward();
                    }
                }
                else{
                    /*You will keep the right side on the right wall still
                    moveForward();
                }
            }
            else{
                /*This means we have lost our wall on the right, so we have to get it back. Going to the right does this.
                turnRight();
                moveForward();
            }
        }
        */
        foundFinish();
        bool iwf = isWallForward();
        bool iwl = isWallLeft();
        bool iwr = isWallRight();
        list coordinates = {0,0};
        list<list<int>> myNeighbors = neighbors(coordinates, 0, iwf, iwl, iwr);
        if(neighbors.front().front()>coordinates.front()){

        }
        count++;

    }


}


/*
 * The following are the eight functions that you can call. Feel free to create your own fuctions as well.
 * Remember that any solution that calls moveForward more than once per call of studentAI() will have points deducted.
 *
 *The following functions return if there is a wall in their respective directions
 *bool isWallLeft();
 *bool isWallRight();
 *bool isWallForward();
 *
 *The following functions move the mouse. Move forward returns if the mouse was able to move forward and can be used for error checking
 *bool moveForward();
 *void turnLeft();
 *void turnRight();
 *
 * The following functions are called when you need to output something to the UI or when you have finished the maze
 * void foundFinish();
 * void printUI(const char *mesg);
*/



/* Checks if you are in the 2x2 square
bool microMouseServer::atFinish(iwl, iwr, iwf, mf, tl, tr){
    /*In the End Square, when you enter, you can always move forward. The other option is to move right or left, but never both.
    if(iwf){
        if(!iwr && iwl){
            mf();
            /*If there is no wall on the right when you enter, then if you move forward, there should be no wall on the right in a 2x2 square. Amd same with left
            if(!iwr){
                return true;
            }
            else{
                /*Revert back to original position
                turnRight();
                turnRight();
                moveForward();
                turnRight();
                turnRight();
            }
        }
        if(isWallRight() && !isWallLeft()){
            moveForward();
            if(!isWallLeft()){
                return true;
            }
            else{
                /*Revert back to original position
                turnRight();
                turnRight();
                moveForward();
                turnRight();
                turnRight();
            }
        }
    }
    return false;
}
*/
