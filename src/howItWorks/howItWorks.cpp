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
  if(d==SPINNING) return d;
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
  cell.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
  cell.grabScreen(orig.x, orig.y, w, h);
  frame.set(1);
  frame.pause();
}

void animCell::draw()
{
  pos=orig+(end-orig)*frame.getPercent()-sin(frame.getPercent()*M_PI)*ofPoint(100,0);
  ofSetColor(white);
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
  blade.loadImage("images/blade.png");
}

void demonstration::setup(imageArea * img)
{
  image=img;
  mode=STPPD;
  bMovedBeforeUnfld=false;
}

void header(string hdr, ofFont & label, int x, int y, int w, int h)
{
  ofSetLineWidth(3);
  ofSetColor(yellow);
  ofLine(x, y, x+w, y);
  
  label.setSize(24);
  label.drawString(hdr, x+20, y+10);
  label.setSize(17);
  ofSetLineWidth(1);
  
  ofLine(x, y+20+label.stringHeight(hdr), x+w, y+20+label.stringHeight(hdr));
}

void demonstration::drawSelectImage()
{
  header("On computer", label, x, y, w, h);
  
  unfldPnt.x=x+w/2;
  unfldPnt.y=y+(h-spiral.height)/2;
  ofSetColor(white);
  spiral.draw(unfldPnt.x,unfldPnt.y);
  if(frame.justExpired()) bWait=true;
}


void demonstration::drawUnfold()
{
  header("On computer", label, x, y, w, h);
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
  if(anim.size()==0){
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
    
    cout << "grabbing screen\n";
    for (int i=0; i<unfold.width/10.; i++) {
      anim.push_back(animCell(ofPoint(unfldPnt.x+i*10,unfldPnt.y),10,unfold.height));
    }
  }
  
  header("On computer", label, x, y, w, h);
  header("On microcontroller on wheel", label, x, unfldPnt.y+unfold.height+80, w, h);
  
  
  if(animIndex<anim.size()&&frame.justExpired()){
    anim[animIndex].beginAnimation(ofPoint(anim[animIndex].orig.x,anim[animIndex].orig.y+spiral.height),1);
    animIndex++;
    cout << animIndex << " is the current frame out of " << anim.size() << endl; 
    frame.set(.025),frame.run();
  }
  
  if(animIndex>=anim.size()&&frame.justExpired()) bWait=true,cout << "waiting\n";
  
  for (unsigned int i=0; i<anim.size(); i++) {
    anim[i].draw();
  }
}

ofColor red2(255,0,0);
ofColor green2(0,255,0);
ofColor blue2(0,0,255);
ofColor yellow2(255,255,0);

ofColor col[4]={red2,green2,blue2,yellow2};

double frmTm=1/10.;
int cnt=0;
int numSpins=4;
bool firstTime=true;

void demonstration::drawWheelFrame(int _x, int _y)
{
  int segAng=360/segment.size();
  
  ofSetColor(black);
  ofRect(_x-12, _y-2, 24,segment[0].height+20+4);
  ofSetColor(white*.3);
  ofRect(_x-10, _y, 20,segment[0].height+20);
  
  ofSetColor(black);
  ofRect(_x-6, y+h/3+segment[0].height*.875, 12,12);
  ofRing(_x, _y, segment[0].height, segment[0].height+5);
  ofCircle(_x, _y, 15);
  
  for (unsigned int i=0; i<8; i++) {
    ofPushMatrix();
    ofTranslate(_x, _y, 0);
    ofRotate((rotateCnt+(360/8)*i)%360);
    ofSetColor(black);
    ofLine(15, 0, 15, segment[0].height);
    ofLine(-15, 0, -15, segment[0].height);
    ofRotate(360/16);
    ofLine(15, 0, 15, segment[0].height);
    ofLine(-15, 0, -15, segment[0].height);
    ofPopMatrix();
  }
  
  for (unsigned int i=0; i<segment.size(); i++) {
    ofPushMatrix();
    ofTranslate(x+w/2-spiral.width, y+h/3, 0);
    ofRotate(rotAng(i));
    ofSetColor(white);
    float hPerc=(spiral.height/2-20.)/blade.height;
    blade.draw(-(spiral.height/2-20)*.1, (spiral.height/2-20)*.075, blade.width*hPerc,spiral.height/2-20);
    for(unsigned int j=0; j<(spiral.height/2-40)/10; j++){
      if(j>(spiral.height/2-40)/10-4&&(rotAng(i)<20||rotAng(i)>340)) ofSetColor(col[i%4]);
      else ofSetColor(black);
      ofRect(-4, 20+(spiral.height/2-20)/13*j, 8, 8);
    }
    ofPopMatrix();
  }
}

