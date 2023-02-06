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
This is the character class that all characters inherit from. 
the hero, missiles and enemies are characters. characters 
interact with the world and each other in different ways.
*/
#include "LCDi2c.h"
#include "mbed.h"
#include "Character.h"

//this returns the height of the screen
unsigned short Character::GetScreenHeight() {
    return SCREEN_HEIGHT;
}

//this returns the width of the screen
unsigned short Character::GetScreenWidth() {
    return SCREEN_WIDTH;
}

//this returns the blank space character
unsigned short Character::GetBlankSpace() {
    return BLANK_SPACE;
}

//this returns the vertical position of the inheritted character
unsigned short Character::GetVerPos() {
    return verPos;
}

//this returns the horizontal position of the inheritted character
unsigned short Character::GetHorPos() {
    return horPos;
}

//returns a 2D array representing the characters
//of the LCD. This way, each of the classes can be
//represented as layers that can be stacked on top
//of one another. 256 represents an empty space
//^^FALSE^^

/* 
This function just returns the index of the frame the character is on.
*/
unsigned short Character::GetCharState() {

    unsigned short thisFrame = charSprites[currentFrame];
    currentFrame = currentFrame == 0 ? 1 : 0;
    return thisFrame;

    /*
    //create the array we will be returning
    unsigned short** screen = 0;
    screen = new unsigned short*[SCREEN_HEIGHT];

    for (unsigned short h = 0; h < SCREEN_HEIGHT; h++) {
        screen[h] = new unsigned short[SCREEN_WIDTH];

        for (unsigned short w = 0; w < SCREEN_WIDTH; w++) {
            if (h == verPos && w == horPos) {
                screen[h][w] = charSprites[currentFrame];
                currentFrame = currentFrame == 0 ? 1 : 0;
            }
            else
                screen[h][w] = BLANK_SPACE;
        }
    }

    return screen;*/
}