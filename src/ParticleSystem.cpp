/*
 *  ParticleSystem.cpp
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	curIndex = 0;
    
    //---------------
    
    billboards.getVertices().resize(MAX_PARTICLES);
	billboards.getColors().resize(MAX_PARTICLES);
	billboards.getNormals().resize(MAX_PARTICLES,ofVec3f(0));
    
    for (int i=0; i < MAX_PARTICLES; i++) {
		
		//billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboards.getVertices()[i].set(ofVec3f(particles[i].getPosition()));
		
		//billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(96, 160), 255, 255));
        billboards.getColors()[i].set(ofColor(255));

        //billboardSizeTarget[i] = ofRandom(4, 64);
		
	}
    
    if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard"); // TOMA ESTE
	}
    
    billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
    
    // we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
    particleTexture.loadImage("particle.png");
	ofEnableAlphaBlending();
    
    //---------------
    
}

void ParticleSystem::updateAndDraw(const msa::fluid::Solver &solver, ofVec2f windowSize, bool drawingFluid) {
    ofVec2f invWindowSize(1.0f / windowSize.x, 1.0f / windowSize.y);

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    //glBlendFunc(GL_ONE,GL_ONE);
//	glEnable(GL_LINE_SMOOTH);
    ofSetLineWidth(1);
	
	for(int i=0; i<MAX_PARTICLES; i++) {
		if(particles[i].alpha > 0) {
			particles[i].update(solver, windowSize, invWindowSize);
			particles[i].updateVertexArrays(drawingFluid, invWindowSize, i, posArray, colArray);
            
            // -------------------
            billboards.getVertices()[i] = particles[i].pos;
            billboards.setNormal(i,ofVec3f(1,0,0));
            float pointSize = particles[i].getVelocity().length();
            billboards.getVbo().setAttributeData(billboardShader.getAttributeLocation("point_size"), &pointSize, 1,  sizeof(particles) / sizeof(*particles)  ,sizeof(float));
            // -------------------

            //billboards.getVbo().s
            //billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i] * ofNoise(t+i),0,0));
            //ofNoFill();
            //ofCircle(billboards.getVertices()[i], 10);
            //particleTexture.draw(billboards.getVertices()[i]);

		}
	}
    
    
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, posArray);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colArray);
	
	glDrawArrays(GL_LINES, 0, MAX_PARTICLES * 2);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_BLEND);
    
    //-----------------
    
    glEnable(GL_BLEND);
    //glEnable(GL_TEXTURE_2D);
    //glBlendFunc(GL_ONE,GL_SRC_COLOR);
    
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    billboardShader.begin();
    
    ofEnablePointSprites(); // not needed for GL3/4
    
    glPointSize(15);

	particleTexture.getTextureReference().bind();
	
    billboards.draw();
    
	particleTexture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	
	billboardShader.end();
    
    
    
    glDisable(GL_BLEND);
    
    //-----------------
    

    
}


void ParticleSystem::addParticles(const ofVec2f &pos, int count){
	for(int i=0; i<count; i++)
		addParticle(pos + msa::Rand::randVec2f() * 15);
}


void ParticleSystem::addParticle(const ofVec2f &pos) {
	particles[curIndex].init(pos.x, pos.y);
	curIndex++;
	if(curIndex >= MAX_PARTICLES) curIndex = 0;
}
