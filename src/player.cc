#include <math.h>
#include <iostream>
#include "player.h"
#include "tools.h"
#include "define.h"

using namespace std;

class Player {
    public :
        Player(int x, int y,  int nbCell);
        void updatePosition(double angle);
        bool touchedAndDead();
        ~Player(){};
        int x(){return xPosition;};
        int y(){return yPosition;};
    private :
        double xPosition, yPosition;
        double radius;
        int nbt;
        double count;
        double velocity;
};

Player::Player(int x0, int y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    radius = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
}

void Player::updatePosition(double angle){
    xPosition += cos(angle) * velocity;
    yPosition += sin(angle) * velocity;
}

bool Player::touchedAndDead(){
    count += 1;
    if (count == MAX_COUNT) { //Dead when equal or sup ? //#askBoulic
        return(true);
    }
    return(false);
}
