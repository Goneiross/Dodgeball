class Circle {
    public :
        Circle(double x, double y, double r);
        ~Circle();
    private :
        double radius;
        double xPosition;
        double yPosition;
};

class Square {
    public :
        Square(double x, double y, double s);
        ~Square();
    private :
        double side;
        double xPosition;
        double yPosition;
};