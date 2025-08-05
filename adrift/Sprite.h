#pragma once

#include "Shape.h"



// Base class for a Sprite. Can be instantiated on it's own (deafult)
// or subclassed to create a custom sprite.  Includes Shape transformations.
// If the sprite has no image set on it, then a simple triangle will be drawn.
//

class Sprite : public Shape {
public:
	Sprite() {
		// default geometry (triangle) if no image attached.
		//
		verts.push_back(glm::vec3(-1, 1.5, 0));
		verts.push_back(glm::vec3(1, 1.5, 0));
		verts.push_back(glm::vec3(0, -1.5, 0));
	}
	
	// some functions for highlighting when selected
	//
	
	void draw() {
		if (bShowImage) {
			ofPushMatrix();
			ofSetColor(ofColor::white);
			ofMultMatrix(getTransform());

			// For some reason, the rotation is off by 90 degrees but ONLY visually.
			// Meaning, the sprites were moving in the correct direction, but the actual image or triangle was not oriented properly.
			// This is a quick fix for this bug.
			ofRotateDeg(90);

			// glm::vec3 scaleVec = glm::vec3(imageScale, imageScale, 1);

			// spriteImage.scale = scaleVec;
			
			// spriteImage.draw(-spriteImage.getWidth() / 2, -spriteImage.getHeight() / 2.0);
			float scaleFactor = imageScale / 300.0;
			ofScale(scaleFactor / 2, scaleFactor / 2);

			spriteImage.draw(-spriteImage.getWidth() / 2, -spriteImage.getHeight() / 2.0);

			ofPopMatrix();
		}
		else
		{
			if (bHighlight) ofSetColor(ofColor::white);
			else ofSetColor(ofColor::yellow);
			ofPushMatrix();
			ofMultMatrix(getTransform());

			// cout << rotation << "\n";

			// For some reason, the rotation is off by 90 degrees but ONLY visually.
			// Meaning, the sprites were moving in the correct direction, but the actual image or triangle was not oriented properly.
			// This is a quick fix for this bug.
			ofRotateDeg(90);

			ofDrawTriangle(verts[0], verts[1], verts[2]);

			ofPopMatrix();
		}
	}
	

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime);
	}

	void setImage(ofImage img) {
		spriteImage = img;
		bShowImage = true;
		width = img.getWidth();
		height = img.getHeight();
	}

	virtual bool inside(const glm::vec3 p);
	virtual void update() {}

	bool insideTriangle(const glm::vec3 p);
	
	// void draw();
	void setSelected(bool state) { bSelected = state; }
	void setHighlight(bool state) { bHighlight = state; }
	bool isSelected() { return bSelected; }
	bool isHighlight() { return bHighlight; }

	bool bHighlight = false;
	bool bSelected = false;
	bool bShowImage = false;

	glm::vec3 velocity = glm::vec3(0, 0, 0);
	float rotationSpeed = 0.0;
	float birthtime = 0; // elapsed time in ms
	float lifespan = -1;  //  time in ms
	string name =  "enemy";
	float width = 40;
	float height = 40;
	int imageScale;

	// glm::vec3 playerPos;

	ofImage spriteImage;

	// default verts for polyline shape if no image on sprite
	//
	vector<glm::vec3> verts;
};

