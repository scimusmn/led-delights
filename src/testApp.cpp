#include "testApp.h"
#define num 100

//--------------------------------------------------------------
void testApp::setup(){
	
	width=ofGetWidth();
	height=ofGetHeight();
	
	ofHideCursor();
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
	
  area.setup();
  title.loadFont("fonts/DinC.ttf",40);
  
  bar.setup(area);
  tBar.setup(area);
  pBar.setup(area);
}

//--------------------------------------------------------------
void testApp::update(){
	if(bar.getMode()==LED_PREDEF) area.update();
}

//--------------------------------------------------------------
void testApp::draw(){
  ofBackground(51, 51, 51);
  //if(bar.getMode()>LED_DEMO) area.draw(0,bar.y+bar.h,ofGetWidth(),ofGetHeight()-(bar.y+bar.h)+2);
  if(bar.getMode()==LED_DRAW) area.draw(tBar.w,title.y+title.h,ofGetWidth()-tBar.w,ofGetHeight()-(title.y+title.h)+2);
  if(bar.getMode()==LED_PREDEF) area.draw(pBar.w,title.y+title.h,ofGetWidth()-pBar.w,ofGetHeight()-(title.y+title.h)+2);
  //if(bar.getMode()==LED_PREDEF) ;
  
  bar.draw(0, title.y+title.h);
  
  if(bar.getMode()==LED_DRAW) tBar.draw(0,title.y+title.h);
  if(bar.getMode()==LED_PREDEF) pBar.draw(0,title.y+title.h);
  
  bar.drawForeground();
  if(bar.getMode()==LED_DRAW) tBar.drawForeground();
  
  string ttl=((bar.getMode()==LED_DRAW)?"DRAW YOUR OWN IMAGE FOR THE WHEEL":(bar.getMode()==LED_PREDEF)?"CHOOSE AN IMAGE TO SEND TO THE WHEEL":(bar.getMode()==LED_HOME)?"LED DELIGHTS":"HOW IT WORKS");
  
  title.draw(ttl,0,0);
  
  area.drawForeground();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  bar.drag(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  area.clickDown(x, y);
  if(bar.getMode()==LED_DRAW) tBar.clickDown(x, y);
  if(bar.getMode()==LED_PREDEF) pBar.clickDown(x, y);
  bar.clickDown(x, y, button);
}
	
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  area.clickUp();
  tBar.clickUp();
  pBar.clickUp();
  bar.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

