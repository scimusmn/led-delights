#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	predefBut.setTitle("Upload an Image");
	drawBut.setTitle("Draw your own image");
	drawBut.setup(64, 64, "images/edit.png");
	demoBut.setup(128, 128, "images/gears.png");
	predefBut.setup(64, 64, "images/predef.png");
	homeBut.setup(64, 64, "images/home.png");
	demoBut.setTitle("How it works");
	predefBut.setTextSize(24);
	drawBut.setTextSize(24);
	demoBut.setTextSize(24);
	
	drawPic.loadImage("images/draw.png");
	predefPic.loadImage("images/predef.jpg");
	demoPic.loadImage("images/demo.jpg");
	
	arial.setMode(OF_FONT_CENTER);
	
	background.loadImage("images/background.jpg");
	
    nImages = DIR.listDir("upload/images/predefined");
    //you can now iterate through the files as you like
    for(int i = 0; i < min(nImages,8); i++){
		string t=DIR.getPath(i);
		string temp=t.substr(t.find_last_of(".")+1,t.find_last_of(".")+4);
		cout << temp<<endl;
		if(!temp.compare("jpg")) preDef.newImage(DIR.getPath(i), false);
		else if(!temp.compare("gif")) preDef.newImage(DIR.getPath(i), true);
    }
	if(preDef.size()>4) preDef.setSelected(2);
	preDef.setPos(20, 20, 70, ofGetHeight()-40);
	
	arial.loadFont("fonts/DinC.ttf");
	arial.setSize(12);
	
	drawing=false;
	
	width=ofGetWidth();
	height=ofGetHeight();
	colGrid.setup(128, 128, 16);
	
	ofHideCursor();
	
	mousePointer.loadImage("pointer.png");
	sideBarBack.loadImage("woodPanel.jpg");
	
	redoButton.setup(0, 0, 64, 64, "images/redo.png");
	undoButton.setup(0,0,64,64,"images/undo.png");
	uploadButton.setup(0,0,240,240,"images/uploadBig.png");
	copyButton.setup(0, 0, 64, 64, "images/copy.png");
	pasteButton.setup(0, 0, 64, 64, "images/paste.png");
	diyButton.setup(0, 0, 64, 64, "images/hand.png");
	
	cancelButton.setTitle("cancel");
	cancelButton.setTextSize(12);
	
	sendButton.setTitle("send to wheel");
	sendButton.setTextSize(12);
	
	fillButton.setTitle("fill screen");
	fillButton.setTextSize(12);
  
  for (int i=0; i<7; i++) {
    group.newImage("upload/images/"+ofToString(i)+".jpg", false);
  }
  
	if(group.size()) group.setSelected(0);
	group.setPos(20, 20, 70, ofGetHeight()-40);
	sizes.setup(6, 5, 50);
	
	currentSize=sizes.getSize();
	currentColor=colGrid.getColor();
	
	bottomBarH=75;
	sideBarWidth=200;
	rightBarWidth=colGrid.w+60;
	
	upXoff=150;
	upYoff=100;
	
	tabs.addTab("User Generated Images");
	tabs.addTab("Stored Images");
	tabs.setDefaultTab(0);
	
	for (unsigned int i=0; i<group.size(); i++) {
		group[i].image.setCompression(OF_COMPRESS_NONE);
	}
	
  area.setup();
  title.loadFont("fonts/DinC.ttf",40);
  
  bar.setup(area);
}

//--------------------------------------------------------------
void testApp::update(){
	undoButton.setAvailable(group.undoAvailable());
	redoButton.setAvailable(group.redoAvailable());
	if (uploadButton.pressed()) {
		upload();
	}
	if(timeout.justExpired()){
		drawBut.clickUp();
		predefBut.clickUp();
		demoBut.clickUp();
	}
}

void testApp::drawPredef()
{
	area.changeImage(preDef.selected().image);
}

void testApp::drawEdit()
{
	area.changeDrawSize(currentSize);
  area.changeDrawColor(currentColor);
  area.changeImage(group.selected().image);
}

