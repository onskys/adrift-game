#include "ofApp.h"
#include <math.h>

// PLEASE NOTE:
// Sprite images go in /bin/images.
// Sound files go in /bin/sounds.
// Please make these directories in /bin to run the game!
// 
// 
// 
// Two sound effects are used in this project.
// Sound effect credits go to Mixkit.co.
// Sounds effects are copyright free, royalty free, and are free assets that are free to download from mixkit.co.
// 
// Sprites created by me, Owen Semersky.
// Four sprites (including the background) are used in this game, all created by me, Owen Semersky.
// The ship sprites and background sprite are new for this project specifically.
// The ship sprite was submitted for Lab 1 and reused here, so credit goes to the original author, Owen Semersky (me).
// 
//--------------------------------------------------------------
void ofApp::setup() {
    gui.setup();
    gui.add(imageToggle.setup("Show Sprites", false));
    gui.add(headingToggle.setup("Show Player Heading", false));
    gui.add(scaleSlider.setup("Player/Agent Scale", 20, 10, 30));
    gui.add(energySlider.setup("Player Energy", 5, 1, 10));
    gui.add(lifespanSlider.setup("Enemy Lifespan", 10, 5, 20));
    gui.add(spawnrateSlider.setup("Enemy Spawnrate", 1, 1, 10));
    gui.add(agentNumSlider.setup("Agents/Spawn", 1, 1, 5));
    gui.add(agentRotationSlider.setup("Agent Rotation Speed", 1, 1, 10));
    gui.add(moveSpeedSlider.setup("Movement Speed", 10, 5, 15));
    gui.add(rotationSpeedSlider.setup("Rotation Speed", 6, 3, 10));

    ship.load("images/shipSprite100.png");

    spaceBackground.load("images/spaceBackground1.png");

    gameOverSound.load("sounds/gameOver1.wav");

    ofSetVerticalSync(true);

    
    // create an image for sprites being spawned by emitter
    //
    if (defaultImage.load("images/enemySprite1.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        //		ofExit();
    }

    ofSetBackgroundColor(ofColor::black);



    // create an array of emitters and set their position;
    //

    started = false;

    gameOver = false;

    playerEnergy = energySlider;

    ofSeedRandom();

    playerPos = glm::vec3(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);

    emitter = new AgentEmitter();  // C++ polymorphism

    emitter->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    emitter->drawable = false;
    emitter->spriteScale = scaleSlider;
    if (imageLoaded) emitter->setChildImage(defaultImage);
    emitter->playerEnergyTracker = playerEnergy;
    // emitter->start();

    bHide = false;
    

}

//--------------------------------------------------------------
TriangleShape::TriangleShape() {

    // Set up verticies for a triangle.
    verts.push_back(glm::vec3(1, 1, 0));
    verts.push_back(glm::vec3(0, -2, 0));
    verts.push_back(glm::vec3(-1, 1, 0));

    // Set scale and position at screen center for a triangle.
    scale = glm::vec3(50, 50, 1);
    pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);

}

//--------------------------------------------------------------
void TriangleShape::draw(float scaleVar, bool headingToggle, bool imageToggle) {

    ofPushMatrix();
    ofMultMatrix(getTransform());

    // draw stuff
    //
    ofFill();
    ofSetColor(255, 0, 0);
    ofDrawTriangle(verts[0], verts[1], verts[2]);

    ofPopMatrix();

    scale = glm::vec3(scaleVar, scaleVar, 1);

    ofPushMatrix();
    ofMultMatrix(getTransform());

    ofSetColor(255, 255, 255);

    // Same comments for image heading apply here.
    if (headingToggle) {
        float headingLineLength = 30 * (scaleVar / 100);
        ofDrawLine(0, 0, verts[1].x, (verts[1].y - headingLineLength));
    }

    ofPopMatrix();

}

//--------------------------------------------------------------
ImageShape::ImageShape() {
    // The image shape uses these vertices for some calculations.
    verts.push_back(glm::vec3(1, 1, 0));
    verts.push_back(glm::vec3(0, -2, 0));
    verts.push_back(glm::vec3(-1, 1, 0));

    int w = ship1.getWidth();
    int h = ship1.getHeight();

    scale = glm::vec3(50, 50, 1);
    pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);

    // Had some problems with the image not loading, so I loaded it in several places just in case.
    // I don't think doing this has an effect on the image itself.
    ship1.load("images/shipSprite100.png");

    if (ship1.load("images/shipSprite100.png")) {
        cout << "image loaded" << endl;
    }

    cout << "image width " << w << endl;
    cout << "image height " << h << endl;

}

