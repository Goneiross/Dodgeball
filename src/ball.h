class Ball {
    public :
        Ball(int x, int y,  int nbCell);
        void updatePosition(double angle);
        ~Ball();
    private :
        double xPosition, yPosition;
        double radius;
        double velocity;
};