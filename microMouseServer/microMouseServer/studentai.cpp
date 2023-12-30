
#include "micromouseserver.h"

void microMouseServer::studentAI()
{
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

}

/* Checks if you are in the 2x2 square*/
bool atFinish(){
    /*In the End Square, when you enter, you can always move forward. The other option is to move right or left, but never both.*/
    if(!isWallForward()){
        if(!isWallRight() && isWallLeft()){
            moveForward();
            /*If there is no wall on the right when you enter, then if you move forward, there should be no wall on the right in a 2x2 square. Amd same with left*/
            if(!isWallRight()){
                return true;
            }
            else{
                /*Revert back to original position*/
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
                /*Revert back to original position*/
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

while(!atFinish()){

}

