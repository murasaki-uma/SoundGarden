//
//  Terrain.h
//  ofxButterflyMesh
//
//  Created by 岩城勇磨 on 4/21/16.
//
//

#ifndef Terrain_h
#define Terrain_h

#include <stdio.h>
#include "ofMain.h"

class Terrain {
public:
    Terrain(void){}
    ~Terrain(void){}
    
    bool DEBUG_MODE;
    
    // ------ mesh ------
    int tileCount = 40;
    int zScale = 40;
    ofMesh terrainMesh;
    vector<ofVec3f> poits;
    ofVbo terrainVbo;
    // ------ noise ------
    int noiseXRange = 10;
    int noiseYRange = 10;
    int octaves = 4;
    float falloff = 0.5;
    float noiseYMax = 0;
    // ------ mesh coloring ------
    ofColor midColor, topColor, bottomColor;
    ofColor strokeColor;
    float threshold = 0.30;
	float center = (0, 0);
    
    
    
    
    // -------- test --------- //
    ofVec3f v[16];
    ofVec3f n[16];
    ofFloatColor c[16];
    ofVbo vbo;
    vector<ofVec3f> meshPos;
    void setup()
    {
       
    }
    
    void draw()
    {
        //terrainVbo.draw(GL_TRIANGLES, 0, terrainMesh.getNumVertices());
        ofPushMatrix();
        ofRotateX(270);
		//ofFill();
        ofNoFill();
        //glPointSize(1);
        //terrainMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        //terrainMesh.draw();
        //terrainMesh.drawWireframe();
		glPointSize(3);
        glEnable(GL_DEPTH_TEST);
        
        terrainVbo.drawElements(GL_TRIANGLES,terrainMesh.getNumIndices());
        
        
        //glPointSize(10.f);
        //vbo.drawElements( GL_TRIANGLES, 60);
        
        if(DEBUG_MODE)
        {
            for (int i = 0; i < terrainMesh.getNumVertices(); i++)
            {
//                ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
//                ofPushMatrix();
//                ofTranslate(terrainMesh.getVertex(i).x,terrainMesh.getVertex(i).y,terrainMesh.getVertex(i).z);
//                ofScale(0.001,0.001);
                ofSetColor(255);
                //ofDrawBitmapString(ofToString(i), terrainMesh.getVertex(i).x,terrainMesh.getVertex(i).y,terrainMesh.getVertex(i).z);
//                ofPopMatrix();
            }
            
            int counter = 0;
            for (int meshY=0; meshY<tileCount; meshY++) {
                
                for (int meshX=0; meshX<tileCount; meshX++) {
                    if(meshX == 0 || meshY == 0){
                        //ofDrawBitmapString(ofToString(meshX)+" "+ofToString(meshY),meshPos[counter].x, meshPos[counter].y, meshPos[counter].z);
                        
                    }
                    counter++;
                }
            }
            
            
        }
        ofPopMatrix();
    }
    
    
    
    float timer;
    void update(float rectWidth, float force)
    {
        meshPos.clear();
        //timer += 0.1;
        noiseXRange = 4;
        noiseYRange = 3;
        float noiseYMax = 0;
        topColor.set(255, 255, 100);
        midColor.set(191, 99, 63);
        bottomColor.set(255, 255, 255);
//
//        strokeColor.set(255, 255, 255);
        terrainMesh.clear();
        terrainVbo.clear();
		//float force = 80;
        float tileSizeY = (float)ofGetHeight()/tileCount;
        float noiseStepY = (float)noiseYRange/tileCount;
        int radCounter;
        for (int meshY=0; meshY<tileCount; meshY++) {
            
            for (int meshX=0; meshX<tileCount; meshX++) {
				
				//cout << "flow : " << flow << endl;

                float x = ofMap(meshX, 0, tileCount-1, -rectWidth/2, rectWidth/2);
                float y = ofMap(meshY, 0, tileCount-1, -rectWidth/2, rectWidth/2);
                float z;

				ofVec2f flow;
				flow = center - ofVec2f(x, y);
				flow.normalize();
				flow *= -force;
                
                float noiseX = ofMap(meshX, 1, tileCount, 0, noiseXRange);
                float noiseY = ofMap(meshY, 1, tileCount, 0, noiseYRange);
                if(meshX == 0 || meshY == 0 || meshX == tileCount-1 || meshY == tileCount-1) {
                    z = 0;
                } else {

                    z = ofNoise(noiseX, noiseY+ofGetElapsedTimef());

                }
               
                terrainMesh.addVertex(ofVec3f(x+flow.x, y + flow.y, z*zScale));//z1*zScale));
                meshPos.push_back(ofVec3f(x + flow.x, y + flow.y, z*zScale));
               
            }
            
        }
        terrainMesh.enableIndices();
        //terrainMesh.clearIndices();
        //terrainVbo.setMesh(terrainMesh, GL_DYNAMIC_DRAW);
        int counter = 0;
        for (int meshY=0; meshY<tileCount-1; meshY++) {
            for (int meshX=0; meshX<tileCount-1; meshX++) {
                
                
                        terrainMesh.addIndex(meshX+meshY*tileCount);               // 0
                        terrainMesh.addIndex((meshX+1)+meshY*tileCount);           // 1
                        terrainMesh.addIndex(meshX+(meshY+1)*tileCount);           // 10
						terrainMesh.addColor(ofColor(230, 230, 230, 200));
						terrainMesh.addColor(ofColor(230, 230, 230, 200));
						terrainMesh.addColor(ofColor(230, 230, 230, 200));

                        terrainMesh.addIndex((meshX+1)+meshY*tileCount);           // 1
                        terrainMesh.addIndex((meshX+1)+(meshY+1)*tileCount);       // 11
                        terrainMesh.addIndex(meshX+(meshY+1)*tileCount);		   // 10
						terrainMesh.addColor(ofColor(230, 230, 230, 200));
						terrainMesh.addColor(ofColor(230, 230, 230, 200));
						terrainMesh.addColor(ofColor(230, 230, 230, 200));
            }
        }
        
        terrainVbo.setMesh(terrainMesh,GL_DYNAMIC_DRAW);
    }
    
    ofColor LerpColor (ofColor from, ofColor to, float inter) {
        ofColor result;
        result.r = ofLerp(from.r, to.r, inter);
        result.b = ofLerp(from.b, to.b, inter);
        result.g = ofLerp(from.g, to.g, inter);
        return result;
    }
private:
};


#endif /* Terrain_h */
