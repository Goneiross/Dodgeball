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
#include "ball.h"
#include "map.h"
#include "player.h"
#include "error.h"

using namespace std;

static void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    vector<Player *> &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, vector<Ball *> &balls) ;
static void largeCheckColision(vector<Player *> players, Map* map, int p, 
                        vector<int> &toCheck);
static void largeCheckColision(vector<Ball *> balls, Map* map, int b, vector<int> &toCheck);
static void checkCollisions(vector<Player *> players, int p, int pmax, double delta) ;
static void checkCollisions(vector<Ball *> balls, int b, int bmax, double delta) ;
static void checkCollisions(vector<Player *> players, vector<Ball *> balls, int p,
                     int b, double delta) ;
static void checkCollisions(vector<Player *> players, Map *map, int p, int o, double delta);
static void checkCollisions(vector<Ball *> balls, Map *map, int b, int o, double delta) ;
static void parseData(Map *&mainMap, int &nbCell, double &MJ, double &ML, string inputData0);
static void parseData(vector<Player *> &players, int p, double ML, 
                            double playerRadius, double playerVelocity, string inputData0, 
                            string inputData1, string inputData2, string inputData3) ;
static void parseData(Map *&mainMap, int nbCell, int o, string inputData0, string inputData1) ;
static void parseData(vector<Ball *> &balls, vector<Player *> &players, Map *&mainMap,
               int nbPlayer, int nbObstacle, double ML, int b, double ballRadius, 
               double ballVelocity, string inputData0, string inputData1, string inputData2) ;


void simulation(std::string inputFile, int mode) {
  int nbCell = 0, nbPlayer = 0, nbObstacle = 0, nbBall = 0;
  vector<Player *> players;
  vector<Ball *> balls;
  Map *mainMap;

  initialization(inputFile, nbCell, nbPlayer, players, nbObstacle, mainMap,
                 nbBall, balls);
  if (mode == 1) {
    cout << FILE_READING_SUCCESS << endl;
    delete mainMap;
    for (int i = 0; i < nbPlayer; i++) {
      delete players[i];
    }
    for (int i = 0; i < nbBall; i++) {
      delete balls[i];
    }
    return;
  } else {
    cout << FILE_READING_SUCCESS << endl;
    delete mainMap;
    for (int i = 0; i < nbPlayer; i++) {
      delete players[i];
    }
    for (int i = 0; i < nbBall; i++) {
      delete balls[i];
    }
    return;
  }
}

static void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    vector<Player *> &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, vector<Ball *> &balls) {
  string inputData[4];
  char charBin;
  int parseType = 0;
  int p = 0, o = 0, b = 0; // Use enum instead of parseType
  double MJ, ML;

  ifstream flux(inputFile, ios::in);
  if (!flux) {
    cout << "Unable to open file " << inputFile << endl;
    exit(0);
  }
  while (flux >> inputData[0]) {
    if (inputData[0] == "#") {
      do {
        flux.get(charBin);
      } while (charBin != '\n');
    } else if (parseType == 0) {
      parseData(mainMap, nbCell, MJ, ML, inputData[0]);
      parseType++;
    } else if (parseType == 1) {
      nbPlayer = stoi(inputData[0]);
      players.reserve(nbPlayer);
      parseType++;
    } else if (parseType == 2) {
      flux >> inputData[1] >> inputData[2] >> inputData[3];
      double playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
      double playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
      parseData(players, p, ML, playerRadius, playerVelocity, inputData[0], inputData[1], inputData[2], inputData[3]);
      p++;
      if (p == nbPlayer) {
        parseType++;
      }
    } else if (parseType == 3) {
      nbObstacle = stoi(inputData[0]);
      parseType++;
    } else if (parseType == 4) {
      flux >> inputData[1];
      parseData(mainMap, nbCell, o, inputData[0], inputData[1]);
      o++;
      if (o == nbObstacle) {
        for (int i = 0; i < nbPlayer; i++) {
          vector<int> toCheck;
          largeCheckColision(players, mainMap, i, toCheck);
          for (auto o : toCheck){
            checkCollisions(players, mainMap, i, o, ML);
          }
        }
        parseType++;
      }
    } else if (parseType == 5) {
      nbBall = stoi(inputData[0]);
      balls.reserve(nbBall + nbPlayer);
      parseType++;
    } else if (parseType == 6) {
      flux >> inputData[1] >> inputData[2];
      double ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
      double ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
      parseData(balls, players, mainMap, nbPlayer, nbObstacle, ML, b, ballRadius, 
                ballVelocity, inputData[0], inputData[1], inputData[2]);
      b++;
      if (b == nbBall) {
        for (int i = 0; i < nbBall; i++) {
          vector<int> toCheck;
          largeCheckColision(balls, mainMap, i, toCheck);
          for (auto o : toCheck){
            checkCollisions(balls, mainMap, i, o, ML);
          }
        }
        parseType++;
      }
    } else {
      flux.get(charBin);
    }
  }
  flux.close();
}

