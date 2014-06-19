//
//  ForceParticle.cpp
//  example
//
//  Created by PleekMan on 29/5/2014.
//
//

#include "ForceParticle.h"

#define K 2.
#define MASS 2.
#define DAMPING 5.
#define VEL_MAX 200.

void ForceParticle::setup(ofVec2f _initPos, string _constantPath){
    
    constantPath.loadPath(_constantPath);
    constantPath.setUsePath(true);
    constantPath.setPosition(ofPoint(0,0));
    constantPath.setDuration(ofRandom(10, 20));
    constantPath.animateTo(ofPoint(ofGetWindowWidth(), ofGetWindowHeight() - 200));
    
    initPos = _initPos;
    previousPos = pos;
    constantVel = ofVec2f(3,0);
    constantPos = ofVec2f(pos);
    forceMultiplier = 10000;
    
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
    //constantPos += constantVel;
    
    if(isVertical){
        pos.x = constantPos.x + (_force * forceMultiplier);
    } else {
        pos.y = constantPos.y + (_force * forceMultiplier);
    }
    
    vel = pos - previousPos;
    previousPos = pos;
    
    if(pos.x > boundary.right){
        pos.x = boundary.left;
        pos.y = initPos.y;
        
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

void ForceParticle::updateXY(ofVec2f _force, float strength, float dt){
    
    vel = ofVec2f(0,0);
    
    constantPath.update(dt);
    
    externalForce = _force;
    forceMultiplier = strength * 30000;
    //----------------
    
    //float accel = dangle - angle;
    ofVec2f accel = externalForce * forceMultiplier;
    accel *= (K/MASS);
    accel -= (DAMPING/MASS) * vel;
    vel += (accel*dt);
    
    // ---------------
    
    //pos.x += constantVel.x + ofRandom(-10,10);
    //pos.y = constantPosY.y + (_force.y * forceMultiplier) + ofRandom(-10,10);
    
    //constantPos += constantVel;
    constantPos = ofVec2f(constantPath.getCurrentPosition().x, constantPath.getCurrentPosition().y) + initPos;
    
    pos = constantPos + vel;
    
    vel = pos - previousPos;
    previousPos = pos;
    
    if (pos.x > boundary.right - 10 || pos.y >= boundary.bottom - 10) {
    //if (pos.x > boundary.right - 5) {
    //if(constantPath.getPercentDone() > 0.95){
        constantPath.setPosition(pathOrigin);
        constantPath.animateTo(pathTarget);
        previousPos = initPos;
    }
    
    
}

void ForceParticle::render(){
    
    //ofSetColor(0,255,0);
    ofSetColor(color);
    ofCircle(pos, 2);
    
    ofSetColor(color);
    ofCircle(pathTarget, 10);
    //ofSetColor(color);
    ofLine(pathOrigin, pathTarget);
    //ofSetColor(255, 0, 0);
    ofCircle(pathOrigin, 10);
    
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
    //return pos - previousPos;
    return vel;
}

ofVec2f ForceParticle::getForceApplied(){
    return externalForce;
}

ofVec2f ForceParticle::getForceAppliedMultiplied(){
    return externalForce * forceMultiplier;
}

void ForceParticle::setPathOrigin(ofVec2f _pathOrigin){
    pathOrigin = _pathOrigin;
}

void ForceParticle::setPathTarget(ofVec2f _pathTarget){
    pathTarget = _pathTarget;
}

void ForceParticle::setColor(ofColor _color){
    color = _color;
}

void ForceParticle::setLastInclination(int _inclination){
    inclination = _inclination;
}

void ForceParticle::setLastInclinationXY(ofVec2f _inclination){
    inclinationXY = _inclination;
}

int ForceParticle::getLastInclination(){
    return inclination;
}
ofVec2f ForceParticle::getLastInclinationXY(){
    return inclinationXY;
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




