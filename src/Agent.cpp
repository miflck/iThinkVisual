//
//  agent.cpp
//  agent
//
//  Created by Flückiger Michael on 15.02.17.
//
//

#include "Agent.hpp"
Agent::Agent(){
}

Agent::~Agent(){
}


//--------------------------------------------------------------
void Agent::setup(){
    position.set(ofGetWidth()/2,ofGetHeight()/2,0);
    
    
    float theta1 = ofRandom(0, TWO_PI);
    float theta2 = ofRandom(0, TWO_PI);
    
    float radius = ofRandom(50,500);

    
    ofVec3f p;
    p.x = cos( theta1 ) * cos( theta2 );
    p.y = sin( theta1 );
    p *= radius;
    
    
    homeposition.set(p);

    position.set(p);
    target.set(ofRandom(-ofGetWidth(),ofGetWidth()),ofRandom(-ofGetHeight(),ofGetHeight()),0);
    startMoving();
}

//--------------------------------------------------------------
void Agent::update(){
    move();    

}

//--------------------------------------------------------------
void Agent::draw(){
    ofSetColor(255,0,0);
    ofPushMatrix();
    ofTranslate(position);
    ofDrawRectangle(0,0, 20, 20);
    ofPopMatrix();
    
}



void Agent::move(){
    if(bIsMoving){
        ofVec3f acc;
        ofVec3f p=position;
        ofVec3f t=target;
        ofVec3f dist=t-p;
        ofVec3f desired=t-p;
        desired.normalize();
        float d=dist.length();
        if(d < 500){
            float m = ofMap(d,0,500,0,maxspeed);
            desired*=m;
            
        }else{
            desired*=maxspeed;
        }
        
        ofVec3f steer=desired-velocity;
        steer.limit(0.09);
        acc+=steer;
        velocity+=acc;
        p+=velocity;
        
        if(d<1){
            //p.set(target);
            stopMoving();
        }
        position.set(p);
    }
}

ofVec3f  Agent::getPosition(){
    return position;
}

void Agent::startMoving(){
    bIsMoving=true;
}

void Agent::stopMoving(){
        bIsMoving=false;
    
   // target.set(ofRandom(-ofGetWidth(),ofGetWidth()),ofRandom(-ofGetHeight(),ofGetHeight()));
   // startMoving();


}

void Agent::setRandomPosition(){
    target.set(ofRandom(-ofGetWidth(),ofGetWidth()),ofRandom(-ofGetHeight(),ofGetHeight()));
    startMoving();
    
}


void Agent::setRandomHomePosition(){
    float theta1 = ofRandom(0, TWO_PI);
    float theta2 = ofRandom(0, TWO_PI);
    
    float radius = ofRandom(50,500);
    
    
    ofVec3f p;
    p.x = cos( theta1 );
    p.y = sin( theta1 );
    p *= radius;startMoving();
    homeposition.set(p);
    target.set(homeposition);
    startMoving();


}

void Agent::goHome(){
    target.set(homeposition);
    startMoving();
}
