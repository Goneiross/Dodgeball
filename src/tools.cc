#include <math.h>
#include "tools.h"

using namespace std;

Circle::Circle(double x, double y, double r){
    xPosition = x;
    yPosition = y;
    radius = r;
}
double Circle::getRadius () const {return radius;}
double Circle::x () const {return xPosition;}
double Circle::y () const {return yPosition;}

Square::Square(double x, double y, double s){
    xPosition = x;
    yPosition = y;
    side = s;
}
double Square::getSide () const {return side;}
double Square::x () const {return xPosition;}
double Square::y () const {return yPosition;}

// Need to optimize distance computation
// For example do a generic function computing distance between two points

double distance(Circle c1, Circle c2){ 
    return sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)) - c1.getRadius - c2.getRadius;
}
double distance(Square s1, Square s2){
    double x = s2.x - s1.x;
    double y = s2.y - s1.y;
    double angle = atan(y /x);
    double included = (s1.getSide / 2 + s2.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}
double distance(Square s, Circle c){
    double x = c.x - s.x;
    double y = c.y - s.y;
    double angle = atan(y /x);
    double included = (c.getRadius / 2 + s.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}
double distance(Circle c, Square s){
    double x = c.x - s.x;
    double y = c.y - s.y;
    double angle = atan(y /x);
    double included = (c.getRadius / 2 + s.getSide / 2) / cos(angle);
    return sqrt(pow(x, 2) + pow(y, 2)) - included;
}