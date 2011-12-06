/*
 *  ofInterCustom.cpp
 *  movieGrabberExample
 *
 *  Created by Exhibits on 1/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "ofInterCustom.h"


bool colorButton::clickDown(int _x, int _y)
{
	if(over(_x, _y)){
		bPressed=true;
	}
	return bPressed;
}
bool colorButton::clickUp()
{
}
void colorButton::draw(int _x,int _y)
{
	x=_x;
	y=_y;
	ofSetColor(color);
	ofRect(x, y, w, h);
}

void colorGrid::setup(int _w, int _h, int numColors)
{
	cSetup(0, 0, _w, _h);
	float aspect=w/h;
	int oneSide=sqrt(numColors);
	if (oneSide*oneSide<numColors) {
		oneSide++;
	}
	across=down=oneSide;
	if (aspect>1) {
		while(across/down<aspect){
			across++;
			down--;
		}
	}
	else if(aspect<1){
		while(across/down>aspect){
			across--;
			down++;
		}
	}
	
	int horCol=2*255/(across-1);
	int verCol=2*255/(down-1);
	for (int i=0; i<down; i++) {
		for (int j=0; j<across; j++) {
			int k=i*across+j;
			int wd=across;
			float sp=255/across;
			if(i<down-1||j<across-2)
			   colors.push_back(colorButton(x+j*(w/across),y+i*h/down,w/across,h/down,(k%wd)*sp,(k/wd)*sp,(k%(wd-1))*(k/(wd-1))*sp));
		}
	}
	colors.push_back(colorButton(x+(across-2)*w/across,y+(down-1)*h/down,w/across,h/down,255,255,0));	
	colors.push_back(colorButton(x+(across-1)*w/across,y+(down-1)*h/down,w/across,h/down,255,255,255));
	
	colors[colors.size()/2].setPressed(true);
}

bool colorGrid::clickDown(int _x,int _y)
{
	for (unsigned int i=0; i<colors.size(); i++) {
		if (colors[i].clickDown(_x,_y)) {
			colors[i].bPressed=true;
			for (unsigned int j=0; j<colors.size(); j++) {
				if(j!=i) colors[j].bPressed=false;
			}
			bPressed=true;
		}
	}
	return bPressed;
}

void colorGrid::draw(int _x, int _y)
{
	x=_x;
	y=_y;
	ofSetColor(0x222222);
	ofRect(x-2, y-2, w+4, h+4);
	int ac=across;
	for (unsigned int i=0; i<colors.size(); i++) {
		colors[i].draw(x+w/across*(i%ac),y+h/down*(i/ac));
	}
	ofSetColor(0x222222);
	ofSetLineWidth(2);
	for (int i=0; i<across; i++) {
		ofLine(colors[i].x+colors[i].w, colors[i].y, colors[i].x+colors[i].w, colors[i+(down-1)*across].y+colors[i+(down-1)*across].w);
	}
	for (int i=0; i<down; i++) {
		ofLine(colors[i*across].x, colors[i*across].y, colors[i*across+across-1].x+colors[i].w, colors[i*across].y);
	}
}

ofColor colorGrid::getColor()
{
	ofColor ret;
	for (unsigned int i=0; i<colors.size(); i++) {
		if (colors[i].bPressed) {
			ret=colors[i].color;
		}
	}
	return ret;
}

void sizeButton::draw(int _x, int _y)
{
	x=_x,y=_y;
	ofColor cur=ofGetStyle().color;
	if (bSelected){
		glColor3f(1-cur.r/255., 1-cur.g/255., 1-cur.b/255.);
		ofRect(x-4, y-4, (4+r)*2,(4+r)*2);
	}
	ofSetColor(128,128,128);
	ofCircle(x+r, y+r, r+4);
	ofFill();
	ofDisableSmoothing();
	ofSetColor(cur);
	ofCircle(x+r, y+r, r);
}

bool sizeButton::clickDown(int _x, int _y)
{
	if (over(_x, _y)) {
		bSelected=true;
	}
	return bOver;
}

void sizeSelect::setup(int num, int startr, int endr)
{
	w=(endr-(endr-startr)/num)*2+20;
	h=(double(num-1)/2)*(endr-startr)*2+(startr*2+20)*num;
	for (int i=0; i<num; i++) {
		int rad=((endr-startr)/num)*i+startr;
		sizes.push_back(sizeButton(rad));
	}
	if(num) sizes[0].bSelected=true;
}
	
void sizeSelect::draw(int _x, int _y)
{
	x=_x-sizes[sizes.size()-1].r-10, y=_y-sizes[0].r-10;
	ofColor cur=ofGetStyle().color;
	ofSetColor(0x60431B);
	ofRect(x, y, w, h);
  ofSetShadowDarkness(.5);
	ofShadowRounded(x-5, y-5, w+10, h+10, 5);
	ofShade(x, y, 10, h, OF_RIGHT,.25);
	ofShade(x+w, y, 10, h, OF_LEFT,.25);
	
	ofSetColor(cur);
	if(sizes.size()) sizes[0].draw(_x-sizes[0].r,(_y-sizes[0].r));
	for (unsigned int i=1; i<sizes.size(); i++) {
		sizes[i].draw(_x-sizes[i].r,sizes[i-1].y+2*sizes[i-1].r+20);
	}
}

bool sizeSelect::clickDown(int _x, int _y)
{
	for (unsigned int i=0; i<sizes.size(); i++) {
		if (sizes[i].clickDown(_x,_y)) {
			for (unsigned int j=0; j<sizes.size(); j++) {
				if(j!=i) sizes[j].bSelected=false;
			}
			bPressed=true;
		}
	}
	return bPressed;
}

bool sizeSelect::clickUp()
{
	bPressed=false;
	return false;
}

int sizeSelect::getSize()
{
	int ret=0;
	for (unsigned int i=0; i<sizes.size(); i++) {
		if(sizes[i].bSelected) ret=sizes[i].r;
	}
	return ret;
}
