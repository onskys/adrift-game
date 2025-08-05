#include "ofApp.h"
//----------------------------------------------------------------------------------

// The start of this code was created by Kevin M. Smith, additions and changes by me.


//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].pos += sprites[i].velocity / ofGetFrameRate();
	}
}

//  Render all the sprites
//
void SpriteList::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}



void Emitter::randomizePosition() {
	pos = glm::vec3(ofRandomWidth(), ofRandomHeight(), 0);
}



Emitter::Emitter() {
	sys = new SpriteList();
	init();
}


void Emitter::init() {
	hitSound.load("sounds/contactSound.wav");

	lifespan = 3000;    // default milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = true;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	stopped = false;
	width = 50;
	height = 50;

	// childImage.load("images/shipSprite100.png");
}


//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {

	// draw the Emitter itself 
	// note: set drawable=false if you want the emitter to be invisible
	//

	drawable = false;

	if (drawable) {

		ofPushMatrix();
		ofMultMatrix(getTransform());

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
		}
		else {
			// draw some default shape (square) if drawable but no image attached.
			//
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2, -height / 2, width, height);
		}

		ofPopMatrix();
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {

	// if (!started) return;

	// cout << "We started\n";

	float time = ofGetElapsedTimeMillis();


	if ((time - lastSpawned) > (1000.0 / rate)) {

		// cout << "We spawning\n";

		// cout << "sprite spawning??\n";

		// Spawn specified number of agents from GUI slider.
		for (int i = 0; i < agentsPerSpawn; i++) {
			randomizePosition();

			// call virtual to spawn a new sprite
			//
			spawnSprite2();
		}
		
		lastSpawned = time;
	}

	for (int i = 0; i < sys->sprites.size(); i++) {
		moveSprite(&sys->sprites[i]);
	}

	// update sprite list
	//
	if (sys->sprites.size() == 0) return;
	vector<Sprite>::iterator s = sys->sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sys->sprites.end()) {

		// cout << "Started: " << started << "\n";

		// cout << "Stopped: " << stopped << "\n";

		if (stopped) {
			tmp = sys->sprites.erase(s);
			s = tmp;
		}
		// cout << s->lifespan << "\n";

		// Case for normal lifespan deletion.
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			// cout << "deleting sprite: " << s->name << endl;
			tmp = sys->sprites.erase(s);
			s = tmp;
		}
		// Case for collision deletion (should also reduce player energy somehow).
		// Pass a pointer for the current sprite and the player position to the collision checking function.
		else if (collisionChecker(*s, playerPos)) {
			cout << "COLLISION with " << s->name << endl;
			hitSound.play();
			tmp = sys->sprites.erase(s);
			s = tmp;

			playerEnergyTracker--;
		}
		else s++;
	}
	
}

void Emitter::clearAll() {
	if (stopped) return;

	if (sys->sprites.empty()) return;

	sys->sprites.clear();
	
}

// A simple collision detecter with basic radius functionality.
// Works well visually for both triangles and images, but could use upgrades for detailed collision.

// Smaller and larger scales don't work too well visually. Not enough time left to figure out different collision detection.
// Definitely should be worked on in the future if this is to be built upon in the future!
bool Emitter::collisionChecker(const Sprite& sprite, const glm::vec3 playerPos) {

	float playerRadius = playerScaleTracker;
	float spriteRadius = playerScaleTracker;

	if (playerScaleTracker <= 15) {
		float playerRadius = playerScaleTracker - 5;
		float spriteRadius = playerScaleTracker - 5;
	}
	else if (playerScaleTracker >= 25 ) {
		float playerRadius = playerScaleTracker + 20;
		float spriteRadius = playerScaleTracker + 20;
	}
	else {
		float playerRadius = playerScaleTracker + 3;
		float spriteRadius = playerScaleTracker + 3;
	}

	float distance = glm::distance(sprite.pos, playerPos);

	return distance < (playerRadius + spriteRadius);
}

// virtual function to move sprite (can be overloaded)
//
void Emitter::moveSprite(Sprite *sprite) {

	glm::vec3 moveDirection = playerPos - sprite->pos;
	float angle = atan2(moveDirection.y, moveDirection.x) * RAD_TO_DEG;

	// Takes the current angle and target angle and returns a certain percentage of that angle.
	sprite->rotation = ofLerpDegrees(sprite->rotation, angle, 0.01 * rotationSpeed);

	sprite->velocity = glm::normalize(moveDirection) * 30;

	sprite->pos += sprite->velocity / ofGetFrameRate();

	// sprite->rotation += sprite->rotationSpeed;
}


// virtual function to spawn sprite (can be overloaded)
//
void Emitter::spawnSprite2() {
	Sprite sprite;

	// cout << "bad" << "\n";

	// if (haveChildImage) sprite.setImage(childImage);

	if (haveChildImage) {
		int fiftyFifty = rand() % 2;

		if (fiftyFifty == 0) {
			childImage.load("images/enemySprite1.png");
			sprite.setImage(childImage);
		}
		else {
			childImage.load("images/enemySprite2.png");
			sprite.setImage(childImage);
		}

	}

	sprite.imageScale = spriteScale;
	sprite.velocity = velocity;
	sprite.lifespan = lifespan;

	// cout << "good" << "\n";

	// This is where the sprite spawns, at the position of the emitter.
	sprite.pos = pos;
	
	// Give the sprite a random starting rotation.
	sprite.rotation = ofRandom(360);

	// cout << sprite.rotation << "\n";

	sprite.birthtime = ofGetElapsedTimeMillis();
	// cout << "Birthtime: " << sprite.birthtime << "\n";
	sys->add(sprite);
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
}

void Emitter::setRate(float r) {
	rate = r;
}
