//
//  ForceParticle.cpp
//  example
//
//  Created by PleekMan on 29/5/2014.
//
//

#include "ForceParticle.h"

void ForceParticle::setup(ofVec2f initPos, ofVec2f initVel){
    
    pos = initPos;
    previousPos = pos;
    vel = initVel;
    constantVel = ofVec2f(3,0);
    constantPosY = ofVec2f(0,initPos.y);
    noiseTime = ofRandom(100);
    
    inclination = 0;
    
}

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

void ForceParticle::update(ofVec2f _force){
    
    
    //vel = constantVel;
    pos.x += constantVel.x;
    pos.y = constantPosY.y + (_force.y * 100);
    
    vel = pos - previousPos;
    
    previousPos = pos;
    
    if(pos.x > ofGetWindowWidth()){
        pos.x = 0;
    }
    
    if (pos.y > ofGetWindowHeight()) {
        pos.y = 0;
    }
    if (pos.y < 0) {
        pos.y = ofGetWindowHeight();
    }
    
    //noiseTime += 0.02;
    
}

void ForceParticle::render(){
    
    //ofSetColor(0,255,0);
    ofSetColor(255,255,0);
    ofCircle(pos, 1);
    
    
    
    
}

ofVec2f ForceParticle::getPosition(){
    return pos;
}

float ForceParticle::getConstantPosY(){
    return constantPosY.y;
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



