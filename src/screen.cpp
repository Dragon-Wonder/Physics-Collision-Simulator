/*****************************************************************************/
#include "screen.h"
/*****************************************************************************/
//Include the xpm files which present the images.
#include "image_ball.xpm"
#include "image_pixel.xpm"
/*****************************************************************************/
clsScreen::clsScreen() {
    /////////////////////////////////////////////////
    /// @brief The default constructor for the SDL screen
    ///        it will try start SDL, and create and window and a renderer,
    ///        then try to load the textures it will need, if any of these fail
    ///        it will set bln_SDL_Started to false and return void, when main in main.cpp
    ///        checks bln_SDL_Started and ends the entire program will it is false.
    ///        If, however, bln_SDL_Started is true it will continue on with the rest of the program.
    /////////////////////////////////////////////////

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

	window.win = SDL_CreateWindow("Cannon Simulation",100,100,
                               window.width, window.height, SDL_WINDOW_SHOWN);

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

	window.ren = SDL_CreateRenderer(window.win, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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
/*****************************************************************************/
clsScreen::~clsScreen() {
    /////////////////////////////////////////////////
    /// @brief This is the default deconstructor, it will just call
    ///        clsScreen::cleanup to ensure everything is cleared from memory,
    ///        and then quit SDL.
    /////////////////////////////////////////////////

    cleanup();
    SDL_Quit();
    if (Global::blnDebugMode) {printf("SDL quit\n");}
}
/*****************************************************************************/
void clsScreen::update() {
    /////////////////////////////////////////////////
    /// @brief Will update the SDL screen with whatever has been drawn
    ///        see clsCannonball::show
    /// @return void
    /////////////////////////////////////////////////

    SDL_RenderPresent(window.ren);
}
/*****************************************************************************/
void clsScreen::clearscreen() {
    /////////////////////////////////////////////////
    /// @brief Will delete all textures currently shown on the screen
    /// @return void
    /////////////////////////////////////////////////

    SDL_RenderClear(window.ren);
}
/*****************************************************************************/
void clsScreen::cleanup() {
    /////////////////////////////////////////////////
    /// @brief This will attempt to delete Textures, and the Window / Renderer
    ///        from memory if their representative loaded boolean is true. Will delete
    ///        * Ball texture
    ///        * Pixel Texture
    ///        * Renderer
    ///        * Window
    ///
    /// @return void
    ///
    /////////////////////////////////////////////////

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
/*****************************************************************************/
void clsScreen::error(void) {
    /////////////////////////////////////////////////
    /// @brief Will print out the error generated by SDL if something goes wrong
    /// @return void
    /////////////////////////////////////////////////

    printf("SDL error: %s\n", SDL_GetError());
	getchar();
}
/*****************************************************************************/
SDL_Texture* clsScreen::loadIMG(std::string filename) {
    /////////////////////////////////////////////////
    /// @brief Will load a specified image into a texture.
    ///        It will first load it from the XPM array that it is embedded as into a surface,
    ///        then will will convert the surface into a texture, if that fails it will set bln_SDL_started to false
    ///        and return nullptr, otherwise returns texture.
    ///
    /// @param filename = The texture to load (currently only supports "ball" or "pixel" as those are the only two textures needed)
    /// @return Pointer to the texture in memory
    /////////////////////////////////////////////////

    SDL_Surface* temp;

    if (filename == "ball") {temp = IMG_ReadXPMFromArray(image_ball_xpm);}
    else if (filename == "pixel") {temp = IMG_ReadXPMFromArray(image_pixel_xpm);}
    else { temp = nullptr; }

	SDL_Texture *tex = (temp == nullptr)
                        ? nullptr : SDL_CreateTextureFromSurface(window.ren,temp);

	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        cleanup();
        error();
        bln_SDL_started = false;
	}

	return tex;
}
/*****************************************************************************/
SDL_Texture* clsScreen::getBallTexture() {
    /////////////////////////////////////////////////
    /// @brief Returns the Ball texture for use in the clsCannonball
    /// @return Pointer to the ball texture in memory
    /////////////////////////////////////////////////

    return ball;
}
/*****************************************************************************/
WINATT clsScreen::getWindow() {
    /////////////////////////////////////////////////
    /// @brief Returns window attributes
    /// @return clsScreen::window
    /////////////////////////////////////////////////

    return window;
}
/*****************************************************************************/
bool clsScreen::getSDLStarted() {
    /////////////////////////////////////////////////
    /// @brief Returns if SDL is started and running without issues
    /// @return bln_SDL_started
    /////////////////////////////////////////////////

    return bln_SDL_started;
}
/*****************************************************************************/
void clsScreen::drawline(LOC Current, LOC Old) {
    /////////////////////////////////////////////////
    /// @brief Will draw a line (using the pixel texture) from the old mouse location to
    ///        the Current mouse location
    /// @param Current = Current Mouse Location in terms of X and Y
    /// @param Old = Old Mouse Location (start of click) in terms of X and Y
    /// @return void
    /////////////////////////////////////////////////

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
/*****************************************************************************/
SDL_Texture* clsScreen::getPixelTexture() {
    /////////////////////////////////////////////////
    /// @brief Returns pointer to the pixel texture
    /// @return Pointer to Pixel texture in memory
    /////////////////////////////////////////////////

    return pixel;
}
/*****************************************************************************/
