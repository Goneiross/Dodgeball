class Circle {
    public :
        Circle(double x, double y, double r);
        ~Circle();
        double getRadius () const;
        double x() const;
        double y() const;
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
        double x() const;
        double y() const;
    private :
        double side;
        double xPosition;
        double yPosition;
};