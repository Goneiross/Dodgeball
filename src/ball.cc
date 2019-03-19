/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module implementation
*/

#include <math.h>
#include "define.h"
#include "ball.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Ball::Ball(double x0, double y0, double a, int nbCell) {
  angle = a;
  radius = COEF_RAYON_BALLE * (SIDE / nbCell);
  velocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
Ball::Ball(double x0, double y0, double a, double r, double v) {
  angle = a;
  radius = r;
  velocity = v;
  hitbox = new Circle(x0, y0, radius);
}
void Ball::updatePosition() {
  hitbox->setX(hitbox->getX() + cos(angle) * velocity);
  hitbox->setY(hitbox->getY() + sin(angle) * velocity);
}
double Ball::getX() const { return hitbox->getX(); }
double Ball::getY() const { return hitbox->getY(); }
double Ball::getRadius() const { return radius; }
Circle *Ball::getHitbox() const { return hitbox; }