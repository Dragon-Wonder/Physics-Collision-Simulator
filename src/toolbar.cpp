/*****************************************************************************/
#include "toolbar.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file toolbar.cpp
/// @brief Holds all of the functions for the toolbar class
/////////////////////////////////////////////////
/*****************************************************************************/
clsToolbar::clsToolbar() {
    //ctor

    selected_tool_ = ToolFire;
    show_toolbox_ = true;
    /** TODO (GamerMan7799#9#): Change toolbar to bottom left. */
    /** TODO (GamerMan7799#9#): Make toolbar bigger? */
    position_ = {0,0,28,28};

    //position_ = {screen::screenatt.height - 28, screen::screenatt.width - 28,
    //              28,28};
}
/*****************************************************************************/
void clsToolbar::show() {
  /////////////////////////////////////////////////
  /// @brief Displays the toolbox if enabled
  /////////////////////////////////////////////////

  // Make SDL rect just for the tool picture inside the frame
  SDL_Rect tool_pic_box = {position_.x+2,position_.y+2,24,24};

  if(show_toolbox_) { // only show if enabled
    SDL_RenderCopy(screen::screenatt.ren,screen::screenatt.toolbox,
                 NULL,&position_); // draw toolbox frame

    // draw tool inside box
    SDL_RenderCopy(screen::screenatt.ren,screen::screenatt.tools,
                   &screen::screenatt.toolclips[selected_tool_], &tool_pic_box);

  }
}
/*****************************************************************************/
void clsToolbar::toggleToolbox() {
  /////////////////////////////////////////////////
  /// @brief Toggles showing the toolbox on/off.
  /////////////////////////////////////////////////

  show_toolbox_ = !(show_toolbox_);
}
/*****************************************************************************/
void clsToolbar::incrementTool(char dir) {
  /////////////////////////////////////////////////
  /// @brief Moves toolbox to the next or previous tool
  ///
  /// @param dir = value to increment tool box up or down
  /////////////////////////////////////////////////

  if (dir == -1 && selected_tool_ != ToolFire) { selected_tool_--; }
  else if (dir == 1 && selected_tool_ != ToolDrag) { selected_tool_++; }
}
/*****************************************************************************/
uchar clsToolbar::getTool() {
  /////////////////////////////////////////////////
  /// @brief Returns current selected tool
  ///
  /// @return Selected tool based on enumTools
  /////////////////////////////////////////////////

  return selected_tool_;
}
/*****************************************************************************/
void clsToolbar::setTool(uchar newtool) {
  /////////////////////////////////////////////////
  /// @brief Sets tool
  ///
  /// @param newtool = new tool
  /////////////////////////////////////////////////

  selected_tool_ = newtool;
}
/*****************************************************************************/
