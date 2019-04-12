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

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "player.h"
#include "ball.h"
#include "map.h"
#endif

#include "error.h"
#include "GUI.h"

using namespace std;

static void largeCollisionCheckPO(int p, vector<int> &toCheck);
static void largeCollisionCheckBO(int b, vector<int> &toCheck);
static void largeCollisionCheckBB(int b, vector<int> &toCheck, int lMax);
static void largeCollisionCheckPP(int b, vector<int> &toCheck, int lMax);
static void collisionCheckPP(int p, int pmax, double delta, bool &error);
static void collisionCheckBB(int b, int bmax, double delta, bool &error);
static void collisionCheckPB(int p ,int b, double delta, bool &error);
static void collisionCheckPO(int p, int o, double delta, bool &error);
static void collisionCheckBO(int b, int o, double delta, bool &error);
static void parseData(double &ingameMargin, double &parsingMargin, string inputData0, bool &error);
static void parsePlayer(int p, int nbCell, double parsingMargin, double playerRadius, double playerVelocity, string inputData0, 
                      	string inputData1, string inputData2, string inputData3, bool &error);
static void parseObstacle(int nbCell, int o, string inputData0, string inputData1, bool &error);
static void parseBall(int nbCell, int nbPlayer, int nbObstacle, double parsingMargin, int b,
               	double ballRadius, double ballVelocity, string inputData0,
               	string inputData1, string inputData2, bool &error);

static PlayerMap* players;
static BallMap* balls;
static Map* mainMap;

void simulation(std::string inputFile, int mode) {
  	players = new PlayerMap(0,0);
  	balls = new BallMap(0,0);
  	mainMap = new Map(0,0);
  	bool success;

  	if (inputFile != ""){
    	success = initialization(inputFile);
  	}
  
  	if (mode == 1) {
    	if (success) { cout << FILE_READING_SUCCESS << endl; }
    	delete mainMap;
    	delete players;
    	delete balls;
    	return;
  	} else {
    	if (not success){
      		PlayerMap* players = new PlayerMap(0,0);
      		BallMap* balls = new BallMap(0,0);
      		Map* mainMap = new Map(0,0);
    	}
    	draw();
    	delete mainMap;
    	delete players;
    	delete balls;
    	return;
  	}
}

bool initialization(string inputFile) {
  	bool error = false;
  	int nbCell = 0, nbPlayer = 0, nbObstacle = 0, nbBall = 0;
  	string inputData[4];
  	char charBin;
  	int parseType = 0;
  	int p = 0, o = 0, b = 0;  // ID of the currently selected object/player/ball
  	double ingameMargin, parsingMargin;

  	ifstream flux(inputFile, ios::in);
  	if (!flux) {
    	cout << "Unable to open file " << inputFile << endl;
    	exit(0);
  	}
  	double ballRadius;
  	double ballVelocity;
  	double playerRadius;
  	double playerVelocity;
  	while (flux >> inputData[0]) {
    	if (inputData[0] == "#") {
      		do {
        		flux.get(charBin);
      		} while (charBin != '\n');
    	} else if (parseType == 0) {
      		parseData(ingameMargin, parsingMargin, inputData[0], error);
      		nbCell = mainMap->getLNb();
      		ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
      		ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
      		playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
      		playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
      		parseType++;
    	} else if (parseType == 1) {
      		nbPlayer = stoi(inputData[0]);
      		players->reserveSpace(nbPlayer);
      		parseType++;
    	} else if (parseType == 2) {
      		flux >> inputData[1] >> inputData[2] >> inputData[3];
      		parsePlayer(p, nbCell, parsingMargin, playerRadius, playerVelocity, inputData[0], 
                		inputData[1], inputData[2], inputData[3], error);
      		p++;
      		if (p == nbPlayer) {
        		parseType++;
      		}
    	} else if (parseType == 3) {
      		nbObstacle = stoi(inputData[0]);
      		parseType++;
    	} else if (parseType == 4) {
      		flux >> inputData[1];
      		parseObstacle(nbCell, o, inputData[0], inputData[1], error);
      		o++;
      		if (o == nbObstacle) {
        		for (int i = 0; i < nbPlayer; i++) {
          			vector<int> toCheck;
          			largeCollisionCheckPO(i, toCheck);
          			int nbToCheck = toCheck.size();
          			for (int j = 0; j < nbToCheck; j++){
            			collisionCheckPO(i, toCheck[j], parsingMargin, error);
          			}
        		}
        		parseType++;
      		}
    	} else if (parseType == 5) {
      		nbBall = stoi(inputData[0]);
      		balls->reserveSpace(nbBall + nbPlayer);
      		parseType++;
    	} else if (parseType == 6) {
      		flux >> inputData[1] >> inputData[2];
      		parseBall(nbCell, nbPlayer, nbObstacle, parsingMargin, b, 
                ballRadius, ballVelocity, inputData[0], inputData[1], inputData[2], error);
      		b++;
      		if (b == nbBall) {
        		for (int i = 0; i < nbBall; i++) {
          			vector<int> toCheck;
          			largeCollisionCheckBO(i, toCheck);
          			for (auto o : toCheck){
            			collisionCheckBO(i, o, parsingMargin , error);
          			}
        		}
        		parseType++;
      		}
    	} else {
      		flux.get(charBin);
    	}
  	}
  	flux.close();
  	if (error){
    	return false;
  	} else { 
    	return true;
  	}
}

