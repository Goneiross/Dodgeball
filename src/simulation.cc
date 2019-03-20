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

void largeCheckColision(vector<Player *> players, Map* map, int p, 
                        vector<int> &toCheck);
void largeCheckColision(vector<Ball *> balls, Map* map, int b, vector<int> &toCheck);
void checkCollisions(vector<Player *> players, int p, int pmax, double delta) ;
void checkCollisions(vector<Ball *> balls, int b, int bmax, double delta) ;
void checkCollisions(vector<Player *> players, vector<Ball *> balls, int p,
                     int b, double delta) ;
void checkCollisions(vector<Player *> players, Map *map, int p, int o, double delta);
void checkCollisions(vector<Ball *> balls, Map *map, int b, int o, double delta) ;
void parseData(Map *&mainMap, int &nbCell, double &MJ, double &ML, string tmp0);
void parseData(vector<Player *> &players, int p, double ML, 
                            double playerRadius, double playerVelocity, string tmp0, 
                            string tmp1, string tmp2, string tmp3) ;
void parseData(Map *&mainMap, int nbCell, int o, string tmp0, string tmp1) ;
void parseData(vector<Ball *> &balls, vector<Player *> &players, Map *&mainMap,
               int nbPlayer, int nbObstacle, double ML, int b, double ballRadius, 
               double ballVelocity, string tmp0, string tmp1, string tmp2) ;
void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    vector<Player *> &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, vector<Ball *> &balls) ;

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

void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    vector<Player *> &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, vector<Ball *> &balls) {
  string tmp0, tmp1, tmp2, tmp3;
  char tmp;
  int part = 0, p = 0, o = 0, b = 0; // Use enum instead of part
  double MJ, ML;

  ifstream flux(inputFile, ios::in);
  if (!flux) {
    cout << "Unable to open file " << inputFile << endl;
    exit(0);
  }
  while (flux >> tmp0) {
    if (tmp0 == "#") {
      do {
        flux.get(tmp);
      } while (tmp != '\n');
    } else if (part == 0) {
      parseData(mainMap, nbCell, MJ, ML, tmp0);
      part++;
    } else if (part == 1) {
      nbPlayer = stoi(tmp0);
      players.reserve(nbPlayer);
      part++;
    } else if (part == 2) {
      flux >> tmp1 >> tmp2 >> tmp3;
      double playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
      double playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
      parseData(players, p, ML, playerRadius, playerVelocity, tmp0, tmp1, tmp2, tmp3);
      p++;
      if (p == nbPlayer) {
        part++;
      }
    } else if (part == 3) {
      nbObstacle = stoi(tmp0);
      part++;
    } else if (part == 4) {
      flux >> tmp1;
      parseData(mainMap, nbCell, o, tmp0, tmp1);
      o++;
      if (o == nbObstacle) {
        for (int i = 0; i < nbPlayer; i++) {
          vector<int> toCheck;
          largeCheckColision(players, mainMap, i, toCheck);
          for (auto o : toCheck){
            checkCollisions(players, mainMap, i, o, ML);
          }
        }
        part++;
      }
    } else if (part == 5) {
      nbBall = stoi(tmp0);
      balls.reserve(nbBall + nbPlayer);
      part++;
    } else if (part == 6) {
      flux >> tmp1 >> tmp2;
      double ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
      double ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
      parseData(balls, players, mainMap, nbPlayer, nbObstacle, ML, b, ballRadius, 
                ballVelocity, tmp0, tmp1, tmp2);
      b++;
      if (b == nbBall) {
        for (int i = 0; i < nbBall; i++) {
          vector<int> toCheck;
          largeCheckColision(balls, mainMap, i, toCheck);
          for (auto o : toCheck){
            checkCollisions(balls, mainMap, i, o, ML);
          }
        }
        part++;
      }
    } else {
      flux.get(tmp);
    }
  }
  flux.close();
}

