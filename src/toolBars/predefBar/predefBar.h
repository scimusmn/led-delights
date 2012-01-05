/*
 *  predefBar.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "imageArea.h"
#include "povImage.h"
#include "dallasEng.h"

class predefBar : public ofInterObj {
protected:
  imageArea * image;
  povImages images;
  
  ofPoint pad;
  
  ofRectangle area;
  
  ofFont label;
  
  dallasButton home;
public:
  predefBar();
  void setup(imageArea & img);
  void draw(int x, int y);
  void drawForeground();
  bool clickDown(int _x, int _y);
  bool clickUp();
  bool setImageToChosen();
};