static void largeCheckColision(vector<Player *> players, Map* map, int p, 
                        vector<int> &toCheck){
  int cPosition = ((players[p]->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((players[p]->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  for (int i = - 1; i <= 1; i++){
    for (int j = - 1; j <= 1; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

static void largeCheckColision(vector<Ball *> balls, Map* map, int b, vector<int> &toCheck){
  int cPosition = ((balls[b]->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((balls[b]->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  for (int i = - 1; i <= 1; i++){
    for (int j = - 1; j <= 1; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

static void checkCollisions(vector<Player *> players, int p, int pmax, double delta) {
  for (int i = 0; i <= pmax; i++) {
    if (i != p) {
      double d = distance(players[i]->getHitbox(), players[p]->getHitbox());
      if (d < (players[i]->getRadius() + players[p]->getRadius() + delta)) {
        cout << PLAYER_COLLISION(i + 1, p + 1) << endl;
        exit(1);
      }
    }
  }
}

static void checkCollisions(vector<Ball *> balls, int b, int bmax, double delta) {
  for (int i = 0; i <= bmax; i++) {
    if (i != b) {
      double d = distance(balls[i]->getHitbox(), balls[b]->getHitbox());
      if (d < (balls[i]->getRadius() + balls[b]->getRadius() + delta)) {
        cout << BALL_COLLISION(i + 1, b + 1) << endl;
        exit(1);
      }
    }
  }
}

static void checkCollisions(vector<Player *> players, vector<Ball *> balls, int p,
                     int b, double delta) {
  double d = distance(players[p]->getHitbox(), balls[b]->getHitbox());
  if (d < (players[p]->getRadius() + balls[b]->getRadius() + delta)) {
    cout << PLAYER_BALL_COLLISION(p + 1, b + 1) << endl;
    exit(1);
  }
}

static void checkCollisions(vector<Player *> players, Map *map, int p, int o, double delta) {
  double d = distance(map->getObstacle()[o]->getHitbox(), players[p]->getHitbox());
  double X = map->getObstacle()[o]->getX() - players[p]->getX();
  double Y = map->getObstacle()[o]->getY() - players[p]->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle()[o]->getHitbox()->getSide() / 2;
  double squareRadius;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    squareRadius = rayon;
  } else if ((abs(angle) == M_PI / 3) || (abs(angle) == 2 * M_PI / 3)) {
    squareRadius = sqrt(2) * rayon;
  } else if ((2 * M_PI / 3 < angle) && (angle < M_PI)) {
    squareRadius = (rayon / X) * d;
  } else if ((-M_PI / 3 < angle) && (angle < M_PI / 3)) {
    squareRadius = (rayon / X) * d;
  } else if ((-2 * M_PI / 3 > angle) && (angle > -M_PI)) {
    squareRadius = (rayon / X) * d;
  } else {
    squareRadius = (rayon / Y) * d;
  }
  if (d < (players[p]->getRadius() + abs(squareRadius) + delta)) {
    cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl;
    exit(1);
  }
}

static void checkCollisions(vector<Ball *> balls, Map *map, int b, int o, double delta) {
  double d = distance(map->getObstacle()[o]->getHitbox(), balls[b]->getHitbox());
  double X = map->getObstacle()[o]->getX() - balls[b]->getX();
  double Y = map->getObstacle()[o]->getY() - balls[b]->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle()[o]->getHitbox()->getSide() / 2;
  double squareRadius;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    squareRadius = rayon;
  } else if ((abs(angle) == M_PI / 3) || (abs(angle) == 2 * M_PI / 3)) {
    squareRadius = sqrt(2) * rayon;
  } else if ((2 * M_PI / 3 < angle) && (angle < M_PI)) {
    squareRadius = (rayon / X) * d;
  } else if ((-M_PI / 3 < angle) && (angle < M_PI / 3)) {
    squareRadius = (rayon / X) * d;
  } else if ((-2 * M_PI / 3 > angle) && (angle > -M_PI)) {
    squareRadius = (rayon / X) * d;
  } else {
    squareRadius = (rayon / Y) * d;
  }
  if (d < (balls[b]->getRadius() + abs(squareRadius) + delta)) {
    cout << COLL_BALL_OBSTACLE(b + 1) << endl;
    exit(1);
  }
}

static void parseData(Map *&mainMap, int &nbCell, double &MJ, double &ML, string inputData0) {
  nbCell = stoi(inputData0);
  mainMap = new Map(nbCell, nbCell);
  /*
  if ((nbCell > MAX_CELL) || (nbCell < MIN_CELL)) {
    cout << "Error, wrong cell number" << endl;
    exit(1);
  }
  */
  MJ = COEF_MARGE_JEU * (SIDE / nbCell);
  ML = MJ / 2;
}

static void parseData(vector<Player *> &players, int p, double ML, 
                            double playerRadius, double playerVelocity, string inputData0, 
                            string inputData1, string inputData2, string inputData3) {
  if (((abs(stod(inputData0)) > DIM_MAX) ||
       (abs(stod(inputData1)  > DIM_MAX)))) {
    cout << PLAYER_OUT(p + 1) << endl;
    exit(1);
  } else {
    players.push_back(
        new Player(stod(inputData0), stod(inputData1), stoi(inputData2), stod(inputData3), playerRadius, 
                    playerVelocity));
    checkCollisions(players, p, p, ML);
  }
}

static void parseData(Map *&mainMap, int nbCell, int o, string inputData0, string inputData1) {
  if (stoi(inputData0) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    exit(1);
  } else if (stoi(inputData1) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    exit(1);
  } else if (stoi(inputData0) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    exit(1);
  } else if (stoi(inputData1) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    exit(1);
  } else if (mainMap->isObstacle(stoi(inputData0), stoi(inputData1))) {
    cout << MULTI_OBSTACLE(stoi(inputData0), stoi(inputData1)) << endl;
    exit(1);
  } else {
    mainMap->addObstacle(stod(inputData0), stod(inputData1), o);
  }
}

static void parseData(vector<Ball *> &balls, vector<Player *> &players, Map *&mainMap,
               int nbPlayer, int nbObstacle, double ML, int b, double ballRadius, 
               double ballVelocity, string inputData0, string inputData1, string inputData2) {
  if ((abs(stoi(inputData0)) > DIM_MAX) || (abs(stoi(inputData1)) > DIM_MAX)) {
    cout << BALL_OUT(b + 1) << endl;
    exit(1);
  } else {
    balls.push_back(new Ball(stod(inputData0), stod(inputData1), stod(inputData2), ballRadius, 
                    ballVelocity));
    checkCollisions(balls, b, b, ML);
    for (int i = 0; i < nbPlayer; i++) {
      checkCollisions(players, balls, i, b, ML);
    }
    for (int o = 0; o < nbObstacle; o++) {
      checkCollisions(balls, mainMap, b, o, ML);
    }
  }
}