//--------------------------------------------------------------
void ImageShape::draw(float scaleVar, bool headingToggle, bool imageToggle, ofImage myShip) {
    // Now uses a square shape instead of a triangle shape!
    scale = glm::vec3(scaleVar, scaleVar, 1);

    // This retrieves the matrix for the shape and applies it to the points, heading, and more!
    ofPushMatrix();
    ofMultMatrix(getTransform());

    // Uses ofScale instead of resize, spent ages wondering why resize made the resolution so bad.

    // Scale does not work as well for image as it does for the triangle, so it is being manually decreased.
    // Not sure why the original scale variable makes the image so large, as it
    // seems to work fine for the triangle.
    float scaleFactor = scaleVar / 300.0;
    ofScale(scaleFactor / 2, scaleFactor / 2);

    // Draw on an offset, otherwise scaling will not be centered.
    myShip.draw(-myShip.getWidth() / 2, -myShip.getHeight() / 2);

    // Once again, the scale variable is being decreased.
    // Heading is rather long, but ends up pretty short for smaller scales.
    // Difficult to find a good in-between.
    if (headingToggle) {
        float headingLineLength = (scaleVar);
        ofDrawLine(0, 0, verts[1].x, (verts[1].y - headingLineLength) * 30);
    }

    ofPopMatrix();

}

//--------------------------------------------------------------
/*
glm::mat4 Shape::getTransform() {
    glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
    glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);

    return (trans * rot * scale);
}
*/


