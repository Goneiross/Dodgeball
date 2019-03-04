class Map {
    public :
        Map(int xSize, int ySize);
        ~Map();
        void addObstacle(int xPosition, int yPosition);
        void removeObstacle(int xPosition, int yPosition);
    private :
        bool** grid;
}