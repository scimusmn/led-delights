/*
 *  toolSelect.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/6/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "dallasEng.h"

class palette : public ofInterObj{
protected:
  ofColor currentColor;
  ofImage pal;
public:
  palette();
  void setup();
  void draw(int _x, int _y, int _w, int _h);
  void draw(int _x, int _y);
  bool clickDown(int _x, int _y);
  bool clickUp();
  void resize(int _w, int _h);
  ofColor & getColor();
};

class sizeBut : public ofButton {
protected:
  bool bChosen;
public:
  sizeBut(int radius);
  void draw(int _x, int _y);
  friend class sizeChoose;
};

class sizeChoose : public ofInterObj {
protected:
  int radius;
  sizeBut * selected;
  vector<sizeBut> sizes;
  ofPoint pad;
public:
  sizeChoose();
  void setup(int h);
  void draw(int _x, int _y);
  bool clickDown(int _x,int _y);
  bool clickUp();
  int getDrawSize();
  void select(int i);
};