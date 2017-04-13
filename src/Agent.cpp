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
    
    float radius = ofRandom(0,ofGetHeight());
    
    

    
    ofVec3f p;
    p.set(1,0,0);
    p.rotate(ofRandom(0,360),ofVec3f(0,0,1));
    //p.x = cos( theta1 ) * cos( theta2 );
    //p.y = sin( theta1 );
    p *= radius;
    
    
    
    
    homeposition.set(p);

    position.set(p);
  //  target.set(ofRandom(-ofGetWidth(),ofGetWidth()),ofRandom(-ofGetHeight(),ofGetHeight()),0);
 //   startMoving();
    target.set(homeposition);

    
    ofVec3f o;
    o.set(ofGetWidth()*2,0,0);
    o.rotate(ofRandom(360), ofVec3f(0,0,1));
    offscreenposition.set(o);
   
    
    
    ofVec3f v;
    v.x = cos( theta1 ) * cos( theta2 );
    v.y = sin( theta1 );
    v *= 0.1;
    
    velocity=v;
    
    seekforce=0.1;
    repulsionforce=0.03;
    spinforce=0.007;
    wanderforce=0.1;
    
    
    o.normalize();
    explosionVec.set(o);
    
    
    homedir=homeposition;
    homedir.normalize();
    
    homelength=homeposition.length();
    
}

//--------------------------------------------------------------
void Agent::update(){
    repulsionTarget.set(0,0,0);
   // ofVec3f d=ofVec3f(0,0,0)-position;
   // d.normalize();
   // d*=10;
    
    //repulsionTarget.set(position+d);

    //move();
    
   // if(ofGetFrameNum()%120==0)repulsionRadius=ofRandom(0,1000);
    repulsionRadius=ofRandom(1000);
   // applyForce(seek(target));
    //applyForce(repulsion(ofVec3f(0,0,0)));
   //applyForce(repulsion(repulsionTarget));

    applyForce(rotateAround(ofVec3f(0,0,0)));
    
    //applyForce(wander());
    //acceleration.limit(0.5);
    
    //ofVec3f dist=target-position;
    //if(dist.length()<200){
     //   makeNewWanderTarget();
   // }

   // applyForce(seek(offscreenposition,offscreenforce));
    applyForce(seek(target,seekforce));
    applyForce(wander(wanderforce));
    applyForce(explosionForce);
    applyForce(implosionForce);


    implosionForce*=0.9;
    explosionForce*=0.9;
    

    if(maxspeed>2)maxspeed*=0.99;
    
    velocity+=acceleration;
    velocity*=0.98;
    position+= velocity;
    acceleration.set(0,0,0);
    
    homedirtheta+=(ofRandom(-homedirthetaDiff,homedirthetaDiff));

    ofVec3f d=homedir*hometheta;
    d.limit(homelength);
    
    target=homeposition-d;
    target.rotateRad(homedirtheta,ofVec3f(0,0,1));
    

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
        
        //desired.normalize();
        float d=dist.length();
        if(d < 500){
           float m = ofMap(d,0,500,0,maxspeed);
          desired*=m;
            
        }else{
            desired*=maxspeed;
        }
        
        ofVec3f steer=desired-velocity;
        steer.limit(0.0005);
        acc+=steer;
        velocity+=acc;
        p+=velocity;
        
        if(d<0.01){
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
    //target.set(ofVec3f(ofRandom(-100,100),ofRandom(-100,100),0));
    startMoving();
}


ofVec3f Agent::repulsion(ofVec3f r){
    ofVec3f p(position);
    ofVec3f rt(r);
    ofVec3f dir=p-rt;
    float d=dir.length();
    dir.normalize();
    if(d < repulsionRadius){
    float m = ofMap(d,0,repulsionRadius,maxrepulsion,0,true);
        dir*=m;
           }else{
               dir*=0;
           }
    dir.limit(repulsionforce);
    
    return dir;
}


void Agent::setSeekForce(float f){
    seekforce=f;
}


void Agent::setOffscreenForce(float f){
    offscreenforce=f;
}

void Agent::setWanderForce(float f){
    wanderforce=f;
}

void Agent::setRepulsionForce(float f){
    repulsionforce=f;
}

void Agent::setSpinForce(float f){
    spinforce=f;
}

float Agent::getSpinForce(){
    return spinforce;
}

ofVec3f Agent::rotateAround(ofVec3f t){
   
    ofPoint delta = position - t;
    float d=delta.length();
    delta.normalize();
    ofVec3f spin;
    spin.x += -delta.y*maxspeed;
    spin.y += delta.x*maxspeed;
    spin.limit(spinforce);

    return spin;
    

}


ofVec3f Agent::seek(ofVec3f t){
    
    
    ofVec3f p(position);
    ofVec3f desired=t-p;
    desired.normalize();
    desired*=maxspeed;
    ofVec3f steer=desired-velocity;
    steer.limit(seekforce);
    return steer;
}

ofVec3f Agent::seek(ofVec3f t, float f){
    ofVec3f p(position);
    ofVec3f desired=t-p;
    desired.normalize();
    desired*=maxspeed;
    ofVec3f steer=desired-velocity;
    steer.limit(f);
    return steer;
}


void Agent::makeNewWanderTarget(){
    float wanderR=30.f;
    float wanderD=50.0f;
    float change=0.25f;
    
   
    ofVec3f circlepos=velocity;
    circlepos.normalize();
    circlepos*=wanderD;
    circlepos+=position;

    ofVec3f rad=velocity;
    rad.normalize();
    rad*=wanderR;
    wandertheta+=ofRandom(-change,change);
    rad.rotate(wandertheta, ofVec3f(0,0,1));
    target=position+rad;
    
    
    
    
    
}

ofVec3f Agent::wander(float f){
    float wanderR=50;
    float wanderD=500.0f;
    float change=0.3f;
    
    
    ofVec3f circlepos=velocity;
    circlepos.normalize();
    circlepos*=wanderD;
    //circlepos+=position;
    
    ofVec3f rad=velocity;
    rad.normalize();
    rad*=wanderR;
    wandertheta+=ofRandom(-change,change);
    rad.rotateRad(wandertheta, ofVec3f(0,0,1));
    ofVec3f wandertarget=position+rad+circlepos;
    
    
    ofVec3f p(position);
    ofVec3f desired=wandertarget-p;
    desired.normalize();
    desired*=maxspeed;
    ofVec3f steer=desired-velocity;
    steer.limit(f);
    return steer;
   }



void Agent::applyForce(ofVec3f f){
    acceleration+= f;
}


void Agent::addExplosion(){
    explosionForce=explosionVec*3;
}

void Agent::addImplosion(){
    
    ofVec3f p(position);
    ofVec3f desired=homeposition-p;
    desired.normalize();
    implosionForce=desired*3;
}

void Agent::setMaxSpeed(float s){
   // if(s>maxspeed)maxspeed+=0.1;
    maxspeed=s;
}

void Agent::addSpeed(){
    // if(s>maxspeed)maxspeed+=0.1;
    maxspeed+=0.2;
}
