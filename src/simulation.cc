/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  Ball module implementation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "simulation.h"
#include "player.h"
#include "ball.h"
#include "obstacle.h"

#include "error.h"
#include "GUI.h"

using namespace std;

static void largeCollisionCheckPO(int p, vector<int> &toCheck);
static void largeCollisionCheckBO(int b, vector<int> &toCheck);
static void largeCollisionCheckBB(int b, vector<int> &toCheck, int lMax);
static void largeCollisionCheckPP(int b, vector<int> &toCheck, int lMax);
static void collisionCheckPP(int p, int pmax, double delta, bool &error, int mode);
static void collisionCheckBB(int b, int bmax, double delta, bool &error, int mode);
static void collisionCheckPB(int p, int b, double delta, bool &error, int mode);
static void collisionCheckPO(int p, int o, double delta, bool &error, int mode);
static void collisionCheckBO(int b, int o, double delta, bool &error, int mode);
static void parseData(double &ingameMargin, double &parsingMargin,
                      string inputData0, bool &error);
static void parsePlayer(int playerIndex, double parsingMargin,
                        double playerRadius, double playerVelocity,
                        string inputData0, string inputData1, string inputData2,
                        string inputData3, bool &error, int mode);
static void parseObstacle(int objectIndex, string
                          inputData0,
                          string inputData1, bool &error, int mode);
static void parseBall(int nbPlayer, int nbObstacle,
                      double parsingMargin, int ballIndex, double ballRadius,
                      double ballVelocity, string inputData0, string inputData1,
                      string inputData2, bool &error, int mode);
static void initConstants(double &ballRadius, double &ballVelocity,
                          double &playerRadius, double &playerVelocity);

static PlayerMap *players;
static BallMap *balls;
static ObstacleMap *obstacles;

void simulation(std::string inputFile, std::string saveFile, int mode) {
    players = new PlayerMap(0, 0);
    balls = new BallMap(0, 0);
    obstacles = new ObstacleMap(0, 0);
    bool success = false;

    if (inputFile != "") {success = initialization(inputFile, mode);}

    if (mode == ERROR_MODE) {
        if (success) {cout << FILE_READING_SUCCESS << endl;}
        delete obstacles;
        delete players;
        delete balls;
        return;
    } else if (mode == STEP_MODE) {
        update();
        check();
        dracarys();
        save(saveFile);
        delete obstacles;
        delete players;
        delete balls;
        return;
    } else {
        if (not success) {
        players = new PlayerMap(0, 0);
        balls = new BallMap(0, 0);
        obstacles = new ObstacleMap(0, 0);
        }
        draw(success);
        delete obstacles;
        delete players;
        delete balls;
        return;
    }
}

void simulation(std::string inputFile, int mode) {
    players = new PlayerMap(0, 0);
    balls = new BallMap(0, 0);
    obstacles = new ObstacleMap(0, 0);
    bool success = false;

    if (inputFile != "") {success = initialization(inputFile, mode);}

    if (mode == ERROR_MODE) {
        if (success) {cout << FILE_READING_SUCCESS << endl;}
        delete obstacles;
        delete players;
        delete balls;
        return;
    } else {
        if (not success) {
        players = new PlayerMap(0, 0);
        balls = new BallMap(0, 0);
        obstacles = new ObstacleMap(0, 0);
        }
        draw(success);
        delete obstacles;
        delete players;
        delete balls;
        return;
    }
}

