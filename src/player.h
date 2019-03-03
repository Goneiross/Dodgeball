class Player {
    public :
        Player(int x, int y,  int nbCell);
        void updatePosition(double angle);
        bool touchedAndDead();
        ~Player(){};
        int x();
        int y();
    private :
        double xPosition, yPosition;
        double radius;
        int nbt;
        double count;
        double velocity;
};