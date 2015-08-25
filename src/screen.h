#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <string>
#include <cstdio>
/**********************************************************************************************************************************************************************/
//typedefs because reasons
typedef unsigned int uint;
typedef unsigned char uchar;
/**********************************************************************************************************************************************************************/
class clsScreen {
    public:
        /** Default constructor */
        clsScreen();
        /** Default destructor */
        ~clsScreen();

        void update(void);
        void updateBall(uint, uint);
        void cleanup(void);

        bool bln_SDL_started;

    private:
        SDL_Texture *ball;
        SDL_Texture *sky;
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

        SDL_Texture* loadIMG(std::string);
        void error(void);
};
/**********************************************************************************************************************************************************************/
#endif // __MY_SCREEN_HEADER__