bool initialization(string inputFile, int mode) {
    bool error = false;
    int nbPlayer = 0, nbObstacle = 0, nbBall = 0, parseType = 0;
    int p = 0, o = 0, b = 0; // ID of the currently selected object/player/ball
    double ballRadius = 0, ballVelocity = 0, playerRadius = 0, playerVelocity = 0;
    double ingameMargin = 0, parsingMargin = 0;
    string inputData[4]; char charBin;
    enum parseType {cellParsing = 0, playerNumberParsing, playerParsing,
                    obstacleNumberParsing, obstacleParsing,
                    ballNumberParsing, ballParsing};
    ifstream flux(inputFile, ios::in);
    if (!flux) {cout << "Unable to open file " << inputFile << endl; exit(0);}
    while (flux >> inputData[0]) {
        if (error) {return false;}
        else if (inputData[0] == "#") {do{flux.get(charBin);} while (charBin != '\n');}
        else { switch (parseType) {
              case cellParsing:
                parseData(ingameMargin, parsingMargin, inputData[0], error);
                initConstants(ballRadius, ballVelocity, playerRadius, playerVelocity);
                parseType++;
                break;
              case playerNumberParsing:
                nbPlayer = stoi(inputData[0]);
                players->reserveSpace(nbPlayer);
                if (nbPlayer == 0){parseType += 2;} else {parseType++;}
                break;
              case playerParsing:
                flux >> inputData[1] >> inputData[2] >> inputData[3];
                parsePlayer(p, parsingMargin, playerRadius, playerVelocity,
                    inputData[0], inputData[1], inputData[2], inputData[3],error,mode);
                p++; if (p == nbPlayer) {parseType++;}
                break;
              case obstacleNumberParsing:
                nbObstacle = stoi(inputData[0]);
                if (nbObstacle == 0){parseType += 2;} else {parseType++;}
                break;
              case obstacleParsing:
                flux >> inputData[1];
                parseObstacle(o, inputData[0], inputData[1], error, mode);
                o++;
                if (o == nbObstacle && not error) {
                    for (int i = 0; i < nbPlayer; i++) {
                        vector<int> toCheck;
                        largeCollisionCheckPO(i, toCheck);
                        int nbToCheck = toCheck.size();
                        for (int j = 0; j < nbToCheck; j++) {
                            collisionCheckPO(i, toCheck[j], parsingMargin, error,mode);
                            if (error) {return false;}
                        } if (error) {return false;}
                    }
                parseType++;
                }
                break;
              case ballNumberParsing:
                nbBall = stoi(inputData[0]);
                balls->reserveSpace(nbBall + nbPlayer);
                parseType++;
                if (nbBall == 0){parseType++;}
                break;
              case ballParsing:
                flux >> inputData[1] >> inputData[2];
                parseBall(nbPlayer, nbObstacle, parsingMargin, b, ballRadius,
                  ballVelocity, inputData[0], inputData[1], inputData[2], error, mode);
                b++;
                if (b == nbBall && not error) {
                    for (int i = 0; i < nbBall; i++) {
                        vector<int> toCheck;
                        largeCollisionCheckBO(i, toCheck);
                        for (auto o : toCheck) {
                            collisionCheckBO(i, o, parsingMargin, error, mode);
                            if (error) {return false;}
                        }
                    }
                    parseType++;
                }
            }
        }
    }
    flux.close();
    if (error) {return false;} else {return true;}
}

static void initConstants(double &ballRadius, double &ballVelocity,
                          double &playerRadius, double &playerVelocity){
    int nbCell = obstacles->getLNb(); // MAP
    ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
    ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
    playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
    playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
}

void save(string filename) {
    ofstream flux(filename, std::ofstream::out);
    int nbCell = obstacles->getLNb(); // Map
    int n = 0;
    flux << nbCell << endl << endl;
    n = players->getNb();
    flux << n << endl;
    for (int i = 0; i < n; i++) {
        Player *p = players->getPlayer(i);
        flux << p->getX() << " " << p->getY() << " " << p->getTimeTouched() << " "
             << p->getCount() << endl;
        p = nullptr;
    }
    n = obstacles->getNb(); // Map
    flux << endl << n << endl;
    for (int i = 0; i < n; i++) {
        Obstacle *o = obstacles->getObstacle(i);
        flux << o->getL() << " " << o->getC() << endl;
        o = nullptr;
    }
    n = balls->getNb();
    flux << endl << n << endl;
    for (int i = 0; i < n; i++) {
        Ball *b = balls->getBall(i);
        flux << b->getX() << " " << b->getY() << " " << b->getAngle() << endl;
        b = nullptr;
    }
    flux.close();
}

bool isOut(double xPosition, double yPosition) {
    if ((abs(xPosition) > DIM_MAX) || (abs(yPosition) > DIM_MAX)) {
        return true;
    } else {
        return false;
    }
}

void check() {
    int ballNb = balls->getNb();
    int playerNb = players->getNb();
    double delta = COEF_MARGE_JEU * (SIDE / (double)obstacles->getLNb()); // Map
    for (int b = 0; b < ballNb; b++) {
        if (isOut(balls->getBall(b)->getX(), balls->getBall(b)->getY())) {
            balls->removeBall(b);
        }
        for (int p = 0; p < playerNb; p++) {
            bool collision = false;
            collisionCheckPB(p, b, delta, collision, 0);
            if (collision) {
                balls->removeBall(b);
                players->getPlayer(p)
                       ->setTimeTouched(players->getPlayer(p)->getTimeTouched() - 1);
            }
        }
    }
}

void update() {
    balls->updatePosition();
    players->updatePosition();
}

