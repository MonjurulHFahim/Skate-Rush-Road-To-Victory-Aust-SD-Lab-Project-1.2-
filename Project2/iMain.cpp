#define _CRT_SECURE_NO_WARNINGS
#include "iGraphics.h"
#include "ui.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"

#include "config.h"

int bg1, bg2, bg3, bg4, bg5, bg6, bg7;
int bgX = 0;

int bg11, bg12, bg13, bg14, bg15, bg16, bg17,bg18,bg19,bg20;
int bg1X = 0;

bool gameStarted = false;
bool levelFinished = false; 
float currentSpeed = 8.0f;

int currentPage = 0;
bool menuMusicPlaying = false;
bool musicOn = true;

void iDraw() {
	iClear();
	updateMenuMusic();
	if (currentPage == 0) drawHomePage();
	else if (currentPage == 1) drawStartPage();
	else if (currentPage == 2) drawAboutUsPage();
	else if (currentPage == 3) drawInstructionPage();
	else if (currentPage == 4) drawLevel1Page();
	else if (currentPage == 5) drawLevel2Page();
	else if (currentPage == 6) drawLevel3Page();
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {
	mouseX = mx;
	mouseY = my;
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentPage == 0) {
			if ((mx >= 701 && mx <= 909) && (my >= 406 && my <= 490))  currentPage = 1;
			else if ((mx >= 701 && mx <= 909) && (my >= 300 && my <= 382))  currentPage = 2;
			else if ((mx >= 701 && mx <= 909) && (my >= 185 && my <= 281)) currentPage = 3;
		}
		else if (currentPage == 1) {
			if ((mx >= 140 && mx <= 340) && (my >= 450 && my <= 530)) {
				currentPage = 4;
				gameStarted = false;
				levelFinished = false; 
				bgX = 0;
				currentSpeed = 8.0f;
				lvl1Hero.y = lvl1Hero.groundY;
			}
			else if ((mx >= 144 && mx <= 344) && (my >= 325 && my <= 405)) {
				currentPage = 5;
				gameStarted = false;
				levelFinished = false; 
				bg1X = 0;
				currentSpeed = 10.0f;
				lvl2Hero.y = lvl2Hero.groundY;

				resetLevel2Coins();
			}
			else if ((mx >= 144 && mx <= 344) && (my >= 200 && my <= 280)) currentPage = 6;
			}
		
			
		
		else if (currentPage == 4) {
			if (levelFinished) {
				if (mx >= 325 && mx <= 475 && my >= 200 && my <= 240){
					if (collectedCount == TOTAL_COINS) {
						
						currentPage = 5;
						levelFinished = false;
						collectedCount = 0;
						bgX = 0; 
					}
					else {
						// RETRY LEVEL 1
						bgX = 0;
						levelFinished = false;
						collectedCount = 0;
						resetLevel1Coins();
						gameStarted = false;
					}
				}
			}
		}
	

		else if (currentPage == 5) {
			if (levelFinished) {
				
				if (mx >= 325 && mx <= 475 && my >= 200 && my <= 240) {
					if (collectedCountL2 == TOTAL_COINS_L2) {
						// GO TO LEVEL 3
						currentPage = 6;
						levelFinished = false;
						gameStarted = false;
					}
					else {
						// RETRY LEVEL 2
						bg1X = 0;
						levelFinished = false;
						gameStarted = false;
						currentSpeed = 10.0f; 
						lvl2Hero.y = lvl2Hero.groundY;
						resetLevel2Coins();   
					}
				}
			}
		}
		else if (currentPage == 6 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			l3MouseThrow(mx, my);
		}
	}
}

void iKeyboard(unsigned char key) {
	if (key == '\b') {
		if (currentPage >= 4) currentPage = 1;
		else if (currentPage >= 1) currentPage = 0;
	}
	// Regular Jump with SPACE
	if (currentPage == 4 && key == ' ') {
		if (!gameStarted && !levelFinished) gameStarted = true;
		if (!lvl1Hero.isJumping) {
			lvl1Hero.isJumping = true;
			lvl1Hero.velocityY = 20;
			PlaySound("Audio\\jump.wav", NULL, SND_ASYNC);
		}
	}

	// High Jump with 'w'
	if (currentPage == 4 && (key == 'w' || key == 'W')) {
		if (!gameStarted && !levelFinished) gameStarted = true;
		if (!lvl1Hero.isJumping) {
			lvl1Hero.isJumping = true;
			lvl1Hero.velocityY = 25;
			//PlaySound("Audio\\jump.wav", NULL, SND_ASYNC);
			mciSendString("play Audio\\jump.wav from 0", NULL, 0, NULL);
		}
	}
	if (currentPage == 5 && key == ' ') {
		if (!gameStarted && !levelFinished) gameStarted = true;
		if (!lvl2Hero.isJumping) {
			lvl2Hero.isJumping = true;
			lvl2Hero.velocityY = 20;
			PlaySound("Audio\\jump.wav", NULL, SND_ASYNC);
		}
	}
	if (currentPage == 6) {
		l3HandleInput(key);
	}

}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_END) exit(0);

	
}




