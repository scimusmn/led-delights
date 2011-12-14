/*
 *  povImage.cpp
 *  emptyExample
 *
 *  Created by Exhibits on 1/21/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "povImage.h"
#include <algorithm>

extern ofColor black, white, yellow, gray, blue, red, orange;

void povImage::loadImage(string filename, bool anm)
{
	bAnimation=anm;
	bChose=false;
	dispTime=0;
	size_t found;
	found=filename.rfind("/");
	if (found==string::npos) found=0;
	file=filename.substr(found+1);
	image.loadImage(filename);
	selected=false;
	saveState();
	times.setup(1, 5, 3, (bAnimation)?" x":" s");
	fps=5;
}

povImage::povImage(string filename, bool anm)
{
	loadImage(filename, anm);
}

void povImage::display(int _x, int _y, int _w, int _h)
{
	if(selected){
		ofSetColor(255,255,255);
		image.draw(_x, _y, _w, _h);
	}
}

void povImage::draw(int _x, int _y)
{
	x=_x;
	y=_y;
	if(selected) ofSetColor(yellow);
	else ofSetColor(white*.3);
  ofRect(x-5,y-5,w+10,h+10);
	ofSetColor(255,255,255);
	image.draw(x, y,w,h);
}

void povImage::draw()
{
	if(selected) ofSetColor(255,128,128);
	else ofSetColor(128,128,128);
  ofRoundedRect(x-5,y-5,w+10,h+10,10);
	glColor3f(1,1,1);
	image.draw(x, y,w,h);
}

bool povImage::clickDown(int _x, int _y)
{
	if (over(_x, _y)) {
		selected=true;
	}
	return over(_x, _y);
}

void povImage::setPos(int _x, int _y, int _w, int _h)
{
	cSetup(_x, _y, _w, _h);
}


void povImage::saveState()
{
  states.recordState(image);
}

void povImage::undo()
{
	image=*states.undoState();
}

void povImage::redo()
{
	image=*states.redoState();
}

bool povImage::undoAvailable()
{
	return states.undoAvailable();
}

bool povImage::redoAvailable()
{
	return states.redoAvailable();
}

void povImage::saveFile(string dirname)
{
	image.saveImage((dirname+"/"+file));
}

void povImage::writePlaylistEntry(ofstream * out,string dirname)
{
	if(bChose||selected){
		if(bAnimation){
			(*out) << "animation2 " << file << " 2 "<<1/double(fps)<< " " << times.getChoice()<<endl;
		}
		else{
			(*out) << "picture2 "<< file << " 3 " << times.getChoice() << ".0" <<endl;
		}
	}
}

void povImage::writePlaylist(string filename)
{
  ofstream plst(ofToDataPath(filename).c_str());
	if(bChose||selected){
		if(bAnimation){
			plst << "animation2 " << file << " 2 "<<1/double(fps)<< " " << times.getChoice()<<endl;
		}
		else{
			plst << "picture2 "<< file << " 3 " << times.getChoice() << ".0" <<endl;
		}
	}
  plst.close();
}

void povImages::loadImages(string dir)
{
  pad.x=pad.y=20;
  int nImages = DIR.listDir(dir);
  //you can now iterate through the files as you like
  for(int i = 0; i < min(nImages,8); i++){
		string t=DIR.getPath(i);
		string temp=t.substr(t.find_last_of(".")+1,t.find_last_of(".")+4);
		if(!temp.compare("jpg")) newImage(DIR.getPath(i), false);
		else if(!temp.compare("gif")) newImage(DIR.getPath(i), true);
  }
  h=pad.y;
  for (unsigned int i=0; i<images.size(); i++) {
    images[i].w=images[i].h=75;
    w=images[i].w+pad.x*2;
    h+=images[i].h+pad.y;
  }
  setSelected(0);
}
	
void povImages::newImage(string filename, bool anm)
{
	images.push_back(povImage(ofToDataPath(filename),anm));
}

void povImages::saveImages(string filename)
{
	string dir=ofGetDirName(filename);
	images[nSelected].saveFile(dir);
}

void povImages::writePlaylist(string filename)
{
	string dir=ofGetDirName(filename);
	ofstream plst(ofToDataPath(filename).c_str());
	//for (unsigned int i=0; i<images.size(); i++) {
		images[nSelected].writePlaylistEntry(&plst,dir);
	//}
	plst.close();
}

void povImages::draw(int _x, int _y)
{
	x=_x,y=_y;
	ofSetColor(0,0,0,128);
	ofRect(x, y, w, h);
  int yPos=y+pad.y;
	for (unsigned int i=0; i<images.size(); i++) {
		images[i].draw(x+(w-images[i].w)/2,yPos);
    yPos+=images[i].h+pad.y;
	}
}


bool povImages::clickDown(int _x, int _y){
	bool ret=false;
	for (unsigned int i=0; i<images.size(); i++) {
		if(images[i].clickDown(_x,_y)){
			nSelected=i;
			ret=true;
			for (unsigned int j=0; j<images.size(); j++) {
				if (j!=i) {
					images[j].selected=false;
				}
			}
		}
	}
	return ret;
}

bool povImages::clickUp(){
  return false;
};

povImage & povImages::operator[](int i)
{
	return images[i];
}

int povImages::size()
{
	return images.size();
}

void povImages::setSelected(int i)
{
	nSelected=i;
	images[nSelected].selected=true;
}

void povImages::copy()
{
	clipboard=images[nSelected].image;
}

void povImages::paste()
{
	images[nSelected].image=clipboard;
}
	
