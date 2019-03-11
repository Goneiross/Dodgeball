class Map {
    public :
        Map(int xSize, int ySize);
        ~Map(){};
        double getX () const;
        double getY () const;
        void addObstacle(int xPosition, int yPosition);
        void removeObstacle(int xPosition, int yPosition);
        bool isObstacle(double x, double y) const;
        void test();
    private :
        bool **grid;
};