#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

class Obstacle {
    public :
        Obstacle(double x, double y, double s);
        ~Obstacle(){};
        double getX () const;
        double getY () const;
        Square* getHitbox() const;
    private :
        double xPosition;
        double yPosition;
        Square* hitbox;
};

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
        int xSize;
        int ySize;
};