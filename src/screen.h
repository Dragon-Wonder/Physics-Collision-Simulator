#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "config.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsScreen screen.h "source/screen.h"
/// @brief This class will hold all of the functions for the SDL screen.
/////////////////////////////////////////////////
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup ScreenStuff Stuff for SDL
/// @brief Important items for the SDL screen
/// @{
/////////////////////////////////////////////////
/*****************************************************************************/
/** @struct stcWinAtt
    This structure will hold certain attributes of the screen. */
struct stcWinAtt {
  unsigned int width; /**< The width of the screen in terms of pixels. */
  unsigned int height; /**< The height of the screen in terms of pixels. */
  SDL_Window *win; /**< A pointer to the SDL window that is being used. */
  SDL_Renderer *ren; /**< A pointer to the SDL renderer that is being used. */
  SDL_Texture *ball; /**< The ball texture. */
  SDL_Texture *pixel; /**< The pixel texture, used to draw lines */
  SDL_Texture *toolbox; /**< Toolbox frame texture. */
  SDL_Texture *tools; /**< Textures for the tools in the toolbox. */
  SDL_Rect toolclips[6]; /**< Clips for the different tools. */
};
/*****************************************************************************/
/** @struct stcLocation
    This structure will hold place values in terms of X and Y*/
struct stcLocation {
  int x; /**< X location */
  int y; /**< Y location */
};

typedef struct stcWinAtt WINATT;
typedef struct stcLocation LOC;
/// @}
/*****************************************************************************/
// It is likely bad practice to hold values like this...
/** @todo (GamerMan7799#9#): Find a better method */
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

  private:

    bool blnWindow_;     /**< If window is loaded. */
    bool blnRenderer_;   /**< If renderer is loaded. */
    bool blnBall_;       /**< If ball image is loaded. */
    bool blnPixel_;      /**< If pixel image is loaded. */
    bool blnToolbox_;    /**< If toolbox frame is loaded. */
    bool blnTools_;      /**< If tools are loaded. */
    bool bln_SDL_started_; /**< If SDL is started and without issues. */
    SDL_Texture* loadIMG(std::string);
    void error(void);
    void setClips(void);
};
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
