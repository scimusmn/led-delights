/*
 *  howItWorks.cpp
 *  LED_Delights
 *
 *  Created by Exhibits on 12/8/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "howItWorks.h"

#define rotAng(x) ((rotateCnt+segAng*x)%360)

extern ofColor black, white, gray, yellow, red, blue,orange;

demoMode& operator++(demoMode& d, int)
{
  int temp=d;
  if(d==ROTATING) return d;
  return d = static_cast<demoMode> (++temp);
}

demoMode& operator--(demoMode& d, int)
{
  int temp=d;
  if(d==STPPD) return d;
  return d = static_cast<demoMode> (--temp);
}

animCell::animCell(ofPoint pnt,int w, int h)
{
  orig=pnt;
  cell.allocate(w, h, OF_IMAGE_COLOR);
  cell.grabScreen(orig.x, orig.y, w, h);
  frame.set(1);
  frame.pause();
}
void animCell::draw()
{
  pos=orig+(end-orig)*frame.getPercent();
  cell.draw(pos.x, pos.y);
}

void animCell::beginAnimation(ofPoint finish, float time)
{
  end=finish;
  frame.set(time);
  frame.run();
}

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
  count=0,rotateCnt=360;
  
  mode=STPPD;
  
  sld.setup(30, 30);
  
  pad.x=pad.y=40;
  
  label.loadFont("fonts/Din.ttf");
  label.setSize(17);
  label.setMode(OF_FONT_TOP);
  
  prev.setup(40, OF_VERT, "images/previous.png", "images/previous_active.png");
  next.setup(40, OF_VERT, "images/next.png", "images/next_active.png");
  
  home.setup("Return to Home", 25);
}

void demonstration::setup(imageArea * img)
{
  image=img;
  mode=STPPD;
  bMovedBeforeUnfld=false;
}

void demonstration::drawSelectImage()
{
  unfldPnt.x=x+w/2;
  unfldPnt.y=y+(h-spiral.height)/2;
  ofSetColor(white);
  spiral.draw(unfldPnt.x,unfldPnt.y);
  if(frame.justExpired()) bWait=true;
}

void demonstration::drawMagnetPosition()
{
  
}



void demonstration::drawUnfold()
{
  if(!bMovedBeforeUnfld){
    unfldPnt.x=x+w/2-frame.getPercent()*(M_PI*spiral.width/2);
    unfldPnt.y=y+(h-spiral.height)/2;
    ofSetColor(white);
    spiral.draw(unfldPnt.x,unfldPnt.y);
    if(frame.justExpired()) bMovedBeforeUnfld=true,frame.set(1/120.);
  }
  else{
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
    else if(count>=360) bWait=true;
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
}

void demonstration::drawImageMove()
{
  /*ofSetColor(255, 255, 255);
	if(frame.running()) unfold.draw(unfldPnt.x,unfldPnt.y+spiral.height*frame.getElapsedf());
  else if (bWait) unfold.draw(unfldPnt.x,unfldPnt.y+spiral.height);
  if(frame.justExpired()) bWait=true,frame.set(1/120.),frame.pause();*/
  
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
  
  if(!anim.size()){
    for (int i=0; i<unfold.width/10.; i++) {
      anim.push_back(animCell(ofPoint(unfldPnt.x+i*10,unfldPnt.y),10,unfold.height));
    }
  }
  
  if(frame.justExpired()) anim[animIndex].beginAnimation(ofPoint(anim[animIndex].orig.x,anim[animIndex].orig.y+spiral.height),1),animIndex++,frame.set(.1),frame.run();
  
  for (unsigned int i=0; i<anim.size(); i++) {
    anim[i].draw();
  }
}

ofColor col[16]={orange,red,blue,white,orange,red,blue,white,orange,red,blue,white,orange,red,blue,white};

double frmTm=1/10.;
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
  double perc=1.-sld.getPercent();
  if(perc<=.2) perc=.2;
  ofRect(unfldPnt.x, unfldPnt.y, unfold.width, 20);
  if(frame.justExpired()&&rotateCnt<3600&&bRunning){
    frame.set(perc*frmTm);
    rotateCnt-=(360/segment.size())/17.;
    if(rotateCnt<0){
      rotateCnt+=360;
      if(cnt<6){
        cnt++;
        sld.setPercent(sld.getPercent()+.16);
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
  
  ofSetColor(black);
  ofSetCircleResolution(90);
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2);
  if(firstTime) persist.grabScreen(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2), persist.width, persist.height),firstTime=false;
  ofSetColor(white);
  persist.draw(int(x+w/2-spiral.width*3/2.), int(y+h/3-spiral.width/2));
  ofSetColor(black.opacity(sqrt(perc)*1/10.));
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2+3);
  for (unsigned int i=0; i<segment.size(); i++) {
    ofPushMatrix();
    ofTranslate(x+w/2-spiral.width, y+h/3, 0);
    ofRotate(rotAng(i));
    ofSetColor(white);
    segment[i].draw(0,0,-segment[i].width,segment[i].height);
    ofPopMatrix();
  }
  ofSetCircleResolution(40);
  persist.grabScreen(int(x+w/2-spiral.width*3/2.), int(y+h/3-spiral.width/2), persist.width, persist.height);
  
  
  if(cnt>=6){
    bWait=true;
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
  side.x=_x;
  x=_x+side.width, side.y=y=_y, w=_w-side.width, side.height=h=_h;
  ofSetColor(gray);
  ofRect(x, y, w, h);
  ofSetColor(black);
  //drawHatching(x,y,w,h, 15,1);
  
  if(bRunning&&mode==SELECT) drawSelectImage();
	if(bRunning&&mode==UNFLD) drawUnfold();
  if(bRunning&&mode==MVING) drawImageMove();
  if(bRunning&&mode==ROTATING) drawImageRotate();
  
  drawSideBar();
}

