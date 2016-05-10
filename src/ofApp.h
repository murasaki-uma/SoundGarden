#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxButterflyPrimitive.h"
#include "Terrain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam cam;
	ofLight pointLight;
	ofLight spotLight;
	ofLight directionalLight;

	ofParameter<ofVec3f> directionOrien;
	ofParameter<ofVec3f> spotOrien;
	ofParameter<ofVec3f> pointPos;
	ofParameter<ofVec3f> spotPos;

	ofParameter<ofColor> directionDiffuseColor;
	ofParameter<ofColor> directionSpecularColor;
	ofParameter<ofColor> spotDiffuse;
	ofParameter<ofColor> spotColor;
	ofParameter<ofColor> spotSpecular;

	ofParameter<ofColor> bgColorA;
	ofParameter<ofColor> bgColorB;

	ofParameter<ofVec3f> camPos;

	ofxPanel gui;
	ofxPanel colorGui;

};
