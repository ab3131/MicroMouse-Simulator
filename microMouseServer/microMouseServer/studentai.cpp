
#include "micromouseserver.h"
using namespace std;
#include <iostream>
#include <string>
#include <qDebug>
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


//returns direction to turn, new direction
list<int> todo(int dir, list<int> origcoord, list<int> newcoord){
    //facing up
    if(dir==0){
        //right
        if(origcoord.front()<newcoord.front()){
            return {1, 1};
        }
        //left
        else if(origcoord.front()>newcoord.front()){
            return {3,3};
        }
        //down
        else if(origcoord.back()>newcoord.back()){
            return {2,2};
        }
        //up
        else if(origcoord.back()<newcoord.back()){
            return {0,0};
        }
        else{
            return {4,4};
        }
    }
    //facing right
    else if(dir==1){
        //right
        if(origcoord.front()<newcoord.front()){
            return {0,1};
        }
        //left
        else if(origcoord.front()>newcoord.front()){
            return {2,3};
        }
        //down
        else if(origcoord.back()>newcoord.back()){
            return {1,2};
        }
        //up
        else if(origcoord.back()<newcoord.back()){
            return {3,0};
        }
        else{
            return {4,4};
        }
    }
    //facing down
    if(dir==2){
        //right
        if(origcoord.front()<newcoord.front()){
            return {3,1};
        }
        //left
        else if(origcoord.front()>newcoord.front()){
            return {1,3};
        }
        //down
        else if(origcoord.back()>newcoord.back()){
            return {0,2};
        }
        //up
        else if(origcoord.back()<newcoord.back()){
            return {2,0};
        }
        else{
            return {4,4};
        }
    }
    //facing left
    if(dir==3){
        //right
        if(origcoord.front()<newcoord.front()){
            return {2,1};
        }
        //left
        else if(origcoord.front()>newcoord.front()){
            return {0,3};
        }
        //down
        else if(origcoord.back()>newcoord.back()){
            return {3,2};
        }
        //up
        else if(origcoord.back()<newcoord.back()){
            return {1,0};
        }
        else{
            return {4,4};
        }
    }

}


