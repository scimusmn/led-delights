/*
 *  imageArea.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

#include "povImage.h"

enum ledControlMode {
  LED_HOME, LED_DEMO, LED_DRAW, LED_PREDEF
};

class imageArea {
protected:
  povImage * pImage;
  ofImage * image;
  bool bCapture;
  
  bool bDrawing;
  bool bFill;
  bool bChanged;
  
  ofColor drawColor;
  double drawSize;
  ofPoint prevPoint;
  
  ofImage mousePointer;
public:
  ledControlMode mode;
  double x,y,w,h;
  void setup();
  void recaptureImage();
  void captureScreen();
  void changeImage(povImage & img);
  void changeDrawSize(double sz);
  void changeDrawColor(ofColor & cl);
  
  void drawTool();
  void fill();
  
  void draw(int _x, int _y, int _w, int _h);
  void drawForeground();
  bool clickDown(int _x, int _y);
  bool clickUp();
  void drag(int _x, int _y);
  bool overCircle(int _x, int _y);
  
  double radius();
  ofPoint imageCenter();
  
  povImage & currentImage();
};
