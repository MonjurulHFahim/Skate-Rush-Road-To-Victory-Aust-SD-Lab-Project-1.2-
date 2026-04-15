#ifndef LEVEL2_H
#define LEVEL2_H

#include "config.h"
#include "obstacle.h"
#include "character.h"

extern int bg11, bg12, bg13, bg14, bg15, bg16,bg17, bg18, bg19, bg20;
extern int bg1X;
extern bool gameStarted;
extern bool levelFinished;
extern float currentSpeed;



int birdImages[2];      // To hold bird1.png and bird2.png
float birdX_pos = 2000; // The initial X position (far to the right)
float birdY_pos = 250;  // Height: 250 is usually jump-able but hit-able
int birdFrame = 0;      // Current animation frame (0 or 1)
float birdAnimTimer = 0;


float bird2X_pos = 6400;
float bird2Y_pos = 100;  
#define TOTAL_OBS_L2 12
#define TOTAL_COINS_L2 8

Player lvl2Hero = { 230, 75, 70, 160, 0, false, 75, -1, STATE_STAND, { -1, -1, -1, -1 } };
Obstacle obstaclesL2[TOTAL_OBS_L2];
Coin lvl2Coins[TOTAL_COINS_L2];

int collectedCountL2 = 0;
int coinImgL2;

