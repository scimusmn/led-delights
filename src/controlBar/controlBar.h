/*
 *  controlBar.h
 *  LED_Delights
 *
 *  Created by Exhibits on 11/30/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

#include "imageArea.h"

#include "dallasEng.h"

class controlBar {
protected:
  dallasButton upload;
  ofButton undo;
  ofButton redo;
  imageArea * image;
public:
  double x,y,w,h;
  void setup();
  void draw(int x, int y);
  void drawForeground();
  void update();
  void setAvailableButtons();
  bool clickDown(int x, int y, int button=0);
  bool clickUp();
  bool mouseLockout(int button);
};