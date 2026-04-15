#ifndef COIN_H
#define COIN_H



struct Coin {
    int x, y;
    bool isCollected;
};

// Drawing logic: only draw if the coin hasn't been picked up
inline void drawCoins(Coin coins[], int count, int bgX, int img) {
    for (int i = 0; i < count; i++) {
        if (!coins[i].isCollected) {
            iShowImage(coins[i].x + bgX, coins[i].y, 60, 60, img);
        }
    }
}
#endif