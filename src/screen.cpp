/**********************************************************************************************************************************************************************/
#include "screen.h"
/**********************************************************************************************************************************************************************/
clsScreen::clsScreen() {
    window.width = Global::Config.values.uintScreenWidth;
    window.height = Global::Config.values.uintScreenHeight;

    blnWindow = blnRenderer = false;
    blnBall = blnPixel = false;
    bln_SDL_started = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		bln_SDL_started = false;
		error();
		return;
	} else {
	    bln_SDL_started = true;
	    if (Global::blnDebugMode) {printf("SDL init successful\n");}
    }

	window.win = SDL_CreateWindow("Cannon Simulation",100,100, window.width, window.height, SDL_WINDOW_SHOWN);
	if (window.win == nullptr) {
        printf("SDL Failed to create window.\n");
        cleanup();
        error();
		bln_SDL_started = false;
		return;
	} else {
	    blnWindow = true;
	    if (Global::blnDebugMode) {printf("Window creation successful\n");}
	}

	window.ren = SDL_CreateRenderer(window.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (window.ren == nullptr) {
        printf("SDL Failed to create renderer.\n");
        cleanup();
        error();
        bln_SDL_started = false;
        return;
	} else {
	    blnRenderer = true;
	    if (Global::blnDebugMode) {printf("Renderer creation successful\n");}
    }

    //Set the background color of the render to black
    SDL_SetRenderDrawColor(window.ren, 0x00, 0x00, 0x00, 0xFF );

    ball = loadIMG("ball");
    if (bln_SDL_started == false) {return;}
    else {
        blnBall = true;
        if (Global::blnDebugMode) {printf("Ball loading successful\n");}
        //Set the blend mode for the texture so I can change the alpha later
        SDL_SetTextureBlendMode(ball, SDL_BLENDMODE_BLEND );
    }

    pixel = loadIMG("pixel");
    if (bln_SDL_started == false) {return;}
    else {
        blnPixel = true;
        if (Global::blnDebugMode) {printf("Pixel loading successful\n");}
    }

}
/**********************************************************************************************************************************************************************/
clsScreen::~clsScreen() {
    cleanup();
    SDL_Quit();
    if (Global::blnDebugMode) {printf("SDL quit\n");}
}
/**********************************************************************************************************************************************************************/
void clsScreen::update(void) {
    //function for just updating the screen, nothing else
    //on its own in case I want to just update the screen at some point
    SDL_RenderPresent(window.ren);
}
/**********************************************************************************************************************************************************************/
void clsScreen::clearscreen() {
    //set the ball texture back to full opaque
    SDL_SetTextureAlphaMod(ball, 0xFF);
    SDL_RenderClear(window.ren);
}
/**********************************************************************************************************************************************************************/
void clsScreen::cleanup(void) {
    if (blnBall) {
        SDL_DestroyTexture(ball);
        blnBall = false;
        if (Global::blnDebugMode) {printf("Ball texture destroyed\n");}
    }
    if (blnBall) {
        SDL_DestroyTexture(pixel);
        blnPixel = false;
        if (Global::blnDebugMode) {printf("Ball texture destroyed\n");}
    }
	if (blnRenderer) {
        SDL_DestroyRenderer(window.ren);
        blnRenderer = false;
        if (Global::blnDebugMode) {printf("Renderer destroyed\n");}
    }
	if (blnWindow) {
        SDL_DestroyWindow(window.win);
        blnWindow = false;
        if (Global::blnDebugMode) {printf("Window destroyed\n");}
    }
}
/**********************************************************************************************************************************************************************/
void clsScreen::error(void) {
    printf("SDL error: %s\n", SDL_GetError());
	getchar();
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::loadIMG(std::string filename) {
    SDL_Surface* temp;

    if (filename == "ball") {temp = IMG_ReadXPMFromArray(image_ball_xpm);}
    else if (filename == "pixel") {temp = IMG_ReadXPMFromArray(image_pixel_xpm);}
    else { temp = nullptr; }

	SDL_Texture *tex = (temp == nullptr) ? nullptr : SDL_CreateTextureFromSurface(window.ren,temp);
	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        cleanup();
        error();
        bln_SDL_started = false;
	}

	return tex;
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::getBallTexture() {
    return ball;
}
/**********************************************************************************************************************************************************************/
WINATT clsScreen::getWindow() {
    return window;
}
/**********************************************************************************************************************************************************************/
bool clsScreen::getSDLStarted() {
    return bln_SDL_started;
}
/**********************************************************************************************************************************************************************/
void clsScreen::drawline(LOC Current, LOC Old) {
    double slope;
    SDL_Rect dst;
    SDL_QueryTexture(pixel, NULL, NULL, &dst.w, &dst.h);
    uint length;
    length = (uint) round( sqrt( pow(Current.x - Old.x, 2) + pow(Current.y - Old.y, 2) ) );
    if (Current.x == Old.x ) {
        dst.x = Current.x;
        for (uint i = 0; i < length; i++) {
            dst.y = i + (Current.y > Old.y ? Old.y : Current.y);
            SDL_RenderCopy(window.ren, pixel, NULL, &dst);
        } //end for length
    } else {
        slope = ((double)Current.y - (double)Old.y) / ((double)Current.x - (double)Old.x);
        uint startpoint = (Old.x < Current.x) ? Old.x : Current.x;
        uint endpoint = (Old.x < Current.x) ? Current.x : Old.x;
        double incamount = (double)(endpoint - startpoint) / length;
        for (double i = startpoint; i < endpoint; i += incamount) {
            dst.x = round(i);
            dst.y = Old.y;
            dst.y += (uint) round( slope * (double) (i - Old.x) );
            SDL_RenderCopy(window.ren, pixel, NULL, &dst);
        } //end for length
    } //end if
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::getPixelTexture() {
    return pixel;
}
/**********************************************************************************************************************************************************************/
