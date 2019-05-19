/*!
  \file   tools.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Tools" module implementation
*/

#include <cmath>
#include <iostream>

#include "tools.h"

using namespace std;

Circle::Circle(double x, double y, double r) {
    xPosition = x;
    yPosition = y;
    radius = r;
}

double Circle::getRadius() const { return radius; }
double Circle::getX() const { return xPosition; }
double Circle::getY() const { return yPosition; }
void Circle::setX(int x) { xPosition = x; }
void Circle::setY(int y) { yPosition = y; }

Square::Square(double x, double y, double s) {
    xPosition = x;
    yPosition = y;
    side = s;
}

double Square::getSide() const { return side; }
double Square::getX() const { return xPosition; }
double Square::getY() const { return yPosition; }
void Square::setX(int x) { xPosition = x; }
void Square::setY(int y) { yPosition = y; }

double distanceAbs(double x1, double x2) { return abs(x1 - x2); }
double distanceAbs(double x1, double y1, double x2, double y2) {
    return sqrt(pow(distanceAbs(x2, x1), 2) + pow(distanceAbs(y2, y1), 2));
}
double distanceAbs(Circle *c1, Circle *c2) {
    return distanceAbs(c1->getX(), c1->getY(), c2->getX(), c2->getY());
}
double distanceAbs(Square *s1, Square *s2) {
    return distanceAbs(s1->getX(), s1->getY(), s2->getX(), s2->getY());
}
double distanceAbs(Square *s, Circle *c) {
    return distanceAbs(s->getX(), s->getY(), c->getX(), c->getY());
}
double distanceAbs(Circle *c, Square *s) {
    return distanceAbs(s->getX(), s->getY(), c->getX(), c->getY());
}

double angle(double x1, double y1, double x2, double y2) {
    return atan2(y2 - y1, x2 - x1);
}