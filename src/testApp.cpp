#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	arial.setMode(OF_FONT_CENTER);
	
  preDef.loadImages("upload/images/predefined");
	
	arial.loadFont("fonts/DinC.ttf");
	arial.setSize(12);
	
	drawing=false;
	
	width=ofGetWidth();
	height=ofGetHeight();
	
	ofHideCursor();
  
	bottomBarH=75;
	sideBarWidth=200;
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
	
  area.setup();
  title.loadFont("fonts/DinC.ttf",40);
  
  bar.setup(area);
  tBar.setup(area);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

void testApp::drawPredef()
{
	area.changeImage(preDef.selected());
}

//--------------------------------------------------------------
void testApp::draw(){
	//if(!buttonPressed()){
//		ofSetColor(128, 128, 128);
//		background.draw(0, 0,ofGetWidth(),ofGetHeight());
//		predefBut.changeSize(200, 200);
//		predefBut.move(ofGetWidth()/4-predefBut.w/2, (ofGetHeight()/2-predefBut.h)/2);
//		drawBut.changeSize(200, 200);
//		drawBut.move(ofGetWidth()/2-drawBut.w/2, (ofGetHeight()-drawBut.h)/2);
//		demoBut.changeSize(200, 200);
//		demoBut.move(3*ofGetWidth()/4-demoBut.w/2, (ofGetHeight()/2-demoBut.h)/2);
//		//predefBut.drawTextButton(ofGetWidth()/4-predefBut.w/2, ofGetHeight()/2);
//		//drawBut.drawTextButton(ofGetWidth()/2-drawBut.w/2, ofGetHeight()/2);
//		//demoBut.drawTextButton(3*ofGetWidth()/4-demoBut.w/2, ofGetHeight()/2);
//		
//		ofSetColor(255, 255, 255);
//		predefPic.draw(predefBut.x, predefBut.y,predefBut.w, predefBut.h);
//		drawPic.draw(drawBut.x, drawBut.y,drawBut.w, drawBut.h);
//		demoPic.draw(demoBut.x, demoBut.y,demoBut.w, demoBut.h);
//		
//		arial.setSize(30);
//		arial.setMode(OF_FONT_CENTER);
//		arial.drawString("Send sample image", predefBut.x+predefBut.w/2, predefBut.y-50);
//		arial.drawString("Draw your own image for the wheel", drawBut.x+drawBut.w/2, drawBut.y+drawBut.h+50);
//		arial.drawString("How it works", demoBut.x+demoBut.w/2, demoBut.y-50);
//		
//		arial.setMode(OF_FONT_LEFT);
//		arial.setSize(10);
//	}
	
  
  if(bar.getMode()==LED_PREDEF) drawPredef();
  
  area.draw(0,bar.y+bar.h,ofGetWidth(),ofGetHeight()-(bar.y+bar.h)+2);
  
  title.draw("Upload images to the wheel",0,0);
  bar.draw(0, title.y+title.h);
  
  tBar.draw(0,bar.y+bar.h);
  
  bar.drawForeground();
  tBar.drawForeground();
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  area.clickDown(x, y);
  tBar.clickDown(x, y);
  bar.clickDown(x, y, button);
}
	
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  area.clickUp();
  tBar.clickUp();
  bar.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

