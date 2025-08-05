#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"

// Agent Stuff

// Parameters for agents:
// Velocity, Birthtime, Age, Lifespan
// Agents should have both an image and triangle shape available, that track each other.

// Agents have a container class, so we can draw or update all agents.

// Can set an emitter to NULL to delete it from memory

/*
class Emitter : public BaseObject {
public:
    Emitter(SpriteSystem *);
    void draw();
    void start();
    void stop();
    void setLifespan(float);
    void setVelocity(ofVec3f);
    void setChildImage(ofImage); // Set sprite for agents
    // void setImage(ofImage); // Optional
    void setRate(float);
    void update();
}
*/

/*
class AgentEmitter : public Emitter {
 public:
    void AgentEmitter::spawnSprite() { // may have to remove AgentEmitter::
        Agent sprite;
        if (haveChildImage) sprite.setImage(childImage);
        sprite.velocity = velocity;
        sprite.lifespan = lifespan;
        sprite.pos = pos;
        sprite.birthtime = ofGetElapsedTimeMillis();
        sys->add(sprite);
    }

*/

class Agent : public Sprite {
public:
	Agent() {
		Sprite::Sprite();
		//		cout << "in Agent Constuctor" << endl;
	}
};

class AgentEmitter : public Emitter {
public:
	void AgentEmitter::spawnSprite() {
		//		cout << "in AgentEmitter::spawnSprite" << endl;
		Agent sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.pos = pos;
		sprite.birthtime = ofGetElapsedTimeMillis();
		sys->add(sprite);
	}
	void moveSprite(Sprite* sprite) {
		Emitter::moveSprite(sprite);

		// Align path of travel to velocity 
		//
		glm::vec3 v = glm::normalize(sprite->velocity);
		glm::vec3 u = glm::vec3(0, -1, 0);
		
        /*
        float a = glm::orientedAngle(u, v, glm::vec3(0, 0, 1));
		sprite->rotation = glm::degrees(a);
        */
	}
};

class ofApp : public ofBaseApp {

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    bool withinBoundsX(float xPos);
    bool withinBoundsY(float yPos);

    ofxPanel gui;

    TriangleShape playerTriangle;
    ImageShape playerImage;

    glm::vec3 playerPos;

	Emitter* emitter = NULL;

	ofImage defaultImage;
	ofVec3f mouse_last;
	bool imageLoaded;
    bool bHide;
    bool started;
    bool gameOver;

    float elapsedTime;
    float fps;
    int playerEnergy;

    ofImage ship;

    ofImage spaceBackground;

    ofSoundPlayer gameOverSound;

    // GUI elements.
    ofxToggle imageToggle;
    ofxToggle headingToggle;
    ofxIntSlider scaleSlider;
    ofxIntSlider lifespanSlider;
    ofxIntSlider spawnrateSlider;
    ofxIntSlider agentNumSlider;
    ofxIntSlider energySlider;
    ofxIntSlider moveSpeedSlider;
    ofxIntSlider rotationSpeedSlider;
    ofxIntSlider agentRotationSlider;


};


