/*
 *  testAppDrawFuncs.cpp
 *  Rot-a-Pic
 *
 *  Created by Exhibits on 3/5/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "testApp.h"

#include "dallasEng.h"

extern ofColor black,gray, white, red, yellow, orange, blue;

void testApp::bottomBarDraw()
{
	ofSetColor(gray);
	//ofShadeBox(0, ofGetHeight()-bottomBarH, ofGetWidth(), bottomBarH, OF_DOWN,(0xCF-0xA8)/255.);
  ofRect(0, ofGetHeight()-bottomBarH, ofGetWidth(), bottomBarH);
	ofSetColor(black);
  drawHatching(0, ofGetHeight()-bottomBarH, ofGetWidth(), bottomBarH, 50, 50);
	homeBut.draw(ofGetWidth()-homeBut.w-20, ofGetHeight()-(bottomBarH+homeBut.h)/2);
	if(drawBut.pressed()){
		redoButton.draw((ofGetWidth()-redoButton.w)/2-50,ofGetHeight()-(bottomBarH+redoButton.h)/2);
		undoButton.draw((ofGetWidth()-undoButton.w)/2+50,ofGetHeight()-(bottomBarH+undoButton.h)/2);
		demoBut.changeSize(64, 64);
		demoBut.draw(homeBut.x-20-demoBut.w, ofGetHeight()-(bottomBarH+demoBut.h)/2,64,64);
		predefBut.draw(demoBut.x-20-predefBut.w, ofGetHeight()-(bottomBarH+predefBut.h)/2,64,64);
	}
	if(predefBut.pressed()){
		demoBut.draw(homeBut.x-20-demoBut.w, ofGetHeight()-(bottomBarH+demoBut.h)/2,64,64);
		drawBut.draw(demoBut.x-20-drawBut.w, ofGetHeight()-(bottomBarH+drawBut.h)/2,64,64);
	}
	if(demoBut.pressed()){
		drawBut.draw(homeBut.x-20-drawBut.w, ofGetHeight()-(bottomBarH+drawBut.h)/2,64,64);
		predefBut.draw(drawBut.x-20-predefBut.w, ofGetHeight()-(bottomBarH+predefBut.h)/2,64,64);
	}
	if(!demoBut.pressed()) uploadButton.draw(ofGetWidth()-240,ofGetHeight()-320);
	
	
}

void testApp::predefBarDraw()
{
	ofSetColor(220, 220, 220);
	double sideBarImgAdj=ofGetHeight()/double(sideBarBack.width);
	sideBarBack.draw(sideBarWidth-sideBarBack.width*sideBarImgAdj, 0,sideBarBack.width*sideBarImgAdj,ofGetHeight());
	ofShade(sideBarWidth, 0, 15, ofGetHeight(), OF_LEFT,.35);
	ofShade(sideBarWidth, 0, 10, ofGetHeight()-bottomBarH, OF_RIGHT,.5);
	ofShade(sideBarWidth, ofGetHeight()-bottomBarH, 5, bottomBarH, OF_RIGHT,.5);
	
	preDef.draw((sideBarWidth-preDef.w)/2, 20);
}

void testApp::drawBarDraw(int x, int y)
{
		//--------------------- Background
		
	//double sideBarImgAdj=ofGetHeight()/double(sideBarBack.width);
//	ofSetColor(220, 220, 220);
//	sideBarBack.draw(ofGetWidth()-rightBarWidth+sideBarBack.width*sideBarImgAdj, 0,-sideBarBack.width*sideBarImgAdj,ofGetHeight());
//	ofShade(ofGetWidth()-rightBarWidth, 0, 20, ofGetHeight(), OF_RIGHT,.15);
//	ofShade(ofGetWidth()-rightBarWidth, 0, 10, ofGetHeight()-bottomBarH, OF_LEFT,.5);
//	ofShade(ofGetWidth()-rightBarWidth, ofGetHeight()-bottomBarH, 5, bottomBarH, OF_LEFT,.5);
	
	/*ofSetColor(220, 220, 220);
	double sideBarImgAdj=ofGetHeight()/double(sideBarBack.width);
	sideBarBack.draw(rightBarWidth-sideBarBack.width*sideBarImgAdj, 0,sideBarBack.width*sideBarImgAdj,ofGetHeight());
	ofShade(rightBarWidth, 0, 15, ofGetHeight(), OF_LEFT,.35);
	ofShade(rightBarWidth, 0, 10, ofGetHeight()-bottomBarH, OF_RIGHT,.5);
	ofShade(rightBarWidth, ofGetHeight()-bottomBarH, 5, bottomBarH, OF_RIGHT,.5);*/
  
  ofRectangle bar(x,y,rightBarWidth,ofGetHeight());
  
  ofSetColor(gray);
  ofRect(bar);
  ofSetColor(black);
  drawHatching(bar.x, bar.y, bar.width, bar.height, 20,20);
  
  drawBorder(bar);
	
	int hPos=20+title.h;
	
	colGrid.draw(x+(rightBarWidth-colGrid.w)/2,hPos);
	
	string cur="current color";
	arial.setSize(10);
	int curW=arial.stringWidth(cur);
	ofRectangle curBox=arial.getBoundingBox(cur, x+((rightBarWidth-curW)/2), hPos+=colGrid.h+60);
	//ofShadowRounded(curBox.x-20, curBox.y-20, curBox.width+40, curBox.height+20, 10);
	ofSetColor(currentColor);
	ofRect(curBox.x-10, curBox.y-10, curBox.width+20, curBox.height);
	ofSetColor(currentColor.inverse());
	arial.drawString(cur, curBox.x, curBox.y);
	
	fillButton.draw(x+(rightBarWidth-fillButton.w)/2,hPos+=curBox.height+20);
	
	ofSetColor(currentColor);
	sizes.draw(x+(colGrid.w/2+20), hPos+=40+fillButton.h);
	ofShade(x+rightBarWidth, ofGetHeight()-bottomBarH, 10, rightBarWidth, OF_UP,.25);
}


void testApp::uploadDraw(){
	ofSetColor(32, 32, 32,196);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
	ofSetColor(196, 196, 196);
	ofRect(upXoff, upYoff, ofGetWidth()-2*upXoff, ofGetHeight()-upYoff*2);
	
	int imageSpacing=100;
	
	ofSetColor(32, 128, 200);
	if(group.size()) 
		tabs.changeSize(group.thumbSize()*4+imageSpacing*3+200,group.thumbSize()*2+imageSpacing+200);
	tabs.draw((ofGetWidth()-tabs.w)/2,(ofGetHeight()-tabs.h)/2-25);
	
	ofSetColor(64, 64, 64);
	ofRect(tabs.x+50, tabs.y+50, tabs.w-100, tabs.h-100);
	//ofSetColor(0, 0, 0);
	//ofRoundShade(tabs.x+50, tabs.y+50, tabs.w-100, tabs.h-100, 20,.3, false);
	
	if(tabs.getTab()==0)
		group.drawUpload(tabs.x+100, tabs.y+100,imageSpacing);
	else if(tabs.getTab()==1)
		preDef.drawUpload(tabs.x+100, tabs.y+100,imageSpacing);
	
	cancelButton.draw(upXoff+200, ofGetHeight()-(upYoff+50));
	sendButton.draw(ofGetWidth()-(upXoff+200+sendButton.w), ofGetHeight()-(upYoff+50));
	
}