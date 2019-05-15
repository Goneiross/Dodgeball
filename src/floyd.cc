/*!
  \file   floyd.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
*/

#include <iostream>
#include <vector>
#include <cmath>
#include "define.h"
#include "tools.h"
#include "player.h"
#include "obstacle.h"
#include "ball.h"


using namespace std;

vector<int> targetting(PlayerMap* players){
	vector<vector<double> > dBP(nbPlayer); //dBP = distance between players

	for (int i=0; i < nbPlayer; i++) {dBP[i][i] = 0;}
	
	for (int i=0; i < nbPlayer; i++) {
		for (int j=0; j < i; j++) {
			dBP[i][j] = distance(players[i], players[0]);
			dBP[j][i] = dBP[i][j];¨
		}
	}
	
	vector<double> minDistance(nbPlayer);
	vector<int> target(nbPlayer);
	
	for (int i=0; i < nbPlayer; i++) {
		minDistance[i] = INFINITY_DIST;

		for (int j=0; j < nbPlayer; j++) {
			if (i != j) {
				if (dBP[i][j] < minDistance[i]) {
					minDistance[i] = dBP[i][j];
					target[i] = j;
				}
			}
		}
	}
	return target; //rend donc l'indice correspondant à players
}

bool isThereObstacleBetween(int l1, int c1, int l2, int c2){
	if (l1 == l2) {									// Case 1 : same line
		if (c1 < c2){
			for (int c = c1; c <= c2; c++){ if (mainMap->isObstacle(l2, c)) { return true; } }
			return false;
		} else {
			for (int c = c2; c <= c1; c++){ if (mainMap->isObstacle(l2, c)) { return true; } }
			return false;
		}
	} else if (c1 == c2) {					// Case 2 : same column
		if (l1 < l2){
			for (int l = l1; l <= l2; l++){ if (mainMap->isObstacle(l, c2)) { return true; } }
			return false;
		} else {
			for (int l = l2; l <= l1; l++){ if (mainMap->isObstacle(l, c2)) { return true; } }
			return false;
		}
	} else {												// Case 3 : different line and column
		enum direction {line = 0, column = 1};
		bool direction = line;
		int l = l1, c = c1; 
		while ((l != l2) && (c != c2)){
			if (direction == line){
				if (l < l2){ l ++; } 
				else if (l > l2){ l--; }
				else {
					direction = column;
					continue;
				}
				if (mainMap->isObstacle(l, c)) { return true; }
				direction = column;
			} else {
				if (c < c2){ l ++; } 
				else if (c > c2){ l--; } 
				else {	
					direction = line;
					continue;
				}
				if (mainMap->isObstacle(l, c)) { return true; }
				direction = line;
			}
		}
	}
}

void whichPath(Player* start, Player* target) {
// ajouter en argument tout ce qui serait nécessaire à complexPath, floyd, diagonalDist, shortestindirectDist
	if (isThereObstacleBetween(start->getL(), start->getC(), target->getL(), target->getC())) {
		complexPath(start, target);
	} else {simplePath(start, target);}
}

void simplePath(Player* start, Player* target) {
	return angle(start->getL(), start->getC(), target->getL(), target->getC());
}


void complexPath(Player* start, Player* target){
	//ajouter les arguments de floyd, diagonalDist, shortestIndirectDist
	/* Description of Floyd's algorithm's implementation */
	floyd(start, target);	//ajouter les arguments nécessaires à floyd, diagonalDist, shortestIndirectDist
}

double floyd(Player* start, Player* target) {
	//avec start et target des Players/positions
	/* "pathAngle" = angle *de départ*, shortestindirectDist cherchera mieux
	 * Faut-il que tabCellDist soit static ou pas ?
	 * S'il l'est, ne faudrait-il pas faire un tabPathAngle plutôt qu'un pathAngle unique ?*/

	//initialisation d'un tableau de distances
	vector <vector<double> > tabCellDist (pow(nbCell, 2));
	for (int i(0); i < pow(nbCell, 2); i++) {
		for (int j(0); j < pow(nbCell, 2); j++) {
			tabCellDist[i][j] = INFINITY_INIT;
		}
		if (Obstacles->isObstacle(i%nbCell, i/nbCell)) {
			for (int j(0); j < pow(nbCell, 2); j++) {
			 	tabCellDist[i][j] = INFINITY_DIST;
				tabCellDist[j][i] = INFINITY_DIST;
			}
		}
		
		tabCellDist[i][i] = 0;

		for (int j(0); j < pow(nbCell, 2); j++) {
			if (((j = i + 1) || (j = i - 1))	//si les cases sont adjacentes
					&& !(((j%nbCell = 0) && ((i + 1)%nbCell = 0))
					|| ((i%nbCell = 0) && (j + 1)%nbCell = 0))) {
				tabCellDist[i][j] = 1;
				tabCellDist[j][i] = 1;
			}			
		}
	}

	for (int k(0); k < nbCell; k++) {
		for (int i(0); i < nbCell - 1; i++) {	//déterminer les distances diagonales
			diagonalDist(k*nbCell, (k + 1)*nbCell - 2);
		}
		for (int i(1); i < nbCell; i++) {
			diagonalDist(k*nbCell + 1, (k + 1)*nbCell - 1);
		}
}

	while (areThereUninitialisedCases(tabCellDist)) {
		for (int i(0); i < pow(nbCell, 2); i++) {
			for (int j(0); j < i; j++) {
				shortestIndirectDist(i, j);
			}
		}

	return pathAngle;
}

bool areThereUninitialisedCases (vector <vector<double> > tab2D) {
	//considers an inferior triangular matrix, with a null-diagonal
	for (int i(0); i < tab2D.size(); i++) {
		for (int j(0); j < i; j++) {
			if (tab2D[i][j] == INFINITY_INIT) { return true;}
		}
	}
	return false;
}

void shortestIndirectDist(Player start, Player target) {
	for (int k(0); k < pow(nbCell, 2); k++) {
		if ((tabCellDist[i][k] < pow(nbCell, 2)) && (tabCellDist[k][j] < pow(nbCell, 2))) {
				int tmpDist(distance(i, k) + distance(k, j));
				if (tmpDist < tabCellDiste[i][j]) {
					tabCellDist[i][j] = tmpDist;
					pathAngle = angle(i/nbCell, i%nbCell, k/nbCell, k%nbCell);
			}
		}
	}
	return pathAngle;
}

void diagonalDistance(unsigned int i, unsigned int j) {
	double iXPos = i/nbCell;
	double iYPos = i%nbCell;
	double jXPos = j/nbCell;
	double jYPos = j%nbCell;
	//si un Obstacle en diagonale
  if (isObstacle(iXPos, jYPos) xor isObstacle(jXPos, iYPos)) {
		tabCellDist[i][j] = 2;
		tabCellDist[j][i] = 2;
	//si aucun Obstacle en diagonale
	} else if (!(isObstacle(iXPos, jYPos)) && !(isObstacle(jXPos, iYPos))) {
		tabCellDist[i][j] = sqrt(2);
		tabCellDist[j][i] = sqrt(2);
	} //si 2 Obstacles, il faut chercher mieux, donc on laisse à INFINITY_INIT
}
