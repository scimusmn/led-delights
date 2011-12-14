/*
 *  predefBar.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "predefBar.h"
#include "dallasEng.h"

extern ofColor white,black,gray,blue,red,orange,yellow;

predefBar::predefBar()
{
  pad.x=20;
}

void predefBar::setup(imageArea & img)
{
  image=&img;
  images.loadImages("upload/images/predefined");
  area.width=w=images.w+pad.x*2;
}

void predefBar::draw(int _x, int _y)
{
  if(image->mode==LED_PREDEF) setImageToChosen();
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
  
  int space = (h-(images.h));
  images.draw(x+(w-images.w)/2,y+space/2);
}

void predefBar::drawForeground()
{
  
}

bool predefBar::clickDown(int _x, int _y)
{
  if(images.clickDown(_x, _y)&&image->mode==LED_PREDEF){
    image->changeImage(images.selected());
  }
}

bool predefBar::clickUp()
{
  images.clickUp();
}

bool predefBar::setImageToChosen()
{
  image->changeImage(images.selected());
}