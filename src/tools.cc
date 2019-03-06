#include <math.h>
#include "tools.h"

using namespace std;

Circle::Circle(double x, double y, double r){
    xPosition = x;
    yPosition = y;
    radius = r;
}
double Circle::getRadius () const {return radius;}
double Circle::getX () const {return xPosition;}
double Circle::getY () const {return yPosition;}

Square::Square(double x, double y, double s){
    xPosition = x;
    yPosition = y;
    side = s;
}
double Square::getSide () const {return side;}
double Square::getX () const {return xPosition;}
double Square::getY () const {return yPosition;}

// Need to optimize distance computation
// For example do a generic function computing distance between two points

double distance(double x, double y){
    return abs(x - y);
}
double distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(distance(x1, x2), 2) + pow(distance(y1, y2), 2));
}
double distance(Circle c1, Circle c2){ 
    return distance(c1.getX, c1.getY, c2.getX, c2.getY) - c1.getRadius - c2.getRadius;
}
double distance(Square s1, Square s2){
    double x = abs(s2.getX - s1.getX);
    double y = abs(s2.getY - s1.getY);
    double angle = atan(y / x);
    double included = (s1.getSide / 2 + s2.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}
double distance(Square s, Circle c){    // other possibility: make one of those functions call the other
    double x = abs(c.getX - s.getX);          //advantage: shorter code
    double y = abs(c.getY - s.getY);          //disadvantage: one more function call
    double angle = atan(y / x);
    double included = (c.getRadius / 2 + s.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}
double distance(Circle c, Square s){
    double x = abs(c.getX - s.getX);
    double y = abs(c.getY - s.getY);
    double angle = atan(y /x);
    double included = (c.getRadius / 2 + s.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}