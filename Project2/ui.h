#ifndef UI_H
#define UI_H

#include <windows.h>
#include <mmsystem.h>
#include "config.h"



extern int currentPage;
extern bool menuMusicPlaying;
extern bool musicOn;

int mouseX = 0;
int mouseY = 0;
bool isHovered(int bx, int by, int bw, int bh) {
	return (mouseX >= bx && mouseX <= bx + bw &&
		mouseY >= by && mouseY <= by + bh);
}

float skaterX = 30.0f;  
float animTick = 0.0f;  


// ----------- FUNCTION DECLARATIONS -----------

void drawHomePage();
void drawStartPage();

void drawAboutUsPage();

void drawInstructionPage();



// ----------- FUNCTION DEFINITIONS -----------

 void updateMenuMusic()
{
    if (!musicOn)
        return;

    if (currentPage <= 3)  // Menu pages
    {
        if (!menuMusicPlaying)
        {
            PlaySound("Audio\\UiSound.wav", NULL, SND_LOOP | SND_ASYNC);
            menuMusicPlaying = true;
        }
    }
    else
    {
        if (menuMusicPlaying)
        {
            PlaySound(NULL, 0, 0);
            menuMusicPlaying = false;
        }
    }
}

 void drawHomePage(){
    iSetColor(255, 255, 255);
    iFilledRectangle(0, 0, 1000, 600);
   

	
	animTick += 0.05f;

	// calculate horizontal vibration using sin
	float skaterOffsetX = sinf(animTick) * 15.0f;  // 15.0f = how far left and right it moves

	// draw background first then skater on top
	iShowBMP(0, 0, "Image\\UIBackGroung\\MenuBackground1.bmp");
	iShowBMP((int)(skaterX + skaterOffsetX), 175, "Image\\UIBackGroung\\Skater.bmp");

	if (isHovered(690, 395, 219, 95))
		iShowBMP(690, 395, "Image\\Button\\StartButton_hover.bmp");
	else
		iShowBMP(690, 395, "Image\\Button\\StartButton.bmp");
	if (isHovered(701, 300, 208, 82))
		iShowBMP(690, 290, "Image\\Button\\AboutUsButton_hover.bmp");
	else
		iShowBMP(690, 290, "Image\\Button\\AboutUsButton.bmp");
	if (isHovered(701, 185, 208, 96))
		iShowBMP(690, 185, "Image\\Button\\Instruction_hover.bmp");
	else
		iShowBMP(690, 185, "Image\\Button\\Instruction.bmp");
}

 void drawStartPage(){
    iSetColor(255, 255, 255);
    iFilledRectangle(0, 0, 1000, 600);
    iShowBMP(0, 0, "Image\\UIBackGroung\\StartPage.bmp");
	if (isHovered(140, 450, 200, 80))
		iShowBMP(140, 450, "Image\\Button\\Level1Button_hover.bmp");
	else
		iShowBMP(140, 450, "Image\\Button\\Level1Button.bmp");
	if (isHovered(144, 325, 200, 80))
		iShowBMP(144, 325, "Image\\Button\\Level2Button_hover.bmp");
	else
		iShowBMP(144, 325, "Image\\Button\\Level2Button.bmp");
	if (isHovered(144, 200, 200, 80))
		iShowBMP(144, 200, "Image\\Button\\Level3Button_hover.bmp");
	else
		iShowBMP(144, 200, "Image\\Button\\Level3Button.bmp");
}


 void drawAboutUsPage(){
    iSetColor(255, 255, 255);
    iFilledRectangle(0, 0, 1000, 600);
    iShowBMP(0, 0, "Image\\UIBackGroung\\AboutUs.bmp");
}


 void drawInstructionPage(){
    iSetColor(255, 255, 255);
    iFilledRectangle(0, 0, 1000, 600);
    iShowBMP(0, 0, "Image\\UIBackGroung\\InstructionPage.bmp");
}



#endif
