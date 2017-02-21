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
    unsigned int width; /**< The width of the screen in terms of pixels */
    unsigned int height; /**< The height of the screen in terms of pixels */
    SDL_Window *win; /**< A pointer to the SDL window that is being used */
    SDL_Renderer *ren; /**< A pointer to the SDL renderer that is being used */
    SDL_Texture *ball; /**< The ball texture */
    SDL_Texture *pixel; /**< The pixel texture, used to draw lines */
};
/*****************************************************************************/
/** This structure will hold place values in terms of X and Y*/
struct stcLocation {
    int x; /**< X location */
    int y; /**< Y location */
};

typedef struct stcWinAtt WINATT;
typedef struct stcLocation LOC;
/*****************************************************************************/
// It is likely bad practice to hold values like this...
/* TODO (GamerMan7799#9#): Find a better method */
namespace screen {
  extern WINATT screenatt;
}
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

    //SDL_Texture* getBallTexture(void);
    //SDL_Texture* getPixelTexture(void);
    //WINATT getWindow(void);

  private:
    /** Window Attributes. */
    //WINATT window;

    //Keeps track of which parts have been loaded
    //so when ending only the ones that are open
    //are closed
    /** If window is loaded. */
    bool blnWindow;

    /** If renderer is loaded. */
    bool blnRenderer;

    /** If ball image is loaded. */
    bool blnBall;

    /** If pixel image is loaded. */
    bool blnPixel;

    /** If SDL is started and without issues. */
    bool bln_SDL_started;

    SDL_Texture* loadIMG(std::string);
    void error(void);
};
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
