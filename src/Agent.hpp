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
    float maxspeed=15;
    
    
    ofPoint pos;               //Position
    ofPoint vel;               //Velocity
    float time;                //Time of living
    float lifeTime;            //Allowed lifetime
    bool live;
    
    ofVec3f  getPosition();
    
    void goHome();
    
    void setRandomPosition();
    void setRandomHomePosition();

    
private:

    ofVec3f position;
    ofVec3f target;
    ofVec3f velocity;

    ofVec3f homeposition;

    
};

#endif /* agent_hpp */
