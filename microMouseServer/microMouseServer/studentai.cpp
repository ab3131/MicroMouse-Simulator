
#include "micromouseserver.h"
using namespace std;
#include <iostream>
#include <string>
#include <qDebug>
bool amIFinished = false ;



list<list<int>> neighbors(list<int> coords, int direction, bool iwf, bool iwl, bool iwr){
    list<list<int>> returnList = {};
    //depending on the different directions, the booleans will act differently(for example, if mouse is facing up and there's a wall forward, that wall will be a wall left if the mouse is facing right)
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
    //coordinates stores the current coordinates in Cartesian coordinate form, previouscoords stores previous coordinates, and extracoords stores local coordinate variables
    //I realized I could use a vector too much time into this project, otherwise I would have combined direction and coordinates into one
    list coordinates = {0,0};
    list previouscoords = {0,0};
    list extracoords = {0,0};
    list<list<int>> myNeighbors;
    int direction=0;
    //"visited" and "visitedtwice" mark which squares have been visited, with priority going to non-visited squares(foundtwice and found store whether some coordinates are in these lists)
    list<list<int>> visited;
    list<list<int>> visitedtwice;
    bool found;
    bool foundtwice;
    bool atJunction = false;
    int marked = 0;
    int min=3;

    list<int> tomove = {0,0};
    list<list<int>> moves = {};
    //notmin ensures that the minimum number wasn't actually 0 so I do not enter an if statement that is later on in the code
    bool notmin = false;
    bool debugFound = false;

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
            //moves.clear() is required since the list "moves" stores every value after the most recent junction
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
                //tomove helps us convert coordinates and direction into a more readable form(otherwise there would be quite a few if and else statements)
                tomove = todo(direction, previouscoords, coordinates);
                direction = tomove.back();
                if(tomove.front()==0){
                    moveForward();
                }
                else if(tomove.front()==1){
                    turnRight();
                    moveForward();

                }
                else if(tomove.front()==2){
                    turnRight();
                    turnRight();
                    moveForward();
                }
                else{
                    turnLeft();
                    moveForward();

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
                        //this if statement signifies an entrance without a mark; the program should break because it has already found something with 0 marks, the minimum
                        if(!found && !foundtwice){
                            previouscoords = coordinates;
                            tomove = todo(direction,coordinates, coords);
                            coordinates = coords;
                            direction=tomove.back();
                            if(tomove.front()==0){
                                moveForward();
                            }
                            else if(tomove.front()==1){
                                turnRight();
                                moveForward();
                            }
                            else if(tomove.front()==2){
                                turnRight();
                                turnRight();
                                moveForward();
                            }
                            else{
                                turnLeft();
                                moveForward();
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
                        }
                        else if(tomove.front()==1){
                            turnRight();
                            moveForward();
                        }
                        else if(tomove.front()==2){
                            turnRight();
                            turnRight();
                            moveForward();
                        }
                        else{
                            turnLeft();
                            moveForward();
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
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);

            }
            else if(!isWallRight()){
                turnRight();
                moveForward();
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);
                direction++;

            }
            else if(!isWallLeft()){
                turnLeft();
                moveForward();
                coordinates = myNeighbors.front();
                moves.push_back(coordinates);
                direction--;
                if(direction<0){
                    direction=3;
                }

            }


            else{
                //backtrack until last junction
                while(moves.size()>0){
                    previouscoords = coordinates;
                    coordinates = moves.back();
                    tomove = todo(direction, previouscoords, coordinates);
                    if(tomove.front()!=4){

                        direction = tomove.back();
                        if(tomove.front()==0){
                            moveForward();
                        }
                        else if(tomove.front()==1){
                            turnRight();
                            moveForward();
                        }
                        else if(tomove.front()==2){
                            turnRight();
                            turnRight();
                            moveForward();
                        }
                        else{
                            turnLeft();
                            moveForward();
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
            //keep checking until you have checked all walls, or something does not line up with the trend of a finish box
            //when you enter, if there is a wall left, there should be consistently a wall left in the two by two square(it'll go all around and check); it should be a similar situation with the right wall
            if(!isWallLeft() && isWallRight() && !isWallForward()){
                moveForward();
                if(isWallRight() && !isWallLeft() && isWallForward()){
                    turnLeft();
                    moveForward();
                    if(isWallForward() && isWallRight() && !isWallLeft()){
                        turnLeft();
                        moveForward();
                        if(!isWallLeft() && isWallRight() && isWallForward()){
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
                else{
                    turnRight();
                    turnRight();
                    moveForward();
                    turnRight();
                    turnRight();
                }
            }
            else if(!isWallRight() && isWallLeft() && !isWallForward()){
                moveForward();
                if(!isWallRight() && isWallLeft() && isWallForward()){
                    turnRight();
                    moveForward();
                    if(isWallForward() && !isWallRight() && isWallLeft()){
                        turnRight();
                        moveForward();
                        if(isWallLeft() && !isWallRight() && isWallForward()){
                            atFinish=true;
                        }
                        else{
                            turnRight();
                            turnRight();
                            moveForward();
                            turnRight();
                            moveForward();
                            turnRight();
                            moveForward();
                            turnRight();
                            turnRight();
                        }
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
                    turnRight();
                    turnRight();
                    moveForward();
                    turnRight();
                    turnRight();
                }
            }
        }
        count++;
        if(direction>3){
            direction = direction%4;
        }
        /*atFinish part*/
        qDebug()<<"coordinates: "<<to_string(coordinates.front())+" " + to_string(coordinates.back());
        printUI((to_string(coordinates.front())+" " +to_string(coordinates.back())).data());







    }
    foundFinish();












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