void save(string filename){
  	ofstream flux(filename, std::ofstream::out);
	int nbCell = mainMap->getLNb();
  	int n = 0;
  	flux << nbCell << endl << endl;
  	n = players->getNb();
  	flux << n << endl;
  	for (int i = 0; i < n; i ++){
    	Player* p = players->getPlayer(i);
    	flux << p->getX() << " " << p->getY() << " " 
         	<< p->getTimeTouched() << " " << p->getCount() << endl;
    	p = nullptr;
  	}
  	n = mainMap->getNb();
  	flux << endl << n << endl;
  	for (int i = 0; i < n; i ++){
    	Obstacle* o = mainMap->getObstacle(i);
    	flux << o->getL() << " " << o->getC() << endl;
    	o = nullptr;
  	}
  	n = balls->getNb();
  	flux << endl << n << endl;
  	for (int i = 0; i < n; i ++){
    	Ball* b = balls->getBall(i);
    	flux << b->getX() << " " << b->getY() << " " 
         	<< b->getAngle() << endl;
    	b = nullptr; 
  	}
  	flux.close();
}

bool isOut(double xPosition, double yPosition){
  	if ((abs(xPosition) > DIM_MAX) || (abs(yPosition) > DIM_MAX)) {
    	return true;
  	} else {
    	return false;
  	}
}

void check(){
  		int ballNb = balls->getNb();
  		int playerNb = players->getNb();
		cout << mainMap->getLNb() << endl;
		int delta = COEF_MARGE_JEU * (SIDE / mainMap->getLNb()); //Better not compute every time
  		cout << delta << endl;
		for (int b = 0; b < ballNb; b++){
    		if (isOut(balls->getBall(b)->getX(), balls->getBall(b)->getY())){
      			balls->removeBall(b);
    		} 
			for (int p = 0; p < playerNb; p++){
				bool collision = false;
				collisionCheckPB(p, b, delta, collision);
				//cout << p << " " << b << endl;
				//cout << players->getPlayer(p)->getX() << " " << players->getPlayer(p)->getY() << " " <<
				//		balls->getBall(b)->getX() << " " << balls->getBall(b)->getY() << endl; 
				if (collision){
					cout << "REMOVE" << endl;
					balls->removeBall(b);
					players->getPlayer(p)->setCount(players->getPlayer(p)->getCount() - 1);
				}
			}
	
			/* else if (collision(balls->getBall(b))){
      
    		}
    		*/
  		}
	}

void update(){
  	balls->updatePosition();
  	players->updatePosition();
}

