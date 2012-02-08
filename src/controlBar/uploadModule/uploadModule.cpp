/*
 *  uploadModule.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/5/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "uploadModule.h"
#include "dallasEng.h"

extern ofColor yellow,black,white;

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
  
  uploaded.set(3);
  uploaded.pause();
}

void uploadModule::upload()
{
  string dir="upload/images/";
  if(image->mode==LED_DRAW) image->currentImage().saveFile(dir);
	image->currentImage().writePlaylist(dir+"playlist");
	command.run("\""+ofToDataPath(dir+"../upload.sh\""));
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
    ofSetColor(black.opacity(.85));
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(white);
    spinner.draw(ofGetWidth()/2, ofGetHeight()/5*3, ofGetHeight()/3);
    ofSetColor(yellow);
    label.drawString("Uploading...", ofGetWidth()/2, ofGetHeight()/4);
  }
  if(uploaded.running()){
    ofSetColor(0, 0, 0,196);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    int wid=label.stringWidth("Turn the crank to view your image.");
    drawStyledBox((ofGetWidth()-wid)/2-50, ofGetHeight()/3-50, wid+100, ofGetHeight()/6+label.stringHeight("Kjhg")+50);
    ofSetColor(255, 255, 255);
    label.drawString("Done uploading:", ofGetWidth()/2, ofGetHeight()/3);
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
