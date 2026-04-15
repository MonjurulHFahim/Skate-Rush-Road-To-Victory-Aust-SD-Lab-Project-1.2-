#ifndef CHARACTER_H
#define CHARACTER_H


#define STATE_STAND 0
#define STATE_MOVE  1
#define STATE_JUMP  2
#define STATE_HIT   3
struct Player {
	int x, y;
	int width, height;
	int velocityY;
	bool isJumping;
	int groundY;
	int gravity;
	
	int state;        // Tracks STAND, MOVE, JUMP, or HIT
	int images[4];
};

// We only keep the DRAWING and PHYSICS logic here, 
// but we pass the object into them as a reference (&p).
inline void drawPlayer(Player &p) {
	// Pick the image based on current state
	int currentImg = p.images[p.state];

	if (currentImg != -1) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		iShowImage(p.x, p.y, p.width, p.height, currentImg);
		glDisable(GL_BLEND);
	}
	else {
		iSetColor(255, 0, 0);
		iFilledRectangle(p.x, p.y, p.width, p.height);
	}
}

inline void handlePlayerJump(Player &p) {
	if (p.isJumping) {
		p.state = STATE_JUMP; // Automatically switch to jump frame in air
		p.velocityY += p.gravity;
		p.y += p.velocityY;
		if (p.y <= p.groundY) {
			p.y = p.groundY;
			p.isJumping = false;
			p.velocityY = 0;
			p.state = STATE_MOVE; // Return to move frame upon landing
		}
	}
}
#endif