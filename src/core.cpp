/*****************************************************************************/
#include "core.h"
#include "rope.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file core.cpp
/// @brief Holds "core" functions. Made so that main.cpp doesn't get too large
/////////////////////////////////////////////////
/*****************************************************************************/
namespace core {
  clsToolbar toolbar;
  clsTick tick;
  clsScreen cannonwindow;

  LOC oldmouse = {0,0};
  LOC currentmouse = {0,0};
  bool holding = false;
} // end namespace core
/*****************************************************************************/
namespace cannonballs {
  uint intCannonBallNum = 0;
  uint intRopeNum = 0;
  VectorCannon balls;
  VectorRope ropes;
}
/*****************************************************************************/
void cannonballs::addNew(LOC mouseC, LOC mouseO, double HoldTime ) {
  /////////////////////////////////////////////////
  /// @brief Will add a new cannonball based on the mouse start and mouse end.
  ///        Mass of the ball will vary based on Hold Time
  ///
  /// @param mouseC = Current Mouse Location in X and Y
  /// @param mouseO = Old (start of mouse click) Mouse Location in X and Y
  /// @param HoldTime = Time (in seconds) that the mouse button was held down for
  /// @return void
  ///
  /////////////////////////////////////////////////
  //Get location, vel, and angle
  double fire_v;
  double angle;
  double radius = (double)global::equations::kTimeSizeRatio * sqrt(HoldTime);

  fire_v = -1 * sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
  fire_v *= (double) global::equations::kVelocityScalar;

  //If the mouse if pointing straight up or straight down make the angle 90
  //Otherwise calculate the angle with atan.
  if (mouseC.x == mouseO.x) {
    angle = (M_PI / 2) * ( (mouseC.y > mouseO.y) ? -1.0 : 1.0 );
  } else {
    angle = (double) -1.0 * atan( (double)(mouseC.y - mouseO.y) /
             (double)(mouseC.x - mouseO.x) );
    angle += (double)( (mouseC.x < mouseO.x) ? M_PI : 0.0 );
  } //end if x = x

  //mod mouse start, once again because the top left is 0,0 to SDL
  mouseO.y = global::config.values.uintScreenHeight - mouseO.y;
  intCannonBallNum++;
  clsCannonball tempBall;
  tempBall.setValues(radius, mouseO,fire_v, angle, intCannonBallNum);
  if (intCannonBallNum > 1) {
    if(balls[0].isPaused()) { tempBall.togglePause(); }
  }
  balls.push_back(tempBall);

  return;
}
/*****************************************************************************/
void cannonballs::checkCollisons(uint j) {

  /////////////////////////////////////////////////
  /// @brief Checks ball number j is colliding with another ball and then do collisions.
  ///
  /// @param  j = the number in the array that ball we are checking is.
  /// @return void (all changes if they are colliding is handled in this function).
  ///
  /////////////////////////////////////////////////

  BOX A, B;
  A = balls[j].getBOX();
  if (balls[j].blncheckphysics_) {
    for (int i = 0; i < balls.size(); ++i) {
      if (balls[i].blncheckphysics_ && balls[j].blncheckphysics_ &&  i != j) {
        B = balls[i].getBOX();
        if ( checkOverlap(A, B) ) {
          doCollide(j, i);
          balls[i].blncheckphysics_ = false;
          balls[j].blncheckphysics_ = false;
        } // end if overlap
      } //end if started and not same ball
    } //end for loop inner
  } //end if check physics
}
/*****************************************************************************/
bool cannonballs::checkOverlap(BOX A, BOX B) {
  /////////////////////////////////////////////////
  /// @brief Checks if two boxes overlap
  ///
  /// @param A = Box for ball A
  /// @param B = Box for ball B
  /// @return TRUE / FALSE if they overlap and therefore collide
  ///
  /////////////////////////////////////////////////

  if( A.right < B.left ){ return false; }
  if( A.left > B.right ){ return false; }
  if( A.bottom < B.top ){ return false; }
  if( A.top > B.bottom ){ return false; }

  return true;
}
/*****************************************************************************/
void cannonballs::doCollide(uint numA, uint numB) {
  /////////////////////////////////////////////////
  /// @brief Will calculate the new velocities of two balls that are colliding
  ///
  /// @image html Equations.PNG "The equations used to get the resulting velocities."
  /// @param numA = the number in the array ball A is
  /// @param numB = the number in the array ball B is
  /// @return void (everything is handled inside the function)
  ///
  /////////////////////////////////////////////////

  dblXY Avel, Bvel;
  PP Aprops, Bprops;


  Avel = balls[numA].getVelocity();
  Bvel = balls[numB].getVelocity();
  Aprops = balls[numA].getPhysicalProps();
  Bprops = balls[numB].getPhysicalProps();

#if DEFINED_USE_R2_VEL_MODDER == 1
  //This part here has no actual basis on real life,
  //it is just my attempt at preventing the cannonballs from sticking together
  LOC CenterA, CenterB, DeltaCenters;
  double VelModder;
  CenterA = balls[numA].getplace();
  CenterB = balls[numB].getplace();

  DeltaCenters.x = abs(CenterA.x - CenterB.x);
  DeltaCenters.y = abs(CenterA.y - CenterB.y);
  //Since it is r^2 and the sqrt of this give us r, we just drop the sqrt part to save time
  VelModder = sqrt(pow((double)DeltaCenters.x,2) + pow((double)DeltaCenters.y,2));
  VelModder = sqrt(200.0) / VelModder; //200 is the highest value possible
  if (VelModder < 1.0) {VelModder = 1.0;}

  Avel.x *= (double) VelModder;
  Avel.y *= (double) VelModder;
  Bvel.x *= (double) VelModder;
  Bvel.y *= (double) VelModder;
  //End of non real stuff
#endif

  double Aangle, Bangle, ContactAngle;
  double Atotal_v, Btotal_v;
  dblXY TotalAMomentum, TotalBMomentum;

  if ( global::physics::collisionmethod != CollidePerfectInelastic ) {
    //The equations for Perfect Inelastic is much simpler than if not
    //so I am handling those a bit differently to speed up that method.

    //Equations used can be found and explained here:
    //https://en.wikipedia.org/wiki/Elastic_collision
    Atotal_v = sqrt( pow(Avel.x,2) + pow(Avel.y,2) );
    Btotal_v = sqrt( pow(Bvel.x,2) + pow(Bvel.y,2) );

    //get the angle for both A and B
    if (Avel.x != 0.0) { Aangle = atan(Avel.y/Avel.x); }
    else { Aangle = (Avel.y >= 0.0) ? M_PI / 2 : -M_PI / 2; }

    if (Bvel.x != 0.0) { Bangle = atan(Bvel.y/Bvel.x); }
    else { Bangle = (Bvel.y >= 0.0) ? M_PI / 2 : -M_PI / 2; }

    //Adjust the angle to be the right one.
    //Since atan will only yield a number between -PI/2 and PI/2 we
    //have to adjust it if xvel is negative.
    Aangle += Avel.x < 0.0 ? M_PI : 0;
    Bangle += Bvel.x < 0.0 ? M_PI : 0;
    //The contact angle has to be the difference between the two angles but
    //since sometimes one or the other is negative, we'll use abs to ensure the right number
    ContactAngle = abs ( abs(Aangle) - abs(Bangle) );

    TotalAMomentum.x = Atotal_v * cos(Aangle - ContactAngle) *
                      (Aprops.mass - Bprops.mass);
    TotalAMomentum.x += 2.0 * Bprops.mass * Btotal_v *
                      cos(Bangle - ContactAngle);
    TotalAMomentum.x /= (Aprops.mass + Bprops.mass);
    //x and y formulas are the same until this point
    TotalAMomentum.y = TotalAMomentum.x;
    TotalAMomentum.x *= cos(ContactAngle);
    TotalAMomentum.y *= sin(ContactAngle);
    TotalAMomentum.x += Atotal_v * sin(Aangle - ContactAngle) *
                        cos(ContactAngle + (M_PI / 2) );
    TotalAMomentum.y += Atotal_v * sin(Aangle - ContactAngle) *
                        sin(ContactAngle + (M_PI / 2) );

    //Now do B
    TotalBMomentum.x = Btotal_v * cos(Bangle - ContactAngle) *
                       (Bprops.mass - Aprops.mass);
    TotalBMomentum.x += 2.0 * Aprops.mass * Atotal_v *
                        cos(Aangle - ContactAngle);
    TotalBMomentum.x /= (Aprops.mass + Bprops.mass);
    //x and y formulas are the same until this point
    TotalBMomentum.y = TotalBMomentum.x;
    TotalBMomentum.x *= cos(ContactAngle);
    TotalBMomentum.y *= sin(ContactAngle);
    TotalBMomentum.x += Btotal_v * sin(Bangle - ContactAngle) *
                        cos(ContactAngle + (M_PI / 2) );
    TotalBMomentum.y += Btotal_v * sin(Bangle - ContactAngle) *
                        sin(ContactAngle + (M_PI / 2) );
  } else {
    TotalAMomentum.x = Aprops.mass * Avel.x + Bprops.mass * Bvel.x;
    TotalAMomentum.y = Aprops.mass * Avel.y + Bprops.mass * Bvel.y;
  } //end if Perfect Inelastic or not

  switch (global::physics::collisionmethod) {
  //figure out what to do based on the collision method
  case CollidePerfectInelastic:
    //The balls collide and stick together
    //Get the new mass.
    Aprops.mass += Bprops.mass;
    //Now we have to calculate the new radius, volume, and area so that drag will reflect the new size
    //we are of course assuming that density stays the same.
    Aprops.volume = (Aprops.mass / (double)global::physics::kBallDensity);
    //cbrt = cube root
    Aprops.radius = cbrt( (double) (3.0*Aprops.volume) / (double) (4.0*M_PI) );
    Aprops.area = (double) (2.0 * M_PI * pow(Aprops.radius, 2) );
    Aprops.interia = (double) (2*Aprops.mass* pow(Aprops.radius,2.0) /5.0);
    //Now calculate the new velocity
    Avel.x = TotalAMomentum.x / Aprops.mass;
    Avel.y = TotalAMomentum.y / Aprops.mass;
    //now "kill" cannonball B and update ball A
    balls[numB].blnstarted_ = false;
    balls[numA].setPhysicalProps(Aprops);
    balls[numA].setVelocity(Avel);
    break;
  case CollideInelastic:
    //uses the same equations as below but some energy is lost.
    TotalAMomentum.x *= (double)global::physics::kCoefficientRestitution;
    TotalAMomentum.y *= (double)global::physics::kCoefficientRestitution;
    TotalBMomentum.x *= (double)global::physics::kCoefficientRestitution;
    TotalBMomentum.y *= (double)global::physics::kCoefficientRestitution;

  case CollideElastic:
    //The balls collide and bounce away from each other

    //All of the heavy lifting is handled above.
    balls[numA].setVelocity(TotalAMomentum);
    balls[numB].setVelocity(TotalBMomentum);

#if DEFINED_COLLISION_NORMAL_FORCE == 1
    if (TotalAMomentum.x != 0.0) { Aangle = atan(TotalAMomentum.y/TotalAMomentum.x); }
    else { Aangle = (TotalAMomentum.y >= 0.0) ? M_PI / 2 : -M_PI / 2; }
    if (TotalBMomentum.x != 0.0) { Bangle = atan(TotalBMomentum.y/TotalBMomentum.x); }
    else { Bangle = (TotalBMomentum.y >= 0.0) ? M_PI / 2 : -M_PI / 2; }
    Aangle += TotalAMomentum.x < 0.0 ? M_PI : 0;
    Bangle += TotalBMomentum.x < 0.0 ? M_PI : 0;

    double normal_force;
    dblXY forces;
    normal_force = -1 * Aprops.mass * global::physics::kGravity;
    forces.x = normal_force * cos (Aangle);
    forces.y = normal_force * sin (Aangle);
    balls[numA].addForce(forces);

    normal_force = -1 * Bprops.mass * global::physics::kGravity;
    forces.x = normal_force * cos (Bangle);
    forces.y = normal_force * sin (Bangle);
    balls[numB].addForce(forces);
#endif

    break;
  default: //the catch all and CollideNone
    //Nothing Happens!
    break;
  } //end switch collide method
} //end do Collide
/*****************************************************************************/
void cannonballs::clean_up() {
  /////////////////////////////////////////////////
  /// @brief Removes any "dead" balls/rope from the balls/rope vector and
  ///        shrinks it to reduce memory usage
  /////////////////////////////////////////////////
  int new_cannon_num = 0; // keeps track of the number of valid balls found

  for(int i = 0; i < intCannonBallNum; ++i) {
    if ( !(balls[i].blnstarted_) ) {
      balls.erase(balls.begin()+i);
    } else { new_cannon_num++; }
  }

  if (global::blnDebugMode) {
    printf("Clean up has been run.\n");
    printf("%3i balls have been cleared\n",intCannonBallNum-new_cannon_num);
    printf("%3i balls remain\n",new_cannon_num);
  }
  intCannonBallNum = new_cannon_num;
  if(!balls.empty()) { balls.shrink_to_fit(); }


  int new_rope_num = 0; // keeps track of the number of valid ropes found

  for(int i = 0; i < intRopeNum; ++i) {
    if ( !(ropes[i].blncheckphysics_) ) {
      ropes.erase(ropes.begin()+i);
    } else { new_rope_num++; }
  }

  if (global::blnDebugMode) {
    printf("%3i ropes have been cleared\n",intRopeNum-new_rope_num);
    printf("%3i ropes remain\n",new_rope_num);
  }
  intRopeNum = new_rope_num;
  if(!ropes.empty()) { ropes.shrink_to_fit(); }
}
/*****************************************************************************/
void core::fireRandom() {
  /////////////////////////////////////////////////
  /// @brief Causes cannonballs to be fired randomly
  /////////////////////////////////////////////////
  Configures cnfg = global::config.values;
  LOC mouseo, mousec;
  // time delay can be anywhere from 0.25 to 10 seconds
  double time_delay = ((rand() % (10000-250) + 250) / 1000);

  mouseo.x = rand() % cnfg.uintScreenWidth;
  mouseo.y = rand() % cnfg.uintScreenHeight;

  mousec.x = rand() % cnfg.uintScreenWidth;
  mousec.y = rand() % cnfg.uintScreenHeight;

  cannonballs::addNew(mousec, mouseo, time_delay);

}
/*****************************************************************************/
char core::handleEvent(SDL_Event* e ) {
  /////////////////////////////////////////////////
  /// @brief Checks an SDL event to see what the program should do.
  /// @param e = Pointer to the event that happened.
  /// @return char for what main should do.
  /////////////////////////////////////////////////

  //if(global::blnDebugMode) {printf("There was an event!\n");}

  int ball_num;

  if ( toolbar.getTool() != ToolFire && toolbar.getTool() != ToolDrag &&
       toolbar.getTool() != ToolDrop && holding ) {
    holding = false;
    tick.stopHolding();
  } // this should prevent strange things from happening if user switches tools
    // while holding down mouse button.

  if ( e->type == SDL_QUIT ) { return 'q'; }

  if ( e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {holding = true;}
    else if (e->type == SDL_MOUSEBUTTONUP) {holding = false;}
    // there is a mouse-based event, so now we have to check what tool we are using.
    //if (global::blnDebugMode) { printf("Mouse event found.\n"); }
    switch ( toolbar.getTool() ) {
    case ToolFire:
    default: // make tool fire the default
      doFireTool(e);
      return 0;
    case ToolDrop:
      doDropTool(e);
      return 0;
    case ToolRope:
      doRopeTool(e);
      return 0;
    case ToolDele:
      doDeleTool(e);
      return 0;
    case ToolDrag:
      doDragTool(e);
      return 0;
    case ToolInfo:
      doInfoTool(e);
      return 0;
    } // end switch tool type
  } else if (e->type == SDL_MOUSEMOTION) {
    SDL_GetMouseState(&currentmouse.x, &currentmouse.y );
    if(toolbar.getTool() == ToolDrag) { doDragTool(e); }
  } else if ( e->type == SDL_KEYDOWN ) {
    switch ( e->key.keysym.sym ) {
    case SDLK_k:
      //kill all the balls
      for (int i = 0; i < cannonballs::balls.size(); ++i)
        { cannonballs::balls[i].blnstarted_ = false;  }
      return 0;
    case SDLK_q:
    case SDLK_ESCAPE:
      return 'q';
    case SDLK_r:
      //stop all motion of balls
      for (int i = 0; i < cannonballs::balls.size(); ++i)
        { cannonballs::balls[i].setVelocity({0.0,0.0}); }
      return 0;
    case SDLK_p:
      //"pauses" the simulation by preventing ball from updating
      for (int i = 0; i < cannonballs::balls.size(); ++i)
        { cannonballs::balls[i].togglePause(); }
      return 0;
    case SDLK_f:
      if(global::blnDebugMode) { printf("Random fire triggered\n"); }
      return 'f';
    case SDLK_t:
      if(global::blnDebugMode) {printf("Toolbox toggled.\n");}
      core::toolbar.toggleToolbox();
      return 0;
    case SDLK_RIGHT:
      core::toolbar.incrementTool(1);
      return 0;
    case SDLK_LEFT:
      core::toolbar.incrementTool(-1);
      return 0;
    case SDLK_1:
      core::toolbar.setTool(ToolFire);
      return 0;
    case SDLK_2:
      core::toolbar.setTool(ToolDrop);
      return 0;
    case SDLK_3:
      core::toolbar.setTool(ToolRope);
      return 0;
    case SDLK_4:
      core::toolbar.setTool(ToolDele);
      return 0;
    case SDLK_5:
      core::toolbar.setTool(ToolDrag);
      return 0;
    case SDLK_6:
      core::toolbar.setTool(ToolInfo);
      return 0;
    default:
      return 0;
    } //end switch key
  } //end if event
  return 0; // main doesn't have to do anything else, return 0.
}
/*****************************************************************************/
int core::findSelectedBall(LOC mouseplace) {
  /////////////////////////////////////////////////
  /// @brief Finds the ball that was clicked on.
  /// @param mouseplace = Where the mouse clicked
  /// @return int = id of the ball selected
  /////////////////////////////////////////////////

  BOX ball_box;

  for (int i = 0; i < cannonballs::balls.size(); ++i){
    ball_box = cannonballs::balls[i].getBOX();

    if ( mouseplace.x >= ball_box.left && mouseplace.x <= ball_box.right) {
      // SDL y coordinates are measured from top of screen
      if ( mouseplace.y >= ball_box.top && mouseplace.y <= ball_box.bottom) {
        return i;
      }
    }
  }

  return -1; // if not ball found to be selected
}
/*****************************************************************************/
void core::doFireTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the fire tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  if(global::blnDebugMode) { printf("Tool Fire event\n"); }
  if ( e->type == SDL_MOUSEBUTTONDOWN ) {
    holding = true;
    tick.startHolding();
    SDL_GetMouseState(&oldmouse.x, &oldmouse.y);
    currentmouse = oldmouse;
  } else if ( e->type == SDL_MOUSEBUTTONUP ) {
    holding = false;
    cannonballs::addNew(currentmouse, oldmouse, tick.stopHolding() );
  } // end if event type
}
/*****************************************************************************/
void core::doDeleTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the delete tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  int ball_num;

  if(global::blnDebugMode) { printf("Tool Dele event\n"); }
  if( e->type == SDL_MOUSEBUTTONDOWN ){
    SDL_GetMouseState(&currentmouse.x, &currentmouse.y);
    ball_num = findSelectedBall(currentmouse);
    if (ball_num == -1) {return;}
    cannonballs::balls[ball_num].blnstarted_ = false;
  }
}
/*****************************************************************************/
void core::doDropTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the drop tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  if(global::blnDebugMode) { printf("Tool Drop event\n"); }
  if ( e->type == SDL_MOUSEBUTTONDOWN ) {
    holding = true;
    tick.startHolding();
    SDL_GetMouseState(&oldmouse.x, &oldmouse.y);
  } else if ( e->type == SDL_MOUSEBUTTONUP ) {
    holding = false;
    cannonballs::addNew(oldmouse, oldmouse, tick.stopHolding() );
  } // end if event type

}
/*****************************************************************************/
void core::doDragTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the drag tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  static int ball_num = -1;

  if (e->type == SDL_MOUSEBUTTONDOWN) { ball_num = findSelectedBall(currentmouse); }
  //else if (e->type == SDL_MOUSEBUTTONUP) { ball_num = -1; }

  if (ball_num == -1) {return;}

  if (e->type == SDL_MOUSEMOTION && holding) {
    currentmouse.y = global::config.values.uintScreenHeight - currentmouse.y;
    cannonballs::balls[ball_num].setplace(currentmouse);
  }

  //if(global::blnDebugMode) { printf("Tool Drag event\n"); }
  if ( holding && !(cannonballs::balls[ball_num].isPaused()) ) {
    cannonballs::balls[ball_num].togglePause();
  } else if (!(holding) && cannonballs::balls[ball_num].isPaused()
             && !(cannonballs::balls.back().isPaused()) &&
                !(cannonballs::balls.front().isPaused())) {
      // will only unpause the dragged ball if the other balls are also
      // unpaused. I check the first and last balls to avoid the chance
      // that the selected ball is the first or last.
    cannonballs::balls[ball_num].togglePause();
    ball_num = -1;
  }
}
/*****************************************************************************/
void core::doInfoTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the info tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  int ball_num;

  if(global::blnDebugMode) { printf("Tool Info event\n"); }
  if ( e->type == SDL_MOUSEBUTTONDOWN ) {
    SDL_GetMouseState(&currentmouse.x, &currentmouse.y);
    ball_num = findSelectedBall(currentmouse);
    if (ball_num == -1) {return;}
    cannonballs::balls[ball_num].writeInfo();
  }
}
/*****************************************************************************/
void core::doRopeTool(SDL_Event* e) {
  /////////////////////////////////////////////////
  /// @brief Function for the rope tool
  /// @param e = SDL_event passed through handle event
  /////////////////////////////////////////////////

  int ball_num;
  clsRope tempRope;

  if ( e->type == SDL_MOUSEBUTTONDOWN ) {
    SDL_GetMouseState(&currentmouse.x, &currentmouse.y);
    ball_num = findSelectedBall(currentmouse);
    //mod mouse start, once again because the top left is 0,0 to SDL
    //currentmouse.y = global::config.values.uintScreenHeight - currentmouse.y;
    if(global::blnDebugMode) { printf("Tool Rope event\n"); }
    if( !cannonballs::ropes.empty() ){
      // not first rope, therefore check if previous rope is incomplete
      // aka missing second attachment point, or if a new rope is needed.
      if (cannonballs::ropes.back().blncheckphysics_) {
        // previous rope is complete, therefore make new rope.
        if (ball_num == -1) {
          // not clicking on a ball therefore attach to wall
          tempRope.setAttachment(1,currentmouse);
        } else {
          // clicking on ball
          tempRope.setAttachment(1,&cannonballs::balls[ball_num]);
        }
        cannonballs::intRopeNum++;
        cannonballs::ropes.push_back(tempRope);
      } else { // previous rope is not complete, there second attachment
        if (ball_num == -1) {
          // not clicking on a ball therefore attach to wall
          cannonballs::ropes.back().setAttachment(2,currentmouse);
        } else {
          // clicking on ball
          cannonballs::ropes.back().setAttachment(2,&cannonballs::balls[ball_num]);
        }
        cannonballs::ropes.back().activate();
      } // end if first or second attachment.
    } else { // first rope.
      // first rope, just set everything up.
      if (ball_num == -1) {
        // not clicking on a ball therefore attach to wall
        tempRope.setAttachment(1,currentmouse);
      } else {
        // clicking on ball
        tempRope.setAttachment(1,&cannonballs::balls[ball_num]);
      }
      cannonballs::intRopeNum++;
      cannonballs::ropes.push_back(tempRope);
    } // end if more than 1 rope
  } // end if mouse down
}
/*****************************************************************************/
