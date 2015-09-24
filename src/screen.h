#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "config.h"
/*****************************************************************************/
/** This structure will hold certain attributes of the screen */
struct stcWinAtt {
    uint width; /**< The width of the screen in terms of pixels */
    uint height; /**< The height of the screen in terms of pixels */
    SDL_Window *win; /**< A pointer to the SDL window that is being used */
    SDL_Renderer *ren; /**< A pointer to the SDL rednder that is being used */
};

/** This structure will hold place values in terms of X and Y*/
struct stcLocation {
    int x; /**< X location */
    int y; /**< Y location */
};

typedef struct stcWinAtt WINATT;
typedef struct stcLocation LOC;
/*****************************************************************************/
#include "cannonball.h"
/*****************************************************************************/
class clsScreen {
    public:
        clsScreen();
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
        /** The ball texture */
        SDL_Texture *ball;

        /** The pixel texture, used to draw lines */
        SDL_Texture *pixel; //used for drawing the line

        /** Window Attributes. */
        WINATT window;

        //Keeps track of which parts have been loaded
        //so when ending only the ones that are open
        //are closed
        /** If window is loaded */
        bool blnWindow;

        /** If renderer is loaded */
        bool blnRenderer;

        /** If ball image is loaded */
        bool blnBall;

        /** If pixel image is loaded */
        bool blnPixel;

        /** If SDL is started and without issues */
        bool bln_SDL_started;

        SDL_Texture* loadIMG(std::string);
        void error(void);
};
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
