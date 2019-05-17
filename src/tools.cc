/*!
  \file   tools.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Tools" module implementation
*/

#include "tools.h"
#include <cmath>
#include <iostream>

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

double distance(double x1, double x2) { return abs(x1 - x2); }
double distance(double x1, double y1, double x2, double y2) {
  return sqrt(pow(distance(x2, x1), 2) + pow(distance(y2, y1), 2));
}
double distance(Circle *c1, Circle *c2) {
  return distance(c1->getX(), c1->getY(), c2->getX(), c2->getY());
}
double distance(Square *s1, Square *s2) {
  return distance(s1->getX(), s1->getY(), s2->getX(), s2->getY());
}
double distance(Square *s, Circle *c) {
  return distance(s->getX(), s->getY(), c->getX(), c->getY());
}
double distance(Circle *c, Square *s) {
  return distance(s->getX(), s->getY(), c->getX(), c->getY());
}

double angle(double x1, double y1, double x2, double y2) {
  cout << "Compute angle from coords : "
       << "(" << x1 << "," << y1 << ") and (" << x2 << "," << y2 << ")" << endl;
  if (x2 > x1 && y2 > y1) {
    return atan(distance(y2, y1) / distance(x2, x1));
  } else if (x2 < x1 && y2 > y1) {
    return M_PI_2 + atan(distance(x2, x1) / distance(y2, y1));
  } else if (x2 < x1 && y2 < y1) {
    return M_PI + atan(distance(y2, y1) / distance(x2, x1));
  } else if (x2 > x1 && y2 < y1) {
    return M_PI + M_PI_2 + atan(distance(x2, x1) / distance(y2, y1));
  } else if (x1 == x2) {
    if (y2 > y1) {
      return M_PI_2;
    } else if (y2 < y1) {
      return -M_PI_2;
    } else {
      std::cout << "UNDEFINED" << endl;
      exit(1);
    }
  } else if (y1 == y2) {
    if (x2 > x1) {
      return 0;
    } else if (x2 < x1) {
      return -M_PI;
    }
  }
}