void initLevel2Assets() {
	coinImgL2 = iLoadImage("Image\\coin.png");
	collectedCountL2 = 0;

	// Position 8 Coins
	
	lvl2Coins[0] = { 1200, 200, false };
	lvl2Coins[1] = { 1900, 350, false };
	lvl2Coins[2] = { 2400, 220, false };
	lvl2Coins[3] = { 3400, 400, false };
	lvl2Coins[4] = { 4100, 200, false };
	lvl2Coins[5] = { 5000, 350, false };
	lvl2Coins[6] = { 5900, 220, false };
	lvl2Coins[7] = { 7000, 350, false };

	birdImages[0] = iLoadImage("Image\\bird1.png");
	birdImages[1] = iLoadImage("Image\\bird2.png");

	birdX_pos = 2500;

}
inline void resetLevel2Coins() {
	collectedCountL2 = 0; // Reset the HUD counter
	for (int i = 0; i < TOTAL_COINS_L2; i++) {
		lvl2Coins[i].isCollected = false; // Make coins visible again
	}
}
inline void drawLevel2Page()
{
	// Draw backgrounds
	iShowImage(bg1X, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg11);
	iShowImage(bg1X + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg12);
	iShowImage(bg1X + 2 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg13);
	iShowImage(bg1X + 3 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg14);
	iShowImage(bg1X + 4 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg15);
	iShowImage(bg1X + 5 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg16);
	iShowImage(bg1X + 6 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg17);
	iShowImage(bg1X + 7 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg18);
	iShowImage(bg1X + 8 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg19);

	// Draw Coins and HUD
	drawCoins(lvl2Coins, TOTAL_COINS_L2, bg1X, coinImgL2);
	iSetColor(255, 255, 255);
	iText(20, 560, "COINS:", GLUT_BITMAP_HELVETICA_18);
	char coinStr[15];
	sprintf(coinStr, "%d / %d", collectedCountL2, TOTAL_COINS_L2);
	iSetColor(255, 215, 0);
	iText(100, 560, coinStr, GLUT_BITMAP_HELVETICA_18);

	drawObstacles(obstaclesL2, TOTAL_OBS_L2, bg1X, lvl2Hero.groundY);
	// Player - Passing our specific lv21Hero object
	drawPlayer(lvl2Hero);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	iShowImage(birdX_pos + bg1X, birdY_pos, 80, 60, birdImages[birdFrame]);

	iShowImage(bird2X_pos + bg1X, bird2Y_pos, 80, 60, birdImages[birdFrame]);
	glDisable(GL_BLEND);


	if (!gameStarted && !levelFinished) {
		iSetColor(255, 255, 255);
		iText(420, 300, "PRESS SPACE TO START", GLUT_BITMAP_HELVETICA_18);
	}
	if (levelFinished) {
		// 1. Draw a Dark Semi-Transparent Overlay
		iSetColor(0, 0, 0);
		iFilledRectangle(200, 150, 400, 300);

		// 2. Draw a Border
		iSetColor(255, 255, 255);
		iRectangle(200, 150, 400, 300);

		// 3. Show the Results
		char finishMsg[50];
		if (collectedCountL2 == TOTAL_COINS_L2) {
			iSetColor(0, 255, 0); // Green for Success
			iText(310, 380, "LEVEL 2 COMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 255, 255);
			sprintf(finishMsg, "All %d Coins Collected!", collectedCountL2);
			iText(320, 330, finishMsg, GLUT_BITMAP_HELVETICA_18);

			// Draw Next Level Button (Level 3)
			iSetColor(50, 50, 255);
			iFilledRectangle(325, 200, 150, 40);
			iSetColor(255, 255, 255);
			iText(350, 215, "NEXT LEVEL", GLUT_BITMAP_HELVETICA_12);
		}
		else {
			iSetColor(255, 0, 0); // Red for Failure
			iText(310, 380, "INCOMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 255, 255);
			sprintf(finishMsg, "Only %d / %d Coins Found", collectedCountL2, TOTAL_COINS_L2);
			iText(320, 330, finishMsg, GLUT_BITMAP_HELVETICA_18);

			// Draw Retry Button
			iSetColor(200, 50, 50);
			iFilledRectangle(325, 200, 150, 40);
			iSetColor(255, 255, 255);
			iText(370, 215, "RETRY", GLUT_BITMAP_HELVETICA_12);
		}
	}
}



inline void updateLevel2()
{
	extern int currentPage;
	if (currentPage != 5) return;
	if (!gameStarted || levelFinished) return;

	
	bg1X -= (int)currentSpeed;
	
	currentSpeed += 0.006f; 

	if (bg1X <= -8000)
	{
		bg1X = -8000;
		levelFinished = true;
		gameStarted = false;
		lvl2Hero.state = STATE_STAND;

		static bool soundPlayedL2 = false;
		if (!soundPlayedL2) {
			if (collectedCountL2 == TOTAL_COINS_L2) PlaySound("Audio\\win.wav", NULL, SND_ASYNC);
			else PlaySound("Audio\\fail.wav", NULL, SND_ASYNC);
			soundPlayedL2 = true;
		}
		return;
	}
	
	handlePlayerJump(lvl2Hero);

	// 1. Handle Animation (Flapping)
	birdAnimTimer += 0.1;
	if (birdAnimTimer > 1.0) {
		birdFrame = (birdFrame == 0) ? 1 : 0;
		birdAnimTimer = 0;
	}


	float currentBirdX = birdX_pos + bg1X; // Bird moves relative to background
	if (lvl2Hero.x + lvl2Hero.width > currentBirdX + 10 &&
		lvl2Hero.x < currentBirdX + 70 &&
		lvl2Hero.y + lvl2Hero.height > birdY_pos + 10 &&
		lvl2Hero.y < birdY_pos + 50) {

		// Standard Crash Logic (same as your obstacles)
		PlaySound("Audio\\crash.wav", NULL, SND_ASYNC);
		lvl2Hero.state = STATE_HIT;
		bg1X = 0;
		lvl2Hero.y = lvl2Hero.groundY;
		gameStarted = false;
		currentSpeed = 10.0f;
		resetLevel2Coins();
	}

	

	
	float currentBird2X = bird2X_pos + bg1X;

	
	if (lvl2Hero.x + lvl2Hero.width > currentBird2X + 10 &&
		lvl2Hero.x < currentBird2X + 70 &&
		lvl2Hero.y + lvl2Hero.height > bird2Y_pos + 10 &&
		lvl2Hero.y < bird2Y_pos + 50) {

		PlaySound("Audio\\crash.wav", NULL, SND_ASYNC);
		lvl2Hero.state = STATE_HIT;
		bg1X = 0;
		lvl2Hero.y = lvl2Hero.groundY;
		lvl2Hero.velocityY = 0;
		lvl2Hero.isJumping = false;
		gameStarted = false;
		currentSpeed = 10.0f; // Level 2 Reset Speed
		resetLevel2Coins();
	}

	// Collision with Obstacles
	if (checkCollision(lvl2Hero.x, lvl2Hero.y, lvl2Hero.width, lvl2Hero.height, obstaclesL2, TOTAL_OBS_L2, bg1X, lvl2Hero.groundY)) {
		PlaySound("Audio\\crash.wav", NULL, SND_ASYNC);
		lvl2Hero.state = STATE_HIT;
		// RESET LOGIC
		bg1X = 0;
		lvl2Hero.y = lvl2Hero.groundY;
		lvl2Hero.velocityY = 0;
		lvl2Hero.isJumping = false;
		gameStarted = false;
		currentSpeed = 10.0f; 

		resetLevel2Coins();
	}

	// Collection of Coins
	for (int i = 0; i < TOTAL_COINS_L2; i++) {
		if (!lvl2Coins[i].isCollected) {
			if (lvl2Hero.x + lvl2Hero.width >(lvl2Coins[i].x + bg1X) &&
				lvl2Hero.x < (lvl2Coins[i].x + bg1X) + 60 &&
				lvl2Hero.y + lvl2Hero.height > lvl2Coins[i].y &&
				lvl2Hero.y < lvl2Coins[i].y + 60) {
				lvl2Coins[i].isCollected = true;
				collectedCountL2++;
				PlaySound("Audio\\ping.wav", NULL, SND_ASYNC);
			}
		}
	}


}



#endif