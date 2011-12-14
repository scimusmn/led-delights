/*
 *  howItWorks.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/8/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "howItWorks.h"
#include "dallasEng.h"

#define rotAng(x) ((rotateCnt+segAng*x)%360)

extern ofColor black, white, gray, yellow, red, blue,orange;

demonstration::demonstration()
{
  spiral.loadImage("images/smile.png");
	spiral.setAnchorPercent(.50, .5);
	frame.set(1/120.);
  frame.pause();
	unfold.allocate(1, spiral.height/2,OF_IMAGE_COLOR_ALPHA);
  persist.allocate(spiral.width, spiral.height,OF_IMAGE_COLOR_ALPHA);
  for (int i=0; i<4; i++) {
    segment.push_back(ofImage());
    segment[i].allocate(20, spiral.height/2, OF_IMAGE_COLOR_ALPHA);
  }
  bRunning=bUnfoldDone=bMoved=false;
  count=0,rotateCnt=360;
  
  sld.setup(30, 30);
}

void demonstration::drawUnfold()
{
  unfldPnt.x=x+(w-M_PI*spiral.width)/2;
  unfldPnt.y=y+(h-spiral.height)/2;
  ofSetColor(255, 255, 255);
	ofPushMatrix();
	ofTranslate(unfldPnt.x+spiral.width*count/(360/M_PI), unfldPnt.y, 0); //spiral.width/2+spiral.width*count/(360/M_PI), spiral.width/2
	ofRotate(count);
	spiral.draw(0,0);
  ofPopMatrix();
  ofSetColor(255, 255, 255);
	unfold.draw(int(unfldPnt.x), int(unfldPnt.y));
  if(frame.justExpired()&&count<360&&bRunning){
		frame.set(1/120.);
		count++;
		unfold.resize(spiral.width*count/(360/M_PI), spiral.height/2);
		unfold.grabScreen(int(unfldPnt.x), int(unfldPnt.y), spiral.width*count/(360/M_PI), spiral.width/2);
	}
  else if(count>=360) bUnfoldDone=true,frame.set(1);
  ofPushMatrix();
	ofTranslate(unfldPnt.x+spiral.width*count/(360/M_PI), unfldPnt.y, 0);
	ofRotate(count);
	ofSetColor(gray);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(0,0);
	for ( int i = 90; i>(90-360*count/360.); i--){
		glVertex2d(0 + spiral.width/1.9*cos((i)*2*M_PI/(360)), 0 + spiral.width/1.9*sin(i*2*M_PI/(360)));
	}
	glEnd();
  ofPopMatrix();
  ofSetColor(255, 255, 255);
	unfold.draw(unfldPnt.x, unfldPnt.y);
}

void demonstration::drawImageMove()
{
  ofSetColor(255, 255, 255);
	unfold.draw(unfldPnt.x,unfldPnt.y+spiral.height*frame.getElapsedf());
  if(frame.justExpired()) bMoved=true,frame.set(1/120.);
}

ofColor col[16]={orange,red,blue,white,orange,red,blue,white,orange,red,blue,white,orange,red,blue,white};

double frmTm=1/20.;
int cnt=0;
bool firstTime=true;

void demonstration::drawImageRotate()
{
  unfldPnt.x=x+(w-M_PI*spiral.width)/2;
  unfldPnt.y=y+(h+spiral.height)/2;
  
  int segAng=360/segment.size();
  
  ofSetColor(white);
  unfold.draw(unfldPnt.x, unfldPnt.y);
  ofSetColor(black);
  ofSetLineWidth(1);
  for (int i=0; i<unfold.height/10.; i++) {
    ofLine(unfldPnt.x, unfldPnt.y+i*10, unfldPnt.x+unfold.width, unfldPnt.y+i*10);
  }
  for (int i=0; i<unfold.width/10.; i++) {
    ofLine(unfldPnt.x+i*10, unfldPnt.y, unfldPnt.x+i*10, unfldPnt.y+unfold.height);
  }
  double perc=1-sld.getPercent();
  if(perc<.03) perc=.03;
  ofRect(unfldPnt.x, unfldPnt.y, unfold.width, 20);
  if(frame.justExpired()&&rotateCnt<3600&&bRunning){
    frame.set(perc*1/20.);
    rotateCnt-=(360/segment.size())/17.;
    //rotateCnt=(float(ofGetElapsedTimeMillis()%500)/500.)*360;
    if(rotateCnt<0){
      rotateCnt+=360;
      if(cnt<6){
        cnt++;
        //segment.push_back(ofImage());
        //segment.back().allocate(20, spiral.height/2, OF_IMAGE_COLOR_ALPHA);
        //frmTm/=1.75;
        sld.setPercent(sld.getPercent()+.16);
        //segAng=360/segment.size();
      }
    }
    for (int i=0; i<segment.size(); i++) {
      segment[i].grabScreen(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y, segment[i].width, segment[i].height);
    }
  }
  
  ofNoFill();
  ofSetLineWidth(2);
  for (int i=0; i<segment.size(); i++) {
    ofSetColor(col[i]);
    ofRect(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y, segment[i].width, segment[i].height);
  }
  ofSetLineWidth(1);
  
  ofFill();
  
  
  //ofSetColor(black);
//  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2);
//  for (unsigned int i=0; i<segment.size(); i++) {
//    ofPushMatrix();
//    ofTranslate(x+w/2-spiral.width, y+h/3, 0);
//    ofRotate(rotAng(i));
//    ofSetColor(white);
//    segment[i].draw(0,0,-segment[i].width,segment[i].height);
//    ofSetColor(col[i]);
//    ofCircle(0, 0+segment[i].height, 3);
//    ofPopMatrix();
//  }
  
  ofSetColor(black);
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2);
  if(firstTime) persist.grabScreen(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2), persist.width, persist.height),firstTime=false;
  ofSetColor(white);
  persist.draw(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2));
  ofSetColor(black.opacity(perc*1/12.));
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2+3);
  for (unsigned int i=0; i<segment.size(); i++) {
    ofPushMatrix();
    ofTranslate(x+w/2-spiral.width, y+h/3, 0);
    ofRotate(rotAng(i));
    ofSetColor(white);
    segment[i].draw(0,0,-segment[i].width,segment[i].height);
    ofPopMatrix();
  }
  persist.grabScreen(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2), persist.width, persist.height);
  
  
  if(cnt>=6){
    ofSetColor(white);
    spiral.draw(x+w/2+spiral.width, y+h/3);
  }
  
  if(sld.getPercent()<.6){
    for (unsigned int i=0; i<segment.size(); i++) {
      ofVector vecEnd=ofVector(0, segment[i].height).rotate(rotAng(i));
      
      ofSetColor(col[i]);
      ofSetLineWidth(4);
      ofLine(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y,(x+w/2-spiral.width)+ vecEnd.x, y+h/3+vecEnd.y);
      ofSetLineWidth(1);
    }
  }
  
  ofSetColor(white*.6);
  sld.draw(x+(w-sld.w)/2,y+50,200,10);
}

void demonstration::draw(int _x, int _y, int _w, int _h)
{
  x=_x, y=_y, w=_w, h=_h;
  ofSetColor(gray);
  ofRect(x, y, w, h);
  ofSetColor(black);
  drawHatching(x,y,w,h, 15,1);
  
	if(bRunning&&!bUnfoldDone) drawUnfold();
  if(bRunning&&bUnfoldDone&&!bMoved) drawImageMove();
  if(bRunning&&bMoved) drawImageRotate();
}

bool demonstration::clickDown(int _x, int _y)
{
  return sld.clickDown(_x, _y);
}

bool demonstration::clickUp()
{
  return sld.clickUp();
}

void demonstration::drag(int _x, int _y)
{
  sld.drag(_x, _y);
}

void demonstration::start()
{
  reset();
  bRunning=true;
  frame.set(1/120.);
  frame.run();
}

void demonstration::pause()
{
  bRunning=false;
}

void demonstration::reset()
{
  cnt=count=0,rotateCnt=360;
  frmTm=1/20.;
  bUnfoldDone=bMoved=false;
  segment.clear();
  for (int i=0; i<4; i++) {
    segment.push_back(ofImage());
    segment[i].allocate(10, spiral.height/2, OF_IMAGE_COLOR_ALPHA);
  }
  unfold.allocate(1, spiral.height/2,OF_IMAGE_COLOR_ALPHA);
}
