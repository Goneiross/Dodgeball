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
    lgnPos = l;
    colPos = c;
    gXPosition = -1;
    gYPosition = -1;
    hitbox = new Circle(x, y, r);
}

double Ball::getX() const { return hitbox->getX(); }
double Ball::getY() const { return hitbox->getY(); }
double Ball::getL() const { return lgnPos; }
double Ball::getC() const { return colPos; }
double Ball::getGX() const { return gXPosition; }
double Ball::getGY() const { return gYPosition; }
void Ball::setGX(double gX) { gXPosition = gX; }
void Ball::setGY(double gY) { gYPosition = gY; }
double Ball::getRadius() const { return hitbox->getRadius(); }
Circle *Ball::getHitbox() const { return hitbox; }
double Ball::getAngle() const {return angle; }
double Ball::getVelocity() const {return velocity; }

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
    int colPos = ((x + DIM_MAX ) / (SIDE / lineNumber) ) - 1 / 2;
    int lgnPos = - ((y - DIM_MAX) / (SIDE / lineNumber) ) - 1 / 2;
    balls.push_back(new Ball(x, y, a, r, v, colPos, lgnPos, ID));
    if (ballGrid[lgnPos][colPos][0] == -1){
        ballGrid[lgnPos][colPos][0] = ID;
    } else {
        ballGrid[lgnPos][colPos].push_back(ID);
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

void BallMap::removeAll(){
    int ballNb = balls.size();
    for (int i = 0; i < ballNb; i++){
        removeBall(0);
    }
}

bool BallMap::isBall(int lgnPos, int colPos){
    if (ballGrid[lgnPos][colPos][0] == -1){
        return false;
    } else {
        return true;
    }
}

vector<int> BallMap::whichBall(int lgnPos, int colPos){
    return ballGrid[lgnPos][colPos];
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

void BallMap::updatePosition(){
    int ballNb = balls.size();
    for (int b = 0; b < ballNb; b++){
		balls[b]->getHitbox()->setX(balls[b]->getHitbox()->getX() + cos(balls[b]
            ->getAngle()) * balls[b]->getVelocity());
		balls[b]->getHitbox()->setY(balls[b]->getHitbox()->getY() + sin(balls[b]
            ->getAngle()) * balls[b]->getVelocity());
    // Compute new Line and Column
  	}
}