void dracarys(){
    double ballRadius = COEF_RAYON_BALLE * (SIDE / obstacles->getLNb()); // Changer ca !
    double playerRadius = COEF_RAYON_JOUEUR * (SIDE / obstacles->getLNb()); //Ca aussi !
    double ballVelocity = COEF_VITESSE_BALLE * (SIDE / obstacles->getLNb()); //Et aussi lui !
    for (int p = 0; p < players->getNb(); p++){
        if (players->getPlayer(p)->getCount() == MAX_COUNT){
            double xPos = players->getPlayer(p)->getX();
            double yPos = players->getPlayer(p)->getY();
            double delta = ballRadius + playerRadius;
            // double angle = nearestPlayerAngle();
            double angle = 0; //TO delete
            balls->addBall(xPos + cos(angle) * delta, yPos + sin(angle) * delta, angle, ballRadius, ballVelocity, balls->getNewID());
            players->getPlayer(p)->setCount(0);
        }
    }
}

static void largeCollisionCheckPO(int p, vector<int> &toCheck) {
    int colPos = ((players->getPlayer(p)->getX() + DIM_MAX)
                / obstacles->getObstacle(0)->getHitbox()->getSide()) 
                - 1 / 2;
    int lgnPos = -((players->getPlayer(p)->getY() - DIM_MAX)
                / obstacles->getObstacle(0)->getHitbox()->getSide()) 
                - 1 / 2;
    int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
    if (lgnPos == 0) {leftL = 0;}
    if (colPos == 0) {leftC = 0;}
    if (lgnPos == obstacles->getCNb() - 1) {rightL = 0;} // Map
    if (colPos == obstacles->getLNb() - 1) {rightC = 0;} // Map
    for (int i = leftL; i <= rightL; i++) {
        for (int j = leftC; j <= rightC; j++) {
            if (obstacles->isObstacle(lgnPos + i, colPos + j)) {
                toCheck.push_back(obstacles->whichObstacle(lgnPos + i, colPos + j));
            }
        }
    }
}

static void largeCollisionCheckBO(int b, vector<int> &toCheck) {
    int colPos = ((balls->getBall(b)->getX() + DIM_MAX)
                   / obstacles->getObstacle(0)->getHitbox()->getSide())
                   - 1 / 2;
    int lgnPos = -((balls->getBall(b)->getY() - DIM_MAX)
                   / obstacles->getObstacle(0)->getHitbox()->getSide())
                   - 1 / 2;
    int leftL = -1, leftC = -1, rightL = 1, rightC = 1;

    if (lgnPos == 0) {leftL = 0;}
    if (colPos == 0) {leftC = 0;}
    if (lgnPos == obstacles->getCNb() - 1) {rightL = 0;} // Map
    if (colPos == obstacles->getLNb() - 1) {rightC = 0;} // Map

    for (int i = leftL; i <= rightL; i++) {
        for (int j = leftC; j <= rightC; j++) {
            if (obstacles->isObstacle(lgnPos + i, colPos + j)) {
                toCheck.push_back(obstacles->whichObstacle(lgnPos + i, colPos + j));
            }
        }
    }
}

static void largeCollisionCheckBB(int b, vector<int> &toCheck, int lMax) {
    int colPos = balls->getBall(b)->getC();
    int lgnPos = balls->getBall(b)->getL();
    int leftL = -1, leftC = -1, rightL = 1, rightC = 1;

    if (lgnPos == 0) {leftL = 0;}
    if (colPos == 0) {leftC = 0;}
    if (lgnPos == lMax - 1) {rightL = 0;}
    if (colPos == lMax - 1) {rightC = 0;} // cMax

    for (int i = leftL; i <= rightL; i++) {
      for (int j = leftC; j <= rightC; j++) {
            if (balls->isBall(lgnPos + i, colPos + j)) {
            int n = balls->whichBall(lgnPos + i, colPos + j).size();
            int tmp = -1;
            for (int k = 0; k < n; k++) {
                tmp = balls->whichBall(lgnPos + i, colPos + j)[k];
                if ((tmp > -1) && (tmp != b)) {
                    toCheck.push_back(tmp);
                }
            }
        }
        }
    }
}

