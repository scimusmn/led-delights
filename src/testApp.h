#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofExtended.h"
#include "ofInterCustom.h"
#include "povImage.h"
#include "ofxDirList.h"
#include <ofxSystemCall.h>

#include "ofSpinner.h"

#include "imageArea.h"

#include "dallasEng.h"

class testApp : public ofBaseApp{
	
	int width;
	int height;
	ofImage mousePointer;
	ofImage sideBarBack;

	public:
	
		void setup();
		void update();
		void draw();
	
	void drawPredef();
	void drawEdit();
	void drawDemo();
	
	void bottomBarDraw();
	void predefBarDraw();
	void drawBarDraw(int x, int y);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void uploadDraw();
		void upload();
	
	ofFont arial;
	
	/****************** drawing variables **************/
	
	ofColor			currentColor;
	int				currentSize;
	povImages		group;
	ofPoint			prevPoint;
	bool			drawing;
	bool			editing;
	
	/***************** interface objects ***************/
	
	ofButton		redoButton;
	ofButton		undoButton;
	ofButton		copyButton;
	ofButton		pasteButton;
	ofButton		uploadButton;
	ofButton		fillButton;
	ofButton		diyButton;
	
	colorGrid		colGrid;
	sizeSelect		sizes;
	
	
	/**************** position variables ***************/
	
	int				sideBarWidth;
	int				rightBarWidth;
	int				bottomBarH;
	
	/***************** upload vars *****************/
	
	ofButton		cancelButton;
	ofButton		sendButton;
	int				upYoff;
	int				upXoff;
	ofTabs			tabs;
	ofProgressSpinner spinner;
	
	ofSystemCall	systemCall;
	ofxDirList   DIR;
	povImages		preDef;
	int			nImages;
  
  imageArea area;
  titleBar title;
	
	/******************* home screen buttons **************/
	
	ofTimer timeout;
	ofButton predefBut;
	ofButton drawBut;
	ofButton demoBut;
	ofButton homeBut;
	ofImage background;
	
	ofImage drawPic;
	ofImage demoPic;
	ofImage predefPic;
	
	bool buttonPressed();
	bool checkButton(ofButton & t, int x, int y);

};

#endif