void demonstration::drawMagnetPosition()
{
  unfldPnt.x=x+(w-M_PI*spiral.width)/2;
  unfldPnt.y=y+(h+spiral.height)/2;
  
  int segAng=360/segment.size();
  
  header("Image displayed on wheel", label, x, y, w, h);
  header("On microcontroller on wheel", label, x, unfldPnt.y-spiral.height+unfold.height+80, w, h);
  
  ofSetColor(white);
  unfold.draw(unfldPnt.x, unfldPnt.y);
  ofSetColor(white*.2);
  ofSetLineWidth(1);
  for (int i=0; i<unfold.height/10.; i++) {
    ofLine(unfldPnt.x, unfldPnt.y+i*10, unfldPnt.x+unfold.width, unfldPnt.y+i*10);
  }
  for (int i=0; i<unfold.width/10.; i++) {
    ofLine(unfldPnt.x+i*10, unfldPnt.y, unfldPnt.x+i*10, unfldPnt.y+unfold.height);
  }
  ofRect(unfldPnt.x, unfldPnt.y, unfold.width, 20);
  
  if(frame.justExpired()&&rotateCnt<3600&&bRunning){
    frame.set(frmTm);
    rotateCnt-=(360/segment.size())/17.;
    if(rotateCnt<0){
      rotateCnt+=360;
    }
  }
  
  
  drawWheelFrame(x+w/2-spiral.width,y+h/3);
  
  ofVector micro=ofVector(segment[0].width/2+30, segment[0].height/8+10).rotate(rotAng(0));
  
  //_-_-_-_-_ Labeling on the wheel //_-_-_-_-_
  
  ofSetColor(yellow);
  label.setMode(OF_FONT_MID);
  label.drawString("magnet", x+w/2-spiral.width/2, y+h/3+spiral.height/2);
  label.drawString("microcontroller", x+w/2-spiral.width/2, y+h/3-spiral.height/2);
  label.setMode(OF_FONT_TOP);
  ofPushStyle();
  ofSetLineWidth(2);
  ofEnableSmoothing();
  ofLine(x+w/2-spiral.width/2, y+h/3+spiral.height/2, x+w/2-spiral.width+6, y+h/3+spiral.height*7/16.+6);
  ofLine(x+w/2-spiral.width/2, y+h/3-spiral.height/2, x+w/2-spiral.width+micro.x, y+h/3+micro.y);
  ofPopStyle();
  
  bWait=true;
}

