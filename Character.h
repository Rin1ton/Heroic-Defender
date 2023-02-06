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

#ifndef _character_h
#define _character_h
class Character {
    public:
        unsigned short GetScreenHeight();
        unsigned short GetScreenWidth();
        unsigned short GetBlankSpace();
        unsigned short GetVerPos();
        unsigned short GetHorPos();
        unsigned short GetCharState();
    protected:
        unsigned short verPos;
        unsigned short horPos;
        unsigned short charSprites[2];
        unsigned short currentFrame;
        const unsigned short SCREEN_HEIGHT = 4;
        const unsigned short SCREEN_WIDTH = 20;
        const unsigned short BLANK_SPACE = 256;
};
#endif //_character_h