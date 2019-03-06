class Ball {
    public :
        Ball(double x, double y, double angle, int nbCell);
        ~Ball(){};
        double getX () const;
        double getY () const;
        double getRadius () const;
        void updatePosition();
    private :
        double xPosition, yPosition;
        double radius;
        double velocity;
        double angle;
};