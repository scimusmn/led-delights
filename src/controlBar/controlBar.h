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

class controlBar : public toolBar {
protected:
  dallasButton upload;
  ofButton undo;
  ofButton redo;
  ofButton drawB,demo,predef;
  imageArea * image;
public:
  void setup(imageArea & img);
  void draw(int x, int y);
  void drawForeground();
  void update();
  void setAvailableButtons();
  bool clickDown(int x, int y, int button=0);
  bool clickUp();
  bool mouseLockout(int button);
};