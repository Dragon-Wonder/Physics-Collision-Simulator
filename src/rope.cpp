/*****************************************************************************/
#include "rope.h"
#include "core.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file rope.cpp
/// @brief Holds all of the functions for the rope
/////////////////////////////////////////////////
/*****************************************************************************/
clsRope::clsRope() {
    //ctor

    blncheckphysics_ = false;
    length_ = 0;
    spot_ = {{0,0},{0,0}};
    attachments_ = {AttachmentNone, AttachmentNone};
    balls_ = {nullptr, nullptr};
}
/*****************************************************************************/
clsRope::~clsRope() {
  blncheckphysics_ = false;
  balls_ = {nullptr, nullptr};
  if(global::blnDebugMode) {printf("Rope deconstructor called.\n");}
}
/*****************************************************************************/
void clsRope::update() {
  /////////////////////////////////////////////////
  /// @brief updates the rope.
  /// @return void
  /////////////////////////////////////////////////
  if (blncheckphysics_) { //skip everything if not checking physics
    // check conditions in which the rope has to be deleted.
    if ( (attachments_.one == AttachmentWall && attachments_.two == AttachmentWall ) ||
         (attachments_.one == AttachmentBall && balls_.one == nullptr) ||
         (attachments_.two == AttachmentBall && balls_.two == nullptr)
       ) {
      // if both spots are attached to walls, delete it, since it is not going
      // to be doing anything
      if(global::blnDebugMode) {printf("Bad rope, killing it.\n");}

      blncheckphysics_ = false;
      return;
    }
    update_spots();

    // do physics stuff

    draw();
  }
}
/*****************************************************************************/
void clsRope::activate() {
  /////////////////////////////////////////////////
  /// @brief activates the rope.
  /// @return void
  ///
  /////////////////////////////////////////////////
  blncheckphysics_ = true;
  update_spots();

  length_ = floor (sqrt ( pow(spot_.one.x - spot_.two.x,2) +
                   pow(spot_.one.y - spot_.two.y,2) ) );

  draw();
}
/*****************************************************************************/
void clsRope::setAttachment(uchar num, LOC place) {
  /////////////////////////////////////////////////
  /// @brief Sets rope attachment to a wall spot at place
  ///
  /// @param place = Wall spot in X and Y
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (num == 1) {
    attachments_.one = AttachmentWall;
    spot_.one = place;
    //if(global::blnDebugMode) {printf("(%i,%i)\n",spot_.one.x,spot_.one.y);}
    balls_.one = nullptr;
  } else if (num == 2) {
    attachments_.two = AttachmentWall;
    spot_.two = place;
    balls_.two = nullptr;
  }
}
/*****************************************************************************/
void clsRope::setAttachment(uchar num, clsCannonball* new_ball) {
  /////////////////////////////////////////////////
  /// @brief Sets rope attachment one to ball new_ball
  ///
  /// @param new_ball = pointer to ball rope is attached to
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (num == 1) {
    attachments_.one = AttachmentBall;
    spot_.one = {0,0};
    balls_.one = new_ball;
  } else if (num == 2) {
    attachments_.two = AttachmentBall;
    spot_.two = {0,0};
    balls_.two = new_ball;
  }
}
/*****************************************************************************/
void clsRope::draw() {
  /////////////////////////////////////////////////
  /// @brief Draws the rope
  /////////////////////////////////////////////////

  // Yay for reusing old code.
  /** @bug (GamerMan7799#9#): Rope will only a straight line from its two points.
      Meaning that if the location of the two spots is less than the length, the rope
      just looks shorter instead of curving like it should. I may or may not
      try to fix this.  */

  core::cannonwindow.drawline(spot_.one,spot_.two);
}
/*****************************************************************************/
void clsRope::update_spots() {
  // if attachment one is a ball, update spot
  if (attachments_.one == AttachmentBall) {
      if (!(balls_.one->blnstarted_)) {
        blncheckphysics_= false;
        return;
      }
      spot_.one = balls_.one->getScreenPlace();
  }

  // if attachment one is a ball, update spot
  if (attachments_.two == AttachmentBall) {
    if (!(balls_.two->blnstarted_)) {
      blncheckphysics_= false;
      return;
    }
    spot_.two = balls_.two->getScreenPlace();
  }


  /*if(global::blnDebugMode) {printf("New spots are now (%i,%i) and (%i,%i).\n",
                                   spot_.one.x,spot_.one.y,spot_.two.x,spot_.two.y);} */
}
/*****************************************************************************/
