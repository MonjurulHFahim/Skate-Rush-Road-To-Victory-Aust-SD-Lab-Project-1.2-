#ifndef LEVEL3_H
#define LEVEL3_H

#include <math.h>
#include <time.h>


#define ARENA_CENTER_X 500.0f  
#define ARENA_CENTER_Y 220.0f  
#define ARENA_RADIUS_A 420.0f  
#define ARENA_RADIUS_B 160.0f  
enum L3PlayerState { L3_STAND, L3_MOVE_LEFT, L3_MOVE_RIGHT, L3_THROW_LEFT, L3_THROW_RIGHT };

struct L3Entity {
	float x, y;
	int   width, height;
	int   hp, maxHp;
	bool  active;
	L3PlayerState state;
};

struct L3Projectile {
    float x, y;
    float dx, dy;
    bool  active;
};

// ============================================================
//  CONSTANTS
// ============================================================

#define MAX_FIREBALLS       6       
#define KNIFE_COOLDOWN      18     
#define INVIC_FRAMES        30      
#define PLAYER_SPEED        10.0f   
#define SCREEN_W            1000
#define SCREEN_H            600

// ============================================================
//  GLOBAL LEVEL-3 STATE
// ============================================================

L3Entity      l3Player  = { 100, 200, 80, 120, 100, 100, true };
L3Entity      villain   = { 700, 300, 100, 150, 100, 100, true };

L3Projectile  pKnife              = { 0, 0, 0, 0, false };
L3Projectile  vFireballs[MAX_FIREBALLS] = {};

// --- Villain visibility state ---
int  vTimer        = 0;
bool vVisible      = true;

// Fireball burst: fires every `vFireInterval` frames WHILE villain is visible
int  vFireTimer    = 0;   // counts up; resets each shot
int  vFireInterval = 60;  // initial gap between shots (frames) — shrinks as HP drops

// --- Timers / cooldowns ---
int  knifeCooldown = 0;
int  l3InvincTimer = 0;

// --- Key state (true = currently held down) ---
// Index: 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT
bool l3Keys[4] = { false, false, false, false };

// --- End-state ---
bool l3GameOver = false;
bool l3Victory  = false;

// --- Image handles ---
int l3BgImg, l3VillainImg, l3KnifeImg, l3FireImg;
int l3Stand, l3MoveLeft, l3MoveRight, l3ThrowLeft, l3ThrowRight;
// ============================================================
//  HELPERS
// ============================================================

static inline float l3Clamp(float val, float lo, float hi) {
    return (val < lo) ? lo : (val > hi) ? hi : val;
}

// Find a free slot in the fireballs array; returns -1 if full
static int l3FreeFireball() {
    for (int i = 0; i < MAX_FIREBALLS; i++)
        if (!vFireballs[i].active) return i;
    return -1;
}

// Spawn `count` fireballs aimed at the player with a small spread angle
static void l3ShootAtPlayer(int count) {
    float hpPct     = (float)villain.hp / (float)villain.maxHp;
    float speed     = 7.0f + (1.0f - hpPct) * 6.0f;   // 7-13 px/frame
    float cx        = villain.x + villain.width  * 0.5f;
    float cy        = villain.y + villain.height * 0.5f;
    float baseAngle = atan2f(l3Player.y + l3Player.height * 0.5f - cy,
                             l3Player.x + l3Player.width  * 0.5f - cx);

    for (int s = 0; s < count; s++) {
        int slot = l3FreeFireball();
        if (slot < 0) break;

        float spread = (count > 1)
            ? (s - (count - 1) * 0.5f) * 0.30f   // fan spread in radians
            : 0.0f;
        float angle = baseAngle + spread;

        vFireballs[slot].active = true;
        vFireballs[slot].x      = cx;
        vFireballs[slot].y      = cy;
        vFireballs[slot].dx     = cosf(angle) * speed;
        vFireballs[slot].dy     = sinf(angle) * speed;
    }
}

// ============================================================
//  INIT
// ============================================================

