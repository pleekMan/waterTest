#pragma once


#include "ofMain.h"
#include "WaterManager.h"
#include "VideoManager.h"
#include "ofxSyphon.h"

#define USE_GUI

#ifdef USE_TUIO
#include "ofxTuio.h"
#define tuioCursorSpeedMult				0.5	// the iphone screen is so small, easy to rack up huge velocities! need to scale down 
#define tuioStationaryForce				0.001f	// force exerted when cursor is stationary
#endif


#ifdef USE_GUI 
#include "ofxSimpleGuiToo.h"
#endif

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);

	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
    
    bool drawingEnabled;

    WaterManager waterManager;
    VideoManager videoManager;
    
    ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;
	
	ofxSyphonClient mClient;

    
private:
    
    ofFbo drawLayer;
    ofFbo maskLayer;
    ofShader maskShader;
    ofFbo finalLayer;
    
    float time;
    
	
};



