#include <math.h>
#include <iostream>
#include "player.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int nbCell) {
  timeTouched = 0;
  count = 0;
  radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
  velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
Player::Player(double x0, double y0, int t, double c, int nbCell) {
  timeTouched = t;
  count = c;
  radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
  velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
void Player::updatePosition(double angle) {
  // xPosition += cos(angle) * velocity;
  // yPosition += sin(angle) * velocity;
}
bool Player::touchedAndDead() {
  timeTouched += 1;
  if (timeTouched == MAX_TOUCH) { // Dead when equal or sup ? //#askBoulic
    return (true);
  }
  return (false);
}
double Player::getX() const { return hitbox->getX(); }
double Player::getY() const { return hitbox->getY(); }
double Player::getRadius() const { return radius; }
double Player::getCount() const { return count; }
double Player::getTimeTouched() const { return timeTouched; }

Circle *Player::getHitbox() const { return hitbox; }