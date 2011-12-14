#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofExtended.h"
#include "povImage.h"


#include "imageArea.h"
#include "controlBar.h"
#include "drawToolBar.h"
#include "predefBar.h"

#include "dallasEng.h"

class testApp : public ofBaseApp{
	
	int width;
	int height;

	public:
	
		void setup();
		void update();
		void draw();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		
	
  imageArea area;
  titleBar title;
  controlBar bar;
	
  drawToolBar tBar;
  predefBar pBar;
};

#endif
