/*
 *  drawToolBar.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/6/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "toolSelect.h"
#include "imageArea.h"

class drawToolBar : public ofInterObj {
protected:
  palette  pal;
  sizeChoose sz;
  dallasButton fill;
  
  imageArea * image;
  
  ofFont label;
  
  ofPoint pad;
  
  ofRectangle area;
  
  ofButton undo;
  ofButton redo;
  
  dallasButton home;
public:
  drawToolBar();
  void setup(imageArea & img);
  void draw(int x, int y);
  void drawForeground();
  bool clickDown(int _x, int _y);
  bool clickUp();
};