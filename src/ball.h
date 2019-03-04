class Ball {
    public :
        Ball(int x, int y,  int nbCell);
        ~Ball();
        double x () const;
        double y () const;
        double getRadius () const;
        void updatePosition(double angle);
    private :
        double xPosition, yPosition;
        double radius;
        double velocity;
};