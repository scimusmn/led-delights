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

void povImage::loadImage(string filename, bool anm)
{
	bAnimation=anm;
	bChose=false;
	dispTime=0;
	stateCount=0;
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
	if(selected) ofSetColor(255,128,128);
	else ofSetColor(128,128,128);
  ofRoundedRect(x-5,y-5,w+10,h+10,10);
	ofSetColor(255,255,255);
	image.draw(x, y,w,h);
}

void povImage::drawUpload(int _x, int _y)
{
	x=_x;
	y=_y;
	if(bChose) ofSetColor(128, 255, 128);
	else ofSetColor(128, 128, 128);
	ofRoundedRect(x-5,y-5,w+10,h+10,10);
	ofSetColor(255,255,255);
	image.draw(x, y,w,h);
	times.draw(x+w/2, y+h+7);
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

bool povImage::clickDown_upload(int _x, int _y)
{
	if (over(_x, _y)) {
		bChose=!bChose;
	}
	if(times.clickDown(_x, _y));
}

void povImage::setPos(int _x, int _y, int _w, int _h)
{
	cSetup(_x, _y, _w, _h);
}


void povImage::saveState()
{
	while (stateCount>0) {
		storedState.pop_front();
		stateCount--;
	}
	storedState.push_front(image);
	if (storedState.size()>10) {
		storedState.pop_back();
	}
}

void povImage::undo()
{
	if (storedState.size()&&++stateCount>storedState.size()-1) {
		stateCount=storedState.size()-1;
	}
	cout <<"State count "<< stateCount << " stored state " << storedState.size()<< endl;
	if(storedState.size())
		image=storedState[stateCount];
}

void povImage::redo()
{
	if (--stateCount<=0) {
		stateCount=0;
	}
	image=storedState[stateCount];
	cout <<"State count "<< stateCount << " stored state " << storedState.size()<< endl;
}

bool povImage::undoAvailable()
{
	return (stateCount<storedState.size()-1);
}

bool povImage::redoAvailable()
{
	return stateCount>0;
}

void povImage::saveFile(string dirname)
{
	image.saveImage((dirname+"/"+file));
}

void povImage::writePlaylistEntry(ofstream * out,string dirname)
{
	if(bChose||selected){
		if(bAnimation){
			cout << file << endl;
			(*out) << "animation2 " << file << " 2 "<<1/double(fps)<< " " << times.getChoice()<<endl;
		}
		else{
			(*out) << "picture2 "<< file << " 3 " << times.getChoice() << ".0" <<endl;
		}
	}
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

void povImages::display(int _x, int _y, int _w, int _h)
{
	
	images[nSelected].display(_x,_y,_w,_h);
}

void povImages::draw(int _x, int _y)
{
	x=_x,y=_y;
	ofSetColor(0x60431B);
	ofRect(x, y, w, h);
  
	ofShadowRounded(x-5, y-5, w+10, h+10, 5);
	ofShade(x, y, 10, h, OF_RIGHT,.25);
	ofShade(x+w, y, 10, h, OF_LEFT,.25);
	for (unsigned int i=0; i<images.size(); i++) {
		images[i].draw(x+(w-images[i].w)/2,y+20+(images[i].h+20)*i);
	}
}

void povImages::drawUpload(int _x, int _y, int space)
{
	x=_x,y=_y;
	for (unsigned int i=0; i<images.size(); i++) {
		images[i].drawUpload(x+(images[i].w+space)*(i%4),y+(images[i].h+space)*(i/4));
	}
}

void povImages::setThumbSize(int _w, int _h){
	wid=_w;
	hgt=_h;
	for (unsigned int i=0; i<images.size(); i++) {
		images[i].changeSize(wid,hgt);
	}
}

void povImages::setPos(int _x, int _y, int _w, int _h)
{
	hgt=_w-20;
	int thumb=max(h/(images.size()*(hgt)),double(_w));
	setThumbSize(thumb, thumb);
	cSetup(_x, _y, thumb+40, _h);
	clipboard=images[0].image;
}

void povImages::grabScreen(int _x, int _y, int _w, int _h)
{
	images[nSelected].image.grabScreen(_x,_y,_w,_h);
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

bool povImages::upClickDown(int _x, int _y){
	bool ret=false;
	for (unsigned int i=0; i<images.size(); i++) {
		ret=images[i].clickDown_upload(_x,_y);
	}
	return ret;
}

povImage & povImages::operator[](int i)
{
	return images[i];
}

int povImages::size()
{
	return images.size();
}

void povImages::saveState()
{
	images[nSelected].saveState();
}

void povImages::setSelected(int i)
{
	nSelected=i;
	images[nSelected].selected=true;
}

void povImages::undo()
{
	images[nSelected].undo();
}

void povImages::redo()
{
	images[nSelected].redo();
}

bool povImages::undoAvailable()
{
	return images[nSelected].undoAvailable();
}

bool povImages::redoAvailable()
{
	return images[nSelected].redoAvailable();
}

void povImages::copy()
{
	clipboard=images[nSelected].image;
}

void povImages::paste()
{
	images[nSelected].image=clipboard;
}
	
