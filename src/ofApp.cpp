#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //int numAgents=50000;
    
     totalNumAgents=70000;
    minNumAgents=100;
    renderdAgents=minNumAgents;
    targetAgentsNum=minNumAgents;
    
    
    for (int i=0;i<totalNumAgents;i++){
        
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
    
    
    //sizes[0]=ofVec3f(50);
    //sizes[1]=ofVec3f(20);
    //sizes[2]=ofVec3f(10);
    
    pointsize=100;
    
    
    // fill in the colors and vertices
    vbo.setNormalData(&sizes[0], totalNumAgents, GL_STATIC_DRAW);

    ofBackgroundHex(0x000000);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // load the texure
    ofDisableArbTex();
    ofLoadImage(texture, "dot.png");
    shader.load("shader/shader");
    
    // set the camera distance
    camDist  = 1305;
    camera.setDistance(camDist);
    
    
    // fbos

    
    
    fbo.allocate(ofGetWidth()*2, ofGetHeight()*2, GL_RGB);

    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();

    

    //Sound
    soundStream.printDeviceList();
    
    

    
    //if you want to set a different device id
   // soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    int bufferSize = 256;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
    
    gui.setup(); // most of the time you don't need a name
    //gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
   // gui.add(homeforce.setup("homeforce", 1, 0, 5));
    ofParameterGroup agentsparams;
    agentsparams.setName("Agents");
    
    renderdAgentsSlider.set("renderd Agents", minNumAgents, 0.0f, totalNumAgents);
    renderdAgentsSlider.addListener(this, &ofApp::totalAgentsChanged);
    agentsparams.add(renderdAgentsSlider);
    wanderforce.set("wanderforce", 0.5, 0.0f, 1.5f);
    agentsparams.add(wanderforce);
    rotationforce.set("rotation", 0.f, -.05f, 0.05f);
    agentsparams.add(rotationforce);
    gui.add(agentsparams);
    
    ofParameterGroup audioparams;
    audioparams.setName("Audio Settings");
    maxvol.set("maxvol", 0.05, 0.001f, 0.07f);
    audioparams.add(maxvol);
    
    
 
    

    
    gui.add(audioparams);

    ofParameterGroup visual;
    visual.setName("Visuals");
    fadespeed.set("fadespeed", 10, 0.0f, 200.0f);
    visual.add(fadespeed);
    alpha.set("alpha", 80, 10.0f, 255.0f);
    visual.add(alpha);
    brightness.set("brightness", 50, 10.0f, 255.0f);
    visual.add(brightness);
    colorangle.set("colorangle", 160, 0.0f, 255.0f);
    visual.add(colorangle);
    targetPointSize.set("Point Size", 100, 1.0f, 200);
    visual.add(targetPointSize);

    
    edge.set("edge",false);
    visual.add(edge);

    gui.add(visual);
    
    ofParameterGroup devices;

    
    list=soundStream.getDeviceList();
    for(auto device : list) {
        ofxButton b;
        buttons.push_back(b);
     //   ofParameter<bool> d;
      //  d.set(device.name,false);
      //  devices.add(d);
        
    }
    // gui.add(devices);
   // devices.addListener(this,&ofApp::buttonPressed);

    
    for(int i=0;i<buttons.size();i++){
        gui.add(buttons[i].setup(list[i].name));
        buttons[i].addListener(this,&ofApp::buttonPressed);

    }
    
    edge.addListener(this, &ofApp::onEdgeChange);

    
    gui.loadFromFile("settings.xml");

    me=1;
    
    
    
    mainOutputSyphonServer.setName("Screen Output");
    //individualTextureSyphonServer.setName("Texture Output");
    
    mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");

    
    
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    current_msg_string = 0;

    
    
}

