#include <math.h>
#include <iostream>
#include "player.h"
#include "tools.h"
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    timeTouched = 0;
    count = 0;
    radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
}
Player::Player(double x0, double y0, int t, double c, int nbCell){
    xPosition = x0;
    yPosition = y0;
    timeTouched = t;
    count = c;
    radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
}
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
double Player::x() const{return xPosition;};
double Player::y() const{return yPosition;};
double Player::getRadius() const{return radius;}
double Player::getCount() const{return count;}
double Player::getTimeTouched() const{return timeTouched;}