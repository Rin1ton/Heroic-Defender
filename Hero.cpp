/*
Vinni Horstkotte
12-15-2022
Heroic Defender

Heroic defender is an embedded systems game using 2 KL25Z boards.
The object of the game is to defeat all the enemies as they come
and get a high score. Enemies move in a straight line to the left
of the screen. if an enemy steps on the player or makes it
to the end, the game is over. The player can shoot missiles at the
enemies to defeat them. Missiles move in a straight line to the right.
if a missile touches an enemy, the missile and the enemy dies, and the
player gets a point.
*/

/*
The hero object. the joystick drives player movement. if an enemy
steps on the player, the player dies and the game is over. the player
can shoot missiles at the enemies.
*/
#include "LCDi2c.h"
#include "mbed.h"
#include "Hero.h"

Hero::Hero() {
    verPos = 2;
    horPos = 0;
    charSprites[0] = 0;
    charSprites[1] = 1;
    currentFrame = 0;
}

//this constructor takes spawn coordinates as an input 
//to place the player in a specific place on screen
Hero::Hero(unsigned short inVerPos, unsigned short inHorPos) {
    verPos = inVerPos;
    horPos = inHorPos;
    charSprites[0] = 0;
    charSprites[1] = 1;
    currentFrame = 0;
}

//This function moves the player, taking horizontal and vertical movement
//as parameters
void Hero::Move(short& ver, short& hor) {
    if (!isDead()) {
        //if move up
        if (ver > 0 && verPos > 0)
            verPos--;
        //if move down
        if (ver < 0 && verPos < SCREEN_HEIGHT - 1)
            verPos++;
        //if move right
        if (hor > 0 && horPos < SCREEN_WIDTH - 1)
            horPos++;
        //if move left
        if (hor < 0 && horPos > 0)
            horPos--;
    }

    //reset controls
    ver = 0;
    hor = 0;
}

//this function kills the player. if the player is dead
//the game is over
void Hero::Die() {
    verPos = 4;
}

//this function returns true if the player is dead, false otherwise
bool Hero::isDead() {
    return verPos >= 4;
}