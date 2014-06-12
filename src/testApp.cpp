#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetFrameRate(60);
	ofBackground(0);
	ofSetVerticalSync(false);
    ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
    
    
    
    windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
    waterManager.setup();
    
    cout << "------------------------" << endl << "------------------------" << endl;
}


void testApp::update(){
    
    waterManager.update();
    
}

void testApp::draw(){
    //ofBackground(0);
    
    ofSetColor(0,0,0, 50);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    waterManager.render();
}







void testApp::keyPressed  (int key){
    switch(key) {
		case '1':
			waterManager.fluidDrawer.setDrawMode(msa::fluid::kDrawColor);
			break;
            
		case '2':
			waterManager.fluidDrawer.setDrawMode(msa::fluid::kDrawMotion);
			break;
            
		case '3':
			waterManager.fluidDrawer.setDrawMode(msa::fluid::kDrawSpeed);
			break;
			
		case '4':
			waterManager.fluidDrawer.setDrawMode(msa::fluid::kDrawVectors);
			break;
            
		case 'd':
			waterManager.drawFluid ^= true;
			break;
			
		case 'p':
			waterManager.drawParticles ^= true;
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'r':
			waterManager.fluidSolver.reset();
			break;
			
		case 'b': {
            //			Timer timer;
            //			const int ITERS = 3000;
            //			timer.start();
            //			for(int i = 0; i < ITERS; ++i) fluidSolver.update();
            //			timer.stop();
            //			cout << ITERS << " iterations took " << timer.getSeconds() << " seconds." << std::endl;
		}
			break;
			
    }
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
    waterManager.mouseMoved(x, y);
    
    /*
    ofVec2f eventPos = ofVec2f(x, y);
    ofVec2f eventPreviousPos = ofVec2f(ofGetPreviousMouseX(), ofGetPreviousMouseY());
    ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
    ofVec2f mouseVel = ofVec2f(eventPos - eventPreviousPos) * 100 / ofGetWindowSize();
    cout << "mouseVel: " + ofToString(mouseVel) << endl;
    waterManager.addToFluid(mouseNorm, mouseVel, true, true);
    //eventPreviousPos = eventPos;
    */
    
}

void testApp::mouseDragged(int x, int y, int button) {
    
    waterManager.mouseDragged(x,y, button);
	/*
     ofVec2f eventPos = ofVec2f(x, y);
     ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
     ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
     addToFluid(mouseNorm, mouseVel, false, true);
     pMouse = eventPos;
     */
    
}