void largeCheckColision(vector<Player *> players, Map* map, int p, 
                        vector<int> &toCheck){
  int cPosition = ((players[p]->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((players[p]->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  cout << cPosition << " " << lPosition << " " << players[p]->getX() << " " 
        << players[p]->getY() << endl << endl;
  for (int i = - 1; i <= 1; i++){
    for (int j = - 1; j <= 1; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

void largeCheckColision(vector<Ball *> balls, Map* map, int b, vector<int> &toCheck){
  int cPosition = ((balls[b]->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((balls[b]->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  cout << cPosition << " " << lPosition << " " << balls[b]->getX() << " " 
        << balls[b]->getY() << endl << endl;
  for (int i = - 1; i <= 1; i++){
    for (int j = - 1; j <= 1; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

void checkCollisions(vector<Player *> players, int p, int pmax, double delta) {
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

void checkCollisions(vector<Ball *> balls, int b, int bmax, double delta) {
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

void checkCollisions(vector<Player *> players, vector<Ball *> balls, int p,
                     int b, double delta) {
  double d = distance(players[p]->getHitbox(), balls[b]->getHitbox());
  if (d < (players[p]->getRadius() + balls[b]->getRadius() + delta)) {
    cout << PLAYER_BALL_COLLISION(p + 1, b + 1) << endl;
    exit(1);
  }
}

void checkCollisions(vector<Player *> players, Map *map, int p, int o, double delta) {
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
  double included;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    included = rayon;
  } else if ((abs(angle) == M_PI / 3) || (abs(angle) == 2 * M_PI / 3)) {
    included = sqrt(2) * rayon;
  } else if ((2 * M_PI / 3 < angle) && (angle < M_PI)) {
    included = (rayon / X) * d;
  } else if ((-M_PI / 3 < angle) && (angle < M_PI / 3)) {
    included = (rayon / X) * d;
  } else if ((-2 * M_PI / 3 > angle) && (angle > -M_PI)) {
    included = (rayon / X) * d;
  } else {
    included = (rayon / Y) * d;
  }
  if (d < (players[p]->getRadius() + abs(included) + delta)) {
    cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl;
    exit(1);
  }
}

void checkCollisions(vector<Ball *> balls, Map *map, int b, int o, double delta) {
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
  double included;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    included = rayon;
  } else if ((abs(angle) == M_PI / 3) || (abs(angle) == 2 * M_PI / 3)) {
    included = sqrt(2) * rayon;
  } else if ((2 * M_PI / 3 < angle) && (angle < M_PI)) {
    included = (rayon / X) * d;
  } else if ((-M_PI / 3 < angle) && (angle < M_PI / 3)) {
    included = (rayon / X) * d;
  } else if ((-2 * M_PI / 3 > angle) && (angle > -M_PI)) {
    included = (rayon / X) * d;
  } else {
    included = (rayon / Y) * d;
  }
  if (d < (balls[b]->getRadius() + abs(included) + delta)) {
    cout << COLL_BALL_OBSTACLE(b + 1) << endl;
    exit(1);
  }
}

void parseData(Map *&mainMap, int &nbCell, double &MJ, double &ML, string tmp0) {
  nbCell = stoi(tmp0);
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

void parseData(vector<Player *> &players, int p, double ML, 
                            double playerRadius, double playerVelocity, string tmp0, 
                            string tmp1, string tmp2, string tmp3) {
  if (((abs(stod(tmp0)) > DIM_MAX) ||
       (abs(stod(tmp1)  > DIM_MAX)))) {
    cout << PLAYER_OUT(p + 1) << endl;
    exit(1);
  } else {
    players.push_back(
        new Player(stod(tmp0), stod(tmp1), stoi(tmp2), stod(tmp3), playerRadius, 
                    playerVelocity));
    checkCollisions(players, p, p, ML);
  }
}

void parseData(Map *&mainMap, int nbCell, int o, string tmp0, string tmp1) {
  if (stoi(tmp0) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp0)) << endl;
    exit(1);
  } else if (stoi(tmp1) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp1)) << endl;
    exit(1);
  } else if (stoi(tmp0) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp0)) << endl;
    exit(1);
  } else if (stoi(tmp1) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp1)) << endl;
    exit(1);
  } else if (mainMap->isObstacle(stoi(tmp0), stoi(tmp1))) {
    cout << MULTI_OBSTACLE(stoi(tmp0), stoi(tmp1)) << endl;
    exit(1);
  } else {
    mainMap->addObstacle(stod(tmp0), stod(tmp1), o);
  }
}

void parseData(vector<Ball *> &balls, vector<Player *> &players, Map *&mainMap,
               int nbPlayer, int nbObstacle, double ML, int b, double ballRadius, 
               double ballVelocity, string tmp0, string tmp1, string tmp2) {
  if ((abs(stoi(tmp0)) > DIM_MAX) || (abs(stoi(tmp1)) > DIM_MAX)) {
    cout << BALL_OUT(b + 1) << endl;
    exit(1);
  } else {
    balls.push_back(new Ball(stod(tmp0), stod(tmp1), stod(tmp2), ballRadius, 
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