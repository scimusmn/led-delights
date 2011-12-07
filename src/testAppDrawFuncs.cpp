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

void testApp::predefBarDraw()
{
	
	preDef.draw((sideBarWidth-preDef.w)/2, 20);
}