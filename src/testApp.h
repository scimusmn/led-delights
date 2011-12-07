#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofExtended.h"
#include "povImage.h"
#include <ofxSystemCall.h>

#include "ofSpinner.h"

#include "imageArea.h"
#include "controlBar.h"
#include "drawToolBar.h"

#include "dallasEng.h"

class testApp : public ofBaseApp{
	
	int width;
	int height;
	ofImage sideBarBack;

	public:
	
		void setup();
		void update();
		void draw();
	
	void drawPredef();
	void drawEdit();
	void drawDemo();
	
	void predefBarDraw();

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
	
	povImages		group;
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
	
	
	/**************** position variables ***************/
	
	int				sideBarWidth;
	int				rightBarWidth;
	int				bottomBarH;
	
	/***************** upload vars *****************/
	
	ofProgressSpinner spinner;
	
	ofSystemCall	systemCall;
	ofxDirList   DIR;
	povImages		preDef;
	int			nImages;
  
  imageArea area;
  titleBar title;
  controlBar bar;
	
  drawToolBar tBar;

};

#endif
