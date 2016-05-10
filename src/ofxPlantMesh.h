//
//  ofxPlantMesh.h
//  SoundGarden
//
//  Created by 岩城勇磨 on 5/8/16.
//
//

#ifndef ofxPlantMesh_h
#define ofxPlantMesh_h

#include <stdio.h>
#include "ofMain.h"

class ofxFlowerMesh {
public:
    ofxFlowerMesh(void) {};
    ~ofxFlowerMesh(void) {};
    void setup(int _SEGMENTS_NUM, int flowervertnum) {
        floawerVertNum = 200;
        arcRad = ofRandom(50, 90);
        direction = ofRandom(-1, 0);
        if (direction == 0) direction = 1;
        
        floawerVertNum = flowervertnum;
        NUM = _SEGMENTS_NUM;
        float yOff = ofRandom(0.01, 0.06);
        for (int i = 0; i < NUM; i++) {
            segments.push_back(ofVec3f(ofNoise(i, yOff), i * 2, 0));
            segmentsLine.addVertex(segments[i]);
        }
        
        segmentsLine.enableIndices();
        
        for (int i = 0; i < segmentsLine.getNumVertices() - 1; i++) {
            segmentsLine.addIndex(i);
            segmentsLine.addIndex(i + 1);
        }
        
    }
    ofVec3f getPosition() {
        return position;
    }
    void setPosition(ofVec3f pos) {
        position = pos;
    }
    void setNum(int num, int flowervertnum) {
        NUM = num;
        floawerVertNum = flowervertnum;
    }
    
    void update(float yOff, float range, float yStep, float rad, float flowerRingRad, float flowerRingHeight, float petalAmp) {
        segmentsLine.clear();
        segments.clear();
        segmentsCicle.clear();
        flowerMesh.clear();
        for (int i = 0; i < NUM; i++) {
            float arc = (60 * direction)*sin(i*0.1);
            segments.push_back(ofVec3f(ofNoise(i, yOff)*range + arc, i*yStep, 0));
            segmentsLine.addVertex(segments[i]);
            circleRadNum = 7;
            for (int j = 0; j < circleRadNum; j++) {
                float radian = ofMap(j, 0, circleRadNum - 1, 0, 2 * PI);
                float x = segments[i].x + rad * cos(radian);
                float y = segments[i].y;
                float z = rad * sin(radian);
                segmentsCicle.addVertex(ofVec3f(x, y, z));
                segmentsCicle.addColor(ofColor(240, i * 20, 240));
                
            }
            
            
        }
        flowerMesh.addColor(ofColor(230, 230, 230));
        flowerMesh.addVertex(segments[NUM - 1]);
        for (int i = 0; i < floawerVertNum; i++) {
            float p = ofMap(i, 0, floawerVertNum, 0, 2 * PI);
            float x = segments[NUM - 1].x + flowerRingRad * sin(p);
            float y = segments[NUM - 1].y + flowerRingHeight + petalAmp * sin(p * 6);
            float z = segments[NUM - 1].z + flowerRingRad * cos(p);
            flowerMesh.addVertex(ofVec3f(x, y, z));
            flowerMesh.addColor(ofColor(230, 230, 230));
            
        }
        float x = segments[NUM - 1].x + flowerRingRad * sin(0);
        float y = segments[NUM - 1].y + flowerRingHeight + petalAmp * sin(0);
        float z = segments[NUM - 1].z + flowerRingRad * cos(0);
        flowerMesh.addVertex(ofVec3f(x, y, z));
        flowerMesh.addColor(ofColor(230, 230, 230));
        segmentsLine.enableIndices();
        
        for (int i = 0; i < segmentsLine.getNumVertices() - 1; i++) {
            segmentsLine.addIndex(i);
            segmentsLine.addIndex(i + 1);
        }
        
        segmentsCicle.enableIndices();
        for (int i = 0; i < segmentsCicle.getNumVertices() - circleRadNum; i++) {
            segmentsCicle.addIndex(i);
            segmentsCicle.addIndex(i + (circleRadNum - 1));
            segmentsCicle.addIndex(i + 1);
            
            segmentsCicle.addIndex(i + 1);
            segmentsCicle.addIndex(i + (circleRadNum - 1));
            segmentsCicle.addIndex(i + (circleRadNum - 1) + 1);
        }
        
        segmentVbo.setMesh(segmentsCicle, GL_DYNAMIC_DRAW);
        flowerVbo.setMesh(flowerMesh, GL_DYNAMIC_DRAW);
    }
    
