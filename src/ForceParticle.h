//
//  ForceParticle.h
//  example
//
//  Created by PleekMan on 29/5/2014.
//
//

#ifndef __example__ForceParticle__
#define __example__ForceParticle__

#endif /* defined(__example__ForceParticle__) */

#include "ofMain.h"

#pragma once

class ForceParticle {
public:
    
    ForceParticle(){};
    
    ofVec2f	pos, vel, previousPos;
    ofVec2f constantVel;
    ofVec2f constantPosY;
    
    int inclination;

	
    void setup(ofVec2f initPos, ofVec2f initVel);
    void update();
    void update(ofVec2f _force);
    void render();
    
    ofVec2f getPosition();
    float getConstantPosY();
    ofVec2f getPreviousPos();
    ofVec2f getVelocity();
    ofVec2f getField(ofVec2f position);
    
    void setLastInclination(int _inclination);
    int getLastInclination();
    
    float noiseTime;
};

