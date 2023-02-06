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
missile moves one space forward every frame. if it
touches an enemy, the missile and the enemy both die
and the player gets a point.
*/
#include "LCDi2c.h"
#include "mbed.h"
#include "stdlib.h"
#include "Missile.h"

//missile constructor, if horPos == 4, the missile isn't on screen
Missile::Missile() {
    verPos = 4;
    horPos = 19;
    charSprites[0] = 2;
    charSprites[1] = 3;
    currentFrame = 0;
}

//move the missile one space to the right
void Missile::Move() {
    if (horPos == 19)
        Die();
    else if (!isDead())
        horPos++;
}

//this function kills the missile
void Missile::Die() {
    verPos = 4;
    //TBI: spawn explosion where we died
}

//returns true if the missile is dead
bool Missile::isDead() {
    return verPos == 4;
}

//spawns the missile in front of the player so it cn march toward the enemies
void Missile::Fire(unsigned short heroVer, unsigned short heroHor) {
    verPos = heroVer;
    horPos = heroHor + 1;
}