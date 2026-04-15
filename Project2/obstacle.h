#ifndef OBSTACLE_H
#define OBSTACLE_H


// Protiti obstacle-er data rakhar jonno structure
struct Obstacle {
    int x;
    int width;
    int height;
    int image;
};

// Protiti Level-e obstacle draw korar shared function
inline void drawObstacles(Obstacle obs[], int total, int bgX, int groundY) {
    for (int i = 0; i < total; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		iShowImage(obs[i].x + bgX, groundY, obs[i].width, obs[i].height, obs[i].image);
		glDisable(GL_BLEND);
    }
}

// Collision detection logic (AABB method)
inline bool checkCollision(int pX, int pY, int pW, int pH, Obstacle obs[], int total, int bgX, int groundY) {
    for (int i = 0; i < total; i++) {
        int obsX = obs[i].x + bgX;
        // Rectangle overlap check
        if (pX < obsX + obs[i].width &&
            pX + pW > obsX &&
            pY < groundY + obs[i].height &&
            pY + pH > groundY) {
            return true; // Dhakka legeche
        }
    }
    return false; // Dhakka lage ni
}

#endif