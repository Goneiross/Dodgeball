#include <math.h>
#include <iostream>
#include "player.h"
#include "tools.h"
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    radius = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
}

double Player::x() const{return xPosition;};
double Player::y() const{return yPosition;};
double Player::getRadius() const{return radius;}
double Player::getCount() const{return count;}
double Player::getTimeTouched() const{return timeTouched;}

void Player::updatePosition(double angle){
    xPosition += cos(angle) * velocity;
    yPosition += sin(angle) * velocity;
}
bool Player::touchedAndDead(){
    timeTouched += 1;
    if (timeTouched == MAX_TOUCH) { //Dead when equal or sup ? //#askBoulic
        return(true);
    }
    return(false);
}
