class Player {
    public :
        Player(double x0, double y0,  int nbCell);
        Player(double x0, double y0, int t, double c, int nbCell);
        ~Player(){};
        double x() const;
        double y() const;
        double getRadius() const;
        double getCount() const;
        double getTimeTouched() const;
        void updatePosition(double angle);
        bool touchedAndDead();
    private :
        double xPosition, yPosition;
        double radius;
        int timeTouched;
        double count;
        double velocity;
};