void initLevel3() {
    srand((unsigned int)time(NULL));

   l3VillainImg = iLoadImage("Image\\villain.png");
    l3KnifeImg   = iLoadImage("Image\\knife.png");
    l3FireImg    = iLoadImage("Image\\fire.png");
    l3BgImg      = iLoadImage("Image\\L3BG\\L3BG.png");
    l3Stand      = iLoadImage("Image\\Level3Character\\stand - Copy.png");
    l3MoveLeft   = iLoadImage("Image\\Level3Character\\moveLeft.png");
    l3MoveRight  = iLoadImage("Image\\Level3Character\\moveRight.png");
    l3ThrowLeft  = iLoadImage("Image\\Level3Character\\throwLeft.png");
    l3ThrowRight = iLoadImage("Image\\Level3Character\\throwRight.png");
	
	l3Player = { 100, 200, 80, 120, 100, 100, true, L3_STAND };
    villain  = { 700, 300, 100, 150, 100, 100, true };

    vVisible      = true;
    vTimer        = 0;
    vFireTimer    = 0;
    vFireInterval = 60;

    pKnife.active = false;
    for (int i = 0; i < MAX_FIREBALLS; i++) vFireballs[i].active = false;

    knifeCooldown  = 0;
    l3InvincTimer  = 0;
    l3Keys[0] = l3Keys[1] = l3Keys[2] = l3Keys[3] = false;
    l3GameOver = false;
    l3Victory  = false;
}

// ============================================================
//  DRAW
// ============================================================

void drawHealthBar(int x, int y, int current, int maxVal, const char* label) {
    iSetColor(20, 20, 20);
    iFilledRectangle(x - 2, y - 2, 204, 24);

    iSetColor(80, 0, 0);
    iFilledRectangle(x, y, 200, 20);

    float pct = (float)current / (float)maxVal;
    if      (pct > 0.5f)  iSetColor(50,  200, 50);
    else if (pct > 0.25f) iSetColor(220, 180, 0);
    else                  iSetColor(220, 40,  40);
    iFilledRectangle(x, y, (int)(pct * 200), 20);

    iSetColor(255, 255, 255);
    iRectangle(x, y, 200, 20);
    iText(x, y + 25, (char*)label, GLUT_BITMAP_HELVETICA_12);
}

