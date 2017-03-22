#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //int numAgents=50000;
    
    int numAgents=50000;

    
    for (int i=0;i<numAgents;i++){
        
    //    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));

        
        shared_ptr<Agent> a(new Agent);
        a->setup();
        agents.push_back(a);
        
        // we are passing the size in as a normal x position
        float size = ofRandom(5, 10);
       // float size = ofRandom(20, 20);

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

    

    //Sound
    soundStream.printDeviceList();
    
    //if you want to set a different device id
    soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

    


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
    ofSetColor(0,0,0,20);
    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();
    
    
    
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    float mappedVol = ofMap(smoothedVol, 0.0, 0.05, 0.0, ofGetHeight()*2, true);
    float mappedVolTheta = ofMap(smoothedVol, 0.0, 0.05, 0.0, 1, true);
    
    float mappedSpeed = ofMap(smoothedVol, 0.0, 0.05, 2, 10, true);
    float mappedForce = ofMap(smoothedVol, 0.0, 0.05, 0, 2, true);


    //cout<<smoothedVol<<" "<<mappedVol<<" "<<mappedSpeed<<endl;

    
    
    for(int i=0;i<agents.size();i++){
        agents[i]->hometheta=mappedVol;
        agents[i]->homedirthetaDiff=mappedVolTheta;
        if(mappedSpeed>2.5) agents[i]->addSpeed();
        
        if(ofRandom(1)<0.2){
            agents[i]->setSeekForce(mappedForce);

        }else{
            agents[i]->setSeekForce(0);

        };

            //  agents[i]->setSeekForce(mx);
        
        
       
        //agents[i]->setOffscreenForce(scaledVol);
        //agents[i]->setRepulsionForce(scaledVol*10);
        
    }

    
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    ofSetColor(255);
    fbo.begin();

    glDepthMask(GL_FALSE);

//    ofSetColor(255, 100, 90,50);
    ofSetColor(90, 100, 255,80);

    
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
   // ofDrawBox(0,0, 0, 10, 10, 10);

    camera.end();
    
    
    
   /*
    for(int i=0; i<agents.size(); i++) {
        ofFill();
        ofSetHexColor(0x90d4e3);
        agents[i].get()->draw();
    }*/

    
    glDepthMask(GL_TRUE);
    /*
    ofSetColor(255, 100);
    ofDrawRectangle(0, 0, 250, 90);
    ofSetColor(0);
    string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
    info += "Total Points "+ofToString((int)points.size())+"\n";

    
    ofDrawBitmapString(info, 20, 20);
    */

   // ofDrawBox(ofGetWidth()/2, ofGetHeight()/2, 0, 10, 10, 10);
    
    
    // draw the average volume:
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()-200, ofGetHeight()-200, 0);
    ofScale(0.5,0.5);
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofDrawRectangle(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofDrawCircle(200, 200, scaledVol * 190.0f);
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);		
    
    ofPopMatrix();
    ofPopStyle();

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
        switch (key) {
            case 'h':
            for(int i=0;i<agents.size();i++){
                agents[i]->goHome();
            }
            break;
                
                
            case 'c':
                for(int i=0;i<agents.size();i++){
                    agents[i]->setSeekForce(ofRandom(0.3,0.5));
                    agents[i]->setRepulsionForce(0.3);

                }
                break;
                
            case 'C':
                for(int i=0;i<agents.size();i++){
                   // agents[i]->setSeekForce(0.3);
                   // agents[i]->setRepulsionForce(ofRandom(0.5,0.9));

                }
                break;
                
                
        
                
            case 'i':
                for(int i=0;i<agents.size();i++){
                    agents[i]->setSpinForce(agents[i]->getSpinForce()+0.01);
                }
                break;
                
                
            case 'I':
                for(int i=0;i<agents.size();i++){
                    agents[i]->setSpinForce(agents[i]->getSpinForce()-0.01);
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
    switch (key) {
            
        case 'e':
            for(int i=0;i<agents.size();i++){
                agents[i]->addExplosion();
            }
            break;
            
        case 'f':
            for(int i=0;i<agents.size();i++){
                agents[i]->addImplosion();
            }
            break;
            
        default:
            break;
    }
   
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    float mx=ofMap(x,0,ofGetWidth(),0,1,true);
    float my=ofMap(y,0,ofGetHeight(),0,5,true);
    
    cout<<mx<<" "<<my<<endl;
    

    for(int i=0;i<agents.size();i++){
        
      //  agents[i]->setSeekForce(mx);
        agents[i]->setWanderForce(my);
        
    }
    
    

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

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}

