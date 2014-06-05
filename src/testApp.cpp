#include "testApp.h"

char sz[] = "[Rd9?-2XaUP0QY[hO%9QTYQ`-W`QZhcccYQY[`b";


float tuioXScaler = 1;
float tuioYScaler = 1;

//--------------------------------------------------------------
void testApp::setup() {
	for(int i=0; i<strlen(sz); i++) sz[i] += 20;
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
	
#ifdef USE_TUIO
	tuioClient.start(3333);
#endif
    
	
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
    gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
    gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
    
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    gui.show();
#endif
	
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
	
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
    
    
    //-------------
    int separation = 50;
    for (int y=0; y < ofGetWindowHeight(); y+= separation) {
        
        for (int x=0; x < ofGetWindowWidth(); x+= separation) {
            ForceParticle fParticle = ForceParticle();
            fParticle.setup(ofVec2f(x ,y), ofVec2f(ofRandom(1,3),ofRandom(-0.5, 0.5)));
            forceParticles.push_back(fParticle);
        }
    }
    
    //-------------
    
    //------------ PERLIN NOISE VARS
    t = ofRandom(3);
    complexity = 3; // NOISE SCALE
    //timeSpeed = .005; // MOTION SPEED
    phase = TWO_PI; // separate u-noise from v-noise
    
    depthMap.loadImage("depthMap.png");
}


void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void testApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
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


void testApp::update(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
#ifdef USE_TUIO
	tuioClient.getMessage();
	
	// do finger stuff
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);
        float vx = tcur->getXSpeed() * tuioCursorSpeedMult;
        float vy = tcur->getYSpeed() * tuioCursorSpeedMult;
        if(vx == 0 && vy == 0) {
            vx = ofRandom(-tuioStationaryForce, tuioStationaryForce);
            vy = ofRandom(-tuioStationaryForce, tuioStationaryForce);
        }
        addToFluid(ofVec2f(tcur->getX() * tuioXScaler, tcur->getY() * tuioYScaler), ofVec2f(vx, vy), true, true);
    }
#endif
    
    //----------------
    
    
    for (int i=0; i<forceParticles.size(); i++) {
        
        ofVec2f fParticlePos = forceParticles[i].getPosition();
        
        //ofVec2f forceAtField = getField(fParticlePos); // 0 -> 1
        //forceAtField = (forceAtField * 2) - 1; // transpose to -1 -> 1

        float fAtDepth = getForceFromDepthMap(&depthMap, &(forceParticles[i]));
        ofVec2f forceAtDepthMap = ofVec2f(0,fAtDepth);
       
       forceParticles[i].update(forceAtDepthMap);
        
       
        ofVec2f eventPos = fParticlePos;
        ofVec2f eventNorm = ofVec2f(eventPos) / ofGetWindowSize();
        ofVec2f eventVel = forceParticles[i].getVelocity() / ofGetWindowSize();
        addToFluid(eventNorm, eventVel, true, true);
        
        /*
        ofVec2f eventPos = forceParticles[i].getPosition();
        ofVec2f eventNorm = ofVec2f(eventPos) / ofGetWindowSize();
        ofVec2f eventVel = forceParticles[i].getVelocity() / ofGetWindowSize();
        addToFluid(eventNorm, eventVel, true, true);
        //pMouse = eventPos;
         */
        
    }
    //----------------
	
	fluidSolver.update();
    
    //t += 0.05;

}

