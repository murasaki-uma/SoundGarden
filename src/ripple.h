#pragma once
#ifndef ripple_h
#define ripple_h

#include <stdio.h>
#include "ofMain.h"

class Ripple
{
public:
	Ripple(void) {};
	~Ripple(void) {};

	void setup(int VertexNum, float Radius, float WaveAmp, ofColor lineColor)
	{
		color = lineColor;
		waveAmp = startWaveAmp = WaveAmp;
		radius = startRadius = Radius;
		vertexNum = VertexNum;
		createMesh();
		speed = 20;
	}
	void init() {
		z_r = 0.0;
		speed = 20;
		color.set(255);
		radius = startRadius;
		createMesh();
	}
	void createMesh() {
		circleMesh.clear();
		z_r = 0;
		waveAmp = startWaveAmp;
		for (int i = 0; i < vertexNum; i++)
		{
			float r = TWO_PI / vertexNum * i;
			z_r += 0.4;
			float x = radius * cos(r);
			float y = radius * sin(r);
			float z = waveAmp * sin(z_r);
			circleMesh.addVertex(ofVec3f(x, y, z));
			circleMesh.addColor(color);
		}

		circleVbo.setMesh(circleMesh, GL_DYNAMIC_DRAW);
	}

	void draw() {
		circleMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
		ofPushStyle();
		//ofSetColor(color);
		if (UPDATE_START)
		{
			//circleMesh.drawWireframe();
			circleVbo.draw(GL_LINE_LOOP, 0, circleMesh.getNumVertices());
		}

		ofPopStyle();
	}

	void update() {
		if (UPDATE_START)
		{
			speed *= 0.98;
			radius += speed;
			createMesh();
			waveAmp += 0.6;
			color.a -= 255 * radius / ofGetWidth() / 2 * 0.1;


			if (color.a <= 0.0 || radius > ofGetWidth() / 2) {
				init();
				UPDATE_START = false;
			}
		}

	}

	void log()
	{
		cout << UPDATE_START << endl;
	}

	void startUpdate() {
		UPDATE_START = true;
	}
private:

	int vertexNum;
	float startRadius;
	float radius;
	ofMesh circleMesh;
	float waveAmp;
	float startWaveAmp;
	float z_r;
	float speed;
	bool UPDATE_START = false;
	ofColor color;
	ofVbo circleVbo;
};


class RippleAnimation {
public:
	RippleAnimation(void) {};
	~RippleAnimation(void) {};

	void setup(int CircleNum)
	{
		circleNum = CircleNum;
		for (int i = 0; i < circleNum; i++)
		{
			Ripple r;
			r.setup(150, 0, 1, ofColor(255));
			ripples.push_back(r);
		}
	}

	void update()
	{
		for (int i = 0; i < circleNum; i++)
		{
			ripples[i].update();
		}
	}

	void draw()
	{
		for (int i = 0; i < circleNum; i++)
		{
			ripples[i].draw();
			//cout << i << endl;
			//ripples[i].log();
		}
	}

	void count() {

		if (counter == circleNum - 1)
		{
			counter = 0;
		}
		else
		{
			counter++;
		}
		if (counter >= 0)
		{
			ripples[counter].startUpdate();
		}


	}

private:
	int counter = -1;
	int circleNum;
	vector<Ripple>ripples;
};

#endif /* ripple_h */