/*
bool isOut(Obstacle* obstacle){
  	int lValue = obstacle->getL();
  	if (lValue >= nbCell) {
    	return true;
  	} else if (stoi(inputData1) >= nbCell) {
    	return true;
  	} else if (stoi(inputData0) < 0) {
    	return true;
  	} else if (stoi(inputData1) < 0) {
    	return true;
  	} else if (mainMap->isObstacle(stoi(inputData0), stoi(inputData1))) {
    	return true;
  	} else {
    	return false;
  	}
}
*/
static void largeCollisionCheckPO(int p, vector<int> &toCheck){
  	int cPosition = ((players->getPlayer(p)->getX() + DIM_MAX ) /
                    mainMap->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  	int lPosition = - ((players->getPlayer(p)->getY() - DIM_MAX) /
                    mainMap->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  	int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  	if (lPosition == 0){leftL = 0;}
  	if (cPosition == 0){leftC = 0;}
  	if (lPosition == mainMap->getCNb() - 1){rightL = 0;}
  	if (cPosition == mainMap->getLNb() - 1){rightC = 0;}
  	for (int i = leftL; i <= rightL; i++){
    	for (int j = leftC; j <= rightC; j++){
      		if(mainMap->isObstacle(lPosition + i, cPosition + j)){
        		toCheck.push_back(mainMap->whichObstacle(lPosition + i, cPosition + j));
      		}
    	}
  	}
}

static void largeCollisionCheckBO(int b, vector<int> &toCheck){
  	int cPosition = ((balls->getBall(b)->getX() + DIM_MAX ) /
                    mainMap->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  	int lPosition = - ((balls->getBall(b)->getY() - DIM_MAX) /
                    mainMap->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  	int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  	if (lPosition == 0){leftL = 0;}
  	if (cPosition == 0){leftC = 0;}
  	if (lPosition == mainMap->getCNb() - 1){rightL = 0;}
  	if (cPosition == mainMap->getLNb() - 1){rightC = 0;}
  	for (int i = leftL; i <= rightL; i++){
    	for (int j = leftC; j <= rightC; j++){
      		if(mainMap->isObstacle(lPosition + i, cPosition + j)){
        		toCheck.push_back(mainMap->whichObstacle(lPosition + i, cPosition + j));
      		}
    	}
  	}
}

static void largeCollisionCheckBB( int b, vector<int> &toCheck, int lMax){
  	int cPosition = balls->getBall(b)->getC();
  	int lPosition = balls->getBall(b)->getL();
  	int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  	if (lPosition == 0){leftL = 0;}
  	if (cPosition == 0){leftC = 0;}
  	if (lPosition == lMax - 1){rightL = 0;}
  	if (cPosition == lMax - 1){rightC = 0;} //cMax
  	for (int i = leftL; i <= rightL; i++){
    	for (int j = leftC; j <= rightC; j++){
      		if(balls->isBall(lPosition + i, cPosition + j)){
        		int n = balls->whichBall(lPosition + i, cPosition + j).size();
        		int tmp = -1;
        		for (int k = 0; k < n; k++) {
          			tmp = balls->whichBall(lPosition + i, cPosition + j)[k];
          			if((tmp > -1) && (tmp != b)){
            			toCheck.push_back(tmp);
          			}
        		}
      		}
    	}
  	}
}

static void largeCollisionCheckPP(int p, vector<int> &toCheck, int lMax){
  	int cPosition = players->getPlayer(p)->getC();
  	int lPosition = players->getPlayer(p)->getL();
  	int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  	if (lPosition == 0){leftL = 0;}
  	if (cPosition == 0){leftC = 0;}
  	if (lPosition == lMax - 1){rightL = 0;}
  	if (cPosition == lMax - 1){rightC = 0;} //cMax
  	for (int i = leftL; i <= rightL; i++){
    	for (int j = leftC; j <= rightC; j++){
      		if(players->isPlayer(lPosition + i, cPosition + j)){
        		int n = players->whichPlayer(lPosition + i, cPosition + j).size();
        		int tmp = -1;
        		for (int k = 0; k < n; k++) {
          			tmp = players->whichPlayer(lPosition + i, cPosition + j)[k];
          			if((tmp > -1) && (tmp != p)){
            			toCheck.push_back(tmp);
          			}
        		}
      		}
    	}
  	}
}

static void collisionCheckPP(int p, int pmax, double delta, bool &error) {
  	for (int i = 0; i <= pmax; i++) {
    	if (i != p) {
      		double d = distance(players->getPlayer(i)->getHitbox(), players->getPlayer(p)->getHitbox());
      		if (d < (players->getPlayer(i)->getRadius() + players->getPlayer(p)->getRadius() + delta)) {
        		cout << PLAYER_COLLISION(i + 1, p + 1) << endl;
        		error = true;
      		}
    	}
  	}
}

static void collisionCheckBB( int b, int c, double delta, bool &error) {
  	double d = distance(balls->getBall(c)->getHitbox(), balls->getBall(b)->getHitbox());
  	if (d < (balls->getBall(c)->getRadius() + balls->getBall(b)->getRadius() + delta)) {
    	cout << BALL_COLLISION(c + 1, b + 1) << endl;
    	error = true;
  	}
}

static void collisionCheckPB( int p, int b, double delta, bool &error) {
  	double d = distance(players->getPlayer(p)->getHitbox(), balls->getBall(b)->getHitbox());
  	if (d < (players->getPlayer(p)->getRadius() + balls->getBall(b)->getRadius() + delta)) {
    	cout << PLAYER_BALL_COLLISION(p + 1, b + 1) << endl;
    	error = true;
  	}
}

static void collisionCheckPO(int p, int o, double delta, bool &error) {
  	double d = distance(mainMap->getObstacle(o)->getHitbox(), players->getPlayer(p)->getHitbox());
  	double X = mainMap->getObstacle(o)->getX() - players->getPlayer(p)->getX();
  	double Y = mainMap->getObstacle(o)->getY() - players->getPlayer(p)->getY();
  	double angle;
  	if (X == 0) {
    	angle = M_PI_2;
  	} else if (Y == 0) {
    	angle = 0;
  	} else {
    	angle = atan(Y / X);
  	}
  	double rayon = mainMap->getObstacle(o)->getHitbox()->getSide() / 2;
  	double squareRadius;
  	if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    	squareRadius = rayon;
  	} else if ((abs(angle) == M_PI / 4) || (abs(angle) == 2 * M_PI / 4)) {
    	squareRadius = sqrt(2) * rayon;
  	} else if ((2 * M_PI / 4 < angle) && (angle < M_PI)) {
    	squareRadius = (rayon / X) * d;
  	} else if ((-M_PI / 4 < angle) && (angle < M_PI / 4)) {
    	squareRadius = (rayon / X) * d;
  	} else if ((-2 * M_PI / 4 > angle) && (angle > -M_PI)) {
    	squareRadius = (rayon / X) * d;
  	} else {
    	squareRadius = (rayon / Y) * d;
  	}
  	if (d < (players->getPlayer(p)->getRadius() + abs(squareRadius) + delta)) {
    	cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl;
    	error = true;
  	}
}

static void collisionCheckBO(int b, int o, double delta, bool &error) {
  	double d = distance(mainMap->getObstacle(o)->getHitbox(), balls->getBall(b)->getHitbox());
  	double X = mainMap->getObstacle(o)->getX() - balls->getBall(b)->getX();
  	double Y = mainMap->getObstacle(o)->getY() - balls->getBall(b)->getY();
  	double angle;
  	if (X == 0) {
    	angle = M_PI_2;
  	} else if (Y == 0) {
    	angle = 0;
  	} else {
    	angle = atan(Y / X);
  	}
  	double rayon = mainMap->getObstacle(o)->getHitbox()->getSide() / 2;
  	double squareRadius;
  	if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    	squareRadius = rayon;
  	} else if ((abs(angle) == M_PI / 4) || (abs(angle) == 2 * M_PI / 4)) {
    	squareRadius = sqrt(2) * rayon;
  	} else if ((2 * M_PI / 4 < angle) && (angle < M_PI)) {
   		squareRadius = (rayon / X) * d;
  	} else if ((-M_PI / 4 < angle) && (angle < M_PI / 4)) {
    	squareRadius = (rayon / X) * d;
  	} else if ((-2 * M_PI / 4 > angle) && (angle > -M_PI)) {
    	squareRadius = (rayon / X) * d;
  	} else {
    	squareRadius = (rayon / Y) * d;
  	}
  	if (d < (balls->getBall(b)->getRadius() + abs(squareRadius) + delta)) {
    	cout << COLL_BALL_OBSTACLE(b + 1) << endl;
    	error = true;
  	}
}

static void parseData(double &ingameMargin, double &parsingMargin, string inputData0, bool &error) {
  	int nbCell = stoi(inputData0);
  	mainMap = new Map(nbCell, nbCell);
  	players =new PlayerMap(nbCell, nbCell);
  	balls = new BallMap(nbCell, nbCell);
  	ingameMargin = COEF_MARGE_JEU * (SIDE / nbCell);
  	parsingMargin = ingameMargin / 2;
}

static void parsePlayer(int p, int nbCell, double parsingMargin, double playerRadius, double playerVelocity, string inputData0,
                      	string inputData1, string inputData2, string inputData3, bool &error) {
  	if (((abs(stod(inputData0)) > DIM_MAX) ||
       	(abs(stod(inputData1)  > DIM_MAX)))) {
    		cout << PLAYER_OUT(p + 1) << endl;
    		error = true;
  	} else {
    	players->addPlayer(stod(inputData0), stod(inputData1), stoi(inputData2), 
                    		stod(inputData3), playerRadius, playerVelocity, p);

    	vector<int> toCheck;
    	largeCollisionCheckPP(p, toCheck, nbCell);
    	for (auto c : toCheck){
      	collisionCheckPP(p, c, parsingMargin, error);
    	}
  	}
}

static void parseObstacle(int nbCell, int o, string inputData0, string inputData1, bool &error) {
  	if (stoi(inputData0) >= nbCell) {
    	cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    	error = true;
  	} else if (stoi(inputData1) >= nbCell) {
    	cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    	error = true;
  	} else if (stoi(inputData0) < 0) {
    	cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    	error = true;
  	} else if (stoi(inputData1) < 0) {
    	cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    	error = true;
  	} else if (mainMap->isObstacle(stoi(inputData0), stoi(inputData1))) {
    	cout << MULTI_OBSTACLE(stoi(inputData0), stoi(inputData1)) << endl;
    	error = true;
  	} else {
    	mainMap->addObstacle(stod(inputData0), stod(inputData1), o);
  	}
}

static void parseBall(int nbCell, int nbPlayer, int nbObstacle, double parsingMargin, int b, 
                      	double ballRadius, double ballVelocity, string inputData0, 
                      	string inputData1, string inputData2, bool &error) {
  	if (isOut(stod(inputData0), stod(inputData1))) {
    	cout << BALL_OUT(b + 1) << endl;
    	error = true;
  	} else {
    	balls->addBall(stod(inputData0), stod(inputData1), stod(inputData2), 
                        ballRadius, ballVelocity, b);
    	vector<int> toCheck;
    	largeCollisionCheckBB(b, toCheck, nbCell);
    	for (auto c : toCheck){
      		collisionCheckBB(b, c, parsingMargin, error);
    	}
    	for (int i = 0; i < nbPlayer; i++) {
      		collisionCheckPB(i, b, parsingMargin, error);
    	} 
  	}
}

int getPlayerNb(){return players->getNb();}
int getPlayerTimeTouched(int p){return players->getPlayer(p)->getTimeTouched();}
double getPlayerRadius(){return players->getPlayer(0)->getRadius();} //With one ?
int getPlayerCount(int p){return players->getPlayer(p)->getCount();}
Circle* getPlayerHitbox(int p){return players->getPlayer(p)->getHitbox();}

int getBallNb(){return balls->getNb();}
double getBallRadius(){return balls->getBall(0)->getRadius();} //With one ?
Circle* getBallHitbox(int b){return balls->getBall(b)->getHitbox();}

int getObstacleNb(){return mainMap->getNb();}
double getObstacleSize(){return mainMap->getObstacle(0)->getSide();}
Square* getObstacleHitbox(int o){return mainMap->getObstacle(o)->getHitbox();}
