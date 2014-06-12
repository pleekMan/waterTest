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
    
    struct boundaries {
        int top = 0;
        int bottom = 0;
        int left = 0;
        int right = 0;
    } boundary;
    
public:
    
    ForceParticle(){};
    
    ofVec2f	pos, vel, previousPos;
    ofVec2f constantVel;
    ofVec2f constantPos;
    float forceMultiplier;
    
    int inclination;
    bool isVertical;
    
    void setup(ofVec2f initPos);
    //void update();
    void update(float _force);
    void render();
    
    void setConstantVelocity(ofVec2f vel);
    void setBoundaries(int upper, int bottom, int left, int right);

    
    ofVec2f getPosition();
    ofVec2f getConstantPos();
    ofVec2f getPreviousPos();
    ofVec2f getVelocity();
    ofVec2f getField(ofVec2f position);
    
    void setLastInclination(int _inclination);
    int getLastInclination();
    bool isFlowVertical();
    
    float noiseTime;
};

