/*!
  \file   ball.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module header
*/

#ifndef BALL_H
#define BALL_H
#include "tools.h"


class Ball {
public:
    Ball(double xPosition, double yPosition, double angle, double radius, 
        	double velocity, int lgnPos, int colPos, int ID);
    ~Ball(){};
	double getX() const;
	double getY() const;
    double getL() const;
    double getC() const;
    double getGX() const;
    double getGY() const;
    int getID() const;
    void setGX(double gY);
    void setGY(double gY);
    void setL(int l);
    void setC(int c);
    double getRadius() const;
    Circle *getHitbox() const;
    double getAngle() const;
    double getVelocity() const;

private:
    int ID;
	double velocity;
	double angle;
	Circle *hitbox;
	int lgnPos, colPos;
	double gXPosition, gYPosition;
};

class BallMap {
public:
    BallMap(int l, int c);
    void addBall(double xPosition, double yPosition, double angle, 
            	double radius, double velocity, int ID);
    void removeBall(int ID);
    void removeAll();
    void reserveSpace(int nbBall);
    bool isBall(int lgnPos, int colPos);
    std::vector<int> whichBall(int lgnPos, int colPos);
    Ball* getBall(int index) const;
    int getNb () const;
    void updatePosition();

private:
    int lineNumber;
    int columnNumber;
    std::vector<Ball *> balls;
    std::vector<int>** ballGrid;
};

#endif