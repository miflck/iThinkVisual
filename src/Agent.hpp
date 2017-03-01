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
    float maxspeed=10;
    float maxrepulsion=20;

    
    
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
    
    ofVec3f repulsion(ofVec3f r);
    
    
    ofVec3f rotateAround(ofVec3f r);

    
    float seekforce;
    float repulsionforce;
    float repulsionRadius;
    
    float spinforce;

    
    
private:

    ofVec3f position;
    ofVec3f target;
    ofVec3f velocity;

    ofVec3f homeposition;
    
    ofVec3f acceleration;
    
    ofVec3f repulsionTarget;

    
};

#endif /* agent_hpp */
