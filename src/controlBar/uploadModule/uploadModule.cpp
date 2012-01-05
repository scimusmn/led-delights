/*
 *  uploadModule.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/5/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "uploadModule.h"

extern ofColor yellow;

void uploadModule::setup(imageArea * img)
{
  image=img;
  label.loadFont("fonts/Din.ttf");
  label.setMode(OF_FONT_CENTER);
  label.setMode(OF_FONT_TOP);
  uploaded.set(0);
  
  button.setup(128,OF_HOR,"images/sendToWheel.png","images/sendToWheel_active.png");
  
  pad.y=20;
  pad.x=40;
  
  label.setSize(22);
  w=max(button.w,double(label.stringWidth("Send image to wheel")))+pad.x;
  h=button.h+label.stringHeight("Kjhg")+pad.y*2;
  
  label.setSize(70);
}

void uploadModule::upload()
{
  string dir="upload/images/";
  if(image->mode==LED_DRAW) image->currentImage().saveFile(dir);
	image->currentImage().writePlaylist(dir+"playlist");
	command.run("sh "+ofToDataPath(dir+"../upload.sh"));
}

void uploadModule::draw(int _x, int _y)
{
  string title="Send image to wheel";
  
  label.setSize(22);
  button.draw(_x+(w-button.w)/2, _y);
  ofSetColor(yellow);
  label.drawString(title, _x+w/2, button.y+button.h+pad.y);
  label.setSize(70);
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
  if(button.clickDown(_x, _y)){
    upload();
  }
}

bool uploadModule::clickUp()
{
  button.clickUp();
}