void microMouseServer::studentAI()
{



    int count = 0;
    bool atFinish=false;
    list coordinates = {0,0};
    list previouscoords = {0,0};
    list extracoords = {0,0};
    list<list<int>> myNeighbors;
    int direction=0;
    //"visited" and "visitedtwice" mark which squares have been visited, with priority going to non-visited squares
    list<list<int>> visited;
    list<list<int>> visitedtwice;
    bool found;
    bool foundtwice;
    bool atJunction = false;
    int marked = 0;
    int min=3;
    list<int> tomove = {0,0};
    list<list<int>> moves = {};
    bool notmin = false;


    while(!atFinish){
        //atJunction will be true if the previous coordinates were a junction...so we also have to mark current coordinates because they are an exit to the junction
        if(atJunction){
            found = ::find(visited.begin(), visited.end(), coordinates) != visited.end();
            if(found){
                visited.remove(coordinates);
                visitedtwice.push_back(coordinates);
            }
            else{
                visited.push_back(coordinates);
            }
        }
        atJunction=false;
        myNeighbors = neighbors(coordinates, direction, isWallForward(), isWallLeft(), isWallRight());

        //myNeighbors size will be greater than one only if the point is a junction
        if(myNeighbors.size()>1){
            moves.clear();
            moves.push_back(coordinates);
            atJunction=true;
            //if junction not at first move
            if((coordinates.front()!=0 || coordinates.back()!=0) && (previouscoords.front()!=0 || previouscoords.back()!=0)){
                //add entrance to respective list
                found = ::find(visited.begin(), visited.end(), previouscoords) != visited.end();
                if(found){
                    visited.remove(previouscoords);
                    visitedtwice.push_back(previouscoords);
                }
                else{
                    visited.push_back(previouscoords);
                }


            }

            //check #1: only entrance you came from is marked, pick arbitrary entrance
            marked=0;
            for(list coords: myNeighbors){
                found = ::find(visited.begin(), visited.end(), coords) != visited.end();
                foundtwice = ::find(visitedtwice.begin(), visitedtwice.end(), coords) != visitedtwice.end();
                if(found || foundtwice){
                    marked++;
                }
            }
            if(marked==0){
                previouscoords= coordinates;
                coordinates = myNeighbors.front();
                tomove = todo(direction, previouscoords, coordinates);
                direction = tomove.back();
                if(tomove.front()==0){
                    moveForward();
                    qDebug()<<"moved forward";
                }
                else if(tomove.front()==1){
                    turnRight();
                    moveForward();
                    qDebug()<<"turned right and moved forward";

                }
                else if(tomove.front()==2){
                    turnRight();
                    turnRight();
                    moveForward();
                    qDebug()<<"turned around and moved forward";
                }
                else{
                    turnLeft();
                    moveForward();
                    qDebug()<<"turned left and moved forward";

                }

            }
            else{
                //check #2: pick entrance you just came from, unless marked twice
                foundtwice = ::find(visitedtwice.begin(), visitedtwice.end(), previouscoords) != visitedtwice.end();

                if(!foundtwice){
                    extracoords = coordinates;
                    coordinates = previouscoords;
                    previouscoords = extracoords;
                    turnRight();
                    turnRight();
                    moveForward();
                    qDebug()<<"turned around and moved forward";
                    direction = direction-2;
                    if(direction<0){
                        direction = direction+4;
                    }
                }
                else{
                    //check #3: pick entrance with the fewest marks
                    min=3;
                    notmin=false;
                    for(list coords: myNeighbors){
                        found = ::find(visited.begin(), visited.end(), coords) != visited.end();
                        foundtwice = ::find(visitedtwice.begin(), visitedtwice.end(), coords) != visitedtwice.end();
                        if(!found && !foundtwice){
                            previouscoords = coordinates;
                            tomove = todo(direction,coordinates, coords);
                            coordinates = coords;
                            direction=tomove.back();
                            if(tomove.front()==0){
                                moveForward();
                                qDebug()<<"moved forward";
                            }
                            else if(tomove.front()==1){
                                turnRight();
                                moveForward();
                                qDebug()<<"turned right and moved forward";
                            }
                            else if(tomove.front()==2){
                                turnRight();
                                turnRight();
                                moveForward();
                                qDebug()<<"turned around and moved forward";
                            }
                            else{
                                turnLeft();
                                moveForward();
                                qDebug()<<"turned left and moved forward";
                            }
                            notmin=true;
                            break;
                        }
                        if(found && !foundtwice){
                            min=1;
                            extracoords = coords;
                            //wait until end to decide to use this
                        }

                    }
                    if(min==1 && !notmin){
                        previouscoords = coordinates;
                        coordinates = extracoords;
                        tomove = todo(direction,previouscoords, coordinates);
                        direction=tomove.back();
                        if(tomove.front()==0){
                            moveForward();
                            qDebug()<<"moved forward";
                        }
                        else if(tomove.front()==1){
                            turnRight();
                            moveForward();
                            qDebug()<<"turned right and moved forward";
                        }
                        else if(tomove.front()==2){
                            turnRight();
                            turnRight();
                            moveForward();
                            qDebug()<<"turned around and moved forward";
                        }
                        else{
                            turnLeft();
                            moveForward();
                            qDebug()<<"turned left and moved forward";
                        }
                    }

                }
            }
            moves.push_back(coordinates);
        }


        else{
            //first three conditions are if neighbors size is 1, otherwise it is 0 and we go to else condition and turn around(dead end)
            previouscoords=coordinates;
            atJunction=false;


            if(!isWallForward()){

                moveForward();
                qDebug()<<"moved forward";
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);

            }
            else if(!isWallRight()){
                turnRight();
                moveForward();
                qDebug()<<"turned right and moved forward";
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);
                direction++;

            }
            else if(!isWallLeft()){
                turnLeft();
                moveForward();
                qDebug()<<"turned left and moved forward";
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);
                direction--;
                if(direction<0){
                    direction=3;
                }

            }


            else{
                while(moves.size()>0){
                    previouscoords = coordinates;
                    coordinates = moves.back();
                    tomove = todo(direction, previouscoords, coordinates);
                    if(tomove.front()!=4){

                        direction = tomove.back();
                        if(tomove.front()==0){
                            moveForward();
                            qDebug()<<"moved forward";
                        }
                        else if(tomove.front()==1){
                            turnRight();
                            moveForward();
                            qDebug()<<"turned right and moved forward";
                        }
                        else if(tomove.front()==2){
                            turnRight();
                            turnRight();
                            moveForward();
                            qDebug()<<"turned around and moved forward";
                        }
                        else{
                            turnLeft();
                            moveForward();
                            qDebug()<<"turned left and moved forward";
                        }
                    }

                    moves.pop_back();
                    found = ::find(visited.begin(), visited.end(), coordinates) != visited.end();
                    if(found){
                        visited.remove(coordinates);
                        visitedtwice.push_back(coordinates);
                    }


                }
            }

        }
        atFinish = false;
        /*atFinish part*/
        if(isWallForward() || (isWallRight() && isWallLeft())){
            atFinish=false;
        }
        else{
            moveForward();

            if(isWallRight() && !isWallLeft() && isWallForward()){
                turnLeft();
                moveForward();

                if(!isWallLeft()){
                    turnRight();
                    moveForward();
                    if(!isWallRight() && isWallForward()){
                        atFinish=true;
                    }
                    else{
                        turnRight();
                        turnRight();
                        moveForward();
                        turnLeft();
                        moveForward();
                        turnLeft();
                        moveForward();
                        turnRight();
                        turnRight();
                    }

                }
                else{
                    turnRight();
                    turnRight();
                    moveForward();
                    turnRight();
                    moveForward();
                    turnRight();
                    turnRight();

                }

            }
            else if(!isWallRight() && isWallLeft() && isWallForward()){
                turnRight();
                moveForward();
                if(!isWallRight()){
                    atFinish=true;

                }
                else{
                    turnRight();
                    turnRight();
                    moveForward();
                    turnLeft();
                    moveForward();
                    turnRight();
                    turnRight();
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
        count++;
        if(direction>3){
            direction = direction%4;
        }
        /*atFinish part*/
        qDebug()<<"test"<<to_string(coordinates.front())+to_string(coordinates.back());
        printUI((to_string(coordinates.front())+to_string(coordinates.back())).data());
        printUI(to_string(direction).data());







    }
    foundFinish();











    //this is code for a testing phase where i tried to code a combination of Tremaux's algorithm and using neighbors
    /*
    while(count<4){
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

        iwf = isWallForward();
        iwl = isWallLeft();
        iwr = isWallRight();
        found = ::find(visited.begin(), visited.end(), coordinates) != visited.end();
        foundtwice = ::find(visitedtwice.begin(), visitedtwice.end(), coordinates) != visitedtwice.end();

        myNeighbors = neighbors(coordinates, direction, iwf, iwl, iwr);
        //current condition for exit, has to be changed later on
        if(myNeighbors.size()==0){
            foundFinish();
            atFinish=true;
        }
        else{
            while(myNeighbors.size()>1){
                for(list<int> neigh: myNeighbors){
                    foundtwice = ::find(visitedtwice.begin(), visitedtwice.end(), neigh) != visitedtwice.end();
                    if(foundtwice && myNeighbors.size()>1){
                        myNeighbors.remove(neigh);
                    }
                }
                break;
            }

            while(myNeighbors.size()>1){
                for(list<int> neigh: myNeighbors){
                    found = ::find(visited.begin(), visited.end(), neigh) != visited.end();
                    if(found && myNeighbors.size()>1){
                        myNeighbors.remove(neigh);
                    }
                }
                break;
            }

            if(myNeighbors.front().back()>coordinates.back()){
                if(direction==0){
                    moveForward();
                }
                else if(direction==1){
                    turnLeft();
                    moveForward();
                }
                else if(direction==2){
                    turnRight();
                    turnRight();
                    moveForward();
                }
                else{
                    turnRight();
                    moveForward();
                }
                direction=0;
                coordinates = {myNeighbors.front().front(),myNeighbors.front().back()};
            }
            else if(myNeighbors.front().back()<coordinates.back()){

                if(direction==2){
                    moveForward();
                }
                else if(direction==3){
                    turnLeft();
                    moveForward();
                }
                else if(direction==0){
                    turnRight();
                    turnRight();
                    moveForward();
                }
                else{
                    turnRight();
                    moveForward();
                }
                direction=2;
                coordinates = {myNeighbors.front().front(),myNeighbors.front().back()};
            }
            else{
                if(myNeighbors.front().front()>coordinates.front()){
                    if(direction==1){
                        moveForward();
                    }
                    else if(direction==2){
                        turnLeft();
                        moveForward();
                    }
                    else if(direction==3){
                        turnRight();
                        turnRight();
                        moveForward();
                    }
                    else{
                        turnRight();
                        moveForward();
                    }
                    direction=1;
                    coordinates = {myNeighbors.front().front(),myNeighbors.front().back()};
                }
                else if(myNeighbors.front().front()<coordinates.front()){
                    if(direction==3){
                        moveForward();
                    }
                    else if(direction==0){
                        turnLeft();
                        moveForward();
                    }
                    else if(direction==1){
                        turnRight();
                        turnRight();
                        moveForward();
                    }
                    else{
                        turnRight();
                        moveForward();
                    }
                    direction=3;
                    coordinates = {myNeighbors.front().front(),myNeighbors.front().back()};
                }

            }
            found = ::find(visited.begin(), visited.end(), coordinates) != visited.end();
            if(found){
                visited.remove(coordinates);
                visitedtwice.push_back(coordinates);
            }
            else{
                visited.push_back(coordinates);
            }
        }

        printUI(to_string(coordinates.front()).data());
        printUI(to_string(coordinates.back()).data());


        printUI(to_string(found).data());
        count++;

    }
    */
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
