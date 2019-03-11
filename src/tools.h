class Circle {
    public :
        Circle(double x, double y, double r);
        ~Circle();
        double getRadius () const;
        double getX() const;
        double getY() const;
    private :
        double radius;
        double xPosition;
        double yPosition;
};

class Square {
    public :
        Square(double x, double y, double s);
        ~Square();
        double getSide () const;
        double getX() const;
        double getY() const;
    private :
        double side;
        double xPosition;
        double yPosition;
};

double distance(double x, double y);
double distance(double x1, double y1, double x2, double y2);
double distance(Circle* c1, Circle* c2);
double distance(Square* s1, Square* s2);
double distance(Square* s, Circle* c);
double distance(Circle* c, Square* s);