    void draw() {
        ofPushMatrix();
        ofTranslate(position);
        ofFill();
        segmentsLine.setMode(OF_PRIMITIVE_TRIANGLES);
        
        /*segmentsCicle.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
         segmentsCicle.draw();
         
         flowerMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
         flowerMesh.draw();
         */
        
        segmentVbo.drawElements(GL_TRIANGLES, segmentVbo.getNumIndices());
        flowerVbo.draw(GL_TRIANGLE_FAN, 0, flowerVbo.getNumVertices());
        ofPopMatrix();
        
    }
    
    void drawDebug() {
        for (int i = 0; i < segmentsCicle.getNumVertices(); i++) {
            ofDrawBitmapString(ofToString(i), segmentsCicle.getVertex(i));
            segmentsLine.draw();
        }
    }
private:
    vector<ofVec3f> segments;
    ofMesh segmentsLine;
    ofMesh segmentsCicle;
    ofMesh flowerMesh;
    int NUM;
    int circleRadNum;
    float arcRad;
    float direction;
    int floawerVertNum;
    ofVec3f position;
    ofVbo flowerVbo;
    ofVbo segmentVbo;
    //float petalAmp;
    //float flowerRingHeight;
    //float flowerRingRad;
};


class ofxZenmaiPrimitive {
public:
    ofxZenmaiPrimitive(void) {};
    ~ofxZenmaiPrimitive(void) {};
    
    float InExp(float t, float b, float c, float d)
    {
        if (t <= d) {
            return c * pow(2, 10 * (t / d - 1)) + b;
        }
        else {
            return c;
        }
        
    }
    
    float OutExp(double t, double totaltime, double max, double min)
    {
        max -= min;
        return t == totaltime ? max + min : max*(-pow(2, -10 * t / totaltime) + 1) + min;
    }
    
    double OutQuad(double t, double totaltime, double max, double min)
    {
        max -= min;
        t /= totaltime;
        return -max*t*(t - 2) + min;
    }
    
    
    
    void setup(float height, float _rad, ofVec3f _pos)
    {
        rotate_y = ofRandom(360);
        pos.set(_pos);
        rad = _rad;
        num = 10;
        float step = PI / 6;
        ystep = height / num;
        float rast = 3 * TWO_PI;
        float ofsett = 0;
        //float _rad = 400;
        float rastY;
        
        
        for (float i = 0; i < num; i++)
        {
            
            float x = 0;
            float y = i*ystep;
            vertexs.push_back(ofPoint(x, y));
            
            for (int j = 0; j < circlePointNum; j++) {
                float radian = ofMap(j, 0, circlePointNum - 1, 0, 2 * PI);
                float _x = x + rad * cos(radian);
                float _y = y;
                float _z = rad * sin(radian);
                zenmaiMesh.addVertex(ofVec3f(_x, _y, _z));
                zenmaiMesh.addColor(ofColor(240, i * 20, 240, 100));
                
            }
            
            //zenmaiMesh.addVertex(ofVec2f(x, y));
        }
        
        rastRad = rad * 8;
        for (float i = 0; i < rast; i += PI / 6)
        {
            float _rad = OutQuad(double(i), (double)rast - PI, (double)rad, (double)rastRad);
            
            float x = _rad * cos(i) - rastRad*cos(0);
            float y = _rad * sin(i) + vertexs[num - 1].y + ystep;
            vertexs.push_back(ofPoint(x, y));
            
            for (int j = 0; j < circlePointNum; j++) {
                float radian = ofMap(j, 0, circlePointNum - 1, 0, 2 * PI);
                float _x = x + rad * cos(radian);
                float _y = y;
                float _z = rad * sin(radian);
                zenmaiMesh.addVertex(ofVec3f(_x, _y, _z));
                zenmaiMesh.addColor(ofColor(240, i * 20, 240, 100));
                //zenmaiMesh.addColor(ofColor(240, i * 20, 240));
                
            }
            //zenmaiMesh.addVertex(ofVec2f(x, y));
        }
        
        
        zenmaiMesh.enableIndices();
        for (int i = 0; i < zenmaiMesh.getNumVertices() - circlePointNum; i++) {
            zenmaiMesh.addIndex(i);
            zenmaiMesh.addIndex(i + (circlePointNum - 1));
            zenmaiMesh.addIndex(i + 1);
            
            zenmaiMesh.addIndex(i + 1);
            zenmaiMesh.addIndex(i + (circlePointNum - 1));
            zenmaiMesh.addIndex(i + (circlePointNum - 1) + 1);
        }
        
        zenmaiVbo.setMesh(zenmaiMesh, GL_DYNAMIC_DRAW);
    }
    
