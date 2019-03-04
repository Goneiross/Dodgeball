#include <math.h>
#include <iostream>
#include "player.h"
#include "tools.h"
#include "define.h"

using namespace std;

double Player::x() const{return xPosition;};
double Player::y() const{return yPosition;};

Player::Player(double x0, double y0, int nbCell){
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