void ofApp::totalAgentsChanged(int & totalagents){
    targetAgentsNum=totalagents;
    renderdAgents=totalagents;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    while(receiver.hasWaitingMessages()){
    
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/agents/add"){
            int anz = m.getArgAsInt32(0);
            addAgents(anz);
        }
        
        
        // check for mouse moved message
        if(m.getAddress() == "/agents/total"){
            int anz = m.getArgAsInt32(0);
            if(anz > totalNumAgents)anz=totalNumAgents;
            renderdAgents=anz;
            targetAgentsNum=anz;
        }
        
        
        if(m.getAddress() == "/agents/remove"){
            int anz = m.getArgAsInt32(0);
            removeAgents(anz);
        }
        
        
        if(m.getAddress() == "/agents/pointsize"){
            int size = m.getArgAsInt32(0);
            if(size<5)size=5;
            if(size>200)size=200;
            targetPointSize=size;
        }
        
        
        if(m.getAddress() == "/rotation"){
            int rot = m.getArgAsInt32(0);
            rotationforce=ofMap(rot,-255,255,-0.05,0.05);
            cout<<rotationforce<<endl;
        }
        
        
        if(m.getAddress() == "/wanderforce"){
            int w = m.getArgAsInt32(0);
            wanderforce=ofMap(w,0,255,0.5,1.5);
            cout<<wanderforce<<endl;
        }
        
        
        
        if(m.getAddress() == "/color"){
            int angle = m.getArgAsInt32(0);
            colorangle=angle;
        }
        
        
        if(m.getAddress() == "/brightness"){
            int b = m.getArgAsInt32(0);
            brightness=b;
        }
 
        if(m.getAddress() == "/fadespeed"){
            int b = m.getArgAsInt32(0);
            fadespeed=b;
        }
        
        if(m.getAddress() == "/alpha"){
            int b = m.getArgAsInt32(0);
            alpha=b;
        }
        
        
        if(m.getAddress() == "/volume"){
            int w = m.getArgAsInt32(0);
            maxvol=ofMap(w,0,255,0.001f,0.07f);

        }
        
        
    }
    
    
    
    /*for(int i=0; i<agents.size(); i++) {
        ofFill();
        ofSetHexColor(0x90d4e3);
        agents[i].get()->update();
    }*/
    
    points.clear();
    sizes.clear();
    
    for(int i=0;i<renderdAgents;i++){
        agents[i]->update();
        points.push_back(agents[i]->getPosition());
        float size = ofRandom(pointsize, pointsize);
        sizes.push_back(ofVec3f(size));
    }
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    // fill in the colors and vertices

    //vbo.setColorData(&sizes[0], 10000, GL_STATIC_DRAW);

    fbo.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofEnableAlphaBlending();
    ofSetColor(0,0,0,fadespeed);
    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();
    
    
    
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    float mappedVol = ofMap(smoothedVol, 0.0, 0.05, 0.0, ofGetHeight()*2, true);
    float mappedVolTheta = ofMap(smoothedVol, 0.0, 0.05, 0.0, 1, true);
    
    float mappedSpeed = ofMap(smoothedVol, 0.0, maxvol, 2, 10, true);
    float mappedForce = ofMap(smoothedVol, 0.0, maxvol, 0, 2, true);


    //cout<<smoothedVol<<" "<<mappedVol<<" "<<mappedSpeed<<endl;

    
    
    for(int i=0;i<agents.size();i++){
        agents[i]->hometheta=mappedVol;
        agents[i]->homedirthetaDiff=mappedVolTheta;
        if(mappedSpeed>2.5) agents[i]->addSpeed();
        
       // if(ofRandom(1)<0.2){
            agents[i]->setSeekForce(mappedForce);
       // }else{
        //    agents[i]->setSeekForce(0);
       // };
        agents[i]->setWanderForce(wanderforce);
        agents[i]->setSpinForce(rotationforce);
    }

    
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

   // cout<<renderdAgents<<endl;
    if(renderdAgents<targetAgentsNum){
        renderdAgents+=5;
    }
 
    // cout<<renderdAgents<<endl;
    if(renderdAgents>targetAgentsNum){
        renderdAgents-=5;
    }
    
    shrink();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    ofSetColor(255);
    fbo.begin();

    glDepthMask(GL_FALSE);

//    ofSetColor(255, 100, 90,50);
    
    ofColor c;
    c.setHsb(colorangle, 150, brightness);
    
    //ofSetColor(90, 100, 255,80);
    ofSetColor(c,alpha);
    
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
    
    camera.begin();
