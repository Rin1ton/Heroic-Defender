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
#include "Hero.h"
#include "Enemy.h"
#include "Missile.h"
SPI ser_port(PTD2, PTD3, PTD1); //mosi, miso, sclk
DigitalOut red_led(PTE30);
DigitalOut green_led(PTE29);
DigitalOut cs(PTD0);   //this acts as "Slave select"
DigitalIn switch_ip1(PTE22); // blue wire
DigitalIn switch_ip2(PTE21); // white wire
short switch_word;   //word we will send
short recd_val;      //value return from slave

uint8_t hero_0[8] = {
    0b00000,    //
    0b01100,    //   ████
    0b01100,    //   ████
    0b11000,    // ████
    0b11111,    // ██████████
    0b01100,    //   ████
    0b10010,    // ██    ██
    0b11011     // ████  ████
};

uint8_t hero_1[8] = {
    0b00000,    //
    0b01100,    //   ████
    0b01100,    //   ████
    0b11000,    // ████
    0b11111,    // ██████████
    0b01100,    //   ████
    0b01000,    //   ██
    0b01100     //   ████
};

//may change the rocket sprites to the middle of the character
uint8_t rocket_2[8] = {
    0b00000,    //
    0b00000,    //
    0b00000,    //
    0b01000,    //   ██
    0b00110,    //     ████
    0b01000,    //   ██
    0b00000,    //
    0b00000,    //
};

uint8_t rocket_3[8] = {
    0b00000,    //
    0b00000,    //
    0b00000,    //
    0b01000,    //   ██
    0b01110,    //   ██████
    0b01000,    //   ██
    0b00000,    //
    0b00000     //
};

uint8_t adversary_4[8] = {
    0b00000,    // 
    0b11111,    // ██████████
    0b10010,    // ██    ██
    0b00011,    //       ████
    0b11111,    // ██████████
    0b00110,    //     ████
    0b01001,    //   ██    ██
    0b01001     //   ██    ██
};

uint8_t adversary_5[8] = {
    0b11111,    // ██████████
    0b10010,    // ██    ██
    0b00010,    //       ██
    0b00011,    //       ████
    0b11111,    // ██████████
    0b00110,    //     ████
    0b00110,    //     ████
    0b00110     //     ████
};

uint8_t explosion_6[8] = {
    0b00000,    //
    0b00000,    //
    0b00100,    //     ██
    0b00101,    //     ██  ██
    0b01110,    //   ██████
    0b10100,    // ██  ██
    0b00100,    //     ██
    0b00000     //
};

uint8_t explosion_7[8] = {
    0b00000,    //
    0b00100,    //     ██
    0b10001,    // ██      ██
    0b00101,    //     ██  ██
    0b01010,    //   ██  ██
    0b10100,    // ██  ██
    0b10001,    // ██      ██
    0b00100     //     ██
};

//LCDi2c(PinName sda, PinName scl, lcd_t type = LCD16x2, int deviceAddress = 0x27);
static LCDi2c LCD(PTC9, PTC8, LCD20x4, 0x27);

//controls
AnalogIn joyVer(PTB1);          //orange wire
AnalogIn joyHor(PTB2);          //yellow wire
InterruptIn joyButton(PTD5);      //green wire
InterruptIn rightButton(PTA13);   //blue wire

//send a message to the slave, using the input
//param as the message
void SendMessage(char message) {
    cs = 0;         //select slave
    ser_port.write(message);    //send signal
    cs = 1;
}

//increment the score on the slave
void IncrementScore() {
    SendMessage(0x80);
}

//reset the score on the slave
void ResetScore() {
    SendMessage(0x40);
}

//play the fire tone
void FireBeep() {
    SendMessage(0x20);
}

//play the "game over" tone
void HeroDieBeep() {
    SendMessage(0x10);
}

//variables
bool isFiring = false;

void ResetBoard() {
    //LCD.cls();
    wait_us(250000);
    return;
}

void FireMissile() {
    isFiring = true;
}


