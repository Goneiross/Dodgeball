/*!
  \file   floyd.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
  to check the size of each line (<=87 character): delete this line:
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <define.h>

using namespace std;

#define INFINITY_INIT(nbCell**2 + 1);
#define INFINITY_DIST(nbCell^2);

vector<size_t> targetting(){
	vector<vector<double> > dBP(nbPlayer); //dBP = distance between players

	for (int i=0; i < nbPlayer; i++) {dBP[i][i] = 0;}
	
	for (int i=0; i < nbPlayer; i++) {
		for (int j=0; j < i; j++) {
			dBP[i][j] = distance(Player start, Player target);
			dBP[j][i] = dBP[i][j];
		}
	}
	
	vector<double> minDistance(nbPlayer);
	vector<size_t> target(nbPlayer);
	
	for (int i=0; i < nbPlayer; i++) {
		if (i == (nbPlayer - 1)) {
			minDistance[i] = dBP[i][i-1];
			target[i] = i - 1;
		} else {
			minDistance[i] = dBP[i][i+1];
			target[i] = i + 1;
		}
		for (int j=0; j < nbPlayer; j++) {
			if (i != j) {
				if (dBP[i][j] < minDistance[i]) {
					minDistance[i] = dBP[i][j];
					target[i] = j;
				}
			}
		}
	}
	return target;
}

void whichPath() {
    /*ajouter les arguments: 2 Players et les tableaux nécessaires
    à isThereObstacle (obstacle et joueurs), simplePath (idem),
    complexPath, floyd, diagonalDist et shortestindirectPath*/
	if (isThereObstacle(Player start, Player target)) {
		complexPath(start, target);
	} else {simplePath(start, target);}
}

void simplePath() {
	//avec start et target des Players/positions
	double pathAngle = atan(distance(start.getL())
						/ distance(target.getL()));
	start.updatePosition(pathAngle);
}

void complexPath(){
	//avec start et target des Players/positions
	/* demande à Floyd de définir le chemin à suivre.
	 * Floyd rend la prochaine position
	 * complexPath appelle simplePath du Player start
	 * à la position target.*/
	double pathAngle = floyd();	//ajouter les arguments nécessaires à Floyd
	//pathAngle est l'angle du PREMIER morceau de chemin
	start.updatePosition(pathAngle);
}

double floyd() {
	//avec start et target des Players/positions
	/* ajouter un "pathAngle" de valeur de retour correspondant à 
	 * l'angle dans lequel start doit partir
	 * Plus tard, shortestindirectDist cherchera mieux
	 * IL FAUT REUSSIR A SPECIFIER QUE pathAngle = angle entre start et la première
	 * position à atteindre*/
	
	//initialisation d'un tableau de distances
	vector <vector<double> > tabCellDist (nbCell**2);
	for (int i(0); i < nbCell**2; i++) {
		for (int j(0); j < nbCell**2; j++) {
			tabCellDist[i][j] = INFINITY_INIT;
		}
/*		if (la case correspondant à la colonne i est un obstacle) {
			for (int j(0); j < nbCell**2; j++) {
			* 	tabCellDist[i][j] = INFINITY_DIST;
			* 	tabCellDist[j][i] = INFINITY_DIST;
		*	}
		* }
*/		
		tabCellDist[i][i] = 0;
		
		for (int j(0); j < nbCell**2; j++) {
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

//on essaie de trouver de meilleures distances
	for (int i(0); i < nbCell**2; i++) {			 
			nbFloyd = INFINITY_DIST;
			do {
				if (tabCellDist[i][j] = INFINITY_INIT) {
				shortestIndirectDist(i, j);
				}
				nbFloyd -= 1;
			} while (nbFloyd > 0);
	}
}

void shortestIndirectDist() {
	for (int k(0); k < nbCell**2; k++) {
//		if ((tabCellDist[i][k] < nbCell**2) && (tabCellDist[k][j] < nbCell**2)) {
				int tmpDist(distance(i, k) + distance(k, j));
				if (tmpDist < tabCellDiste[i][j]) {
					tabCellDist[i][j] = tmpDist;
					//pathAngle = l'angle entre i et k;
			}
//		}
	}
}

void diagonalDistance(unsigned int i, unsigned int j) {
	double iXPos = i%nbCell;
	double iYPos = i/nbCell;
	double jXPos = 
	/* 	avec 	1x le 	x de la coordonnée correspondant à la case 	i
	 * 			1y		y											i
	 * 			2x		x											j
	 * 			2y		y											j
	 * if (isObstacle(1x, 2y) xor isObstacle(2x, 1y)) {
	 * 	 		tabCellDist[i][j] = 2;
	 * 			tabCellDist[j][i] = 2;
	 * 	} else if (!(isObstacle(1x, 2y) || isObstacle(2x, 1y))) {
	 * 		tabCellDist[i][j] = sqrt(2);
	 * 		tabCellDist[j][i] = sqrt(2);
	 * 	}*/
}
