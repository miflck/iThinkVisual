#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    int numAgents=50000;
    
    for (int i=0;i<numAgents;i++){
        
    //    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));

        
        shared_ptr<Agent> a(new Agent);
        a->setup();
        agents.push_back(a);
        
        // we are passing the size in as a normal x position
        float size = ofRandom(5, 5);
        sizes.push_back(ofVec3f(size));
        colors.push_back(ofRandom(1));


    }
    // fill in the colors and vertices
    vbo.setNormalData(&sizes[0], numAgents, GL_STATIC_DRAW);

    ofBackgroundHex(0x000000);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // load the texure
    ofDisableArbTex();
    ofLoadImage(texture, "dot.png");
    shader.load("shader/shader");
    
    // set the camera distance
    camDist  = 1605;
    camera.setDistance(camDist);
    
    
    // fbos

    
    
    fbo.allocate(ofGetWidth()*2, ofGetHeight()*2, GL_RGB);

    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();

    



}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    /*for(int i=0; i<agents.size(); i++) {
        ofFill();
        ofSetHexColor(0x90d4e3);
        agents[i].get()->update();
    }*/
    
    points.clear();
    
    for(int i=0;i<agents.size();i++){
    agents[i]->update();
       points.push_back(agents[i]->getPosition());
    }
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    // fill in the colors and vertices

    //vbo.setColorData(&sizes[0], 10000, GL_STATIC_DRAW);

    fbo.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofEnableAlphaBlending();
    ofSetColor(0,0,0,10);
    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    ofSetColor(255);
    fbo.begin();

    glDepthMask(GL_FALSE);

    ofSetColor(255, 100, 90,50);
    
    // this makes everything look glowy :)
   ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    // bind the shader and camera
    // everything inside this function
    // will be effected by the shader/camera
    shader.begin();
    camera.begin();
    
    // bind the texture so that when all the points
    // are drawn they are replace with our dot image
    texture.bind();
    vbo.draw(GL_POINTS, 0, (int)points.size());
    texture.unbind();
    
    camera.end();
    shader.end();
 
    
    ofDisablePointSprites();
    ofDisableBlendMode();
    
      fbo.end();
    camera.begin();
    fbo.draw(-fbo.getWidth()/2,-fbo.getHeight()/2);
    camera.end();
    
    
    
   /*
    for(int i=0; i<agents.size(); i++) {
        ofFill();
        ofSetHexColor(0x90d4e3);
        agents[i].get()->draw();
    }*/

    
    glDepthMask(GL_TRUE);
    
    ofSetColor(255, 100);
    ofDrawRectangle(0, 0, 250, 90);
    ofSetColor(0);
    string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
    info += "Total Points "+ofToString((int)points.size())+"\n";

    
    ofDrawBitmapString(info, 20, 20);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
        switch (key) {
            case 'h':
            for(int i=0;i<agents.size();i++){
                agents[i]->goHome();
            }
            break;
            
            case 'r':
            for(int i=0;i<agents.size();i++){
                agents[i]->setRandomPosition();
            }
            break;
                
            case 'a':
                for(int i=0;i<agents.size();i++){
                    agents[i]->setRandomHomePosition();
                }
                break;
            
            default:
            break;
        }
        

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
