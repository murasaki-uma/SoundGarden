#include "ofApp.h"
#include "ofxPlantMesh.h"
#include "particleWave.h"
#include "ripple.h"


RippleAnimation rippleAnimation;

vector<ofxButterflyPrimitive> butterfly;
bool PAUSE = true;
bool GRID;
bool bSmoothLighting;
bool bShiny;
bool bPointLight, bSpotLight, bDirLight;
Terrain terrain;
ofVec3f center;
float radius;

bool TERRAIN;

bool bGUI;

ofVec3f easyCamPos;
float yScale = 0.6;
float rectWidth;
float timer;


vector<ofxZenmaiPrimitive> plants;
vector<ofxFlowerMesh> flowers;
ofVec3f garden_rotate;

bool DRAW_LIGHT;

ParticleWave wave;

float addRad;
float rad;
float force = 0;
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	ofEnableSmoothing();

	cam.setDistance(3.0f);
	cam.setNearClip(0.01f);
	cam.setFarClip(2000.0f);


	cam.setOrientation(ofVec3f(90, 0, 0));
	cam.setPosition(0.0f, 20.0f, 50.0f);
	cam.lookAt(ofVec3f(0, 0, 0));

	ofBackground(0);
	for (int i = 0; i< 100; i++) {
		ofxButterflyPrimitive b;
		b.setup(ofVec3f(ofRandom(20, 400), 0, ofRandom(200, 280)), 1);
		butterfly.push_back(b);
	}

	terrain.setup();
	rectWidth = ofGetWidth()*0.4;


	ofSetFrameRate(60);
	ofBackground(10, 10, 10);
	ofEnableDepthTest();

	// turn on smooth lighting //
	bSmoothLighting = true;
	ofSetSmoothLighting(true);

	// lets make a high-res sphere //
	// default is 20 //
	ofSetSphereResolution(128);


	// Point lights emit light in all directions //
	// set the diffuse color, color reflected from the light source //
	pointLight.setDiffuseColor(ofColor(0.f, 255.f, 0.f));

	// specular color, the highlight/shininess color //
	pointLight.setSpecularColor(ofColor(255.f, 255.f, 0.f));
	pointLight.setPointLight();

	radius = 180.f;
	center.set(0, 0, 0);


	spotLight.setDiffuseColor(ofColor(255.f, 0.f, 0.f));
	spotLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	// turn the light into spotLight, emit a cone of light //
	spotLight.setSpotlight();

	// size of the cone of emitted light, angle between light axis and side of cone //
	// angle range between 0 - 90 in degrees //
	spotLight.setSpotlightCutOff(50);

	// rate of falloff, illumitation decreases as the angle from the cone axis increases //
	// range 0 - 128, zero is even illumination, 128 is max falloff //
	spotLight.setSpotConcentration(45);


	// Directional Lights emit light based on their orientation, regardless of their position //
	directionalLight.setDiffuseColor(ofColor(200.f, 200.f, 200.0f));
	directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	directionalLight.setDirectional();

	// set the direction of the light
	// set it pointing from left to right -> //
	directionalLight.setOrientation(ofVec3f(0, 90, 0));


	bShiny = true;

	bPointLight = bSpotLight = bDirLight = true;

	// tex coords for 3D objects in OF are from 0 -> 1, not 0 -> image.width
	// so we must disable the arb rectangle call to allow 0 -> 1
	ofDisableArbTex();



	gui.setup();
	gui.add(directionOrien.set("directionOrien", ofVec3f(91, 0, 0), ofVec3f(0, 0, 0), ofVec3f(360, 360, 360)));
	gui.add(pointPos.set("pointPos", ofVec3f(0, 125, 0), ofVec3f(-ofGetWidth(), -ofGetHeight(), -500), ofVec3f(ofGetWidth(), ofGetHeight(), 500)));
	gui.add(spotPos.set("spotPos", ofVec3f(-31, 172, -25), ofVec3f(-ofGetWidth(), -ofGetHeight(), -500), ofVec3f(ofGetWidth(), ofGetHeight(), 500)));
	gui.add(spotOrien.set("spotOrien", ofVec3f(300, 0, 0), ofVec3f(0, 0, 0), ofVec3f(360, 360, 360)));
	gui.add(camPos.set("camPos", ofVec3f(0.f, 0.f, 0.f), ofVec3f(-1000.f, -1000.f, -1000), ofVec3f(1000.f, 1000.f, 1000.f)));

	colorGui.setup();
	colorGui.add(directionDiffuseColor.set("directionDiffuseColor", ofColor(13.f, 48.f, 222.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));
	colorGui.add(directionSpecularColor.set("directionSpecularColor", ofColor(98.f, 166.f, 183.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));
	colorGui.add(spotDiffuse.set("spotDiffuse", ofColor(166.f, 104.f, 199.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));
	colorGui.add(spotSpecular.set("spotSpecular", ofColor(123.f, 26.f, 79.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));
	colorGui.add(bgColorA.set("bgColorA", ofColor(166.f, 104.f, 199.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));
	colorGui.add(bgColorB.set("bgColorB", ofColor(123.f, 26.f, 79.f), ofColor(0.f, 0.f, 0.f), ofColor(255.f, 255.f, 255.f)));

	easyCamPos.set(0.f, -480.f, -600.0f);


	for (int i = 0; i < 40; i++)
	{
		ofxZenmaiPrimitive zenmai;
		float randomrange = ofRandom(200,30);
		float randomRad = ofRandom(0, TWO_PI);
		float x = randomrange*cos(randomRad);
		float y = 0;
		float z = randomrange*sin(randomRad);
		float height = ofMap(randomrange, 40, 100, 100, 60);
		zenmai.setup(height, ofRandom(0.5,2), ofVec3f(x,y,z));
		plants.push_back(zenmai);
	}

	for (int i = 0; i < 1; i++) {
		ofxFlowerMesh f;
		f.setup(5,20);
		f.setPosition(ofVec3f(0, 0, 0));
		flowers.push_back(f);
		f.setup(3, 20);
		f.setPosition(ofVec3f(ofRandom(-30,-20), 0, ofRandom(-30, -20)));
		flowers.push_back(f);
		f.setPosition(ofVec3f(ofRandom(30, 20), 0, ofRandom(-30, -20)));
		f.setup(3, 10);
		flowers.push_back(f);
	}

	rippleAnimation.setup(10);
	wave.setup(rectWidth, rectWidth, ofVec3f(0, rectWidth*0.3, 0), 50, 0.02);
}

//--------------------------------------------------------------
void ofApp::update() {
	timer += 0.1;
	//addRad *= 0.98;
	//pointLight.setPosition(pointPos);
	pointLight.setDiffuseColor(ofColor(spotDiffuse->r, spotDiffuse->g, spotDiffuse->b));
	pointLight.setSpecularColor(ofColor(spotSpecular->r, spotSpecular->g, spotSpecular->b));

	directionalLight.setDiffuseColor(ofColor(directionDiffuseColor->r, directionDiffuseColor->g, directionDiffuseColor->b));
	directionalLight.setSpecularColor(ofColor(directionSpecularColor->r, directionSpecularColor->g, directionSpecularColor->b));


	spotLight.setOrientation(spotOrien);
	spotLight.setPosition(spotPos);
	directionalLight.setOrientation(directionOrien);
	pointLight.setPosition(pointPos);
	rad += (addRad - rad)*0.2;
	if (!PAUSE) {
		for (int i = 0; i < butterfly.size(); i++) {
			butterfly[i].update(timer);
			butterfly[i].addRadius(rad);
			//wave.update();
		}
	}
	addRad *= 0.98;

	for (int i = 0; i < plants.size(); i++)
	{
		//plants[i].update();
	}
	terrain.update(rectWidth, force);

	//cam.setPosition(easyCamPos);
	//cout << easyCamPos << endl;

	//for (int i = 0; i < flowers.size(); i++) {
	flowers[0].update(ofNoise(flowers[0].getPosition().x, ofGetElapsedTimef())*0.3,7,38,2,50,31,7);
	flowers[1].update(ofNoise(flowers[1].getPosition().x, ofGetElapsedTimef())*0.3, 7, 38, 2, 20, 20, 3);
	flowers[2].update(ofNoise(flowers[2].getPosition().x, ofGetElapsedTimef())*0.3, 7, 38, 2, 30, 20, 3);

	//}

	rippleAnimation.update();
	force = addRad;

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();
	ofBackgroundGradient(bgColorA, bgColorB);

	cam.begin();
	ofTranslate(easyCamPos);
	ofPushMatrix();
	ofRotateX(garden_rotate.y);
	ofRotateY(garden_rotate.x);
	
	if (GRID) {
		
		ofDrawGrid(10, 10, 10, true, true, true);
		
	}


	ofEnableLighting();
	pointLight.enable();
	spotLight.enable();
	directionalLight.enable();




	if (TERRAIN) {
		ofPushMatrix();
		ofTranslate(0,100);
		terrain.draw();
		ofPopMatrix();
	}

	
	for (int i = 0; i < plants.size(); i++)
	{
		//ofPushMatrix();
		//ofRotateX(-garden_rotate.y);
		//ofRotateY(-garden_rotate.x);
		plants[i].draw();
		//ofPopMatrix();
	}

	for (int i = 0; i < flowers.size(); i++) {
		flowers[i].draw();
	}

	for (int i = 0; i < butterfly.size(); i++) {
		butterfly[i].draw();
	}

	ofPushMatrix();
	ofRotateX(90);
	ofTranslate(0, -200);
	//rippleAnimation.draw();
	ofPopMatrix();

	//wave.draw();
	//if (!bPointLight) pointLight.disable();
	//if (!bSpotLight) spotLight.disable();
	//if (!bDirLight) directionalLight.disable();

	//ofSetColor( pointLight.getDiffuseColor() );
	if (bPointLight) pointLight.draw();

	//ofSetColor(255, 255, 255);
	//ofSetColor( spotLight.getDiffuseColor() );
	if (bSpotLight) spotLight.draw();

	//ofDisableLighting();

	//ofSetColor(255, 255, 255);
	//    ofDrawBitmapString("Point Light On (1) : "+ofToString(bPointLight) +"\n"+
	//                       "Spot Light On (2) : "+ofToString(bSpotLight) +"\n"+
	//                       "Directional Light On (3) : "+ofToString(bDirLight)+"\n"+
	//                       "Shiny Objects On (s) : "+ofToString(bShiny)+"\n"+
	//                       "Spot Light Cutoff (up/down) : "+ofToString(spotLight.getSpotlightCutOff(),0)+"\n"+123
	//                       "Spot Light Concentration (right/left) : " + ofToString(spotLight.getSpotConcentration(),0)+"\n"+
	//                       "Smooth Lighting enabled (x) : "+ofToString(bSmoothLighting,0)+"\n"+
	//                       "Textured (t) : ",
	//                       20, 20);


	ofPushMatrix();
	
	ofRotateX(270);
	ofNoFill();

	ofDrawRectangle(-rectWidth / 2, -rectWidth / 2, rectWidth, rectWidth);
	ofTranslate(0, 0, rectWidth*yScale);
	ofDrawRectangle(-rectWidth / 2, -rectWidth / 2, rectWidth, rectWidth);
	//ofTranslate(0, rectWidth*0.6);

	ofPopMatrix();

	ofDrawLine(-rectWidth / 2, 0, rectWidth / 2, -rectWidth / 2, rectWidth*yScale, rectWidth / 2);
	ofDrawLine(rectWidth / 2, 0, rectWidth / 2, rectWidth / 2, rectWidth*yScale, rectWidth / 2);
	ofDrawLine(-rectWidth / 2, 0, -rectWidth / 2, -rectWidth / 2, rectWidth*yScale, -rectWidth / 2);
	ofDrawLine(rectWidth / 2, 0, -rectWidth / 2, rectWidth / 2, rectWidth*yScale, -rectWidth / 2);

	//ofDrawRectangle(-rectWidth/2, -rectWidth/2, rectWidth, rectWidth);
	ofPopMatrix();
	cam.end();
	ofDisableDepthTest();
	ofDisableLighting();
	if (bGUI)
	{
		gui.draw();
		colorGui.draw();
	}

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd') {
		for (int i = 0; i < butterfly.size(); i++) {
			butterfly[i].DEBUG_MODE = !butterfly[i].DEBUG_MODE;
		}
		terrain.DEBUG_MODE = !terrain.DEBUG_MODE;
		GRID = !GRID;
	}

	if (key == 'p') {
		PAUSE = !PAUSE;
	}

	if (key == 'g') {
		bGUI = !bGUI;
	}

	if (key == 't') {
		TERRAIN = !TERRAIN;
	}

	if (key == 'l')
	{
		DRAW_LIGHT = !DRAW_LIGHT;
	}


	switch (key) {
	case '1':
		bPointLight = !bPointLight;
		break;
	case '2':
		bSpotLight = !bSpotLight;
		break;
	case '3':
		bDirLight = !bDirLight;
		break;
		//        case 's':
		//            bShiny	= !bShiny;
		//            if (bShiny) material.setShininess( 120 );
		//            else material.setShininess( 30 );
		//            break;
	case 'x':
		bSmoothLighting = !bSmoothLighting;
		ofSetSmoothLighting(bSmoothLighting);
		break;
		//        case 't':
		//            bUseTexture = !bUseTexture;
		//            break;
	case OF_KEY_UP:
		// setSpotlightCutOff is clamped between 0 - 90 degrees //
		//easyCamPos.y += 10.0f;
		garden_rotate.y += 10.0f;
		break;
	case OF_KEY_DOWN:
		//easyCamPos.y -= 10.0f;
		garden_rotate.y -= 10.0f;
		break;
	case OF_KEY_RIGHT:
		//easyCamPos.x += 10.0f;
		garden_rotate.x += 10.0f;
		break;
	case OF_KEY_LEFT:
		//easyCamPos.x -= 10.0f;
		garden_rotate.x -= 10.0f;
		break;
	case 'w':
		//easyCamPos.z += 10.0f;
		break;
	case 's':
		//easyCamPos.z -= 10.0f;
		break;
	default:
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	addRad += 90;
	rippleAnimation.count();
	force = 200;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