int main() {
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Skate Rush");
	initLevel1Coins();
	lvl1Hero.images[STATE_STAND] = iLoadImage("Image\\stand.png");
	lvl1Hero.images[STATE_MOVE] = iLoadImage("Image\\move.png");
	lvl1Hero.images[STATE_JUMP] = iLoadImage("Image\\jump.png");
	lvl1Hero.images[STATE_HIT] = iLoadImage("Image\\hit.png");

	lvl2Hero.images[STATE_STAND] = iLoadImage("Image\\stand.png");
	lvl2Hero.images[STATE_MOVE] = iLoadImage("Image\\move.png");
	lvl2Hero.images[STATE_JUMP] = iLoadImage("Image\\jump.png");
	lvl2Hero.images[STATE_HIT] = iLoadImage("Image\\hit.png");
	bg1 = iLoadImage("Image\\Level1Backgrouds\\level1_bg1.png");
	bg2 = iLoadImage("Image\\Level1Backgrouds\\level1_bg2.png");
	bg3 = iLoadImage("Image\\Level1Backgrouds\\level1_bg3.png");
	bg4 = iLoadImage("Image\\Level1Backgrouds\\level1_bg4.png");
	bg5 = iLoadImage("Image\\Level1Backgrouds\\level1_bg5.png");
	bg6 = iLoadImage("Image\\Level1Backgrouds\\level1_bg6.png");

	bg11 = iLoadImage("Image\\Level2Backgrounds\\level2_bg1.png");
	bg12 = iLoadImage("Image\\Level2Backgrounds\\level2_bg2.png");
	bg13 = iLoadImage("Image\\Level2Backgrounds\\level2_bg3.png");
	bg14 = iLoadImage("Image\\Level2Backgrounds\\level2_bg4.png");
	bg15 = iLoadImage("Image\\Level2Backgrounds\\level2_bg5.png");
	bg16 = iLoadImage("Image\\Level2Backgrounds\\level2_bg4.png");
	bg17 = iLoadImage("Image\\Level2Backgrounds\\level2_bg3.png");
	bg18 = iLoadImage("Image\\Level2Backgrounds\\level2_bg4.png");
	bg19 = iLoadImage("Image\\Level2Backgrounds\\level2_bg10.png");
	bg20 = iLoadImage("Image\\Level1Backgrouds\\level1_bg6.png");
	
	int cactusImg = iLoadImage("Image\\cactus.png");
	int pillarImg = iLoadImage("Image\\pillar2.png");
	int carImg = iLoadImage("Image\\car.png");
	int brickImg = iLoadImage("Image\\brick.png");
	int stoneImg = iLoadImage("Image\\stone.png");
	int stone2Img = iLoadImage("Image\\stone2.png");

	int woodImg = iLoadImage("Image\\wood.png");

	obstacles[0] = { 800, 100, 80, brickImg };
	obstacles[1] = { 1400, 100, 80, brickImg };
	//obstacles[2] = { 2000, 35, 70, pillarImg };
	obstacles[2] = { 2000, 100, 80, carImg };
	obstacles[3] = { 2600, 100, 80, brickImg };
	obstacles[4] = { 3200, 100, 80, carImg };
	obstacles[5] = { 3800, 35, 70, pillarImg };
	obstacles[6] = { 4400, 100, 80, brickImg };
	obstacles[7] = { 5000, 100, 80, carImg };
	
	initLevel2Assets();
	obstaclesL2[0] = { 1200, 120, 80, stone2Img };
	//obstaclesL2[1] = { 1600, 75, 80, carImg };
	obstaclesL2[2] = { 2000, 120, 100, stoneImg };
	//obstaclesL2[3] = { 2600, 120, 100, stoneImg }; // Lowered pillar
	obstaclesL2[4] = { 3400, 120, 100, stone2Img };
	obstaclesL2[5] = { 4000, 120, 100, woodImg };
	obstaclesL2[6] = { 4600, 120, 100, woodImg };
	obstaclesL2[7] = { 5200, 120, 100, stone2Img };
	obstaclesL2[8] = { 5800, 120, 100, woodImg };
	//obstaclesL2[9] = { 6400, 75, 80, carImg };
	obstaclesL2[10] = { 7000, 75, 80, woodImg };
	obstaclesL2[11] = { 7600, 75, 80, carImg };
	initLevel3();
	
	iSetTimer(20, updateLevel1);
	iSetTimer(20, updateLevel2);
	
	iSetTimer(20, updateLevel3);
	
	iStart();
	return 0;
}