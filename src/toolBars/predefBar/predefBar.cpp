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
  pad.x=40,pad.y=30;
}

void predefBar::setup(imageArea & img)
{
  image=&img;
  images.loadImages("upload/images/predefined");
  area.width=w=images.w+pad.x*2;
  label.loadFont("fonts/Din.ttf");
  label.setSize(17);
  label.setMode(OF_FONT_TOP);
  
  home.setup("Return to Home", 25);
}

void predefBar::draw(int _x, int _y)
{
  if(image->mode==LED_PREDEF) setImageToChosen();
  y=_y, area.x=x=_x;
  h=ofGetHeight()-y;
  
  ofRectangle botBox(x,y+h-(home.h+pad.y*2),w,home.h+pad.y*2);
  
  string title="Select an image\nto send";
  ofRectangle titleArea(x,y,area.width,label.stringHeight(title)+pad.y*2);
  area.y=y+titleArea.height;
  area.height=h-titleArea.height-botBox.height;
  
  ofSetColor(gray);
  ofRect(area);
  ofRectangle r(x+w, y, 10, h);
  ofRect(r);
  
  ofSetColor(black);
  ofRect(titleArea);
  drawHatching(area.x, area.y, area.width, area.height, 70, 70);
  drawBorder(area);
  drawBorder(r);
  drawBorder(titleArea);
  
  ofSetColor(gray);
  ofRect(botBox);
  drawBorder(botBox);
  
  home.draw(botBox.x+(botBox.width-home.w)/2, botBox.y+(botBox.height-home.h)/2);
  
  ofSetColor(yellow);
  label.drawString(title, area.x+(area.width-label.stringWidth(title))/2, titleArea.y+pad.y);
  
  int space = (area.height-(images.h));
  images.draw(x+(w-images.w)/2,area.y+space/2);
}

void predefBar::drawForeground()
{
  
}

bool predefBar::clickDown(int _x, int _y)
{
  if(images.clickDown(_x, _y)&&image->mode==LED_PREDEF){
    image->changeImage(images.selected());
  }
  if(home.clickDown(_x, _y)){
    image->mode=LED_HOME;
  }
}

bool predefBar::clickUp()
{
  images.clickUp();
  home.clickUp();
}

bool predefBar::setImageToChosen()
{
  image->changeImage(images.selected());
}