static void largeCollisionCheckPP(int p, vector<int> &toCheck, int lMax) {
    int colPos = players->getPlayer(p)->getC();
    int lgnPos = players->getPlayer(p)->getL();
    int leftL = -1, leftC = -1, rightL = 1, rightC = 1;

    if (lgnPos == 0) {leftL = 0;}
    if (colPos == 0) {leftC = 0;}
    if (lgnPos == lMax - 1) {rightL = 0;}
    if (colPos == lMax - 1) {rightC = 0;} // cMax

    for (int i = leftL; i <= rightL; i++) {
        for (int j = leftC; j <= rightC; j++) {
            if (players->isPlayer(lgnPos + i, colPos + j)) {
                int n = players->whichPlayer(lgnPos + i, colPos + j).size();
                int tmp = -1;
                for (int k = 0; k < n; k++) {
                    tmp = players->whichPlayer(lgnPos + i, colPos + j)[k];
                    if ((tmp > -1) && (tmp != p)) {
                        toCheck.push_back(tmp);
                    }
                }
            }
        }
    }
}

static void collisionCheckPP(int p, int pmax, double delta, bool &error, int mode) {
    for (int i = 0; i <= pmax; i++) {
        if (i != p) {
            double d = distance(players->getPlayer(i)->getHitbox(),
                                players->getPlayer(p)->getHitbox());
            if (d < (players->getPlayer(i)->getRadius()
                    + players->getPlayer(p)->getRadius() + delta)) {
            cout << PLAYER_COLLISION(i + 1, p + 1) << endl;
            error = true;
            return;
            }
        }
    }
}

static void collisionCheckBB(int b, int c, double delta, bool &error, int mode) {
  double d =
      distance(balls->getBall(c)->getHitbox(), balls->getBall(b)->getHitbox());
    if (d<(balls->getBall(c)->getRadius() + balls->getBall(b)->getRadius() +  delta)) {
        cout << BALL_COLLISION(c + 1, b + 1) << endl;
        error = true;
        return;
    }
}

static void collisionCheckPB(int p, int b, double delta, bool &error, int mode) {
    double d = distance(players->getPlayer(p)->getHitbox(), 
            balls->getBall(b)->getHitbox());
    if (d<(players->getPlayer(p)->getRadius() + balls->getBall(b)->getRadius() + delta)){
        cout << PLAYER_BALL_COLLISION(p + 1, b + 1) << endl;
        error = true;
        return;
    }
}

static void collisionCheckPO(int p, int o, double delta, bool &error,
                             int mode) {
    double d = distance(obstacles->getObstacle(o)->getHitbox(),
                        players->getPlayer(p)->getHitbox());
    double X = obstacles->getObstacle(o)->getX() - players->getPlayer(p)->getX();
      double Y = obstacles->getObstacle(o)->getY() - players->getPlayer(p)->getY();
    double angle;

    if (X == 0) {angle = M_PI_2;}
    else if (Y == 0) {angle = 0;}
    else {angle = atan(Y / X);}

    double rayon = obstacles->getObstacle(o)->getHitbox()->getSide() / 2;
    double squareRadius;

    if ((abs(angle) == M_PI) || (abs(angle) == M_PI_2) || (angle == 0)) {
        squareRadius = rayon;
    } else if ((abs(angle) == M_PI_4) || (abs(angle) == M_PI_2)) {
        squareRadius = sqrt(2) * rayon;
    } else if ((M_PI_2 < angle) && (angle < M_PI)) {
        squareRadius = (rayon / X) * d;
    } else if ((- M_PI_4< angle) && (angle < M_PI_4)) {
        squareRadius = (rayon / X) * d;
    } else if ((- M_PI_2 > angle) && (angle > -M_PI)) {
        squareRadius = (rayon / X) * d;
    } else {
        squareRadius = (rayon / Y) * d;
    }
    if (d < (players->getPlayer(p)->getRadius() + abs(squareRadius) + delta)) {
        cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl;
        error = true;
        return;
    }
}

static void collisionCheckBO(int b, int o, double delta, bool &error, int mode) {
    double d = distance(obstacles->getObstacle(o)->getHitbox(),
                        balls->getBall(b)->getHitbox());
    double X = obstacles->getObstacle(o)->getX() - balls->getBall(b)->getX();
    double Y = obstacles->getObstacle(o)->getY() - balls->getBall(b)->getY();
    double angle;

    if (X == 0) { angle = M_PI_2;}
    else if (Y == 0) {angle = 0;} 
    else {angle = atan(Y / X);}

    double rayon = obstacles->getObstacle(o)->getHitbox()->getSide() / 2;
    double squareRadius;

    if ((abs(angle) == M_PI) || (abs(angle) == M_PI_2) || (angle == 0)) {
        squareRadius = rayon;
    } else if ((abs(angle) == M_PI_4) || (abs(angle) == M_PI_2)) {
        squareRadius = sqrt(2) * rayon;
    } else if ((M_PI_2 < angle) && (angle < M_PI)) {
        squareRadius = (rayon / X) * d;
    } else if ((- M_PI_4< angle) && (angle < M_PI_4)) {
        squareRadius = (rayon / X) * d;
    } else if ((- M_PI_2 > angle) && (angle > -M_PI)) {
        squareRadius = (rayon / X) * d;
    } else {
        squareRadius = (rayon / Y) * d;}
    if (d < (balls->getBall(b)->getRadius() + abs(squareRadius) + delta)) {
        cout << COLL_BALL_OBSTACLE(b + 1) << endl;
        error = true;
        return;
    }
}