/*
    if(ofGetFrameNum()%3==0){
        me++;
        if(me>renderdAgents)me=1;
        other=me-1;
        polyline.addVertex(points[me]);
        
        
        if (polyline.size() > 5){
            polyline.getVertices().erase(
            polyline.getVertices().begin()
            );
        }
        
       // if(polyline>10)polyline.
    }
    ofSetColor(255, 100, 90,10);
    ofSetLineWidth(15);
    //ofDrawLine(points[me], points[other]);
    
    polyline.draw();
  */

    
    camera.end();


    
    ofDisableBlendMode();
    
      fbo.end();
    camera.begin();
    fbo.draw(-fbo.getWidth()/2,-fbo.getHeight()/2);
   // ofDrawBox(0,0, 0, 10, 10, 10);

    camera.end();
    

    
    /*
    ofEnableAlphaBlending();
    camera.begin();
    for (unsigned int i=0; i<points.size(); i++) {
        ofSetColor(255, 80);
        ofVec3f mid = points[i];
        mid.normalize();
        mid *= 1000;
        ofDrawLine(points[i], mid);
    } 
    camera.end();
*/
    
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

   // ofSetColor(0, 255, 0);

   // ofDrawBox(ofGetWidth()/2, ofGetHeight()/2, 0, 10, 10, 10);
    
    
    
    
    
    
    
    mClient.draw(50, 50);
    
    mainOutputSyphonServer.publishScreen();
        
    
    // Off-Syphonscreen stuff...
    if(!bHide){
        gui.draw();
    }
    
    
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

void ofApp::addAgents(int num){
    targetAgentsNum=targetAgentsNum+num;
    if(targetAgentsNum>totalNumAgents)targetAgentsNum=totalNumAgents;
    cout<<"targetAgentsNum "<<targetAgentsNum<<" "<<totalNumAgents<<endl;
}


void ofApp::removeAgents(int num){
    targetAgentsNum=targetAgentsNum-num;
    if(targetAgentsNum<minNumAgents)targetAgentsNum=minNumAgents;
    cout<<"targetAgentsNum "<<targetAgentsNum<<" "<<totalNumAgents<<endl;
}


void ofApp::shrink(){
    int diff=targetPointSize-pointsize;
    if(diff>0)pointsize+=1;
    if(diff<0)pointsize-=1;
    //cout<<"t"<<targetPointSize<<" diff "<<diff<<endl;
    }

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    
    if(key == 'p'){
        bHide = !bHide;
        gui.saveToFile("settings.xml");

    }
    
    
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
                    agents[i]->setSpinForce(rotationforce);
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
                //for(int i=0;i<agents.size();i++){
                  //  agents[i]->setRandomHomePosition();
               // }
                //renderdAgents+=100;
                addAgents(1000);
                
            
                
               // if(renderdAgents>agents.size())renderdAgents=agents.size();
                break;
            
                
            case'1':
                targetPointSize=200;
                break;
                
                case'2':
                targetPointSize=50;
                break;
                
            case'3':
                targetPointSize=20;
                break;
                
                
            case'4':
                targetPointSize=10;
                break;
                
            case'5':
                targetPointSize=5;
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
    
  //  cout<<mx<<" "<<my<<endl;
    

    for(int i=0;i<agents.size();i++){
        
      //  agents[i]->setSeekForce(mx);
       // agents[i]->setWanderForce(my);
        
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
void ofApp::onEdgeChange(bool &val){
    if(val) {
        ofLoadImage(texture, "dot.png");
    }else{
        ofLoadImage(texture, "dot_sharp.png");
    }

}

void ofApp::buttonPressed(const void * sender){
    ofxButton * button = (ofxButton*)sender;
    cout<<button->getName()<<endl;


    for(int i=0;i<list.size();i++){
        if(list[i].name==button->getName()){
            soundStream.close();
            soundStream.setDevice(list[i]);
            soundStream.setup(this, 0, 2, 44100, 256, 4);
        }
        
    }
    
    
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

