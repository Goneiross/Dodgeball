/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module implementation
*/
#include <vector>
#include <math.h>
#include "define.h"
#include "ball.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Ball::Ball(double x, double y, double a, double r, double v, int l, int c, int i) {
  angle = a;
  ID = i;
  velocity = v;
  lPosition = l;
  cPosition = c;
  hitbox = new Circle(x, y, r);
}
void Ball::updatePosition() {
  hitbox->setX(hitbox->getX() + cos(angle) * velocity);
  hitbox->setY(hitbox->getY() + sin(angle) * velocity);
}
double Ball::getX() const { return hitbox->getX(); }
double Ball::getY() const { return hitbox->getY(); }
double Ball::getL() const { return lPosition; }
double Ball::getC() const { return cPosition; }
double Ball::getRadius() const { return hitbox->getRadius(); }
Circle *Ball::getHitbox() const { return hitbox; }
double Ball::getAngle() const {return angle; }

BallMap::BallMap(int l, int c){
  lineNumber = l;
  columnNumber = c;

  ballGrid = new vector<int>* [lineNumber];
  for (int i = 0; i < lineNumber; i++) {
    ballGrid[i] = new vector<int>[columnNumber];
    for (int j = 0; j < columnNumber; j++) {
      ballGrid[i][j].push_back(-1);
    }
  }
}

void BallMap::addBall(double x, double y, double a, double r, double v, int ID){
  int cPosition = ((x + DIM_MAX ) / (SIDE / lineNumber) ) - 1 / 2;
  int lPosition = - ((y - DIM_MAX) / (SIDE / lineNumber) ) - 1 / 2;
  balls.push_back(new Ball(x, y, a, r, v, cPosition, lPosition, ID));
  if (ballGrid[lPosition][cPosition][0] == -1){
    ballGrid[lPosition][cPosition][0] = ID;
  } else {
    ballGrid[lPosition][cPosition].push_back(ID);
  }
}
void BallMap::removeBall(int ID){
  int l = balls[ID]->getL();
  int c = balls[ID]->getC();
  int posSize = ballGrid[l][c].size();
  if (ballGrid[l][c][0] == ID){
      ballGrid[l][c][0] = -1;
  } else {
    for (int i = 1; i < posSize; i++){
      if (ballGrid[l][c][i] == ID){
        ballGrid[l][c].erase(ballGrid[l][c].begin()+i);
      }
    }
  }
  balls.erase(balls.begin()+ ID );
}

bool BallMap::isBall(int lPosition, int cPosition){
  if (ballGrid[lPosition][cPosition][0] == -1){
    return false;
  } else {
    return true;
  }
}

vector<int> BallMap::whichBall(int lPosition, int cPosition){
  return ballGrid[lPosition][cPosition];
}

void BallMap::reserveSpace(int nbBall){
  balls.reserve(nbBall);
}

Ball* BallMap::getBall(int i) const {
  return balls[i];
}
int BallMap::getNb() const{
  return balls.size();
}