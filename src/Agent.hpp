//
//  agent.hpp
//  agent
//
//  Created by Flückiger Michael on 15.02.17.
//
//

#ifndef Agent_hpp
#define Agent_hpp

#include <stdio.h>
#include "ofMain.h"
class Agent {
public:
    
    Agent();
    virtual ~Agent();

    void setup();              //Start particle
    void update();   //Recalculate physics
    void draw();               //Draw particle
    
    
    
    void move();
    void startMoving();
    void stopMoving();
    bool bIsMoving=false;
    float maxspeed=8;
    float maxrepulsion=10;

    
    
    ofPoint pos;               //Position
    ofPoint vel;               //Velocity
    float time;                //Time of living
    float lifeTime;            //Allowed lifetime
    bool live;
    
    ofVec3f  getPosition();
    
    void goHome();
    
    void setRandomPosition();
    void setRandomHomePosition();

    ofVec3f cicleVec;
    
    void applyForce(ofVec3f f);
    
    
    ofVec3f seek(ofVec3f t);
    
    ofVec3f seek(ofVec3f t, float f);

    
    ofVec3f repulsion(ofVec3f r);
    ofVec3f adhesion(ofVec3f r);
    
    
    ofVec3f wander(float f);

    
    
    ofVec3f rotateAround(ofVec3f r);
    
    void makeNewWanderTarget();

    
    float seekforce;
    float repulsionforce;
    float repulsionRadius;
    float wanderforce;
    float offscreenforce;
    float spinforce;


    void setSpinForce(float f);
    void setSeekForce(float f);
    void setRepulsionForce(float f);
    void setWanderForce(float f);
    void setOffscreenForce(float f);


    
    float getSpinForce();
    void addExplosion();
    
    void addImplosion();

    float hometheta;
    float homedirthetaDiff;
    
    void setMaxSpeed(float s);
    void addSpeed();

    
    
private:

    ofVec3f position;
    ofVec3f target;
    ofVec3f velocity;

    ofVec3f homeposition;
    
    ofVec3f acceleration;
    
    ofVec3f repulsionTarget;
    
    
    ofVec3f offscreenposition;
    
    ofVec3f explosionVec;
    ofVec3f explosionForce;

    ofVec3f implosionVec;
    ofVec3f implosionForce;
    
    

    float wandertheta;
    
    float homedirtheta;


    ofVec3f homedir;
    float homelength;
    
};

#endif /* agent_hpp */
