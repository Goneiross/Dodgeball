/*!
  \file   player.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module implementation
*/

#include <math.h>
#include <vector>
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
  double radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
  velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
Player::Player(double x0, double y0, int t, double c, int nbCell) {
  timeTouched = t;
  count = c;
  double radius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
  velocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
Player::Player(double x0, double y0, int t, double c, double r, double v) {
  timeTouched = t;
  count = c;
  velocity = v;
  hitbox = new Circle(x0, y0, r);
}
void Player::updatePosition(double angle) {
  hitbox->setX(hitbox->getX() + cos(angle) * velocity);
  hitbox->setY(hitbox->getY() + sin(angle) * velocity);
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
double Player::getRadius() const { return hitbox->getRadius(); }
double Player::getCount() const { return count; }
double Player::getTimeTouched() const { return timeTouched; }

Circle *Player::getHitbox() const { return hitbox; }

PlayerMap::PlayerMap(int l, int c){
  lineNumber = l;
  columnNumber = c;

  playerGrid = new vector<int>* [lineNumber];
  for (int i = 0; i < lineNumber; i++) {
    playerGrid[i] = new vector<int>[columnNumber];
    for (int j = 0; j < columnNumber; j++) {
      playerGrid[i][j].push_back(-1);
    }
  }
}

void PlayerMap::addPlayer(double x, double y, int t, double c, double r, double v, int ID){
  players.push_back(new Player(x, y, t, c, r, v));
  int cPosition = ((x + DIM_MAX ) / columnNumber ) - 1 / 2;
  int lPosition = - ((y - DIM_MAX) / lineNumber ) - 1 / 2;
  if (playerGrid[lPosition][cPosition][0] == -1){
    playerGrid[lPosition][cPosition][0] = ID;
  } else {
    playerGrid[lPosition][cPosition].push_back(ID);
  }
}

void PlayerMap::reserveSpace(int nbPlayer){
  players.reserve(nbPlayer);
}

Player* PlayerMap::getPlayer(int p) const {
  return players[p];
}