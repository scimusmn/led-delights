/*
 *  toolSelect.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/6/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "toolSelect.h"

extern ofColor white,black,gray,blue,red,orange,yellow;

palette::palette()
{
  pal.loadImage("images/palette2.jpg");
  w=pal.width, h=pal.height;
}

void palette::setup()
{
  
}

void palette::draw(int _x, int _y, int _w, int _h)
{
  resize(_w,_h);
  draw(_x,_y);
}

void palette::draw(int _x, int _y)
{
  x=_x, y=_y;
  pal.draw(_x, _y);
}

bool palette::clickDown(int _x, int _y)
{
  bool ret=0;
  if(over(_x, _y)){
    int pixPos=(_y-y)*3*pal.width+(_x-x)*3;
    unsigned char * pix=pal.getPixels();
    if(pix[pixPos]&&pix[pixPos+1]&&pix[pixPos+2]) currentColor=ofColor(pix[pixPos],pix[pixPos+1],pix[pixPos+2]), ret=1;
  }
  return ret;
}

bool palette::clickUp()
{
  
}

ofColor & palette::getColor()
{
  return currentColor;
}

void palette::resize(int _w, int _h)
{
  w=_w, h=_h;
  if(w!=pal.width||h!=pal.height) pal.resize(w, h);
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_ sizeButton//_-_-_-_-_//_-_-_-_-_

sizeBut::sizeBut(int radius)
{
  w=radius*2, h=w;
  bChosen=false;
}

void sizeBut::draw(int _x, int _y)
{
  x=_x, y=_y;
  ofColor k = ofGetStyle().color;
  ofCircle(x+w/2, y+h/2, w/2);
  ofPushStyle();
  ofNoFill();
  ofSetColor(white*.2);
  ofSetLineWidth(3);
  ofCircle(x+w/2, y+h/2, w/2);
  ofEnableSmoothing();
  if(bChosen){
    ofSetColor(yellow);
    ofCircle(x+w/2, y+h/2, w/2);
  }
  ofPopStyle();
  /*ofDisableSmoothing();
  ofSetLineWidth(1);
  ofFill();
  ofSetColor(k);*/
}

sizeChoose::sizeChoose()
{
  radius=0;
  selected=0;
  pad.x=pad.y=10;
}

void sizeChoose::setup(int hgt)
{
  double n=5;
  //int maxSz=30;
  //double maxSz=2*((hgt/(numSz+1.))-pad.y);
  h=pad.y;
  double m=(hgt-n*pad.y)/(2*(n-(n-1)/2));
  for (unsigned int i=0; i<n; i++) {
    sizes.push_back(sizeBut(m-(m/n)*i));
    h+=sizes.back().h;
  }
  if(h+pad.y*n>hgt) pad.y-=1;
  h=hgt;
  if(sizes.size()) w=sizes.front().w+pad.x*2;
}

void sizeChoose::draw(int _x, int _y)
{
  x=_x, y=_y;
  int yPos=y+pad.y;
  ofColor k=ofGetStyle().color;
  ofSetColor(white*.3);
  ofRectangle r(x,y,w,h);
  ofRect(r);
  //drawBorder(r);
  ofSetColor(white*.6);
  for (unsigned int i=0; i<sizes.size(); i++) {
    sizes[i].draw(x+(w-sizes[i].w)/2,yPos);
    yPos+=sizes[i].h+pad.y;
  }
}

bool sizeChoose::clickDown(int _x,int _y)
{
  bool ret=0;
  for (unsigned int i=0; i<sizes.size(); i++) {
    if(sizes[i].clickDown(_x,_y)){
      if(selected) selected->bChosen=false;
      select(i);
      ret=1;
    }
  }
  return ret;
}

void sizeChoose::select(int i)
{
  sizes[i].bChosen=true;
  selected=&sizes[i];
  radius=sizes[i].w/2;
}

bool sizeChoose::clickUp()
{
  for (unsigned int i=0; i<sizes.size(); i++) {
    sizes[i].clickUp();
  }
}

int sizeChoose::getDrawSize()
{
  return radius;
}

