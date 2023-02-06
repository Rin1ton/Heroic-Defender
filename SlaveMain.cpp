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

#include "mbed.h"
SPISlave ser_port(PTD2, PTD3, PTD1, PTD0); //mosi, miso, sclk, ssel
DigitalOut red_led(PTE30);
DigitalOut green_led(PTE29);
DigitalIn switch_ip1(PTE22); // blue wire
DigitalIn switch_ip2(PTE21); // white wire
DigitalOut buzzOut(PTC9);
short switch_word;   //word we will send
short recd_val;      //value return from master
// segments a,b,c,d,e,f,g,dp
// segments 7,6,4,2,1,9,10,5
// A is LSB of display bus. I.E., 0x01 lights up only A.
BusOut display1(PTA16, PTC17, PTB10, PTB9, PTB8, PTA17, PTE31, PTB11); //this one
BusOut display10(PTA2, PTA1, PTC0, PTC3, PTC4, PTD4, PTA12, PTC7); //this one

//use this function to display a two digit number to the 7-segment display
// input - number to be displayed
void DisplayNumber(int input) {
    switch (input % 10){
        case 0x0: display1 = 0x3F; break;      //display 0
        case 0x1: display1 = 0x06; break;      //display 1
        case 0x2: display1 = 0x5B; break;      //display 2
        case 0x3: display1 = 0x4F; break;      //display 3
        case 0x4: display1 = 0x66; break;      //display 4
        case 0x5: display1 = 0x6D; break;      //display 5
        case 0x6: display1 = 0x7D; break;      //display 6
        case 0x7: display1 = 0x07; break;      //display 7
        case 0x8: display1 = 0x7F; break;      //display 8
        case 0x9: display1 = 0x6F; break;      //display 9
        default: display1 = 0xF9; break;     //ERROR! display E.
      }                 //end of switch
    switch (input / 10){
        case 0x0: display10 = 0x3F; break;      //display 0
        case 0x1: display10 = 0x06; break;      //display 1
        case 0x2: display10 = 0x5B; break;      //display 2
        case 0x3: display10 = 0x4F; break;      //display 3
        case 0x4: display10 = 0x66; break;      //display 4
        case 0x5: display10 = 0x6D; break;      //display 5
        case 0x6: display10 = 0x7D; break;      //display 6
        case 0x7: display10 = 0x07; break;      //display 7
        case 0x8: display10 = 0x7F; break;      //display 8
        case 0x9: display10 = 0x6F; break;      //display 9
        default: display10 = 0xF9; break;     //ERROR! display E.
      }                 //end of switch
}

//Plays a tone with the "reverse pitch" as a parameter. the higher
//the parameter, the lower the pitch.
void Beep(int reversePitch) {
    const int LENGTH_TO_BEEP = 100000;
    bool fbit = 0;
    for (int i = 0; i < LENGTH_TO_BEEP / (reversePitch * 2); i++) {
        buzzOut = fbit;
        wait_us(reversePitch);
        fbit = !fbit;
        buzzOut = fbit;
        wait_us(reversePitch);
        fbit = !fbit;
    }
}

//the score of the player
unsigned short score = 0;

int main() {


    switch_ip1.mode(PullDefault);
    switch_ip2.mode(PullDefault);

    for (int i = 0; i < 100; i++) {
        DisplayNumber(i);
        wait_us(5000);
    }

    Beep(400);

    DisplayNumber(0);

    while(1) {
        
        //reset our recorded value
        recd_val = 0x00;

        //see if the master has sent a master
        if (ser_port.receive()) {
            recd_val = ser_port.read();
            ser_port.reply(switch_word);
        }

        //if we've recieved the signal to increment the score...
        if (recd_val == 0x80) {
            DisplayNumber(++score);
            Beep(200);
        }

        //if we've recieved the signal to reset the score...
        if (recd_val == 0x40) {
            score = 0;
            DisplayNumber(score);
            Beep(300);
            Beep(400);
            Beep(200);
        }

        //if we've recieved the signal we've fired...
        if (recd_val == 0x20) {
            Beep(300);
        }

        //if signal we've died
        if (recd_val == 0x10) {
            Beep(600);
            Beep(800);
        }
            

        //wait_us(333333);

        /*if (ser_port.receive()) {       //test if data transfer has occurred
            recd_val = ser_port.read();
            //ser_port.reply(switch_word);
            //DisplayDigit(recd_val);
            if (recd_val == 0x80)
                ;//DisplayDigit(++score);
        }   //end of if */
        //DisplayDigit(recd_val | switch_word);
    }     //end of while
}  //end of main