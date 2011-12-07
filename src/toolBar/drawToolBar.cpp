/*
 *  drawToolBar.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/6/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "drawToolBar.h"

extern ofColor white,black,gray,blue,red,orange,yellow;

drawToolBar::drawToolBar()
{
  pad.x=20;
  pal.resize(128, 128);
  area.width=w=max(pal.w, sz.w)+pad.x*2;
}

void drawToolBar::setup(imageArea & img)
{
  image=&img;
  sz.select(0);
  image->changeDrawSize(sz.getDrawSize());
  fill.setup("Fill screen", 20);
}

void drawToolBar::draw(int _x, int _y)
{
  area.y=y=_y, area.x=x=_x;
  area.height=h=ofGetHeight()-y;
  
  ofSetColor(gray);
  ofRect(area);
  ofRectangle r(x+w, y, 10, h);
  ofRect(r);
  
  ofSetColor(black);
  drawHatching(area.x, area.y, area.width, area.height, 20, 20);
  drawBorder(area);
  drawBorder(r);
  
  int space = (h-(pal.h+sz.h+fill.h))/3;
  ofSetColor(white);
  pal.draw(x+(w-pal.w)/2, y+space/2);
  
  fill.draw(x+(w-fill.w)/2, pal.y+pal.h+space);
  
  ofSetColor(pal.getColor());
  sz.draw(x+(w-sz.w)/2,fill.y+fill.h+space);
}

void drawToolBar::drawForeground()
{
  
}

bool drawToolBar::clickDown(int _x, int _y)
{
  if(pal.clickDown(_x, _y)){
    image->changeDrawColor(pal.getColor());
  }
  if (sz.clickDown(_x, _y)) {
    image->changeDrawSize(sz.getDrawSize());
  }
  if(fill.clickDown(_x, _y)){
    image->fill();
  }
}

bool drawToolBar::clickUp()
{
  fill.clickUp();
  sz.clickUp();
  pal.clickUp();
}
