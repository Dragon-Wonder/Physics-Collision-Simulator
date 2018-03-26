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
    double angle;
    LOC differencexy;
    dblXY tension;
    dblXY ball_one_forces, ball_two_forces;
    differencexy.x = abs (spot_.one.x - spot_.two.x);
    differencexy.y = abs (spot_.one.y - spot_.two.y);

    // get the angle measure from spot one to spot two.
    // some of these equations are to correct the angle value.
    angle = (differencexy.x != 0) ? atan ((double)differencexy.y / (double)differencexy.x) :
                                    M_PI / 2;

    if (angle == M_PI/2) {angle += (spot_.one.y > spot_.two.y) ? M_PI : 0;}
    angle += (spot_.two.x < spot_.one.x) ? M_PI : 0;

    // forces only happen when rope is "tense" aka at or greater than length
    if (sqrt(pow(differencexy.x,2) + pow(differencexy.y,2)) >= length_) {
      if (attachments_.one == AttachmentBall && attachments_.two == AttachmentWall) {
        ball_one_forces = balls_.one->getForces();
        tension = ballWallForces(ball_one_forces,angle);
        balls_.one->addForce(tension);
      } else if (attachments_.one == AttachmentWall && attachments_.two == AttachmentBall ) {
        ball_one_forces = balls_.two->getForces();
        tension = ballWallForces(ball_one_forces,angle);
        balls_.two->addForce(tension);
      } else {
        // rope attached to two balls

      }
    }
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

  length_ = (uint)(sqrt ( pow(spot_.one.x - spot_.two.x,2) +
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
  /** @bug (GamerMan7799#9#): Rope will only draw a straight line from its two points.
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
dblXY clsRope::ballWallForces(dblXY ball_one_forces, double angle) {
  dblXY tension;
  if (angle == M_PI / 2 || angle == 3/2 * M_PI ) {
    tension.x = 0;
    tension.y = ball_one_forces.y;
    if (!(signbit((double)spot_.one.y-(double)spot_.two.y) ^ signbit(ball_one_forces.y))) {
      // force and direction to spot two are both positive or both negative, therefore
      // reverse the force for tension
      tension.y *= -1;
    }
  } else if (angle < 2*M_PI && angle > M_PI) {
    tension.y = ball_one_forces.y;
    tension.x = tension.y / tan(angle);
  } else if (angle > 0 && angle < M_PI) {
    tension.y = -1 * ball_one_forces.y;
    tension.x = tension.y / tan(angle);
  } else {
    tension.y = 0;
    tension.x = ball_one_forces.x;
    if ((signbit((double)spot_.one.x-(double)spot_.two.x) ^ signbit(ball_one_forces.x))) {
      // force and direction to spot two are both positive or both negative, therefore
      // reverse the force for tension
      tension.x *= -1;
    }
  }
  return tension;
}
/*****************************************************************************/
