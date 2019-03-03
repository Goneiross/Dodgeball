#include "player.h"
#include "tools.h"
#include "define"

using namespace std;

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

Ball::Ball(int x0, int y0, int nbCell){
    xPosition = x0;
    yPosition = y0;
    radius = COEF_VITESSE_BALLE * (SIDE / nbCell);
    velocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
}

void Ball::updatePosition(double angle){
    xPosition += cos(angle) * velocity;
    yPosition += sin(angle) * velocity;
}