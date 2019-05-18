/*!
  \file   tools.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Tools" module header
*/

#ifndef TOOLS_H
#define TOOLS_H

class Circle {
  public:
    Circle(double x, double y, double r);
    ~Circle();
    double getRadius() const;
    double getX() const;
    double getY() const;
    void setX(int x);
    void setY(int y);

  private:
    double radius;
    double xPosition;
    double yPosition;
};

class Square {
  public:
    Square(double x, double y, double s);
    ~Square();
    double getSide() const;
    double getX() const;
    double getY() const;
    void setX(int x);
    void setY(int y);

  private:
    double side;
    double xPosition;
    double yPosition;
};

double distance(double x, double y);
double distance(double x1, double y1, double x2, double y2);
double distance(Circle *c1, Circle *c2);
double distance(Square *s1, Square *s2);
double distance(Square *s, Circle *c);
double distance(Circle *c, Square *s);

double angle(double x1, double y1, double x2, double y2);

#endif