/**********************************************************************************************************************************************************************/
#include "screen.h"
/**********************************************************************************************************************************************************************/
clsScreen::clsScreen() {
    window.width = Global::Config.values.uintScreenWidth;
    window.height = Global::Config.values.uintScreenHeight;

    blnWindow = false;
    blnRenderer = false;
    blnSky = false;
    blnBall = false;
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

	sky = loadIMG("sky");
    if (bln_SDL_started == false) {return;}
    else {
        blnSky = true;
        if (Global::blnDebugMode) {printf("Sky loading successful\n");}
    }

    ball = loadIMG("ball");
    if (bln_SDL_started == false) {return;}
    else {
        blnBall = true;
        if (Global::blnDebugMode) {printf("Ball loading successful\n");}
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
    SDL_RenderClear(window.ren);
    //Copy sky
    SDL_RenderCopy(window.ren,sky,NULL,NULL);
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
	if (blnSky) {
        SDL_DestroyTexture(sky);
        blnSky = false;
        if (Global::blnDebugMode) {printf("Sky texture destroyed\n");}
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
    //SDL_Surface* temp = IMG_Load(filename.c_str());
    SDL_Surface* temp;

    if (filename == "ball") {temp = IMG_ReadXPMFromArray(image_ball_xpm);}
    else if (filename == "sky") {temp = IMG_ReadXPMFromArray(image_sky_xpm);}
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

	//Do some additional stuff to the ball texture
	//so that parts are transparent.
	if ( filename == "ball" ) {
        //I don't completely understand all of this got it from Lazy Foo's tutorial
        //here: http://lazyfoo.net/tutorials/SDL/40_texture_manipulation/index.php
        SDL_Rect rect_ball;
        SDL_QueryTexture(tex, NULL, NULL, &rect_ball.w, &rect_ball.h);
        void* mPixels = NULL;
        int mPitch;
        if (SDL_LockTexture(tex, NULL, &mPixels, &mPitch) == 0) {
            Uint32* pixels = (Uint32*)mPixels;
            int pixelCount = (mPitch / 4) * rect_ball.h;
            Uint32 colorKey = SDL_MapRGB( SDL_GetWindowSurface( window.win )->format, 0xFF, 0xFF, 0xFF ); //the color we are trying to replace
            Uint32 transparent = SDL_MapRGBA( SDL_GetWindowSurface( window.win )->format, 0xFF, 0xFF, 0xFF, 0x00 ); //transparent
            for (int i = 0; i < pixelCount; i ++) {
                if(pixels[i] == colorKey) {
                    pixels[i] = transparent;
                } //end if white
            } //end for all pixels
            printf("Texture unlocking\n");
            SDL_UnlockTexture(tex);
            printf("Texture unlocked\n");
        } else {
            if (Global::blnDebugMode) {printf("Failed to make ball background transparent\n");}
        }
	} //end if ball
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
