#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include "config.h"
#include "global.h"
/**********************************************************************************************************************************************************************/
struct stcWinAtt { //Attribute of the window
    uint width;
    uint height;
    SDL_Window *win;
    SDL_Renderer *ren;
};

struct stcLocation {
    int x;
    int y;
};

typedef struct stcWinAtt WINATT;
typedef struct stcLocation LOC;
/**********************************************************************************************************************************************************************/
#include "cannonball.h"
/**********************************************************************************************************************************************************************/
class clsScreen {
    public:
        /** Default constructor */
        clsScreen();
        /** Default destructor */
        ~clsScreen();

        void update(void);
        void cleanup(void);
        void clearscreen(void);
        bool getSDLStarted(void);
        void drawline(LOC, LOC);

        SDL_Texture* getBallTexture(void);
        SDL_Texture* getPixelTexture(void);
        WINATT getWindow(void);

    private:
        SDL_Texture *ball;
        SDL_Texture *pixel; //used for drawing the line

        WINATT window;

        //Keeps track of which parts have been loaded
        //so when ending only the ones that are open
        //are closed
        bool blnWindow;
        bool blnRenderer;
        bool blnBall;
        bool blnPixel;
        bool bln_SDL_started;

        SDL_Texture* loadIMG(std::string);
        void error(void);
};
/**********************************************************************************************************************************************************************/
#endif // __MY_SCREEN_HEADER__
