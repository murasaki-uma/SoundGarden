//
//  ofxButterflyPrimitive.h
//  ofxButterflyMesh
//
//  Created by Â≤©ÂüéÂãáÁ£® on 4/20/16.
//
//

#ifndef ofxButterflyPrimitive_h
#define ofxButterflyPrimitive_h
#include "ofMain.h"

#include <stdio.h>
enum DIRECTION {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class ofxButterflyPrimitive {
public:
    ofxButterflyPrimitive(void) {};
    ~ofxButterflyPrimitive(void) {};
    ofQuaternion latRot, longRot, spinQuat;
    float longitude = ofRandom(0,360);
    float localRad = ofRandom(50,150);
	float radius = ofRandom(0, 200);
	float radian = ofRandom(0, TWO_PI);
	float speed = ofRandom(0.005, 0.01);
	float addRad = 0;
    void setup(ofVec3f _position, float _scale) {
        
        // --------------- init -------------- //
        position = _position;
        scale = _scale;
        butterflyMesh.clear();
        
        // ----------- add Vertex ------------ //
        butterflyMesh.enableIndices();
        butterflyMesh.addVertex(ofVec3f(0,0,0));
        butterflyMesh.addIndex(0);
        // left
        butterflyMesh.addVertex(ofVec3f(-10,9,0)*_scale);
        butterflyMesh.addVertex(ofVec3f(-5,-7,0)*_scale);
        butterflyMesh.addIndex(1);
        butterflyMesh.addIndex(2);
        
        // right
        butterflyMesh.addVertex(ofVec3f(10,9,0)*_scale);
        butterflyMesh.addVertex(ofVec3f(5,-7,0)*_scale);
        butterflyMesh.addIndex(0);
        butterflyMesh.addIndex(3);
        butterflyMesh.addIndex(4);
        
        butterflyVbo.setMesh(butterflyMesh, GL_DYNAMIC_DRAW);
        
        wingWidth = butterflyMesh.getVertex(0).x - butterflyMesh.getVertex(3).x;
        //wingHeight = abs(butterflyMesh.getVertex(1).y) + butterflyMesh.getVertex(3).
        
		ampSpeed = ofRandom(1.5, 2.5);
        
        // --------- tween setup ------- //
        //unsigned delay = 2000;
        //unsigned duration = 1000;
        //tweenquint.setParameters(10,easingquint,ofxTween::easeOut,0,40,duration,delay);
        
        // --------- quatanion --------- //
        
        //latRot.makeRotate(cities[i].latitude, 1, 0, 0);
        //longRot.makeRotate(cities[i].longitude, 0, 1, 0);
        //spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
        
        //our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
        //ofVec3f center = ofVec3f(0,0,300);

    }
    
//    
//    void setDirection(DIRECTION d) {
//        switch (d) {
//            case LEFT:
//                direction.
//        }
//    }
    
    
    void draw () {
        ofPushMatrix();
        
        //longRot.makeRotate(longitude,0,1,0);
        //ofVec3f worldPoint = longRot * spinQuat * position;
 
        //ofTranslate(position);
        
        
        ofRotateX(-90);
		ofRotateZ(ofRadToDeg(radian));
		ofTranslate(position);
		ofTranslate(addRad, 0);
		ofSetColor(255, 200);
        ofFill();
        butterflyVbo.draw(GL_TRIANGLES, 0, butterflyVbo.getNumIndices());
        
        
        
        // ************************** DEBUG ************************** //
        
        if(DEBUG_MODE) {
            
//            for(int i = 0; i < butterflyMesh.getNumVertices(); i++) {
//                ofSetColor(200,200,0);
//                ofDrawSphere(butterflyMesh.getVertex(i), 0.2);
//            }
//            
//            ofPolyline XZ;
//            for(float i = 0; i <= 2*PI; i+= PI/12) {
//                float x = wingWidth * cos(i);
//                float z = wingWidth * sin(i);
//                float y = 0;
//                ofSetColor(255, 100, 0);
//                ofDrawBitmapString(ofToString(i), x,y,z);
//                XZ.addVertex(ofVec3f(x,y,z));
//            }
//            XZ.close();
//            ofSetColor(255, 0, 0);
//            XZ.draw();
//            
            
            
            ofPushStyle();
            
            ofSetColor(0, 255, 0);
            ofDrawLine(0, 0, 4, 0);
            
            ofSetColor(0, 0, 255);
            ofDrawLine(0, 0, 0, 4);
            
            ofSetColor(255, 0, 0);
            ofDrawLine(0, 0, 0, 0, 0,4);
            
            ofPopStyle();
            
            
            
//            float x = wingWidth * cos(piCounterTop);
//            float z = wingWidth * sin(piCounterTop);
//            float y = 0;
//            ofDrawSphere(x, y, z, 1);
            //cout <<  << endl;
            //ofDrawRectangle(0,20,tweenquint.update(),20);
        }
        
        
        
        
        ofPopMatrix();
    }

