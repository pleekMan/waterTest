//
//  WaterManager.cpp
//  example
//
//  Created by PleekMan on 6/6/2014.
//
//
#include "WaterManager.h"

//--------------------------------------------------------------
void WaterManager::setup() {
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
    velocityMult = 1;
	
	//windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
	
	//  THE  GUI NEEDS TO BE INITIALIZED IN ORDER TO INIT/UPDATE NECESSARY VARS FOR PARTICLE VIZ
    initGUI();
    
    
    //-------------
    // FORCE PARTICLES - BOTTOM
    int separation = 30;
    for (int y=600; y < ofGetWindowHeight(); y+= separation) {
        for (int x=0; x < ofGetWindowWidth(); x+= separation) {
            ForceParticle fParticle = ForceParticle();
            fParticle.setup(ofVec2f(x,y));
            fParticle.setConstantVelocity(ofVec2f(3,0));
            fParticle.setBoundaries(600, ofGetWindowHeight(), 0, ofGetWindowWidth());
            forceParticles.push_back(fParticle);
        }
    }
    
    // FORCE PARTICLES - RIGHT
    //separation = 30;
    for (int y=400; y < ofGetWindowHeight(); y+= separation) {
        for (int x=0; x < 400; x+= separation) {
            ForceParticle fParticle = ForceParticle();
            fParticle.setup(ofVec2f(x,y));
            fParticle.setConstantVelocity(ofVec2f(0,3));
            fParticle.setBoundaries(400, ofGetWindowHeight(), 0, 400);
            forceParticles2.push_back(fParticle);
        }
    }
    
    //-------------
    
    //------------ PERLIN NOISE VARS
    t = ofRandom(3);
    complexity = 3; // NOISE SCALE
    //timeSpeed = .005; // MOTION SPEED
    phase = TWO_PI; // separate u-noise from v-noise
    
    depthMap.loadImage("depthMap.png");
    waterBack.loadImage("riverBed.png");
    
    waterBuffer.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGB);
    waterBuffer.begin();
    ofClear(255);
    waterBuffer.end();
}


void WaterManager::update(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
    
    //----------------
    
    // FORCE PARTICLES - BOTTOM
    //---
    for (int i=0; i<forceParticles.size(); i++) {
        
        //ofVec2f forceAtField = getField(fParticlePos); // 0 -> 1
        //forceAtField = (forceAtField * 2) - 1; // transpose to -1 -> 1
        
        float forceAtDepthMap = getForceFromDepthMap(&depthMap, &(forceParticles[i]));
        
        forceParticles[i].update(forceAtDepthMap);
        
        
        ofVec2f eventPos = forceParticles[i].getPosition();
        ofVec2f eventNorm = ofVec2f(eventPos) / ofGetWindowSize();
        ofVec2f eventVel = forceParticles[i].getVelocity() * 4 / ofGetWindowSize();
        addToFluid(eventNorm, eventVel, true, true);
        
    }
    
    // FORCE PARTICLES - RIGHT
    //---
    for (int i=0; i<forceParticles2.size(); i++) {
        
        //ofVec2f forceAtField = getField(fParticlePos); // 0 -> 1
        //forceAtField = (forceAtField * 2) - 1; // transpose to -1 -> 1
        
        float forceAtDepthMap = getForceFromDepthMap(&depthMap, &(forceParticles[i]));
        
        forceParticles2[i].update(forceAtDepthMap);
        
        
        ofVec2f eventPos = forceParticles2[i].getPosition();
        ofVec2f eventNorm = ofVec2f(eventPos) / ofGetWindowSize();
        ofVec2f eventVel = forceParticles2[i].getVelocity() * 4 / ofGetWindowSize();
        addToFluid(eventNorm, eventVel, true, true);
        
    }
    
    //----------------
	
	fluidSolver.update();
    
    //t += 0.05;
    
    //-----------------------------
    //- WATER BUFFER DRAW - BEGIN
    waterBuffer.begin();
    
    //ofBackground(0);
    ofSetColor(255);
    waterBack.draw(0, 0);
    //ofSetColor(0,0,0, 50);
    //ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    /*
     if(drawFluid) {
     ofClear(0);
     glColor3f(1, 1, 1);
     fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
     } else {
     //		if(ofGetFrameNum()%5==0)
     fadeToColor(0, 0, 0, 0.05);
     }
     */
	if(drawParticles){
		particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid);
	}
    //ofDrawBitmapString(ofToString(fluidSolver.getAvgSpeed()), 20, 20);
    
    
    drawForceParticles();
    
    //-----------------
    //PERLIN NOISE
    
    //drawNoiseField();
    
    //----------------
    
    //-------------
    
    
    
