/*
 *  uploadModule.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/5/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "uploadModule.h"

void uploadModule::setup(imageArea * img)
{
  image=img;
  label.loadFont("fonts/DinC.ttf");
  label.setSize(70);
  label.setMode(OF_FONT_CENTER);
  uploaded.set(0);
}

void uploadModule::upload()
{
  string dir="upload/images/";
  if(image->mode==LED_DRAW) image->currentImage().saveFile(dir);
	image->currentImage().writePlaylist(dir+"playlist");
	command.run("sh "+ofToDataPath(dir+"../upload.sh"));
}

bool uploadModule::drawForeground()
{
  if (isUploading()) {
    ofSetColor(0, 0, 0,196);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
    spinner.draw(ofGetWidth()/2, ofGetHeight()/5*3, ofGetHeight()/3);
    ofSetColor(255, 255, 255);
    label.drawString("Uploading...", ofGetWidth()/2, ofGetHeight()/4);
  }
  if(uploaded.running()){
    ofSetColor(0, 0, 0,196);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
    label.drawString("Done uploading:", ofGetWidth()/2, ofGetHeight()/4);
    label.drawString("Turn the crank to view your image.", ofGetWidth()/2, ofGetHeight()/2);
  }
}

bool uploadModule::isUploading()
{
  if(!command.isRunning()&&bRunning){
    //prog.stop();
    uploaded.set(3);
    uploaded.run();
  }
  bRunning=command.isRunning();
  return bRunning;
}

void uploadModule::stopUpload()
{
  
}

bool uploadModule::clickDown(int _x, int _y)
{
  
}
