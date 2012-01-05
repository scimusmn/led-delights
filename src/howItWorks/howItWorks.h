/*
 *  howItWorks.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/8/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "dallasEng.h"
#include "imageArea.h"

class demonstration {
protected:
  bool bRunning;
  bool bUnfoldDone,bMoved,bRotated;
  ofImage	spiral;
	ofImage unfold;
  
  ofImage persist;
  vector<ofImage> segment;
	double count;
  int rotateCnt;
	ofTimer frame;
  
  ofPoint unfldPnt;
  
  ofSlider sld;
  ofPoint pad;
  
  ofFont label;
  
  dallasButton home;
  imageArea * image;
public:
  double x, y,w,h;
  ofRectangle side;
  demonstration();
  void setup(imageArea * img);
  void drawUnfold();
  void drawImageMove();
  void drawImageRotate();
  void drawSideBar();
  void draw(int _x, int _y, int _w, int _h);
  bool clickDown(int _x, int _y);
  bool clickUp();
  void drag(int _x, int _y);
  void start();
  void pause();
  void reset();
};