static void parseData(double &ingameMargin, double &parsingMargin,
                      string inputData0, bool &error) {
    int nbCell = stoi(inputData0);
    obstacles = new ObstacleMap(nbCell, nbCell);
    players = new PlayerMap(nbCell, nbCell);
    balls = new BallMap(nbCell, nbCell);
    ingameMargin = COEF_MARGE_JEU * (SIDE / nbCell);
    parsingMargin = ingameMargin / 2;
}

static void parsePlayer(int p, double parsingMargin, double playerRadius,
                        double playerVelocity, string inputData0, string inputData1,
                        string inputData2, string inputData3, bool &error, int mode) {
    int nbCell = obstacles->getLNb(); // Map
    if (((abs(stod(inputData0)) > DIM_MAX) || (abs(stod(inputData1) > DIM_MAX)))) {
        cout << PLAYER_OUT(p + 1) << endl;
        error = true;
        return;
    } else {
        players->addPlayer(stod(inputData0), stod(inputData1), stoi(inputData2),
                           stod(inputData3), playerRadius, playerVelocity, p);
        vector<int> toCheck;
        largeCollisionCheckPP(p, toCheck, nbCell);
        for (auto c : toCheck) {
            collisionCheckPP(p, c, parsingMargin, error, mode);
            if (error) {return;}
        }
    }
}

static void parseObstacle(int o, string inputData0, string inputData1,
                          bool &error, int mode) {
    int nbCell = obstacles->getLNb(); // Map
    if (stoi(inputData0) >= nbCell) {
        cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
        error = true;
        return;
    } else if (stoi(inputData1) >= nbCell) {
        cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
        error = true;
        return;
    } else if (stoi(inputData0) < 0) {
        cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
        error = true;
        return;
    } else if (stoi(inputData1) < 0) {
        cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
        error = true;
        return;
    } else if (obstacles->isObstacle(stoi(inputData0), stoi(inputData1))) {
        cout << MULTI_OBSTACLE(stoi(inputData0), stoi(inputData1)) << endl;
        error = true;
        return;
    } else {
        obstacles->addObstacle(stod(inputData0), stod(inputData1), o);
    }
}

static void parseBall(int nbPlayer, int nbObstacle,
                      double parsingMargin, int b, double ballRadius,
                      double ballVelocity, string inputData0, string inputData1,
                      string inputData2, bool &error, int mode) {
    int nbCell = obstacles->getLNb(); // Map
    if (isOut(stod(inputData0), stod(inputData1))) {
        cout << BALL_OUT(b + 1) << endl;
        error = true;
        return;
    } else {
        balls->addBall(stod(inputData0), stod(inputData1), stod(inputData2),
                            ballRadius, ballVelocity, b);
        vector<int> toCheck;
        largeCollisionCheckBB(b, toCheck, nbCell);
        for (auto c : toCheck) {
            collisionCheckBB(b, c, parsingMargin, error, mode);
            if (error) {return;}
        }
        for (int i = 0; i < nbPlayer; i++) {
            collisionCheckPB(i, b, parsingMargin, error, mode);
            if (error) {return;}
        }
    }
}

int getPlayerNb() { return players->getNb(); }
int getPlayerTimeTouched(int p) {return players->getPlayer(p)->getTimeTouched();}
double getPlayerRadius() {return players->getPlayer(0)->getRadius();} //With one ?
int getPlayerCount(int p) { return players->getPlayer(p)->getCount(); }
Circle *getPlayerHitbox(int p) { return players->getPlayer(p)->getHitbox(); }

int getBallNb() { return balls->getNb(); }
double getBallRadius() { return balls->getBall(0)->getRadius(); } // With one ?
Circle *getBallHitbox(int b) { return balls->getBall(b)->getHitbox(); }

int getObstacleNb() { return obstacles->getNb(); }
double getObstacleSize() { return obstacles->getObstacle(0)->getSide(); }
Square *getObstacleHitbox(int o) {return obstacles->getObstacle(o)->getHitbox();}