// main() runs in its own thread in the OS
int main()
{

    //func
    joyButton.rise(&ResetBoard);
    rightButton.rise(&FireMissile);

    //declare all the sprites
    LCD.create(0, hero_0);
    LCD.create(1, hero_1);
    LCD.create(2, rocket_2);
    LCD.create(3, rocket_3);
    LCD.create(4, adversary_4);
    LCD.create(5, adversary_5);
    LCD.create(6, explosion_6);
    LCD.create(7, explosion_7);

    //declare missile array
    //vector<Missile> Missiles;

    while (true) {
        LCD.display(DISPLAY_ON);
        LCD.cls();
        //locate: (column, line) index at 0

        //place the cursor on the second line and print the name of the game
        LCD.locate(2, 1);
        LCD.printf("Heroic Defender!");
        wait_us(1000000);

        //move the name of the game left and right before the game starts
        for (int i = 0; i < 2; i++) {
            LCD.display(SCROLL_RIGHT);
            wait_us(75000);
        }
            for (int i = 0; i < 4; i++) {
            LCD.display(SCROLL_LEFT);
            wait_us(75000);
        }
            for (int i = 0; i < 2; i++) {
            LCD.display(SCROLL_RIGHT);
            wait_us(75000);
        }
        wait_us(750000);
        LCD.cls();
        LCD.locate(0, 0);

        //Get ready to play the actual game
        bool playing = true;
        while (playing) {

            //create our hero
            Hero theHero(3, 0);
            bool heroIsDead = false;

            //create our screen
            unsigned short 
            screen[theHero.GetScreenHeight()][theHero.GetScreenWidth()];

            //create our list of enemies
            const unsigned short NUMBER_OF_ENEMIES = 20;
            Enemy Enemies[NUMBER_OF_ENEMIES];
            const unsigned short FRAMES_BETWEEN_SPAWNING = 3;
            unsigned short framesUntilSpawnEnemy = 0;
            //if this is NUMBER_OF_ENEMIES, that means we don't have a slot to spawn an enemy from
            unsigned short enemyToSpawnNext = 0;

            //firing missiles
            const unsigned short NUMBER_OF_MISSILES = 10;
            Missile Missiles[NUMBER_OF_MISSILES];
            const unsigned short FRAMES_BETWEEN_FIRING = 2;
            unsigned short framesSinceLastFired = 4;
            unsigned short missileToFireNext = 0;

            //hero moving
            //deadzone values. move value closer to 33000 to reduce deadzone
            const unsigned short maxValToMoveUp = 10000;
            const unsigned short minValToMoveDown = 55000;
            const unsigned short minValToMoveRight = 55000;
            const unsigned short maxValToMoveLeft = 10000;
            short verMoveDir = 0;
            short horMoveDir = 0;

            //time to game
            //this is where the game runs

            //reset the score
            ResetScore();

            while(!theHero.isDead()) {
                
                //clear screen
                LCD.cls();

                //move the hero
                //read joystick value and save it
                if (joyVer.read_u16() < maxValToMoveUp)
                    verMoveDir = 1;
                else if (joyVer.read_u16() > minValToMoveDown)
                    verMoveDir = -1;

                if (joyHor.read_u16() > minValToMoveRight)
                    horMoveDir = 1;
                else if (joyHor.read_u16() < maxValToMoveLeft)
                    horMoveDir = -1;
                //pass move data to hero
                theHero.Move(verMoveDir, horMoveDir);

                //Check if hero is hit
                for (int enemy = 0; enemy < NUMBER_OF_ENEMIES; enemy++) {
                    if (Enemies[enemy].GetHorPos() == theHero.GetHorPos() &&
                    Enemies[enemy].GetVerPos() == theHero.GetVerPos()) {
                        heroIsDead = true;
                        theHero.Die();
                    }
                }

                //debug joystick
                /*LCD.locate(0, 0);
                LCD.printf("%d", joyVer.read_u16());
                LCD.locate(0, 1);
                LCD.printf("%d", joyHor.read_u16());//*/

                //move enemies
                for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
                    if (Enemies[i].GetHorPos() == 0) {
                        heroIsDead = true;
                        theHero.Die();
                    }
                    Enemies[i].Move();
                }

                //kill enemies
                for (int missile = 0; missile < NUMBER_OF_MISSILES; missile++) {
                    for (int enemy = 0; enemy < NUMBER_OF_ENEMIES; enemy++) {
                        if (Missiles[missile].GetHorPos() == Enemies[enemy].GetHorPos() &&
                        Missiles[missile].GetVerPos() == Enemies[enemy].GetVerPos() &&
                        !Missiles[missile].isDead() &&
                        !Enemies[enemy].isDead()) {
                            Enemies[enemy].Die();
                            Missiles[missile].Die();
                            
                            //increment score
                            IncrementScore();
                        }
                    }
                }

                //Check if hero is hit
                for (int enemy = 0; enemy < NUMBER_OF_ENEMIES; enemy++) {
                    if (Enemies[enemy].GetHorPos() == theHero.GetHorPos() &&
                    Enemies[enemy].GetVerPos() == theHero.GetVerPos()) {
                        heroIsDead = true;
                        theHero.Die();
                    }
                }

                //move missiles
                for (int i = 0; i < NUMBER_OF_MISSILES; i++)
                    Missiles[i].Move();

                //try to spawn enemy. we'll do this every 10 turns
                if (framesUntilSpawnEnemy == 0) {

                    //spawn the enemy and seed the rand function with player input
                    if (enemyToSpawnNext < NUMBER_OF_ENEMIES && theHero.GetHorPos() < 17)
                        rand() % 2 == 1 ? Enemies[enemyToSpawnNext].Spawn(joyHor.read_u16()) : Enemies[enemyToSpawnNext].Spawn();
                
                    //communicate we need a slot for an enemy to spawn next
                    enemyToSpawnNext = NUMBER_OF_ENEMIES;

                    //whether or not we successfully spawned an enemy, reset this counter
                    framesUntilSpawnEnemy = FRAMES_BETWEEN_SPAWNING;
                } else
                    framesUntilSpawnEnemy--;

                //check if next round we can spawn an enemy
                if (enemyToSpawnNext >= NUMBER_OF_ENEMIES) {
                    for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
                        if (Enemies[i].isDead()) enemyToSpawnNext = i;
                }

                //shoot missile
                if (isFiring && framesSinceLastFired >= FRAMES_BETWEEN_FIRING) {
                    isFiring = false;

                    //fire missile
                    if (missileToFireNext < NUMBER_OF_MISSILES && theHero.GetHorPos() < 19)
                        Missiles[missileToFireNext].Fire(theHero.GetVerPos(), theHero.GetHorPos());
                    
                    //communicate we've fired
                    framesSinceLastFired = 0;
                    missileToFireNext = NUMBER_OF_MISSILES;
                    FireBeep();
                } else 
                    framesSinceLastFired++;

                if (isFiring && framesSinceLastFired < FRAMES_BETWEEN_FIRING)
                    isFiring = false;

                //determine which missile will fire next
                if (missileToFireNext >= NUMBER_OF_MISSILES) {
                    for (int i = 0; i < NUMBER_OF_MISSILES ; i++)
                        if (Missiles[i].isDead()) missileToFireNext = i;
                }

                //kill enemies
                for (int missile = 0; missile < NUMBER_OF_MISSILES; missile++) {
                    for (int enemy = 0; enemy < NUMBER_OF_ENEMIES; enemy++) {
                        if (Missiles[missile].GetHorPos() == Enemies[enemy].GetHorPos() &&
                        Missiles[missile].GetVerPos() == Enemies[enemy].GetVerPos() &&
                        !Missiles[missile].isDead() &&
                        !Enemies[enemy].isDead()) {
                            Enemies[enemy].Die();
                            Missiles[missile].Die();

                            //increment score
                            IncrementScore();
                        }
                    }
                }

                /*
                * DRAW SCREEN
                */
                //draw the hero
                if (!heroIsDead) LCD.character(theHero.GetHorPos(), theHero.GetVerPos(), theHero.GetCharState());
                
                //draw the enemies
                for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
                    if (!Enemies[i].isDead())
                        LCD.character(Enemies[i].GetHorPos(), Enemies[i].GetVerPos(), Enemies[i].GetCharState());

                //draw the missiles
                for (int i = 0; i < NUMBER_OF_MISSILES; i++)
                    if (!Missiles[i].isDead())
                        LCD.character(Missiles[i].GetHorPos(), Missiles[i].GetVerPos(), Missiles[i].GetCharState());
				
				//reset the message sent to the slave
                ser_port.write(0x00);

                //lock the frame rate to 3 fps
                wait_us(333333);
            }
        
            //print game over
            LCD.cls();
            LCD.locate(5, 1);
            LCD.printf("GAME OVER!");
            HeroDieBeep();
            wait_us(2000000);

        }

        
    }
}

