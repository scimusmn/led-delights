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
  redo.setup(64, 64, "images/redo.png");
	undo.setup(64,64,"images/undo.png");
  
  drawB.setup(64, OF_HOR, "images/edit.png");
	demo.setup(64,OF_HOR, "images/gears.png");
	predef.setup(64, OF_HOR, "images/predef.png");
  
  upload.setup("Send image to wheel", 25);
  toolBar::setup(OF_HOR);
  area.width=w=ofGetWidth();
  addSegment(20, &undo,&redo);
  addSegment(0, &upload);
  addSegment(30, &drawB, &demo, &predef);
}

void controlBar::draw(int _x, int _y)
{
  x=_x,y=_y;
  ofSetColor(gray);
  ofRect(area);
  ofSetColor(black);
  drawHatching(area.x, area.y, area.width, area.height, 50, 50);
  objHolder::draw(_x,_y);
}

void controlBar::drawForeground()
{
  
}

void controlBar::update()
{
  
}

void controlBar::setAvailableButtons()
{
  if(1);
}

bool controlBar::clickDown(int x, int y, int button)
{
  
}

bool controlBar::clickUp()
{
  
}

bool controlBar::mouseLockout(int button)
{
  
}
