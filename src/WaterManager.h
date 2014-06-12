#pragma once

#include "MSAFluid.h"
#include "ParticleSystem.h"

#include "ofMain.h"
#include "ForceParticle.h"

#define USE_GUI

#ifdef USE_GUI
#include "ofxSimpleGuiToo.h"
#endif


class WaterManager {
    
public:
	void setup();
	void update();
	void render();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
    
	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
    
    
    
    
    float                   colorMult;
    float                   velocityMult;
	int                     fluidCellsX;
	bool                    resizeFluid;
	bool                    drawFluid;
	bool                    drawParticles;
	
	msa::fluid::Solver      fluidSolver;
	msa::fluid::DrawerGl	fluidDrawer;
	
	ParticleSystem          particleSystem;
	
	ofVec2f                 pMouse;
    
    vector<ForceParticle> forceParticles;
    vector<ForceParticle> forceParticles2;

    
private:
    
    ofVec2f getForceFromNoiseField(ofVec2f position);
    float getForceFromDepthMap(ofImage *depthMap, ForceParticle *fParticle);
    
    void drawNoiseField();
    void drawForceParticles();
    
    ofImage depthMap;
    ofImage waterBack;
    
    ofFbo waterBuffer;
    
    //------
    float phase;
    float t;
    float complexity;
    //------
    
    void initGUI();
	
};



