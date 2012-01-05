/*
 *  uploadModule.h
 *  LED_Delights
 *
 *  Created by Exhibits on 12/5/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

#include "imageArea.h"
#include "ofxSystemCall.h"

class uploadModule {
protected:
  imageArea * image;
  ofSystemCall command;
	ofProgressSpinner spinner;
  ofFont label;
  //progRead prog;
  bool bRunning;
  ofTimer uploaded;
  ofButton button;
  
  ofPoint pad;
public:
  double x,y,w,h;
  void setup(imageArea * img);
  void upload();
  void draw(int _x, int _y);
  bool drawForeground();
  bool isUploading();
  void stopUpload();
  bool clickDown(int _x, int _y);
  bool clickUp();
};