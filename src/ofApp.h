#pragma once

#include "ofMain.h"
#include "Agent.hpp"


class ofApp : public ofBaseApp{
    
    

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    ofEasyCam camera;
    float camDist;

    
    //list<Agent*> agents;
    vector<shared_ptr<Agent> > agents;
    
    
    // vector to store all values
    vector <ofVec3f > points;
    vector <ofVec3f > sizes;
    vector <int > colors;

    
    ofVbo vbo;
    ofShader shader;
    
    ofTexture texture;
    
    ofFbo fbo;
    
    int numAgents;


    
    
    
    // Sound
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;

    
    

		
};
