class Player {
    public :
        Player(double x, double y,  int nbCell);
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