void demonstration::drawImageRotate()
{
  unfldPnt.x=x+(w-M_PI*spiral.width)/2;
  unfldPnt.y=y+(h+spiral.height)/2;
  
  int segAng=360/segment.size();
  
  header("Image displayed on wheel", label, x, y, w, h);
  header("On microcontroller on wheel", label, x, unfldPnt.y-spiral.height+unfold.height+80, w, h);
  
  ofSetColor(white);
  unfold.draw(unfldPnt.x, unfldPnt.y);
  ofSetColor(white*.2);
  ofSetLineWidth(1);
  for (int i=0; i<unfold.height/10.; i++) {
    ofLine(unfldPnt.x, unfldPnt.y+i*10, unfldPnt.x+unfold.width, unfldPnt.y+i*10);
  }
  for (int i=0; i<unfold.width/10.; i++) {
    ofLine(unfldPnt.x+i*10, unfldPnt.y, unfldPnt.x+i*10, unfldPnt.y+unfold.height);
  }
  ofRect(unfldPnt.x, unfldPnt.y, unfold.width, 20);
  
  double perc=1.-sld.getPercent();
  if(perc<=.2) perc=.2;
  if(frame.justExpired()&&rotateCnt<3600&&bRunning){
    frame.set(perc*frmTm);
    rotateCnt-=(360/segment.size())/17.;
    if(rotateCnt<0){
      rotateCnt+=360;
      if(cnt<numSpins){
        if(cnt==numSpins-1) mode++,bWait=true;
        cnt++;
        sld.setPercent(sld.getPercent()+1/(float(numSpins)));
      }
    }
    for (int i=0; i<segment.size(); i++) {
      segment[i].grabScreen(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y, segment[i].width, segment[i].height);
    }
  }
  
  ofNoFill();
  ofSetLineWidth(2);
  for (int i=0; i<segment.size(); i++) {
    ofSetColor(col[i%4]);
    ofRect(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y, segment[i].width, segment[i].height);
  }
  ofSetLineWidth(1);
  
  ofFill();
  
  ofSetColor(white*.2);
  ofSetCircleResolution(90);
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2);
  if(firstTime) persist.grabScreen(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2), persist.width, persist.height),firstTime=false;
  ofSetColor(white);
  persist.draw(int(x+w/2-spiral.width*3/2.), int(y+h/3-spiral.width/2));
  ofSetColor((white*.2).opacity(sqrt(perc)*1/10.));
  ofCircle(x+w/2-spiral.width, y+h/3, spiral.width/2+3);
  for (unsigned int i=0; i<segment.size(); i++) {
    ofPushMatrix();
    ofTranslate(x+w/2-spiral.width, y+h/3, 0);
    ofRotate(rotAng(i));
    ofSetColor(white);
    segment[i].draw(segment[i].width/2,0,-segment[i].width,segment[i].height-10);
    ofPopMatrix();
  }
  ofSetCircleResolution(40);
  persist.grabScreen(int(x+w/2-spiral.width*3/2.), int(y+h/3-spiral.width/2), persist.width, persist.height);
  
  unsigned char * pix=persist.getPixels();
  
  for (unsigned int i=0; i<persist.width*persist.height*4; i+=4) {
    if(abs(51-pix[i])<20&&abs(51-pix[i+1])<20&&abs(51-pix[i+2])<20) pix[i+3]=abs(51-pix[i]);
  }
  
  persist.setFromPixels(pix, persist.width, persist.height,OF_IMAGE_COLOR_ALPHA);
  
  ofSetColor(white*.2);
  ofRect(x+w/2-spiral.width*3/2., int(y+h/3-spiral.width/2), persist.width, persist.height);
  
  drawWheelFrame(x+w/2-spiral.width,y+h/3);
  
  ofSetColor(white);
  persist.draw(int(x+w/2-spiral.width*3/2.), int(y+h/3-spiral.width/2));
  
  if(cnt>=numSpins){
    ofSetColor(white);
    spiral.draw(x+w/2+spiral.width, y+h/3);
  }
  
  if(sld.getPercent()<.6){
    for (unsigned int i=0; i<segment.size(); i++) {
      ofVector vecEnd=ofVector(0, segment[i].height).rotate(rotAng(i));
      
      ofSetColor(col[i%4]);
      ofSetLineWidth(4);
      ofLine(int(unfldPnt.x+(unfold.width-segment[i].width)*(rotAng(i)/360.)), unfldPnt.y,(x+w/2-spiral.width)+ vecEnd.x, y+h/3+vecEnd.y);
      ofSetLineWidth(1);
    }
  }
  
  ofSetColor(white*.6);
  sld.draw(x+(w-sld.w)/2,y+h/4,200,10);
  ofSetColor(yellow);
  label.setMode(OF_FONT_CENTER);
  label.drawString("Rotation speed", sld.x+sld.w/2, sld.y+sld.h+20);
  label.setMode(OF_FONT_LEFT);
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
  if(bRunning&&mode==MAGNET) drawMagnetPosition();//
  if(bRunning&&(mode==ROTATING||mode==SPINNING)) drawImageRotate();
  
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
  drawBorder(side);
  drawBorder(r);
  
  ofRectangle botBox(side.x,side.y+side.height-(home.h+pad.y*2),side.width,home.h+pad.y*2);
  ofSetColor(gray);
  ofRect(botBox);
  drawBorder(botBox);
  
  if(bWait&&mode>=ROTATING&&(ofGetElapsedTimeMillis()/250)%2){
    ofSetColor(yellow);
    trimmedRect(home.x-5, home.y-5, home.w+10, home.h+10);
  }
  
  home.draw(botBox.x+(botBox.width-home.w)/2, botBox.y+(botBox.height-home.h)/2);
  
  
  ofSetColor(yellow);
  label.drawString(text+add[int(mode)-1], side.x+pad.x,side.y+100);//side.y+(side.height-label.stringHeight(text+add[int(mode)-1])-botBox.height)/2
  
  if(bWait&&mode>=UNFLD){
    prev.draw(side.x+40, side.y+side.height-botBox.height-prev.h-20);
  }
  if(bWait&&mode<ROTATING){
    next.draw(side.x+side.width-next.w-40, side.y+side.height-botBox.height-next.h-20);
    if((ofGetElapsedTimeMillis()/500)%2){
      ofSetColor(yellow);
      ofNoFill();
      ofSetLineWidth(6);
      ofBeginShape();
      ofVertex(next.x, next.y);
      ofVertex(next.x+next.w-3*next.h/4, next.y);
      ofVertex(next.x+next.w, next.y+next.h/2);
      ofVertex(next.x+next.w-3*next.h/4, next.y+next.h);
      ofVertex(next.x, next.y+next.h);
      ofVertex(next.x, next.y);
      ofEndShape();
      ofSetLineWidth(1);
      ofFill();
    }
  }
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
    if(mode==ROTATING) mode--;
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
    if(mode==MAGNET) frame.set(1/10.);
    if(mode==SELECT) frame.set(1);
    if(mode==ROTATING) frame.set(1/120.), cnt=0, sld.setPercent(0);
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
