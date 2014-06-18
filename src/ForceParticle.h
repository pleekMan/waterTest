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
#include "ofxAnimatableOfPoint.h"

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
    
    ofVec2f initPos;
    ofVec2f	pos, vel, previousPos;
    ofxAnimatableOfPoint constantPath;
    ofVec2f pathOrigin;
    ofVec2f pathTarget;
    ofVec2f constantVel;
    ofVec2f constantPos;
    ofVec2f externalForce;
    float forceMultiplier;
    
    int inclination;
    bool isVertical;
    ofVec2f inclinationXY;
    
    void setup(ofVec2f _initPos, string _constantPath);
    //void update();
    void update(float _force);
    void updateXY(ofVec2f _force, float strength, float dt);

    void render();
    
    void setConstantVelocity(ofVec2f vel);
    void setBoundaries(int upper, int bottom, int left, int right);
    void setPathOrigin(ofVec2f _pathOrigin);
    void setPathTarget(ofVec2f _pathTarget);

    
    ofVec2f getPosition();
    ofVec2f getConstantPos();
    ofVec2f getPreviousPos();
    ofVec2f getVelocity();
    ofVec2f getForceApplied();
    ofVec2f getForceAppliedMultiplied();
    ofVec2f getField(ofVec2f position);
    
    void setLastInclination(int _inclination);
    void setLastInclinationXY(ofVec2f _inclination);

    int getLastInclination();
    ofVec2f getLastInclinationXY();

    bool isFlowVertical();
    
    float noiseTime;
};

