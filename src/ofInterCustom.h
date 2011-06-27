/*
 *  ofInterCustom.h
 *  movieGrabberExample
 *
 *  Created by Exhibits on 1/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _CUSTOM_CLASSES
#define _CUSTOM_CLASSES

#include "ofMain.h"
#include "ofExtended.h"

class ofSaveButton : public ofButton {
	int r,g,b;
public:
	ofSaveButton():ofButton(){
		r=.3*255, g=.6*255, b=.9*255;
	}
	bool clickDown(int _x, int _y);
	bool clickUp();
	void draw(int _x,int _y);
	void drag(int _x,int _y);
};

class colorButton : public ofButton {
public:
	ofColor color;
	colorButton():ofButton(){
	}
	colorButton(int _x, int _y, int _w, int _h, long hexcolor):ofButton(_x,_y,_w,_h){
		color.set(hexcolor);
	}
	colorButton(int _x, int _y, int _w, int _h, int r, int g, int b):ofButton(_x,_y,_w,_h){
		color.set(r,g,b);
	}
	bool clickDown(int _x, int _y);
	bool clickUp();
	void draw(int _x,int _y);
	friend class colorGrid;
};

class colorGrid : public ofInterGroup {
public:
	double across;
	double down;
	vector<colorButton> colors;
	colorGrid():ofInterGroup(){
		across=down=0;
	}
	void setup(int _w, int _h, int numColors);
	bool clickDown(int _x,int _y);
	void draw(int _x,int _y);
	ofColor getColor();
};

class sizeButton : public ofButton {
	bool bSelected;
	double r;
public:
	sizeButton(double rad):ofButton(0,0,2*rad, 2*rad){
		r=rad;
	}
	bool clickDown(int _x, int _y);
	void draw(int _x, int _y);
	friend class sizeSelect;
};

class sizeSelect : public ofInterGroup {
	vector<sizeButton> sizes;
public:
	void setup(int num, int startr, int endr);
	void draw(int _x, int _y);
	bool clickDown(int _x, int _y);
	bool clickUp();
	int getSize();
};

#endif