void testApp::draw(){
    //ofBackground(0);
    ofSetColor(255);
    depthMap.draw(0, 0);
    ofSetColor(0,0,100, 10);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
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
	if(drawParticles)
		particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid);
	
    //	ofDrawBitmapString(sz, 50, 50);
    
    
    //-----------------
    //PERLIN NOISE
    
    //drawNoiseField();
    
    //----------------
    
    //-------------
    
    for (int i=0; i<forceParticles.size(); i++) {
        forceParticles[i].render();
        
        if(i == 10){
            ofFill();
            ofSetColor(0, 255, 0);
            ofDrawBitmapString(ofToString(i) + " : " + ofToString(forceParticles[i].getPosition().y), forceParticles[i].getPosition());
            
           //ofDrawBitmapString(ofToString(getForceFromDepthMap(depthMap, forceParticles[i].getPosition())), forceParticles[i].getPosition() + ofVec2f(0,-10));
            
            /*
            float pxBrightness = depthMap.getColor((int)forceParticles[i].getPosition().x, (int)forceParticles[i].getConstantPosY()).r; // IS WHITE, ANYWAY...
            float bottomPxBrightness = depthMap.getColor((int)forceParticles[i].getPosition().x, (int)(forceParticles[i].getConstantPosY() + 1)).r;
            float finalForce = pxBrightness / 255;
            
            //cout << "- " + ofToString((int)forceParticles[i].getPosition().x) + " . " + ofToString((int)forceParticles[i].getConstantPosY()) << endl;
            //cout << "Px: "+ ofToString(pxBrightness) + " / Bttm Px: " + ofToString(bottomPxBrightness) + " / Diff: " + ofToString(bottomPxBrightness - pxBrightness) << endl;
            
            if (bottomPxBrightness - pxBrightness > 0.) {
                finalForce *= -1;
            }
            
            cout << "LastIncl: " + ofToString(forceParticles[i].getLastInclination()) << endl;

            
            //cout << "Ff: " + ofToString(finalForce) << endl;
            //cout << "------------------------------------" << endl;
            
            ofRect(300, ofGetWindowHeight() * 0.5, 20, ofMap(finalForce, -1, 1, -300,300));
            */
            //ofDrawBitmapString(ofToString(pxBrightness) + " : " + ofToString(bottomPxBrightness), forceParticles[i].getPosition() + ofVec2f(0,-20));
            //ofDrawBitmapString(ofToString(finalForce), forceParticles[i].getPosition() + ofVec2f(-100,-20));



        }
    }
    
    
    
    //-------------
    

    
#ifdef USE_GUI
	gui.draw();
#endif
}

ofVec2f testApp::getField(ofVec2f position) {
    
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

void testApp::drawNoiseField(){
    
    ofNoFill();
    int cellSize = 20;
    
    for (int y=0; y < ofGetWindowHeight(); y+= cellSize) {
        for (int x=0; x < ofGetWindowWidth(); x+= cellSize) {
            
            
            ofVec2f coordinates = ofVec2f(x,y);
            ofVec2f force = getField(coordinates); // 0 -> 1
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

float testApp::getForceFromDepthMap(ofImage *depthMap, ForceParticle *fParticle){
    
    int fParticleX = (int)fParticle->getPosition().x;
    int fParticleY = (int)fParticle->getConstantPosY(); // TO ALWAYS GRAB THE SAME PIXEL.Y
    
    // GET BRIGHTNESS FROM PX AND THE ONE BELOW
    float pxBrightness = depthMap->getColor(fParticleX, fParticleY).r; // IS WHITE, ANYWAY...
    float bottomPxBrightness = depthMap->getColor(fParticleX, fParticleY + 1).r;
    
    //float pxBrightness = 0.5;
    //float bottomPxBrightness = 0.5;


    float finalForce = pxBrightness / 255;
    
    // GET SLOPE (INCLINATION)
    int difference = bottomPxBrightness - pxBrightness;
    
    // IF PIXELS ARE THE SAME, USE LAST INCLINATION
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




void testApp::keyPressed  (int key){
    switch(key) {
		case '1':
			fluidDrawer.setDrawMode(msa::fluid::kDrawColor);
			break;
            
		case '2':
			fluidDrawer.setDrawMode(msa::fluid::kDrawMotion);
			break;
            
		case '3':
			fluidDrawer.setDrawMode(msa::fluid::kDrawSpeed);
			break;
			
		case '4':
			fluidDrawer.setDrawMode(msa::fluid::kDrawVectors);
			break;
            
		case 'd':
			drawFluid ^= true;
			break;
			
		case 'p':
			drawParticles ^= true;
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'r':
			fluidSolver.reset();
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
    /*
	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, true, true);
	pMouse = eventPos;
     */
}

void testApp::mouseDragged(int x, int y, int button) {
	
    ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, false, true);
	pMouse = eventPos;
    
}