void demonstration::drawSideBar()
{
  string text="Steps to project the image\non the wheel:\n\n";
  string add[6]={\
    "1. The user selects an\n  image to display\n   on the wheel.",\
    "2. The computer unrolls\n   the selected image into\n   an array of radial lines\n   of pixels (triplets of numbers\n   representing red, blue and\n   green values).",\
    "3. This array is sent\n   wirelessly to the\n   microcontroller on the wheel\n   which controls the LEDs",\
    "4. The microcontroller\n   receives an electrical\n   pulse everytime a line\n   of LEDs passes a magnet on\n   the frame. The microcontroller\n   uses this signal to\n   determine the position and\n   speed of the wheel.",\
    "5. Once the processor\n   knows the position and\n   speed of the wheel,\n   it can then use the\n   array of pixels stored\n   in memory to sequence\n   the flashing of the LEDs",\
    "6. Although the LEDs rapidly\n   blink on and off, your eye merges\n   the flashes into a solid image."};
    
  
    
  for (int i=0; i<6; i++) {
    side.width=max(label.stringWidth(text+add[i])+pad.x*2,side.width);
  }
  
  ofSetColor(gray);
  ofRect(side);
  ofRectangle r(side.x+side.width, side.y, 10, side.height);
  ofRect(r);
  
  ofSetColor(white*.15);
  drawHatching(side.x, side.y, side.width, side.height, 20, 20);
  //ofRect(titleArea);
  //drawBorder(titleArea);
  drawBorder(side);
  drawBorder(r);
  
  ofRectangle botBox(side.x,side.y+side.height-(home.h+pad.y*2),side.width,home.h+pad.y*2);
  ofSetColor(gray);
  ofRect(botBox);
  drawBorder(botBox);
  
  home.draw(botBox.x+(botBox.width-home.w)/2, botBox.y+(botBox.height-home.h)/2);
  
  ofSetColor(yellow);
  label.drawString(text+add[int(mode)-1], side.x+pad.x,side.y+(side.height-label.stringHeight(text+add[int(mode)-1])-botBox.height)/2);
  
  if(bWait&&mode>=UNFLD) prev.draw(side.x+40, side.y+side.height-botBox.height-prev.h-20);
  if(bWait&&mode<ROTATING) next.draw(side.x+side.width-next.w-40, side.y+side.height-botBox.height-next.h-20);
}

bool demonstration::clickDown(int _x, int _y)
{
  if(home.clickDown(_x, _y))
    image->mode=LED_HOME;
  bool change=false;
  if(bWait&&prev.clickDown(_x, _y)){
    bWait=false;
    change=true;
    mode--;
  }
  if(bWait&&next.clickDown(_x, _y)){
    change=true;
    bWait=false;
    mode++;
  }
  if(change){
    if(mode==MVING){
      animIndex=0;
      anim.clear();
      frame.set(1);
      unfldPnt.x=x+(w-M_PI*spiral.width)/2;
      unfldPnt.y=y+(h-spiral.height)/2;
    }
    if(mode==UNFLD){
      bMovedBeforeUnfld=false;
      frame.set(1.);
      unfold.allocate(1, spiral.height/2,OF_IMAGE_COLOR_ALPHA);
      cnt=count=0,rotateCnt=360;
    }
    if(mode==SELECT) frame.set(1);
    if(mode==ROTATING) frame.set(1/120.);
    frame.run();
  }
  return sld.clickDown(_x, _y);
}

bool demonstration::clickUp()
{
  home.clickUp();
  next.clickUp();
  prev.clickUp();
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
  mode=SELECT;
  bMovedBeforeUnfld=false;
  frame.set(1);
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
  mode=STPPD;
  segment.clear();
  for (int i=0; i<4; i++) {
    segment.push_back(ofImage());
    segment[i].allocate(10, spiral.height/2, OF_IMAGE_COLOR_ALPHA);
  }
  unfold.allocate(1, spiral.height/2,OF_IMAGE_COLOR_ALPHA);
}
