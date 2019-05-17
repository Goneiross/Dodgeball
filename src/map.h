/*!
  \file   map.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  "Map" module header
*/

#ifndef MAP_H
#define MAP_H

class Map {
public:
  Map(int l, int c);
  virtual ~Map(){};
  int getLNb() const;
  int getCNb() const;
  // virtual int getNb() const {};
protected:
  int lineNumber, columnNumber;
};

#endif
