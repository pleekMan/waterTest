#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    cout << "------------------------" << endl << "------------------------" << endl;

    
    
    ofSetFrameRate(60);
	ofBackground(0);
	ofSetVerticalSync(false);
    ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
    
    time = ofGetElapsedTimef();
    
    windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
    waterManager.setup();
    
    // INIT FBOs - BEGIN
    drawLayer.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    maskLayer.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    maskShader.load("maskShaderSimple");
    finalLayer.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    
    drawLayer.begin();
    ofClear(0,0);
    drawLayer.end();
    
    maskLayer.begin();
    ofClear(0,255);
    maskLayer.end();
    
    finalLayer.begin();
    ofClear(0,0);
    finalLayer.end();
    
    // INIT FBOs - END
    

    
}


void testApp::update(){
    
    float t = ofGetElapsedTimef();
    float dt = t - time;
    time = t;
    
    waterManager.update(dt);
    
    // MASKLAYER BEGIN: DRAWING MASK
    maskLayer.begin();
    ofSetColor(255,255);
    ofCircle(ofGetMouseX(), ofGetMouseY(), 50);
    
    maskLayer.end();
    
    // MASKLAYER END ------------------------
    
    
    
    
    // DRAWLAYER BEGIN: DRAWING ALL VISUALS HERE
    drawLayer.begin();
    ofClear(255);
    ofSetColor(255);
    //ofSetColor(0,0,0, 50);
    //ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    waterManager.render();
    
    drawLayer.end();
    
    // DRAWLAYER END ------------------------
    
    
    
    // DRAWING THROUGH SHADER - BEGIN ----------------------
    ofSetColor(255);
    finalLayer.begin();
    ofClear(0,0);
    maskShader.begin();
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    //maskShader.setUniform2f("scaling", horizontalProportion , verticalProportion);
    //maskShader.setUniform1f("nearThreshold", 0.9);
    //maskShader.setUniform1f("farThreshold", 0.1);
    
    maskShader.setUniformTexture("backTex", drawLayer.getTextureReference(), 0);
    maskShader.setUniformTexture("depthTex", maskLayer.getTextureReference(), 1);
    
    //ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    drawLayer.draw(0,0);
    maskShader.end();
    
    finalLayer.end();
    
    // DRAWING THROUGH SHADER - END ----------------------

    
}

void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    drawLayer.draw(0, 0);
    //maskLayer.draw(0,0);
    
    ofSetColor(255);
    //finalLayer.draw(0, 0);
    
    
    ofPushStyle();
    ofSetColor(0, 0, 255);
    ofDrawBitmapString("X: " + ofToString(ofGetMouseX()) + " / Y: " + ofToString(ofGetMouseY()), ofPoint(ofGetMouseX(),ofGetMouseY()));
    ofPopStyle();
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