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
	
}

//--------------------------------------------------------------
void testApp::draw(){
  
  if(bar.getMode()>LED_DEMO) area.draw(0,bar.y+bar.h,ofGetWidth(),ofGetHeight()-(bar.y+bar.h)+2);
  
  bar.draw(0, title.y+title.h);
  
  if(bar.getMode()==LED_DRAW) tBar.draw(0,bar.y+bar.h);
  if(bar.getMode()==LED_PREDEF) pBar.draw(0,bar.y+bar.h);
  
  bar.drawForeground();
  if(bar.getMode()==LED_DRAW) tBar.drawForeground();
  area.drawForeground();
  
  title.draw("Upload images to the wheel",0,0);
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
  pBar.clickDown(x, y);
  bar.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

