/**********************************************************************************************************************************************************************/
#include "screen.h"
/**********************************************************************************************************************************************************************/
//These are the paths to the images needed for the program
//They are relative to the exe
/**********************************************************************************************************************************************************************/
clsScreen::clsScreen(uint ball_radius) {
    /* TODO (GamerMan7799#5#): Add check if images path is correct */
    width = Global::Config.values.uintScreenWidth;
    height = Global::Config.values.uintScreenHeight;

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

	win = SDL_CreateWindow("Cannon Simulation",100,100,width, height, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
        printf("SDL Failed to create window.\n");
        cleanup();
        error();
		bln_SDL_started = false;
		return;
	} else {
	    blnWindow = true;
	    if (Global::blnDebugMode) {printf("Window creation successful\n");}
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
        printf("SDL Failed to create renderer.\n");
        cleanup();
        error();
        bln_SDL_started = false;
        return;
	} else {
	    blnRenderer = true;
	    if (Global::blnDebugMode) {printf("Renderer creation successful\n");}
    }

    std::string pathstring = Global::Config.values.PathToImages;
    pathstring += "sky.png";

	sky = loadIMG(pathstring);
    if (bln_SDL_started == false) {return;}
    else {
        blnSky = true;
        if (Global::blnDebugMode) {printf("Sky loading successful\n");}
    }

    std::string ballpath = Global::Config.values.PathToImages;

    switch (ball_radius) {
    case 1:
        ballpath += "ball_1.png";
        break;
    case 2:
        ballpath += "ball_2.png";
        break;
    case 3:
        ballpath += "ball_3.png";
        break;
    case 4:
        ballpath += "ball_4.png";
        break;
    case 5:
        ballpath += "ball_5.png";
        break;
    case 6:
        ballpath += "ball_6.png";
        break;
    case 7:
        ballpath += "ball_7.png";
        break;
    case 8:
        ballpath += "ball_8.png";
        break;
    case 9:
        ballpath += "ball_9.png";
        break;
    default :
        ballpath += "ball_10.png";
        break;
    };

    ball = loadIMG(ballpath);
    if (bln_SDL_started == false) {return;}
    else {
        blnBall = true;
        if (Global::blnDebugMode) {printf("Ball loading successful\n");}
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
    //static uint uintUpdateCounter;
    //function for just updating the screen, nothing else
    //on its own in case I want to just update the screen at some point
    SDL_RenderPresent(ren);
    //uintUpdateCounter++;
    //printf("%d\n",uintUpdateCounter);
}
/**********************************************************************************************************************************************************************/
void clsScreen::updateBall(LOC newplace) {
    //Make sure the values aren't going off screen
    //not sure if that would cause an error, but I feel like
    //dealing with it if it does.
    if (newplace.x > width) {newplace.x = width;}
    if (newplace.y > height) {newplace.y = 0;}
    //Clear renderer
    SDL_RenderClear(ren);
    //Copy sky
    SDL_RenderCopy(ren,sky,NULL,NULL);

    //Because the top left is 0,0 I have to change the y value to match this
    newplace.y = height - newplace.y;

    SDL_Rect dst;
    dst.x = newplace.x;
    dst.y = newplace.y;
    //Query ball texture to get its width and height
    SDL_QueryTexture(ball,NULL,NULL, &dst.w, &dst.h);
    //Place the ball
    SDL_RenderCopy(ren,ball,NULL,&dst);

    update(); //update screen
}
/**********************************************************************************************************************************************************************/
void clsScreen::cleanup(void) {
    if (blnBall) {
        SDL_DestroyTexture(ball);
        blnBall = false;
        if (Global::blnDebugMode) {printf("Ball texture destroyed\n");}
    }
	if (blnSky) {
        SDL_DestroyTexture(sky);
        blnSky = false;
        if (Global::blnDebugMode) {printf("Sky texture destroyed\n");}
    }
	if (blnRenderer) {
        SDL_DestroyRenderer(ren);
        blnRenderer = false;
        if (Global::blnDebugMode) {printf("Renderer destroyed\n");}
    }
	if (blnWindow) {
        SDL_DestroyWindow(win);
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
    SDL_Surface* temp = IMG_Load(filename.c_str());

	if (temp == nullptr) {
        printf("Failed to load img.\n");
        cleanup();
        error();
        bln_SDL_started = false;
        return nullptr;
	} else {
	    if (Global::blnDebugMode) {printf("img to surface successful\n");}
    }

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        cleanup();
        error();
        bln_SDL_started = false;
	} else {
	    if (Global::blnDebugMode) {printf("Surface to texture successful\n");}
    }

	return tex;
}
/**********************************************************************************************************************************************************************/
