/*!
  \file   player.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module implementation
*/

#include <math.h>
#include <iostream>
#include <vector>
#include "player.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int t, double c, double r, 
              int line, int column, double v, int i) {
  timeTouched = t;
  ID = i;
  count = c;
  velocity = v;
  lPosition = line;
  cPosition = column;
  hitbox = new Circle(x0, y0, r);
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
double Player::getL() const { return lPosition; }
double Player::getC() const { return cPosition; }
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
  int cPosition = ((x + DIM_MAX ) / (SIDE / lineNumber) ) - 1 / 2;
  int lPosition = - ((y - DIM_MAX) / (SIDE / lineNumber) ) - 1 / 2;
  players.push_back(new Player(x, y, t, c, r, lPosition, cPosition, v, ID));
  if (playerGrid[lPosition][cPosition][0] == -1){
    playerGrid[lPosition][cPosition][0] = ID;
  } else {
    playerGrid[lPosition][cPosition].push_back(ID);
  }
}

bool PlayerMap::isPlayer(int lPosition, int cPosition){
  if (playerGrid[lPosition][cPosition][0] == -1){
    return false;
  } else {
    return true;
  }
}

vector<int> PlayerMap::whichPlayer(int lPosition, int cPosition){
  return playerGrid[lPosition][cPosition];
}

void PlayerMap::reserveSpace(int nbPlayer){
  players.reserve(nbPlayer);
}

Player* PlayerMap::getPlayer(int p) const {
  return players[p];
}