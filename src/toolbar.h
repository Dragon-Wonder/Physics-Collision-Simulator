#ifndef __TOOLBAR_HEADER__
#define __TOOLBAR_HEADER__
/*****************************************************************************/
#include "screen.h"
/*****************************************************************************/
class clsToolbar {
    public:
      clsToolbar();
      ~clsToolbar();
      void show(void);
      void setTool(uchar);
      uchar getTool(void);
      void incrementTool(char);
      void toggleToolbox(void);

    private:

      uchar selected_tool_;
      bool show_toolbox_;
      SDL_Rect position_;

};
/*****************************************************************************/
#endif // __TOOLBAR_HEADER__