#ifdef USE_GUI
	gui.draw();
#endif

    
    waterBuffer.end();
    
    //- WATER BUFFER DRAW - END
    //-----------------------------

    
}

void WaterManager::render(){
    ofSetColor(255);
    waterBuffer.draw(0,0);
}

void WaterManager::drawNoiseField(){
    
    ofNoFill();
    int cellSize = 20;
    
    for (int y=0; y < ofGetWindowHeight(); y+= cellSize) {
        for (int x=0; x < ofGetWindowWidth(); x+= cellSize) {
            
            
            ofVec2f coordinates = ofVec2f(x,y);
            ofVec2f force = getForceFromNoiseField(coordinates); // 0 -> 1
            ofSetColor(force.length() * 200);
            
            force = (force * 2) - 1; // transpose to -1 -> 1
            
            
            
            //ofColor depthMapBrightness = depthMap.getColor(x, y).getBrightness();
            
            //ofSetColor(depthMapBrightness);
            ofCircle(x, y, 3);
            ofSetColor(255);
            ofLine(x, y, x + (force.x * 20), y + (force.y * 20));
            
            //ofRect(x, y, cellSize, cellSize);
            
        }
    }
    
}

ofVec2f WaterManager::getForceFromNoiseField(ofVec2f position) {
    
    /*
     This is the magic method that samples a 2d slice of the 3d noise field. When
     you call this method with a position, it returns a direction (a 2d vector). The
     trick behind this method is that the u,v values for the field are taken from
     out-of-phase slices in the first dimension: t + phase for the u, and t - phase
     for the v.
     */
    
    
    float normx = ofNormalize(position.x, 0, ofGetWidth());
    float normy = ofNormalize(position.y, 0, ofGetHeight());
    float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
    float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
    return ofVec2f(u, v);
}

float WaterManager::getForceFromDepthMap(ofImage *depthMap, ForceParticle *fParticle){
    
    int fParticleX;
    int fParticleY;
    float pxBrightness;
    float nextPxBrightness;
    
    if (fParticle->isFlowVertical()) {
        fParticleX = (int)fParticle->getConstantPos().x; // TO ALWAYS GRAB THE SAME PIXEL.X
        fParticleX = ofClamp(fParticleX, 0, ofGetWindowWidth());
        fParticleY = (int)fParticle->getPosition().y;
        
        cout << ofToString(fParticleX) + " : " + ofToString(fParticleY) << endl;
        
        // GET BRIGHTNESS FROM PX AND THE ONE NEXT TO IT
        pxBrightness = depthMap->getColor(fParticleX, fParticleY).r; // IS WHITE, ANYWAY...
        nextPxBrightness = depthMap->getColor(fParticleX + 1, fParticleY).r;
    } else {
        fParticleX = (int)fParticle->getPosition().x;
        fParticleX = ofClamp(fParticleX, 0, ofGetWindowWidth());
        fParticleY = (int)fParticle->getConstantPos().y; // TO ALWAYS GRAB THE SAME PIXEL.Y
        
        // GET BRIGHTNESS FROM PX AND THE ONE BELOW
        pxBrightness = depthMap->getColor(fParticleX, fParticleY).r; // IS WHITE, ANYWAY...
        nextPxBrightness = depthMap->getColor(fParticleX, fParticleY + 1).r;
    }
    
    float finalForce = pxBrightness / 255;
    
    // GET SLOPE (INCLINATION)
    int difference = nextPxBrightness - pxBrightness;
    
    // IF PIXELS ARE THE SAME, USE LAST SLOPE
    if (difference == 0) {
        difference = (int)fParticle->getLastInclination();
    }
    
    // CORRECT DIRECTION OF SLOPE
    if (difference > 0.) {
        finalForce *= -1;
    }
    
    // UPDATE SLOPE
    fParticle->setLastInclination(difference);
    
    
    
    
    //ofVec2f finalForce = ofVec2f(0,0);
    return finalForce;
}

