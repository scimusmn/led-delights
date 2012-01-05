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
  pad.x=40,pad.y=30;
  //pal.resize(128, 128);
  //area.width=w=max(pal.w, sz.w)+pad.x*2;
  area.width=w=pal.w+sz.w+pad.x*3;
  label.loadFont("fonts/Din.ttf");
  label.setSize(17);
  label.setMode(OF_FONT_TOP);
}

void drawToolBar::setup(imageArea & img)
{
  image=&img;
  sz.setup(pal.h);
  area.width=w=pal.w+sz.w+pad.x*3;
  sz.select(0);
  image->changeDrawSize(sz.getDrawSize());
  fill.setup("Fill screen", 18);
  fill.w=pal.w+sz.w+pad.x;
  undo.setup(50,OF_HOR,"images/undo.png","images/undo_active.png");
  redo.setup(50,OF_HOR,"images/redo.png","images/redo_active.png");
  
  home.setup("Return to Home", 25);
}

void drawToolBar::draw(int _x, int _y)
{
  y=_y, area.x=x=_x;
  h=ofGetHeight()-y;
  
  string title="Select a color\nand a brush size";
  
  ofRectangle titleArea(x,y,area.width,label.stringHeight(title)+pad.y*2);
  ofRectangle selCol(pal.x, pal.y+pal.h+pad.y*1.5+label.stringHeight("Kjhg"),fill.w,fill.h);
  ofRectangle botBox(x,y+h-(home.h+pad.y*2),w,home.h+pad.y*2);
  
  ofSetColor(gray);
  ofRect(area);
  ofRectangle r(x+w, y, 10, h);
  ofRect(r);
  
  area.y=titleArea.y+titleArea.height;
  area.height=ofGetHeight()-area.y;
  
  ofSetColor(black);
  drawHatching(area.x, area.y, area.width, area.height, 20, 20);
  ofRect(titleArea);
  drawBorder(titleArea);
  drawBorder(area);
  drawBorder(r);
  
  ofSetColor(gray);
  ofRect(botBox);
  drawBorder(botBox);
  
  home.draw(botBox.x+(botBox.width-home.w)/2, botBox.y+(botBox.height-home.h)/2);
  
  //********* draw undo and redo
  undo.draw(x+4*w/3-undo.w/2, ofGetHeight()-undo.h-label.stringHeight("Kjhg")-pad.y);
  redo.draw(undo.x+botBox.width/3,undo.y);
  ofSetColor(yellow);
  label.setMode(OF_FONT_CENTER);
  label.drawString("Undo", undo.x+undo.w/2,undo.y+undo.h+pad.y/2);
  label.drawString("Redo", redo.x+redo.w/2,redo.y+redo.h+pad.y/2);
  label.setMode(OF_FONT_LEFT);
  
  ofSetColor(yellow);
  label.drawString(title, area.x+(area.width-label.stringWidth(title))/2, titleArea.y+pad.y);
  
  ofSetColor(white);
  pal.draw(x+pad.x, area.y+pad.y*1.5+label.stringHeight("Kjhg"));
  
  fill.draw(x+(w-fill.w)/2, selCol.y+selCol.height+pad.y);
  
  ofSetColor(pal.getColor());
  ofRect(selCol);
  ofSetColor(yellow);
  ofNoFill();
  ofRect(selCol);
  ofFill();
  label.drawString("Selected color", selCol.x,selCol.y-label.stringHeight("Color")-pad.y/2);
  
  ofSetColor(pal.getColor());
  sz.draw(x+area.width-(sz.w+pad.x),pal.y);
  
  ofSetColor(yellow);
  label.drawString("Color", pal.x,pal.y-label.stringHeight("Color")-pad.y/2);
  label.drawString("Brush size", sz.x,sz.y-label.stringHeight("Color")-pad.y/2);
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
  if(image->currentImage().undoAvailable()&&undo.clickDown(_x, _y)){
    image->currentImage().undo();
  }
  if(image->currentImage().redoAvailable()&&redo.clickDown(_x, _y)){
    image->currentImage().redo();
  }
  if(fill.clickDown(_x, _y)){
    image->fill();
  }
  if(home.clickDown(_x,_y)){
    image->mode=LED_HOME;
  }
}

bool drawToolBar::clickUp()
{
  fill.clickUp();
  sz.clickUp();
  pal.clickUp();
  undo.clickUp();
  redo.clickUp();
  
  home.clickUp();
}
