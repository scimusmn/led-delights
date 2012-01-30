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
#include "uploadModule.h"
#include "howItWorks.h"

#include "dallasEng.h"

class controlBar : public toolBar {
protected:
  ofButton uploadBut;
  ofButton drawB,demo,predef;
  dallasButton home;
  imageArea * image;
  
  uploadModule upload;
  
  povImage drawImage;
  
  ofFont label;
  
  demonstration HIW;
  
  ofPoint pad;
public:
  void setup(imageArea & img);
  void draw(int x, int y);
  void drawForeground();
  void update();
  void setAvailableButtons();
  bool clickDown(int x, int y, int button=0);
  bool clickUp();
  void drag(int _x, int _y);
  bool mouseLockout(int button);
  ledControlMode getMode();
};