    void update() {
        zenmaiVbo.clear();
        effectValue = ofGetElapsedTimef();
        zenmaiMesh.clear();
        rastRad = rad * 8;
        float rast = 3 * TWO_PI;
        for (float i = 0; i < num; i++)
        {
            
            float x = 0;
            float y = i*ystep;
            vertexs.push_back(ofPoint(x, y));
            
            for (int j = 0; j < circlePointNum; j++) {
                float radian = ofMap(j, 0, circlePointNum - 1, 0, 2 * PI);
                float _x = x + rad * cos(radian); //+ 10*sin(effectValue) * sin(i*PI/num);
                float _y = y;
                float _z = rad * sin(radian); //+ 10 * sin(effectValue) * sin(i*PI / num);
                zenmaiMesh.addVertex(ofVec3f(_x, _y, _z));
                zenmaiMesh.addColor(ofColor(60 + 60 * i * 2, i * 60, 60 + 60 * i * 2));
                
            }
            
            zenmaiMesh.addVertex(ofVec2f(x, y));
        }
        
        rastRad = rad * (8 + 2 * sin(effectValue));
        for (float i = 0; i < rast; i += PI / 6)
        {
            float _rad = OutQuad(double(i), (double)rast - PI, (double)rad, (double)rastRad);
            
            float x = _rad * cos(i) - rastRad*cos(0);
            float y = _rad * sin(i) + vertexs[num - 1].y + ystep;
            vertexs.push_back(ofPoint(x, y));
            
            for (int j = 0; j < circlePointNum; j++) {
                float radian = ofMap(j, 0, circlePointNum - 1, 0, 2 * PI);
                float _x = x + rad * cos(radian);
                float _y = y;
                float _z = rad * sin(radian);
                zenmaiMesh.addVertex(ofVec3f(_x, _y, _z));
                
                
            }
            zenmaiMesh.addVertex(ofVec2f(x, y));
        }
        
        
        zenmaiMesh.enableIndices();
        for (int i = 0; i < zenmaiMesh.getNumVertices() - circlePointNum; i++) {
            zenmaiMesh.addIndex(i);
            zenmaiMesh.addIndex(i + (circlePointNum - 1));
            zenmaiMesh.addIndex(i + 1);
            
            zenmaiMesh.addIndex(i + 1);
            zenmaiMesh.addIndex(i + (circlePointNum - 1));
            zenmaiMesh.addIndex(i + (circlePointNum - 1) + 1);
            
            for (int j = 0; j < circlePointNum; j++)
            {
                zenmaiMesh.addColor(ofColor(240 - i * 3, 200 + i, 240 - i * 3));
            }
        }
        zenmaiVbo.setMesh(zenmaiMesh, GL_DYNAMIC_DRAW);
        
    }
    
    
    void draw()
    {
        //zenmaiMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        ofPushMatrix();
        ofTranslate(pos);
        //zenmaiMesh.draw();
        ofRotateY(rotate_y);
        ofFill();
        zenmaiVbo.drawElements(GL_TRIANGLES, zenmaiVbo.getNumIndices());
        ofPushStyle();
        
        ofSetColor(0, 255, 0);
        ofDrawLine(0, 0, 4, 0);
        
        ofSetColor(0, 0, 255);
        ofDrawLine(0, 0, 0, 4);
        
        ofSetColor(255, 0, 0);
        ofDrawLine(0, 0, 0, 0, 0, 4);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
private:
    ofMesh zenmaiMesh;
    vector<ofPoint> vertexs;
    int circlePointNum = 6;
    float rad;
    int num;
    float rastRad;
    float ystep;
    float effectValue;
    ofVec3f pos;
    ofVbo zenmaiVbo;
    float rotate_y;
};

#endif /* ofxPlantMesh_h */
