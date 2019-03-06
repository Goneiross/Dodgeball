#include <math.h>
#include "define.h"
#include "ball.h"
#include "tools.h"

using namespace std;

Ball::Ball(double x0, double y0, double a, int nbCell){
    xPosition = x0;
    yPosition = y0;
    angle = a;
    radius = COEF_RAYON_BALLE* (SIDE / nbCell);
    velocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
}
void Ball::updatePosition(){
    xPosition += cos(angle) * velocity;
    yPosition += sin(angle) * velocity;
}
double Ball::getX () const {return xPosition;}
double Ball::getY () const {return yPosition;}
double Ball::getRadius () const {return radius;}