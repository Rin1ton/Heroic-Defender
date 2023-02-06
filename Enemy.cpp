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
This is the enemy object. The enemies walk forward until they reach the end
of the screen, at which point the game ends. if the player steps where the 
enemy is, the game is over. if a missile hits an enemy, the enemy dies.
*/
#include "LCDi2c.h"
#include "mbed.h"
#include "stdlib.h"
#include "Enemy.h"

//enemy constructor. if verPos == 4, the enemy isn't on screen
Enemy::Enemy() {
    verPos = 4;
    horPos = 19;
    charSprites[0] = 4;
    charSprites[1] = 5;
    currentFrame = 0;
}

//this function moves the enemy one column to the left
void Enemy::Move() {
    if (horPos == 0)
        Die();
    else if (!isDead())
        horPos--;
}

//This function calls rand to decide where to spawn an enemy in the last column
void Enemy::Spawn() {
    horPos = 19;
    verPos = rand() % 4;
    currentFrame = rand() % 2;
}

//this function also spawns the enemy, but it will seed the rand function as well
void Enemy::Spawn(unsigned short seed) {
    srand(seed);
    horPos = 19;
    verPos = rand() % 4;
}

//this function kills the enemy
void Enemy::Die() {
    verPos = 4;
    //TBI: spawn explosion where we died
}

//this function returns true if the enemy is dead, false if it is alive
bool Enemy::isDead() {
    return verPos == 4;
}