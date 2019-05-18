/*!
  \file   map.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  "Map" module implementation
*/

#include "map.h"

using namespace std;

Map::Map(int l, int c) {
    columnNumber = c;
    lineNumber = l;
}

int Map::getLNb() const { return lineNumber; }
int Map::getCNb() const { return columnNumber; }