void WaterManager::drawForceParticles(){
    
    for (int i=0; i<forceParticles.size(); i++) {
        
        forceParticles[i].render();
        /*
        if(i == 100){
            ofFill();
            ofSetColor(0, 255, 0);
            ofDrawBitmapString(ofToString(i) + " : " + ofToString(forceParticles[i].getVelocity()), forceParticles[i].getPosition());
        }
         */
    }
    
    for (int i=0; i<forceParticles2.size(); i++) {
        
        forceParticles2[i].render();
        /*
        if(i == 100){
            ofFill();
            ofSetColor(0, 255, 0);
            ofDrawBitmapString(ofToString(i) + " : " + ofToString(forceParticles[i].getVelocity()), forceParticles[i].getPosition());
        }
         */
    }
    
}

void WaterManager::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void WaterManager::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
            //			Color drawColor(CM_HSV, (getElapsedFrames() % 360) / 360.0f, 1, 1);
			ofColor drawColor;
			drawColor.setHsb((ofGetFrameNum() % 255), 255, 255);
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			
			if(drawParticles)
				particleSystem.addParticles(pos * ofVec2f(ofGetWindowSize()), 10);
		}
		
		if(addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
		
    }
}

void WaterManager::initGUI(){
    
#ifdef USE_GUI
    gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
    gui.addButton("resizeFluid", resizeFluid);
    gui.addSlider("colorMult", colorMult, 0, 100);
    gui.addSlider("velocityMult", velocityMult, 0, 100);
    gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
    gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
    gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
    gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
    gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
    gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
    gui.addToggle("fs.doRGB", fluidSolver.doRGB);
    gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
    gui.addToggle("drawFluid", drawFluid);
    gui.addToggle("drawParticles", drawParticles);
    gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
    gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    //gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
    //gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
    
    gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
    gui.setDefaultKeys(true);
    gui.setAutoSave(true);
    gui.show();
#endif
    
}

//--------------------------------------------------------------
void WaterManager::mouseMoved(int x, int y){
    
    
    ofVec2f eventPos = ofVec2f(x, y);
    //ofVec2f eventPreviousPos = ofVec2f(ofGetPreviousMouseX(), ofGetPreviousMouseY());
    ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
    ofVec2f mouseVel = ofVec2f(eventPos - pMouse) * 200 / ofGetWindowSize();
    mouseVel.x = ofClamp(mouseVel.x, -3, 3);
    mouseVel.y = ofClamp(mouseVel.y, -3, 3);

    cout << "mouseVel: " + ofToString(mouseVel) << endl;
    addToFluid(mouseNorm, mouseVel, true, true);
    pMouse = eventPos;
    
    /*
    ofVec2f eventPos = ofVec2f(x, y);
    ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
    ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
    addToFluid(mouseNorm, mouseVel, true, true);
    pMouse = eventPos;
     */
    
}

void WaterManager::mouseDragged(int x, int y, int button) {
	
    ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, false, true);
	pMouse = eventPos;
    
}