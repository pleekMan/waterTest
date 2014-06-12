//
//  ForceParticle.cpp
//  example
//
//  Created by PleekMan on 29/5/2014.
//
//

#include "ForceParticle.h"

void ForceParticle::setup(ofVec2f initPos){
    
    pos = initPos;
    previousPos = pos;
    //constantVel = ofVec2f(3,0);
    constantPos = ofVec2f(pos);
    forceMultiplier = 100;
    
    noiseTime = ofRandom(100);
    
    inclination = 0;
    
    
    
}

/*
 void ForceParticle::update(){
 
 float yNoise = ofNoise(noiseTime);
 yNoise *= (yNoise - 0.5)  * 10;
 pos += vel;
 pos.y += yNoise;
 
 if(pos.x > ofGetWindowWidth()){
 pos.x = 0;
 }
 
 if (pos.y > ofGetWindowHeight()) {
 pos.y = 0;
 }
 if (pos.y < 0) {
 pos.y = ofGetWindowHeight();
 }
 
 noiseTime += 0.02;
 
 }
 */

void ForceParticle::update(float _force){
    
    
    //pos.x += constantVel.x + ofRandom(-10,10);
    //pos.y = constantPosY.y + (_force.y * forceMultiplier) + ofRandom(-10,10);
    
    pos += constantVel;
    
    if(isVertical){
        pos.x = constantPos.x + (_force * forceMultiplier);
    } else {
        pos.y = constantPos.y + (_force * forceMultiplier);
    }
    
    vel = pos - previousPos;
    previousPos = pos;
    
    if(pos.x > boundary.right * 1.0){
        pos.x = boundary.left;
        //previousPos.x = boundary.left;
    }
    
    if (pos.y > boundary.bottom){
        pos.y = boundary.top;
        previousPos.y = boundary.top;
    }
    if (pos.y < boundary.top) {
        pos.y = boundary.bottom;
        pos.y = boundary.bottom;
    }
    
    //noiseTime += 0.02;
    
}

void ForceParticle::render(){
    
    //ofSetColor(0,255,0);
    ofSetColor(255,255,0);
    ofCircle(pos, 1);
    
}

void ForceParticle::setConstantVelocity(ofVec2f vel){
    constantVel = vel;
    if(constantVel.x > constantVel.y){
        isVertical = false;
    } else {
        isVertical = true;
    }
}

ofVec2f ForceParticle::getPosition(){
    return pos;
}

ofVec2f ForceParticle::getConstantPos(){
    return constantPos;
}

ofVec2f ForceParticle::getVelocity(){
    return vel;
}

void ForceParticle::setLastInclination(int _inclination){
    inclination = _inclination;
}

int ForceParticle::getLastInclination(){
    return inclination;
}

bool ForceParticle::isFlowVertical(){
    return isVertical;
}


void ForceParticle::setBoundaries(int upper, int bottom, int left, int right){
    boundary.top = upper;
    boundary.bottom = bottom;
    boundary.left = left;
    boundary.right = right;

    
}




