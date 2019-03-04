#include <math.h>
#include "define.h"
#include "ball.h"
#include "tools.h"

using namespace std;

Ball::Ball(int x0, int y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    radius = COEF_RAYON_BALLE* (SIDE / nbCell);
    velocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
}
void Ball::updatePosition(double angle){
    xPosition += cos(angle) * velocity;
    yPosition += sin(angle) * velocity;
}
double Ball::x () const {return xPosition;}
double Ball::y () const {return yPosition;}
double Ball::getRadius () const {return radius;}