	void setSpeed(float Speed) {
		speed = Speed;
	}

	void addRadius(float Radius) {
		addRad = Radius;
	}
    
    void update(float timer) {
		//position.x = radius * cos(radian);
		//position.z = radius * sin(radian);
		radian += speed;

		wingWidth = butterflyMesh.getVertex(0).x - butterflyMesh.getVertex(3).x;
        ofMesh updateMesh = butterflyMesh;
        int backRadStepScale = 4;
        int topRadStepScale = 4;
        float radStep =  PI / 12;
		//timer += 0.01;
		float wingAmp = radStep*4.5;

		//timer += 2* fmod(ofGetElapsedTimef(),TWO_PI);
		//timer += ofNoise(timer, position.x)*ofRandom(0.4, 0.8);
		timer *= ampSpeed;
		//cout << timer << endl;
        
        float radDiff;
        if(DEBUG_MODE){
            radDiff = ofMap( ofGetMouseX(), 0, ofGetWidth(), -wingAmp, wingAmp);
        } else {
			radDiff = wingAmp * sin(timer);
			//cout << radDiff << endl;
        }
       
        
  
        
        float x1,x2,z1,z2, x3,x4,z3,z4;
        //if(DEBUG_MODE) {
            float radLeftTop = 2*PI + radDiff;
            float radLeftUnder = 2*PI + radDiff*0.65;
            float radRightTop = PI + radDiff;
            float radRightUnder = PI + radDiff*0.65;
            ampZ = 5 * sin(radLeftTop);
            x1  = wingWidth * cos(radLeftTop);
            x2  = wingWidth * cos(radLeftUnder);
            x3 = wingWidth * cos(radRightTop);
            x4 = wingWidth * cos(radRightUnder);
            z1  = wingWidth * sin(radLeftTop);
            z2  = wingWidth * sin(radLeftUnder);
            z3 = wingWidth * sin(radLeftTop);
            z4 = wingWidth * sin(radLeftUnder);
            

        
        // set calcurated vertex date in updated Mesn
        updateMesh.setVertex(1, ofVec3f(x1,updateMesh.getVertex(1).y,z1));
        updateMesh.setVertex(2, ofVec3f(x2-wingWidth/2,updateMesh.getVertex(2).y,z2));
        updateMesh.setVertex(3, ofVec3f(x3,updateMesh.getVertex(3).y,z3));
        updateMesh.setVertex(4, ofVec3f(x4+wingWidth/2,updateMesh.getVertex(4).y,z4));
        
        // set updated mesh in VBO
        butterflyVbo.setMesh(updateMesh, GL_DYNAMIC_DRAW);
        
        
        
        
        // up down moving
        //position.y -= 0.09;
        //longitude += speed;
        if(ampZ > 5*0.90) {
        //    position.y += 1.0;
        }
        
    }
    bool DEBUG_MODE = false;
   
private:
    ofMesh butterflyMesh;
    ofVbo butterflyVbo;
    ofVec3f position;
    float wingWidth;
    float wingHeight;
    float scale;
    ofVec3f direction;
	
    
    // ------ tween value ------- //
    float length_float;
    //ofxTween tweenquint;
    //ofxTween tweenquintB;
    //ofxEasingQuint 	easingquint;
    float ampZ;
    float timer;
	float ampSpeed;
    
    
    // animation value
    float piCounterTop = 2*PI;
    float piCounterUnder = 2*PI;
    bool bAddPi = true;
    
    
};




#endif /* ofxButterflyPrimitive_h */
