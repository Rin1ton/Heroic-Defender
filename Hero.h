#ifndef _hero_h
#define _hero_h
#include "Character.h"

class Hero: public Character {
    public:
        Hero();
        Hero(unsigned short, unsigned short);
        void Move(short&, short&);
        void Die();
        bool isDead();
};
#endif // _hero_h