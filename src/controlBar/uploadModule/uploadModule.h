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

#include "povImage.h"
#include "ofxSystemCall.h"

class uploadModule {
protected:
  povImage * image;
  ofSystemCall command;
	ofProgressSpinner spinner;
  ofFont label;
  //progRead prog;
  bool bRunning;
  ofTimer uploaded;
public:
  void setup();
  void upload(povImage * img);
  bool drawForeground();
  bool isUploading();
  void stopUpload();
  bool clickDown(int _x, int _y);
};