//--------------------------------------------------------------
void testApp::draw(){
	if(!buttonPressed()){
		ofSetColor(128, 128, 128);
		background.draw(0, 0,ofGetWidth(),ofGetHeight());
		predefBut.changeSize(200, 200);
		predefBut.move(ofGetWidth()/4-predefBut.w/2, (ofGetHeight()/2-predefBut.h)/2);
		drawBut.changeSize(200, 200);
		drawBut.move(ofGetWidth()/2-drawBut.w/2, (ofGetHeight()-drawBut.h)/2);
		demoBut.changeSize(200, 200);
		demoBut.move(3*ofGetWidth()/4-demoBut.w/2, (ofGetHeight()/2-demoBut.h)/2);
		//predefBut.drawTextButton(ofGetWidth()/4-predefBut.w/2, ofGetHeight()/2);
		//drawBut.drawTextButton(ofGetWidth()/2-drawBut.w/2, ofGetHeight()/2);
		//demoBut.drawTextButton(3*ofGetWidth()/4-demoBut.w/2, ofGetHeight()/2);
		
		ofSetColor(255, 255, 255);
		predefPic.draw(predefBut.x, predefBut.y,predefBut.w, predefBut.h);
		drawPic.draw(drawBut.x, drawBut.y,drawBut.w, drawBut.h);
		demoPic.draw(demoBut.x, demoBut.y,demoBut.w, demoBut.h);
		
		arial.setSize(30);
		arial.setMode(OF_FONT_CENTER);
		arial.drawString("Send sample image", predefBut.x+predefBut.w/2, predefBut.y-50);
		arial.drawString("Draw your own image for the wheel", drawBut.x+drawBut.w/2, drawBut.y+drawBut.h+50);
		arial.drawString("How it works", demoBut.x+demoBut.w/2, demoBut.y-50);
		
		arial.setMode(OF_FONT_LEFT);
		arial.setSize(10);
		
		ofSetColor(255,255,255);
		mousePointer.draw(mouseX-5, mouseY-2);
	}
	else{
		int imageSize=ofGetHeight()-bottomBarH;
		
		ofVector relCursorPos(mouseX-ofGetWidth()/2,mouseY-imageSize/2);
		
		//---------------------- Drawing Stage --------------------------
		
		if(drawBut.pressed()) drawEdit();
		else if(predefBut.pressed()) drawPredef();
		
    
    area.draw(0,bar.y+bar.h,ofGetWidth(),ofGetHeight()-(bar.y+bar.h)+2);
    if(area.overCircle(mouseX, mouseY)){
      ofSetColor(currentColor.inverse());
      ofRing(mouseX, mouseY, currentSize-3, currentSize);
    }
		//------------------------- Bottom Bar ------------------------
		//bottomBarDraw();
		
		//------------------------- Right Sidebar -----------------------
			if(drawBut.pressed()) drawBarDraw(area.x ,bar.y+bar.h);
		
		//------------------------- Left Sidebar ------------------------
			if(predefBut.pressed()) predefBarDraw();
		
		
		if (systemCall.isRunning()) {
			ofSetColor(0, 0, 0,196);
			ofRect(0, 0, ofGetWidth(), ofGetHeight());
			ofSetColor(255, 255, 255);
			spinner.draw(ofGetWidth()/2, ofGetHeight()/3, 200);
			ofSetColor(255, 255, 255);
			arial.setSize(100);
			arial.setMode(OF_FONT_CENTER);
			arial.drawString("uploading...", ofGetWidth()/2, ofGetHeight()/4*3);
			arial.setSize(12);
			arial.setMode(OF_FONT_LEFT);
		}
		
		if(relCursorPos.mag()>imageSize/2||(uploadButton.pressed()||!drawBut.pressed())){
			ofSetColor(255,255,255);
			mousePointer.draw(mouseX-5, mouseY-2);
		}
	}
  title.draw("Upload images to the wheel",0,0);
  bar.draw(0, title.y+title.h);
}


void testApp::upload(){
	string file="upload/images/playlist";
	if(!drawBut.pressed()) 
		preDef.writePlaylist(file);
	else group.saveImages(file),group.writePlaylist(file);
	systemCall.run("sh "+ofToDataPath(ofGetDirName(file)+"/../upload.sh"));
	uploadButton.setPressed(false);
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

bool testApp::buttonPressed(){ return drawBut.pressed()||predefBut.pressed()||demoBut.pressed(); }

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if (buttonPressed()) {
		timeout.set(60);
		if(!demoBut.pressed()&&uploadButton.clickDown(x, y));
		if(homeBut.clickDown(x, y)){
			drawBut.clickUp();
			predefBut.clickUp();
			demoBut.clickUp();
		}
	}
	if(checkButton(drawBut, x, y));
	else if(checkButton(predefBut, x, y));
	else if(checkButton(demoBut, x, y));
	if(drawBut.pressed()){
		int imageSize=ofGetHeight()-bottomBarH;
		ofVector relCursorPos(x-ofGetWidth()/2,y-imageSize/2);
		if (relCursorPos.mag()<imageSize/2) {
			drawing=true;
			prevPoint=ofPoint(x,y);
		}
		else if(undoButton.clickDown(x, y)) group.undo();
		else if(redoButton.clickDown(x, y)) group.redo();
		else if(sizes.clickDown(x, y)) currentSize=sizes.getSize();
		else if(fillButton.clickDown(x, y)) area.fill();
		else if (colGrid.clickDown(x, y)>=0) {
			currentColor=colGrid.getColor();
		}
	}
	else if(predefBut.pressed()){
		if(preDef.clickDown(x, y)){
			diyButton.setPressed(false);
		}
	}
  area.clickDown(x, y);
}

bool testApp::checkButton(ofButton & t,int x, int y)
{
	ofButton * buts[3];
	buts[0]=&predefBut;
	buts[1]=&drawBut;
	buts[2]=&demoBut;
	bool ret=0;
	int excl=-1;
	for (unsigned int i=0; i<3; i++)
		if(buts[i]->pressed()){
			excl=i; 
			buts[i]->setAvailable(false);
		}
	for (unsigned int i=0; i<3; i++) {
		if(buts[i]==&t&&i!=excl){
			if(t.clickDown(x, y)){
				ret=true;
				for (unsigned int j=0; j<3; j++) {
					if(j!=i){
						buts[j]->clickUp();
					}
				}
				timeout.set(60);
			}
		}
	}
	for (unsigned int i=0; i<3; i++)
		buts[i]->setAvailable(true);
	return ret;
}
	
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	sizes.clickUp();
	undoButton.clickUp();
	redoButton.clickUp();
	copyButton.clickUp();
	pasteButton.clickUp();
	if(fillButton.clickUp()) group.saveState();
	if(drawing&&drawBut.pressed()){
		drawing=false;
		group.saveState();
	}
  area.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
  group.setPos(20, 20, 70, ofGetHeight()-40);
}

