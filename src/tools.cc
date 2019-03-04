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