#pragma once

#include "ofMain.h"
#include "Agent.hpp"
#include "ofxGui.h"
#include "ofxSyphon.h"

#include "ofxOsc.h"


// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20




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

    int renderdAgents;
    int pointsize;
    
    
    
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

    
    bool bHide;

     ofParameter<float> homeforce;
    ofParameter<float> wanderforce;
     ofParameter<float> rotationforce;

    
   // ofxFloatSlider wanderforce;


    ofParameter<float> maxvol;

    
    ofParameter<float> fadespeed;
    ofParameter<float> alpha;
    ofParameter<float> brightness;
    ofParameter<int> colorangle;
    ofParameter<int> targetPointSize;

    


    ofParameter<bool> edge;
    void onEdgeChange(bool &val);
    
    vector<ofxButton > buttons;
    
    vector<ofParameter<bool>> devices;
    
    void buttonPressed(const void * sender);

    vector<ofSoundDevice> list;
    
    ofxColorSlider color;
    ofxPanel gui;

    

    int me, other;
    
    vector<ofPolyline> polylines;
    ofPolyline polyline;
    
    void addAgents(int num);
    
    void removeAgents(int num);

    
   int targetAgentsNum;
    
    int totalNumAgents;
    int minNumAgents;
    
    void shrink();
    


    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    
    ofxSyphonClient mClient;
    
    
    ofxOscReceiver receiver;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];

    
		
};
