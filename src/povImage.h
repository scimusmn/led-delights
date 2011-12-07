/*
 *  povImage.h
 *  emptyExample
 *
 *  Created by Exhibits on 1/21/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _povImage
#define _povImage

#include "ofMain.h"
#include "ofExtended.h"
#include "ofxDirList.h"

class numButton : public ofButton {
	ofColor color;
	ofFont arial;
	string label;
public:
	int nNum;
	void setup(int number, string suffix){
		nNum=number;
		label=(ofToString(nNum)+suffix);
	}
	numButton(int number, string suffix):ofButton(){
		color.set(.3*255,.6*255,.9*255);
		bPressed=false;
		arial.loadFont("Arial.ttf");
		arial.setSize(12);
		setup(number, suffix);
		ofRectangle k=arial.getBoundingBox(label, 0, 0);
		cSetup(k.x-3, k.y-3, k.width+6, k.height+6);
	}
	bool clickDown(int _x, int _y)
	{
		bool ret=false;
		if(ret=over(_x, _y)) bPressed=true;
		return ret;
	}
	bool clickUp(){
		bPressed=false;
	}
	void draw(int _x,int _y){
		x=_x, y=_y;
		if(bPressed) ofSetColor(color-.2*255);
		else ofSetColor(color);
    ofFlat();
		ofRoundedRect(x,y,w,h, 0);
		ofSetColor(0, 0, 0);
		arial.drawString(label, x+3, y+13);
	}
	friend class numButtons;
};

class numButtons : public ofInterGroup {
	vector<numButton> choices;
	int nSelected;
public:
	numButtons():ofInterGroup(){
		nSelected=0;
	}
	void setup(int low, int high, int steps, string label){
		for (int i=0; i<steps; i++) {
			choices.push_back(numButton(i*(high-low)/(steps-1)+low,label));
		}
		choices[0].setPressed(true);
	}
	void draw(int _x, int _y){
		for (unsigned int i=0; i<choices.size(); i++) {
			choices[i].draw(_x-(double(choices.size())/2)*(choices[i].w)-((choices.size()-1)*5)/2+i*(choices[i].w+5),_y);
		}
	}
	bool clickDown(int _x, int _y){
		for (unsigned int i=0; i<choices.size(); i++) {
			if(choices[i].clickDown(_x,_y)){
				nSelected=choices[i].nNum;
				for (unsigned int j=0; j<choices.size(); j++) {
					if (j!=i) {
						choices[j].clickUp();
					}
				}
			}
		}
	}
	int getChoice(){ 
		return (nSelected*2+1); 
	}
};


class povImage : public ofInterObj {
	numButtons times;
public:
	bool bAnimation, selected, bChose;
	float fps,dispTime;
	string file;
	ofImage image;
	//deque<ofImage> storedState;
  stateSave<ofImage> states;
	//int stateCount;
	povImage():ofInterObj(){
	}
	povImage(string filename, bool anm=false);
	void loadImage(string filename, bool anm=false);
	void display(int _x, int _y, int _w, int _h);
	void draw(int _x, int _y);
	void setPos(int _x, int _y, int _w, int _h);
	void saveState();
	void undo();
	void redo();
	bool undoAvailable();
	bool redoAvailable();
	void saveFile(string dir);
	void writePlaylistEntry(ofstream * out,string dirname);
  void writePlaylist(string filename);
	void draw();
	bool clickDown(int _x, int _y);
	friend class povImages;
};

class povImages : public ofInterGroup {
	int wid,hgt;
	int nSelected;
	ofImage clipboard;
  ofxDirList DIR;
  ofPoint pad;
public:
	povImages():ofInterGroup(){
		nSelected=0;
	}
	vector<povImage> images;
	int	thumbSize(){ return hgt; }
  void loadImages(string dir);
	void newImage(string filename, bool anm=false);
	void draw(int _x, int _y);
	bool clickDown(int _x, int _y);
	void setSelected(int i);
  povImage & selected(){ return images[nSelected]; }
	povImage & operator[](int i);
	int size();
	void saveImages(string file);
	void writePlaylist(string filename);
	void copy();
	void paste();
};

#endif
