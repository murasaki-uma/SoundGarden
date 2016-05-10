//
//  particleWave.h
//  ofxButterflyMesh
//
//  Created by 岩城勇磨 on 4/21/16.
//
//

#ifndef particleWave_h
#define particleWave_h

#include <stdio.h>
#include "ofMain.h"

class ParticleWave
{
public:
	ParticleWave(void) {};
	~ParticleWave(void) {};
	void setup(float Width, float Height, ofVec3f Position, int Density, float RadStep)
	{
		width = Width;
		height = Height;
		position = Position;
		density = Density;
		xStep = width / (float)density;
		zStep = height / (float)density;
		radStep = RadStep;
		amp = 40;
		for (float z = -width/2; z < width/2; z +=xStep)
		{
			rad = ofNoise(z*01,radStep)*amp;

			for (float x = -height/2; x < height/2; x += zStep)
			{
				rad += radStep;
				rads.push_back(rad);
				float y = 0;
				y = amp * sin(rad)* sin(rad);
				vertex.push_back(ofPoint(x, y, z));
				waveMesh.addVertex(ofPoint(x, y, z));
			}
		}

		waveVbo.setMesh(waveMesh, GL_DYNAMIC_DRAW);

	}

	void update() {
		for (int i = 0; i < vertex.size(); i++) {
			rads[i] += radStep;
			waveMesh.setVertex(i, ofVec3f(vertex[i].x, amp * sin(rads[i])* sin(rad), vertex[i].z));	
		}
		//waveVbo.setMesh
		//waveVbo.clear();
		waveVbo.setMesh(waveMesh, GL_DYNAMIC_DRAW);
	}

	void draw() {
		ofPushMatrix();
		
		ofTranslate(position);
		ofDrawLine(0,0,width,0);
		glPointSize(2.0);
		//waveVbo.drawElements(OF_PRIMITIVE_POINTS, waveVbo.getNumVertices());
		//waveMesh.setMode(OF_PRIMITIVE_POINTS);
		//waveMesh.draw();
		waveVbo.draw(GL_POINTS,0,waveMesh.getNumVertices());
		ofPopMatrix();
	}
private:
	float width, height;
	float xStep, yStep, zStep, radStep;
	int density;
	ofVec3f position;
	vector<ofPoint>vertex;
	ofMesh waveMesh;
	float rad;
	float amp;

	ofVbo waveVbo;
	vector<float>rads;
};

#endif /* particleWave_h */
