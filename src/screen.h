#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include "cannonball.h"
#include "config.h"
/**********************************************************************************************************************************************************************/
//Include the xpm files which present the images.
#include "image_ball.xpm"
#include "image_sky.xpm"
#include "image_cannon.xpm"
/**********************************************************************************************************************************************************************/
//typedefs because reasons
typedef unsigned int uint;
typedef unsigned char uchar;
/**********************************************************************************************************************************************************************/
namespace Global {
    extern const bool blnDebugMode;
    extern clsConfig Config;
}
/**********************************************************************************************************************************************************************/
class clsScreen {
    public:
        /** Default constructor */
        clsScreen(uint);
        /** Default destructor */
        ~clsScreen();

        void update(void);
        void updateBall(LOC);
        void cleanup(void);

        bool bln_SDL_started;

    private:
        SDL_Texture *ball;
        SDL_Texture *sky;
        SDL_Texture *cannon;
        SDL_Window *win;
        SDL_Renderer *ren;

        uint width;
        uint height;

        //Keeps track of which parts have been loaded
        //so when ending only the ones that are open
        //are closed
        bool blnWindow;
        bool blnRenderer;
        bool blnSky;
        bool blnBall;
        bool blnCannon;

        SDL_Texture* loadIMG(std::string);
        void error(void);
};
/**********************************************************************************************************************************************************************/
#endif // __MY_SCREEN_HEADER__
