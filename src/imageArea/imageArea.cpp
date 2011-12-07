/*
 *  imageArea.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "imageArea.h"
#include "dallasEng.h"

extern bool bCloseShape;

extern ofColor black, white, gray, red, orange,yellow;

void circleSubHatch(int x, int y, int w, int h, int rad, int hatchSpace, int hatchWidth)
{
  bCloseShape=false;
  ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);
  ofBeginShape();
  drawHatching(0, 0, ofGetWidth(), ofGetHeight(), 15,1);
  ofNextContour();
  for (int i = 0; i < 80; i++){
    float anglef = ((float)i / 79.0f) * TWO_PI;
    float x_ = x+w/2 + rad * cos(anglef);
    float y_ = y+h/2 + rad * sin(anglef); 
    ofVertex(x_,y_);
  }
  ofEndShape();
  ofSetPolyMode(OF_POLY_WINDING_ODD);
  bCloseShape=true;
}

void circleSubRect(int x, int y, int w, int h,int rad){
  ofBeginShape();
  ofVertex(x, y);
  ofVertex(x+w, y);
  ofVertex(x+w, y+h);
  ofVertex(x, y+h);
  ofNextContour();
  for (int i = 0; i < 80; i++){
    float anglef = ((float)i / 79.0f) * TWO_PI;
    float x_ = x+w/2 + rad * cos(anglef);
    float y_ = y+h/2 + rad * sin(anglef); 
    ofVertex(x_,y_);
  }
  ofEndShape();
}

void imageArea::setup()
{
  image=0;
  bCapture=bDrawing=bFill=false;
  mousePointer.loadImage("pointer.png");
}

void imageArea::recaptureImage()
{
  bCapture=true;
}

void imageArea::changeImage(ofImage & img)
{
  image=&img;
}

void imageArea::changeDrawSize(double sz)
{
  drawSize=sz;
}

void imageArea::changeDrawColor(ofColor & cl)
{
  drawColor=cl;
}

void imageArea::captureScreen()
{
  if(image){
    //image->allocate(h, h, OF_IMAGE_COLOR);
    image->grabScreen(x+w/2-radius(), y+h/2-radius(),radius()*2,radius()*2);
    bCapture=false;
  }
}

void imageArea::drawTool()
{
  if(bDrawing) {
    int mouseX=ofGetAppPtr()->mouseX;
    int mouseY=ofGetAppPtr()->mouseY;
		ofSetColor(drawColor);
		ofCircle(mouseX, mouseY, drawSize);
		ofPoint curPoint(mouseX,mouseY);
		ofVector inv=(ofVector(curPoint-prevPoint).unit()).ortho()*drawSize;
		
    ofBeginShape();
		ofVertex(curPoint+inv);
		ofVertex(curPoint-inv);
		ofVertex(prevPoint-inv);
		ofVertex(prevPoint+inv);
		ofEndShape();
    
		prevPoint=curPoint;
    recaptureImage();
	}
}

void imageArea::fill()
{
  bFill=true;
}

void imageArea::draw(int _x, int _y, int _w, int _h)
{
  x=_x,y=_y, w=_w,h=_h;
  ofSetColor(white);
  if(image) image->draw(int(x+w/2-radius()), int(y+h/2-radius()),int(radius()*2),int(radius()*2));
  if(bDrawing) drawTool();
  if(bFill){
    ofSetColor(drawColor);
    ofRect(x, y, w, h);
    bFill=false;
    recaptureImage();
  }
  
  ofSetColor(gray);
  circleSubRect(x, y, w, h,radius());
  
  if(bCapture) captureScreen();
  
  ofSetColor(black);
  circleSubHatch(x,y,w,h,radius(),15,1);
  ofSetColor(yellow);
  ofRing(x+w/2, y+h/2, radius()-2, radius()+2);
}

void imageArea::drawForeground()
{
  int mouseX=ofGetAppPtr()->mouseX;
  int mouseY=ofGetAppPtr()->mouseY;
  if(!overCircle(mouseX, mouseY)||mode!=LED_DRAW){
    ofSetColor(255,255,255);
    mousePointer.draw(mouseX-5, mouseY-2);
  }
  else {
    ofSetColor(drawColor.inverse());
    ofRing(ofGetAppPtr()->mouseX, ofGetAppPtr()->mouseY, drawSize-3, drawSize);
  }
}

bool imageArea::clickDown(int _x, int _y)
{
  bool ret=0;
  if(overCircle(_x, _y)&&mode==LED_DRAW){
    bDrawing=ret=true;
    prevPoint=ofPoint(_x,_y);
  }
  return ret;
}

bool imageArea::clickUp()
{
  bDrawing=false;
  return false;
}

void imageArea::drag(int _x, int _y)
{
  
}

bool imageArea::overCircle(int _x, int _y)
{
  return (ofVector(imageCenter()-ofPoint(_x,_y)).mag()<radius());
}

double imageArea::radius()
{
  return h/2-20;
}

ofPoint imageArea::imageCenter()
{
  return ofPoint(x+w/2,y+h/2);
}
