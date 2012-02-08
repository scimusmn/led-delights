/*
 *  controlBar.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 11/30/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "controlBar.h"

extern ofColor gray, black, white, red, orange, yellow, blue;

void controlBar::setup(imageArea & img)
{
  image=&img;
  
  image->mode=LED_HOME;
  
  label.loadFont("fonts/Din.ttf");
  label.setSize(24);
  label.setMode(OF_FONT_CENTER);
  label.setMode(OF_FONT_TOP);
  
  drawB.setup(256, OF_HOR, "images/BigPaint.png");
	demo.setup(256,OF_HOR, "images/bigHow.png");
	predef.setup(256, OF_HOR, "images/BigImage.png");
  
  home.setup("Return to Home", 25);
  
  drawImage.loadImage("upload/images/0.jpg");
  drawImage.bChose=true;
  
  upload.setup(image);
  HIW.setup(image);
  
  image->changeImage(drawImage);
  
  pad.x=pad.y=40;
}

void controlBar::draw(int _x, int _y)
{
  x=_x,y=_y;
  if(getMode()>=LED_DRAW) upload.draw(ofGetWidth()-upload.w, ofGetHeight()-upload.h);
}

void controlBar::drawForeground()
{
  int mouseX=ofGetAppPtr()->mouseX;
  int mouseY=ofGetAppPtr()->mouseY;
  if(getMode()==LED_HOME){
    //ofSetColor(black.opacity(.85));
    //ofRectangle r(x,y,ofGetWidth(),ofGetHeight());
    //ofRect(r);
    
    /*ofSetColor(black);
    drawHatching(r.x, r.y, r.width, r.height, 15, 1);*/
    predef.draw((ofGetWidth()/3-predef.w)/2, (ofGetHeight()-predef.h)/2);
    label.drawString("Choose image to upload", predef.x+predef.w/2, predef.y+predef.h+10);
    drawB.draw((ofGetWidth()-drawB.w)/2,(ofGetHeight()-drawB.h)/2);
    label.drawString("Draw image to upload", drawB.x+drawB.w/2, drawB.y+drawB.h+10);
    demo.draw((5*ofGetWidth()/3-demo.w)/2,(ofGetHeight()-demo.h)/2);
    label.drawString("See how it works", demo.x+demo.w/2, demo.y+demo.h+10);
    label.setMode(OF_FONT_LEFT);
    ofSetColor(yellow);
    label.drawString("Choose an option", x+pad.x, predef.y-label.stringHeight("Kj")-pad.y);
    label.setMode(OF_FONT_CENTER);
  }
  if(getMode()==LED_DEMO){
    HIW.draw(x, y+h+1, ofGetWidth(), ofGetHeight()-(y+h-1));
  }
  upload.drawForeground();
}

void controlBar::update()
{
  if (timeout.justExpired()) {
    image->mode=LED_HOME;
  }
}

void controlBar::setAvailableButtons()
{
}

bool controlBar::clickDown(int _x, int _y, int button)
{
  timeout.set(180);
  timeout.run();
  if(getMode()==LED_HOME){
    if(drawB.clickDown(_x, _y)) image->mode=LED_DRAW, image->changeImage(drawImage);
    else if(demo.clickDown(_x, _y)) image->mode=LED_DEMO, HIW.start();
    else if(predef.clickDown(_x, _y)) image->mode=LED_PREDEF;
  }
  if(getMode()==LED_DRAW||getMode()==LED_PREDEF) 
    if(upload.clickDown(_x, _y));
  
  if(getMode()==LED_DEMO) HIW.clickDown(_x, _y);
}

bool controlBar::clickUp()
{
  home.clickUp();
  drawB.clickUp();
  demo.clickUp();
  predef.clickUp();
  image->clickUp();
  
  HIW.clickUp();
  
  upload.clickUp();
  
  uploadBut.clickUp();
  
  setAvailableButtons();
}

void controlBar::drag(int _x, int _y)
{
  HIW.drag(_x, _y);
}

bool controlBar::mouseLockout(int button)
{
  bool ret=false;
  if (upload.isUploading()) {
    ret=true;
  }
  return ret;
}

ledControlMode controlBar::getMode()
{ 
  return image->mode;
}
