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

#include "LCDi2c.h"
#include "mbed.h"
#ifndef HEADER_H
#define HEADER_H
#include "Character.h"
#endif

class Enemy: public Character {
    public:
        Enemy();
        void Move();
        void Spawn();
        void Spawn(unsigned short);
        void Die();
        bool isDead();

};