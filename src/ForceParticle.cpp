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
    vel = initVel;
    noiseTime = ofRandom(100);
    
    
    
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

void ForceParticle::render(){
    
    //ofSetColor(0,255,0);
    ofSetColor(255);
    ofCircle(pos, 1);
    
    
    
    
}

ofVec2f ForceParticle::getPosition(){
    return pos;
}

ofVec2f ForceParticle::getVelocity(){
    return vel;
}


