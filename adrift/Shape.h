#pragma once

#include "ofMain.h"


// Basic Shape class supporting matrix transformations and drawing.
// 
//
class Shape {
public:
	Shape() {


	}
	virtual void draw() {

		// draw a box by defaultd if not overridden
		//
		ofPushMatrix();
		ofMultMatrix(getTransform());
		ofDrawBox(defaultSize);
		ofPopMatrix();
	}

	virtual bool inside(glm::vec3 p) {
		return false;
	}

	glm::mat4 getTransform() {
		glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);      // added this scale if you want to change size of object
		return T*R*S;
	}
	glm::vec3 pos;
	float rotation = 0.0;    // degrees 
	glm::vec3 scale = glm::vec3(1, 1, 1);
	float defaultSize = 20.0;

	
	
};

class TriangleShape : public Shape {
public:
	TriangleShape();
	// bool inside(glm::vec3 p);
	void draw(float scaleVar, bool headingToggle, bool imageToggle);

	vector<glm::vec3> verts;
};


class ImageShape : public Shape {
public:
	ImageShape();
	// bool inside(glm::vec3 p);
	void draw(float scaleVar, bool headingToggle, bool imageToggle, ofImage myShip);

	// Same verts as triangle, since the ship is vaguely a triangle shape.
	vector<glm::vec3> verts;

	// The ship actually being drawn.
	ofImage ship1;
};