void drawLevel3Page() {
    // Background
    iShowImage(0, 0, SCREEN_W, SCREEN_H, l3BgImg);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bool drawPlayer = true;
	if (l3InvincTimer > 0)
		drawPlayer = ((l3InvincTimer / 4) % 2 == 0);

	if (drawPlayer) {
		int currentImg = l3Stand;
		if (l3Player.state == L3_MOVE_LEFT) currentImg = l3MoveLeft;
		else if (l3Player.state == L3_MOVE_RIGHT) currentImg = l3MoveRight;
		else if (l3Player.state == L3_THROW_LEFT) currentImg = l3ThrowLeft;
		else if (l3Player.state == L3_THROW_RIGHT) currentImg = l3ThrowRight;

		iShowImage(l3Player.x, l3Player.y, l3Player.width, l3Player.height, currentImg);
	}
    // ---- Player (blink while invincible) ----
    

    // ---- Villain ----
    if (vVisible && villain.hp > 0)
        iShowImage(villain.x, villain.y, villain.width, villain.height, l3VillainImg);

    // ---- Knife ----
    if (pKnife.active)
        iShowImage(pKnife.x - 15, pKnife.y - 7, 30, 15, l3KnifeImg);

    // ---- Fireballs ----
    for (int i = 0; i < MAX_FIREBALLS; i++)
        if (vFireballs[i].active)
            iShowImage(vFireballs[i].x - 20, vFireballs[i].y - 20, 40, 40, l3FireImg);
		glDisable(GL_BLEND);
    // ---- HUD ----
    drawHealthBar(50,  550, l3Player.hp, l3Player.maxHp, "PLAYER");
    drawHealthBar(750, 550, villain.hp,  villain.maxHp,  "VILLAIN");

    // Knife reload bar (fills as cooldown expires)
    iSetColor(40, 40, 40);
    iFilledRectangle(50, 540, 200, 6);
    iSetColor(100, 200, 255);
    int cdFill = (int)((1.0f - (float)knifeCooldown / KNIFE_COOLDOWN) * 200);
    if (cdFill > 200) cdFill = 200;
    iFilledRectangle(50, 540, cdFill, 6);

    // ---- Win / Game-Over overlay ----
	// ---- Win / Game-Over overlay ----
	if (l3Victory || l3GameOver) {
		iSetColor(0, 0, 0);
		iFilledRectangle(250, 220, 500, 220);
		iSetColor(60, 60, 60);
		iRectangle(252, 222, 496, 216);

		if (l3Victory) {
			iSetColor(255, 215, 0);
			iText(440, 390, (char*)"YOU WIN!", GLUT_BITMAP_TIMES_ROMAN_24);

		
			iSetColor(80, 40, 40);
			iFilledRectangle(425, 280, 150, 40);
			iSetColor(255, 255, 255);
			iRectangle(425, 280, 150, 40);
			iText(450, 295, (char*)"MAIN MENU", GLUT_BITMAP_HELVETICA_18);
		}
		else {
			iSetColor(220, 50, 50);
			iText(430, 390, (char*)"GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

			
			// Retry button
			iSetColor(60, 60, 160);
			iFilledRectangle(340, 280, 150, 40);
			iSetColor(255, 255, 255);
			iRectangle(340, 280, 150, 40);
			iText(385, 295, (char*)"RETRY", GLUT_BITMAP_HELVETICA_18);

			// Main Menu button
			iSetColor(80, 40, 40);
			iFilledRectangle(510, 280, 150, 40);
			iSetColor(255, 255, 255);
			iRectangle(510, 280, 150, 40);
			iText(535, 295, (char*)"MAIN MENU", GLUT_BITMAP_HELVETICA_18);
		}
	}

    // Controls hint
    if (!l3Victory && !l3GameOver) {
        iSetColor(200, 200, 200);
        iText(10, 10, (char*)"Arrow Keys: Move (diagonals work)    Click: Throw Knife", GLUT_BITMAP_HELVETICA_12);
    }
}

// ============================================================
//  UPDATE  (iSetTimer callback, 20 ms interval)
// ============================================================

void updateLevel3() {
    extern int currentPage;
    if (currentPage != 6) return;
    if (l3GameOver || l3Victory) return;

    // Terminal checks
    if (l3Player.hp <= 0) { l3GameOver = true; return; }
    if (villain.hp  <= 0) { l3Victory  = true; return; }

    // ---- Cooldowns ----
    if (knifeCooldown > 0) knifeCooldown--;
    if (l3InvincTimer > 0) l3InvincTimer--;

  
	// ---- Diagonal player movement (key-state based) ----
	float dx = 0.0f, dy = 0.0f;

	// Check states for all 4 directions
	if (l3Keys[0]) dy += PLAYER_SPEED;   // UP
	if (l3Keys[1]) dy -= PLAYER_SPEED;   // DOWN
	if (l3Keys[2]) dx -= PLAYER_SPEED;   // LEFT
	if (l3Keys[3]) dx += PLAYER_SPEED;   // RIGHT

	
	if (dx != 0.0f && dy != 0.0f) {
		dx *= 0.7071f;   // 1 divided by sqrt(2)
		dy *= 0.7071f;
	}

	l3Player.x = l3Clamp(l3Player.x + dx, 0.0f, (float)(SCREEN_W - l3Player.width));
	l3Player.y = l3Clamp(l3Player.y + dy, 0.0f, (float)(SCREEN_H - l3Player.height));

   
    float hpPct    = (float)villain.hp / (float)villain.maxHp;
    int   stayTime = (int)(120.0f * hpPct) + 60;   
    int   hideTime = (int)( 80.0f * hpPct) + 30;  

    vTimer++;

    if (vVisible) {
       
        vFireInterval = (int)(35.0f * hpPct) + 25;

        vFireTimer++;
        if (vFireTimer >= vFireInterval) {
            vFireTimer = 0;

            // More projectiles per burst at lower HP
            int numShots = 1;
            if (hpPct < 0.66f) numShots = 2;
            if (hpPct < 0.33f) numShots = 3;

            l3ShootAtPlayer(numShots);
        }

       
        if (vTimer > stayTime) {
            vVisible   = false;
            vTimer     = 0;
            vFireTimer = 0;
        }
    } else {
       
        if (vTimer > hideTime) {
            villain.x  = 200 + rand() % 600;
            villain.y  = 80  + rand() % 380;
            vVisible   = true;
            vTimer     = 0;
            
            vFireTimer = vFireInterval;
        }
    }

    // ---- Knife movement & collision ----
    if (pKnife.active) {
        pKnife.x += pKnife.dx;
        pKnife.y += pKnife.dy;

        if (pKnife.x > SCREEN_W || pKnife.x < 0 ||
            pKnife.y > SCREEN_H || pKnife.y < 0) {
            pKnife.active = false;
        }

        if (vVisible && pKnife.active &&
            pKnife.x > villain.x && pKnife.x < villain.x + villain.width &&
            pKnife.y > villain.y && pKnife.y < villain.y + villain.height) {
            villain.hp -= 5;
            if (villain.hp < 0) villain.hp = 0;
            pKnife.active = false;
            PlaySound("Audio\\Knife.wav", NULL, SND_ASYNC);
        }
    }

	float px = l3Player.x + dx + (l3Player.width / 2.0f);
	float py = l3Player.y + dy;

	float check = (pow(px - ARENA_CENTER_X, 2) / pow(ARENA_RADIUS_A, 2)) +
		(pow(py - ARENA_CENTER_Y, 2) / pow(ARENA_RADIUS_B, 2));

	if (check <= 1.0f) {
		l3Player.x += dx;
		l3Player.y += dy;
	}

	// Update movement state only if not currently throwing
	if (l3Player.state != L3_THROW_LEFT && l3Player.state != L3_THROW_RIGHT) {
		if (l3Keys[2] || (currentPage == 6 && (GetKeyState('A') & 0x8000))) l3Player.state = L3_MOVE_LEFT;
		else if (l3Keys[3] || (currentPage == 6 && (GetKeyState('D') & 0x8000))) l3Player.state = L3_MOVE_RIGHT;
		else l3Player.state = L3_STAND;
	}

	// Reset Throwing animation to Stand once cooldown is halfway through
	if ((l3Player.state == L3_THROW_LEFT || l3Player.state == L3_THROW_RIGHT) && knifeCooldown < (KNIFE_COOLDOWN - 10)) {
		l3Player.state = L3_STAND;
	}
    // ---- Fireball movement & collision ----
    for (int i = 0; i < MAX_FIREBALLS; i++) {
        if (!vFireballs[i].active) continue;

        vFireballs[i].x += vFireballs[i].dx;
        vFireballs[i].y += vFireballs[i].dy;

        if (vFireballs[i].x < -60 || vFireballs[i].x > SCREEN_W + 60 ||
            vFireballs[i].y < -60 || vFireballs[i].y > SCREEN_H + 60) {
            vFireballs[i].active = false;
            continue;
        }

        
        if (l3InvincTimer == 0 &&
            vFireballs[i].x > l3Player.x &&
            vFireballs[i].x < l3Player.x + l3Player.width &&
            vFireballs[i].y > l3Player.y &&
            vFireballs[i].y < l3Player.y + l3Player.height) {
            l3Player.hp  -= 10;
            if (l3Player.hp < 0) l3Player.hp = 0;
            l3InvincTimer = INVIC_FRAMES;
            vFireballs[i].active = false;
            PlaySound("Audio\\Damage.wav", NULL, SND_ASYNC);
        }
    }
}

// ============================================================
//  INPUT HANDLERS  (called from iMain.cpp)
// ============================================================



void l3HandleInput(unsigned char key) {
	if (l3GameOver || l3Victory) return;

	float moveAmount = 15.0f;
	float nextX = l3Player.x;
	float nextY = l3Player.y;

	if (key == 'w' || key == 'W') nextY += moveAmount;
	else if (key == 's' || key == 'S') nextY -= moveAmount;
	else if (key == 'a' || key == 'A') nextX -= moveAmount;
	else if (key == 'd' || key == 'D') nextX += moveAmount;

	
	float px = nextX + (l3Player.width / 2.0f);
	float py = nextY; 

	
	float normalizedDist = (pow(px - ARENA_CENTER_X, 2) / pow(ARENA_RADIUS_A, 2)) +
		(pow(py - ARENA_CENTER_Y, 2) / pow(ARENA_RADIUS_B, 2));

	if (normalizedDist <= 1.0f) {
		l3Player.x = nextX;
		l3Player.y = nextY;
	}
}


void l3MouseThrow(int mx, int my) {
    
	if (l3Victory) {
		
		if (mx >= 425 && mx <= 575 && my >= 280 && my <= 320) {
			extern int currentPage;
			currentPage = 1;
			return;
		}
	}

	if (l3GameOver) {
		
		if (mx >= 340 && mx <= 490 && my >= 280 && my <= 320) {
			initLevel3();
			return;
		}
		
		if (mx >= 510 && mx <= 660 && my >= 280 && my <= 320) {
			extern int currentPage;
			currentPage = 1;
			return;
		}
	}

    if (knifeCooldown > 0 || pKnife.active) return;
	// Determine throw direction based on mouse X relative to player center
    if (mx < (l3Player.x + l3Player.width / 2)) l3Player.state = L3_THROW_LEFT;
    else l3Player.state = L3_THROW_RIGHT;
    pKnife.active = true;
    pKnife.x      = l3Player.x + l3Player.width  * 0.5f;
    pKnife.y      = l3Player.y + l3Player.height * 0.5f;
    float angle   = atan2f((float)my - pKnife.y, (float)mx - pKnife.x);
    pKnife.dx     = cosf(angle) * 12.0f;
    pKnife.dy     = sinf(angle) * 12.0f;
    knifeCooldown = KNIFE_COOLDOWN;
}


void l3Retry() { initLevel3(); }

#endif 
