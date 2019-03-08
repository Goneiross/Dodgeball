#include <math.h>
#include <iostream>
#include "player.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    timeTouched = 0;
    count = 0;
    radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
    Circle* hitbox = new Circle(xPosition, yPosition, radius);
}
Player::Player(double x0, double y0, int t, double c, int nbCell){
    xPosition = x0;
    yPosition = y0;
    timeTouched = t;
    count = c;
    radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
    velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
    Circle* hitbox = new Circle(xPosition, yPosition, radius);
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
double Player::getX() const{return xPosition;};
double Player::getY() const{return yPosition;};
double Player::getRadius() const{return radius;}
double Player::getCount() const{return count;}
double Player::getTimeTouched() const{return timeTouched;}