//--------------------------------------------------------------
void ofApp::update() {

    // emitter->started = true;
    // emitter->update();

    if (started) {

        // emitter->playerEnergyTracker = playerEnergy;
        playerEnergy = emitter->playerEnergyTracker;

        emitter->started = true;

        if (playerEnergy <= 0) {
            gameOver = true;
            emitter->clearAll();
            emitter->stopped = true;
            started = false;
            gameOverSound.play();
            return;
        }

        emitter->spriteScale = scaleSlider;

        if (imageToggle) {
            emitter->haveChildImage = true;
        }
        else {
            emitter->haveChildImage = false;
        }
        emitter->setRate(spawnrateSlider);
        emitter->setLifespan(lifespanSlider * 1000);    // convert to milliseconds 
        emitter->setVelocity(ofVec3f(moveSpeedSlider, moveSpeedSlider, 0));
        emitter->playerScaleTracker = scaleSlider;
        emitter->rotationSpeed = agentRotationSlider;
        emitter->agentsPerSpawn = agentNumSlider;
        emitter->update();

        for (int i = 0; i < emitter->sys->sprites.size(); i++) {

            // Get values from sliders and update sprites dynamically
            //
            float sc = scaleSlider;
            float rs = rotationSpeedSlider;
            emitter->sys->sprites[i].scale = glm::vec3(sc, sc, sc);
            emitter->sys->sprites[i].rotationSpeed = rs;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {

    if (gameOver) {
        ofDrawBitmapString("GAME OVER", (ofGetWindowWidth() / 2) - 45, (ofGetWindowHeight() / 2) - 50);
        ofDrawBitmapString("YOUR TIME: " + ofToString(elapsedTime, 1) + " seconds", (ofGetWindowWidth() / 2) - 100, (ofGetWindowHeight() / 2));
        ofDrawBitmapString("PRESS ENTER TO PLAY AGAIN", (ofGetWindowWidth() / 2) - 100, (ofGetWindowHeight() / 2) + 75);
        ofDrawBitmapString("CHANGE SETTINGS HERE BEFORE PLAYING AGAIN", (ofGetWindowWidth() / 2) - 160, (ofGetWindowHeight() / 2) + 100);

        playerEnergy = energySlider;
        emitter->playerEnergyTracker = playerEnergy;

        emitter->started = false;

        gui.draw();
        return;
    }

    if (!started) {
        ofDrawBitmapString("A D R I F T", (ofGetWindowWidth() / 2) - 40, (ofGetWindowHeight() / 2) - 30);
        ofDrawBitmapString("PRESS SPACE TO START -- EVADE AS LONG AS POSSIBLE -- GOOD LUCK", glm::vec3((ofGetWindowWidth() / 2) - 250, ofGetWindowHeight() / 2, 0));
        gui.draw();
        playerEnergy = energySlider;
        emitter->playerEnergyTracker = playerEnergy;
    }
    else {

        if (imageToggle) {
            // cout << "Drawing background\n";
            spaceBackground.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        else {
            ofBackground(0, 0, 0, 0);
        }

        elapsedTime = ofGetElapsedTimef();
        fps = ofGetFrameRate();

        ofDrawBitmapString("Time: " + ofToString(elapsedTime, 1), ofGetWindowWidth() - 100, 20);
        ofDrawBitmapString("FPS: " + ofToString(fps, 0), ofGetWindowWidth() - 100, 50);
        ofDrawBitmapString("Energy: " + ofToString(playerEnergy), (ofGetWindowWidth() / 2) - 250, 20);


        emitter->draw();

        if (!bHide) {
            gui.draw();
        }

        if (!imageToggle) {
            playerTriangle.draw(scaleSlider, headingToggle, imageToggle);

            playerImage.pos.x = playerTriangle.pos.x;
            playerImage.pos.y = playerTriangle.pos.y;

            playerImage.rotation = playerTriangle.rotation;

            playerPos = playerImage.pos;
            emitter->playerPos = playerPos;
        }
        else {
            playerTriangle.pos.x = playerImage.pos.x;
            playerTriangle.pos.y = playerImage.pos.y;

            playerPos = playerImage.pos;
            emitter->playerPos = playerPos;

            playerTriangle.rotation = playerImage.rotation;

            ship.load("images/shipSprite100.png");

            float scaleVar = scaleSlider;

            playerImage.draw(scaleSlider, headingToggle, imageToggle, ship);

        }
    }
}

//--------------------------------------------------------------
void ofApp::exit() {

}

//--------------------------------------------------------------
bool ofApp::withinBoundsX(float xPos) {
    if (xPos >= 0 && xPos <= ofGetWindowWidth()) {
        return true;
    }
    else {
        return false;
    }
}

//--------------------------------------------------------------
bool ofApp::withinBoundsY(float yPos) {
    if (yPos >= 0 && yPos <= ofGetWindowHeight()) {
        return true;
    }
    else {
        return false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    switch (key) {
    case ' ': {
        if (started == false) {
            started = true;
            emitter->started = true;
            ofResetElapsedTimeCounter();
        }
        break;
    }
    case OF_KEY_RETURN: {
        if (gameOver == true) {
            // emitter->clearAll();
            emitter->stopped = false;
            gameOver = false;
            started = true;
            emitter->started = true;
            playerEnergy = energySlider;
            emitter->playerEnergyTracker = playerEnergy;
            emitter->lastSpawned = 0.0;
            ofResetElapsedTimeCounter();
        }
        break;
    }
    case 'f': {
        ofToggleFullscreen();
        break;
    }
    case 'F': {
        ofToggleFullscreen();
        break;
    }
    case OF_KEY_LEFT: {
        if (imageToggle) {
            playerImage.rotation -= rotationSpeedSlider;
        }
        else {
            playerTriangle.rotation -= rotationSpeedSlider;
        }
        break;
    }
    case OF_KEY_RIGHT: {
        if (imageToggle) {
            playerImage.rotation += rotationSpeedSlider;
            ship.rotate90(0.022);
        }
        else {
            playerTriangle.rotation += rotationSpeedSlider;
        }
        break;
    }
    case OF_KEY_UP: {
        if (imageToggle) {
            float radAngle1 = ofDegToRad(playerImage.rotation);
            float forwardMoveX = sin(radAngle1);
            float forwardMoveY = cos(radAngle1);

            // This screen bound thing needs work                
            if (playerImage.pos.x <= 0 || playerImage.pos.x >= ofGetWindowWidth() || playerImage.pos.y <= 0 || playerImage.pos.y >= ofGetWindowHeight()) {
                float tempPosX = playerImage.pos.x + moveSpeedSlider * forwardMoveX;
                float tempPosY = playerImage.pos.y - moveSpeedSlider * forwardMoveY;

                if (withinBoundsX(tempPosX) && withinBoundsY(tempPosY)) {
                    playerImage.pos.x += moveSpeedSlider * forwardMoveX;
                    playerImage.pos.y -= moveSpeedSlider * forwardMoveY;
                }
                
                break;
            }
            else {
                playerImage.pos.x += moveSpeedSlider * forwardMoveX;
                playerImage.pos.y -= moveSpeedSlider * forwardMoveY;
            }
        }
        else {
            float radAngle1 = ofDegToRad(playerTriangle.rotation);
            float forwardMoveX = sin(radAngle1);
            float forwardMoveY = cos(radAngle1);

            // Move triangle forward.
            // cout << tri.rotation;

            if (playerTriangle.pos.x <= 0 || playerTriangle.pos.x >= ofGetWindowWidth() || playerTriangle.pos.y <= 0 || playerTriangle.pos.y >= ofGetWindowHeight()) {
                float tempPosX = playerTriangle.pos.x + moveSpeedSlider * forwardMoveX;
                float tempPosY = playerTriangle.pos.y - moveSpeedSlider * forwardMoveY;

                if (withinBoundsX(tempPosX) && withinBoundsY(tempPosY)) {
                    playerTriangle.pos.x += moveSpeedSlider * forwardMoveX;
                    playerTriangle.pos.y -= moveSpeedSlider * forwardMoveY;
                }

                break;
            }
            else {
                playerTriangle.pos.x += moveSpeedSlider * forwardMoveX;
                playerTriangle.pos.y -= moveSpeedSlider * forwardMoveY;
            }
        }
        break;
    }
    case OF_KEY_DOWN: {
        float radAngle1 = ofDegToRad(playerTriangle.rotation);
        float forwardMoveX = sin(radAngle1);
        float forwardMoveY = cos(radAngle1);

        if (imageToggle) {

            // Move image backward.
            // cout << tri.rotation;

            if (playerImage.pos.x <= 0 || playerImage.pos.x >= ofGetWindowWidth() || playerImage.pos.y <= 0 || playerImage.pos.y >= ofGetWindowHeight()) {
                float tempPosX = playerImage.pos.x - moveSpeedSlider * forwardMoveX;
                float tempPosY = playerImage.pos.y + moveSpeedSlider * forwardMoveY;

                if (withinBoundsX(tempPosX) && withinBoundsY(tempPosY)) {
                    playerImage.pos.x -= moveSpeedSlider * forwardMoveX;
                    playerImage.pos.y += moveSpeedSlider * forwardMoveY;
                }

                break;
            }
            else {
                playerImage.pos.x -= moveSpeedSlider * forwardMoveX;
                playerImage.pos.y += moveSpeedSlider * forwardMoveY;
            }

            // playerImage.pos.x -= moveSpeedSlider * forwardMoveX;
            // playerImage.pos.y += moveSpeedSlider * forwardMoveY;
        }
        else {
            float radAngle1 = ofDegToRad(playerTriangle.rotation);
            float forwardMoveX = sin(radAngle1);
            float forwardMoveY = cos(radAngle1);

            // Move triangle backward.
            // cout << tri.rotation;

            if (playerTriangle.pos.x <= 0 || playerTriangle.pos.x >= ofGetWindowWidth() || playerTriangle.pos.y <= 0 || playerTriangle.pos.y >= ofGetWindowHeight()) {
                float tempPosX = playerTriangle.pos.x - moveSpeedSlider * forwardMoveX;
                float tempPosY = playerTriangle.pos.y + moveSpeedSlider * forwardMoveY;

                if (withinBoundsX(tempPosX) && withinBoundsY(tempPosY)) {
                    playerTriangle.pos.x -= moveSpeedSlider * forwardMoveX;
                    playerTriangle.pos.y += moveSpeedSlider * forwardMoveY;
                }

                break;
            }
            else {
                playerTriangle.pos.x -= moveSpeedSlider * forwardMoveX;
                playerTriangle.pos.y += moveSpeedSlider * forwardMoveY;
            }
            
            // playerTriangle.pos.x -= moveSpeedSlider * forwardMoveX;
            // playerTriangle.pos.y += moveSpeedSlider * forwardMoveY;
        }
        break;
    }
    default: {
        break;
    }
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {

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
