#ifndef LEVEL1_H
#define LEVEL1_H

#include "config.h"
#include "obstacle.h"
#include "character.h"

extern int bg1, bg2, bg3, bg4, bg5, bg6, bg7;
extern int bgX;
extern bool gameStarted;
extern bool levelFinished;
extern float currentSpeed;


#define TOTAL_OBS 9 
#include "coin.h"

#define TOTAL_COINS 5
Coin lvl1Coins[TOTAL_COINS];
int collectedCount = 0;
int coinImg;
void initLevel1Coins() {
	coinImg = iLoadImage("Image\\coin.png");
	collectedCount = 0;

	lvl1Coins[0] = { 1000, 220, false }; 
	lvl1Coins[1] = { 1600, 350, false }; 
	lvl1Coins[2] = { 2200, 220, false };
	lvl1Coins[3] = { 2800, 380, false }; 
	lvl1Coins[4] = { 3500, 220, false };
}

Player lvl1Hero = { 200, 100, 70, 160, 0, false, 100, -1, STATE_STAND, { -1, -1, -1, -1 } };

Obstacle obstacles[TOTAL_OBS];

// -------------------- DRAW FUNCTION --------------------
inline void drawLevel1Page()
{
	// Draw backgrounds
	iShowImage(bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg1);
	iShowImage(bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg2);
	iShowImage(bgX + 2 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg3);
	iShowImage(bgX + 3 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg4);
	iShowImage(bgX + 4 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg5);
	iShowImage(bgX + 5 * SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg6);


	// 1. Draw Level 1 Coins
	drawCoins(lvl1Coins, TOTAL_COINS, bgX, coinImg);

	// 2. "Always-on" Coin Counter (HUD)
	iSetColor(255, 255, 255);
	iText(20, 560, "COINS:", GLUT_BITMAP_HELVETICA_18);

	char coinStr[15];
	sprintf(coinStr, "%d / %d", collectedCount, TOTAL_COINS);
	iSetColor(255, 215, 0); // Gold
	iText(100, 560, coinStr, GLUT_BITMAP_HELVETICA_18);
	// Player - Passing our specific lvl1Hero object
	drawPlayer(lvl1Hero);

	// Obstacles - Using lvl1Hero's groundY
	drawObstacles(obstacles, TOTAL_OBS, bgX, lvl1Hero.groundY);

	if (!gameStarted && !levelFinished) {
		iSetColor(255, 255, 255);
		iText(420, 300, "PRESS SPACE TO START", GLUT_BITMAP_HELVETICA_18);
	}

	if (levelFinished) {
		// 1. Draw a Dark Semi-Transparent Overlay
		iSetColor(0, 0, 0);
		iFilledRectangle(200, 150, 400, 300); // Center of an 800x600 screen

		// 2. Draw a Border
		iSetColor(255, 255, 255);
		iRectangle(200, 150, 400, 300);

		// 3. Show the Results
		char finishMsg[50];
		if (collectedCount == TOTAL_COINS) {
			iSetColor(0, 255, 0); // Green for Success
			iText(310, 380, "LEVEL COMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 255, 255);
			sprintf(finishMsg, "All %d Coins Collected!", collectedCount);
			iText(320, 330, finishMsg, GLUT_BITMAP_HELVETICA_18);

			// Draw Next Level Button
			iSetColor(50, 50, 255);
			iFilledRectangle(325, 200, 150, 40);
			iSetColor(255, 255, 255);
			iText(350, 215, "NEXT LEVEL", GLUT_BITMAP_HELVETICA_12);
		}
		else {
			iSetColor(255, 0, 0); // Red for Failure
			iText(310, 380, "INCOMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 255, 255);
			sprintf(finishMsg, "Only %d / %d Coins Found", collectedCount, TOTAL_COINS);
			iText(320, 330, finishMsg, GLUT_BITMAP_HELVETICA_18);

			// Draw Retry Button
			iSetColor(200, 50, 50);
			iFilledRectangle(325, 200, 150, 40);
			iSetColor(255, 255, 255);
			iText(370, 215, "RETRY", GLUT_BITMAP_HELVETICA_12);
		}
	}
}
inline void resetLevel1Coins() {
	collectedCount = 0; // Reset the HUD counter
	for (int i = 0; i < TOTAL_COINS; i++) {
		lvl1Coins[i].isCollected = false; // Make coins visible again
	}
}
// -------------------- UPDATE FUNCTION --------------------
inline void updateLevel1()
{
	extern int currentPage;
	if (currentPage != 4) return;
	if (!gameStarted || levelFinished) return;

	bgX -= (int)currentSpeed;
	currentSpeed += 0.002f;

	if (bgX <= -5000)
	{
		bgX = -5000;
		levelFinished = true;
		gameStarted = false;
		lvl1Hero.state = STATE_STAND;
		
		static bool soundPlayed = false;
		if (!soundPlayed) {
			if (collectedCount == TOTAL_COINS) PlaySound("Audio\\win.wav", NULL, SND_ASYNC);
			else PlaySound("Audio\\fail.wav", NULL, SND_ASYNC);
			soundPlayed = true;
		}
		return;
	}
	

	// Logic for specific player
	handlePlayerJump(lvl1Hero);

	// Collision Check using lvl1Hero data
	if (checkCollision(lvl1Hero.x, lvl1Hero.y, lvl1Hero.width, lvl1Hero.height, obstacles, TOTAL_OBS, bgX, lvl1Hero.groundY)) {
		PlaySound("Audio\\crash.wav", NULL, SND_ASYNC);
		lvl1Hero.state = STATE_HIT;
		bgX = 0;
		lvl1Hero.y = lvl1Hero.groundY;
		lvl1Hero.velocityY = 0;
		lvl1Hero.isJumping = false;
		gameStarted = false;
		currentSpeed = 8.0f;
		resetLevel1Coins();
	}
	for (int i = 0; i < TOTAL_COINS; i++) {
		if (!lvl1Coins[i].isCollected) {
			// Check if Hero overlaps with the moving coin (coin.x + bgX)
			if (lvl1Hero.x + lvl1Hero.width >(lvl1Coins[i].x + bgX) &&
				lvl1Hero.x < (lvl1Coins[i].x + bgX) + 30 &&
				lvl1Hero.y + lvl1Hero.height > lvl1Coins[i].y &&
				lvl1Hero.y < lvl1Coins[i].y + 30) {

				lvl1Coins[i].isCollected = true;
				collectedCount++;
				// Play sound effect here if you have one!
				PlaySound("Audio\\ping.wav", NULL, SND_ASYNC);
				
			}
		}
	}
}
#endif