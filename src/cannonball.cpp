/*****************************************************************************/
#include "cannonball.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file cannonball.cpp
/// @brief Holds all of the functions for the cannonballs
/////////////////////////////////////////////////
/*****************************************************************************/
clsCannonball::clsCannonball() {
  /////////////////////////////////////////////////
  /// @brief The default Constructor for the cannonballs. It has to call
  ///         the values as default so we can create it in the array. The values are changed
  ///         later. The default values are as follows:
  ///         * deltat = 1 / 60 (for 60 fps)
  ///         * acc.x = 0
  ///         * acc.y = Global::Physics::fGravity
  ///         * props.radius = 5
  ///         * props.density = Global::Physics::uBallDensity
  ///         * place = 0
  ///         * dblLoc = place casted as a double
  ///         * Vel = 0
  /////////////////////////////////////////////////

  //Put some default values in; on the off chance I forget to set them later.
  blndragenabled_ = false;
  blnstarted_ = false;
  blncheckphysics_ = true;
  deltat_ = (1.00 / 60.00);
  forces_ = {0,0};
  acc_ = {0.00, global::physics::kGravity};
  spin_ = 1.0;

  props_.radius = 5.0; //in meters
  props_.density = global::physics::kBallDensity; //density of steel in kg/m^3
  props_.volume = (double) ((4.0/3.0) * M_PI * pow(props_.radius,3));
	props_.mass = props_.density * props_.volume;
  props_.interia = 2 * props_.mass * pow(props_.radius,2) / 5.0;

  place_ = {0,0};
  dblLOC_.x = (double) place_.x;
  dblLOC_.y = (double) place_.y;

  vel_ = {0,0};

  srand (time(NULL));

  color_ = {(Uint8)rand() % 255,(Uint8)rand() % 255,(Uint8)rand() % 255};

  if(global::config.values.blnDrawPathOnScreen) {
    // Reserve element spots equal to the number of max
    // locations we are going to keep track of
    path_.reserve(global::config.values.uintMaxNumPastPoints);

    // fill the new vector with "empty" values
    for(int i = 0; i < global::config.values.uintMaxNumPastPoints; ++i) {
      path_.push_back({0,0});
    } // end for max past points
    if (global::blnDebugMode) { printf("Path values initialized.\n"); }
  } // end if draw path

  if (global::blnDebugMode) { printf("Ball created.\n"); }
}
/*****************************************************************************/
clsCannonball::~clsCannonball() {
  // clear the vector path

  path_.clear();
  path_.shrink_to_fit();
  path_ = VPath();
}
/*****************************************************************************/
void clsCannonball::dragCalcValues(void) {
  /////////////////////////////////////////////////
  /// @brief This will calculate additional properties of the cannonball based on its radius.
  ///        These include:
  ///        * area
  ///        * volume
  ///        * mass
  ///
  /// @param void
  /// @return void
  ///
  /////////////////////////////////////////////////

	props_.area = (double) (2.0 * M_PI * pow(props_.radius,2));
	props_.volume = (double) ((4.0/3.0) * M_PI * pow(props_.radius,3));
	props_.mass = props_.density * props_.volume;
	props_.interia = 2 * props_.mass * pow(props_.radius,2) / 5.0;
	if(global::blnDebugMode) { printf("Ball %i mass is %5.5f kg.\n",ballID_,props_.mass);}
}
/*****************************************************************************/
void clsCannonball::dragUpdateAcc(void) {
  /////////////////////////////////////////////////
  /// @brief This will update the acceleration of the ball due to Friction
  ///        and Drag (if enabled)
  ///
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (vel_.x != 0.0 && vel_.y != 0.0 && props_.mass != 0.0) {
    double flow_velocity = sqrt(pow(vel_.x,2) + pow(vel_.y,2));
    double Re = (global::physics::kDensityAir * props_.radius * 2 * flow_velocity);
    Re /= global::physics::kAirDynViscosity;
    double drag_force;

    if (Re < 1) {
      // use Stokes' Law for drag
      drag_force = 6.0 * M_PI * global::physics::kAirDynViscosity *
                    props_.radius * flow_velocity;
    } else {
      // use newton drag

      drag_force = (double) (0.5 * global::physics::kDensityAir *
                                pow(flow_velocity,2) * global::physics::kDragCofficient
                                * props_.area);
    } // end if re < 1

    // convert force to acceleration
    double angle;

    angle = (vel_.x != 0.0) ? atan(vel_.y / vel_.x) : M_PI / 2;
    angle += vel_.x < 0.0 ? M_PI : 0.0;

    // update force values
    forces_.x -= drag_force * cos (angle);
    forces_.y -= drag_force * sin (angle);


    // Calculate Buoyancy
    double force_buoyancy;
    force_buoyancy = global::physics::kDensityAir * props_.volume *
                     -1 * global::physics::kGravity;

     forces_.y += force_buoyancy;
  } //end if things equal 0
}
/*****************************************************************************/
void clsCannonball::update(double newdeltat) {
  /////////////////////////////////////////////////
  /// @brief This will do the following:
  ///        * Update ball's position
  ///        * Update ball's velocity
  ///        * Call clsCannonball::dragUpdateAcc if blnDragEnabled is true
  ///        * Log the ball's location (if enabled)
  ///        * Update CollisionBox
  ///        * Set the ball's blnstarted to false (stopping future updates) if total velocity is less than Global::Physics::fMinVelocity or equals NaN
  ///
  /// @param newdeltat = the time (in seconds) that have passed since the last update (see tick.cpp)
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (!paused_) {
    blncheckphysics_ = true; // enable on update
    deltat_ = newdeltat;

    // reset the forces so strange things don't happen
    forces_ = {0.0, props_.mass * global::physics::kGravity};

    if (blndragenabled_) {
      doMagnusEffect();
      dragUpdateAcc();
    }
    doFriction();

    acc_.x = forces_.x / props_.mass;
    acc_.y = forces_.y / props_.mass;

    vel_.x = (vel_.x + acc_.x * deltat_);
    vel_.y = (vel_.y + acc_.y * deltat_);

    // get new velocities if collision with edges
    double coefres = (global::config.values.uchrCollisionMethod == CollideInelastic) ?
                     global::physics::kCoefficientRestitution : 1.0;

    if (collisionbox_.left < screen_place_.w / 2) {
      vel_.x *= -1 * coefres;
      vel_.y *= coefres;
      dblLOC_.x = screen_place_.w;
    }

    if (collisionbox_.right > (screen::screenatt.width)) {
      vel_.x *= -1 * coefres;
      vel_.y *= coefres;
      dblLOC_.x = screen::screenatt.width - screen_place_.w / 2;
    }

    if (collisionbox_.bottom > (screen::screenatt.height)) {
      vel_.x *= coefres;
      vel_.y *= -1 * coefres;
      dblLOC_.y = screen_place_.h / 2;
    }

    if (collisionbox_.top < 0 ) {
      vel_.x *= coefres;
      vel_.y *= -1 * coefres;
      dblLOC_.y = (screen::screenatt.height) - screen_place_.h / 2;
    }

    dblLOC_.x = dblLOC_.x + vel_.x * deltat_ /*+ 0.5 * acc_.x * pow(deltat_,2)*/;
    dblLOC_.y = dblLOC_.y + vel_.y * deltat_ /*+ 0.5 * acc_.y * pow(deltat_,2)*/;

    // update place and collision box again in case something changed
    place_.x = round(dblLOC_.x);
    place_.y = round(dblLOC_.y);
    updateCollisionBox();

    if (global::config.values.blnLogging) {
      FILE* logfile = fopen("logfile.log","a");
      fprintf(logfile,"Ball %3u \t (%.3f, %.3f)\n",ballID_, dblLOC_.x,dblLOC_.y);
      fclose(logfile);
    }

    double total_v;
    total_v = sqrt( pow(vel_.x,2) + pow(vel_.y,2) );
    if (total_v < global::physics::kMinVelocity || isnan(total_v) ) {
      blnstarted_ = false;
      if (global::blnDebugMode) {
        if ( isnan(total_v) ) { printf("Ball velocity is NaN; killing it.\n"); }
        else { printf("Ball moving too slow; killing it.\n"); }
      } //end if debug mode
    } //end if should kill
  } // end if not paused
	show(); //show the ball on the screen
}
/*****************************************************************************/
void clsCannonball::show() {
  /////////////////////////////////////////////////
  /// @brief Places the ball on the SDL Screen
  /// @return void
  /////////////////////////////////////////////////

  screen_place_.x = collisionbox_.left;
  screen_place_.y = collisionbox_.top;

  if (global::config.values.blnDrawPathOnScreen) { drawPath(place_); }

  Uint8 alpha = 0xFF;
  double dblAlpha;

  // log = ln
  dblAlpha = (double) global::equations::kMassAlphaRatio * log(props_.mass) +
              global::equations::kMassAlphaOffset;

  if (dblAlpha < global::equations::kAlphaMinimum)
    { dblAlpha = global::equations::kAlphaMinimum; }

  alpha = dblAlpha > 255.0 ? 255 : (Uint8) dblAlpha;

  //set the ball alpha
  SDL_SetTextureAlphaMod(screen::screenatt.ball, alpha);
  SDL_SetTextureColorMod(screen::screenatt.ball,
                         color_.Red, color_.Green, color_.Blue);

  //Place the ball
  SDL_RenderCopy(screen::screenatt.ren,screen::screenatt.ball,
                 NULL,&screen_place_);

  //reset ball Alpha and color so it doesn't effect the next ball
  SDL_SetTextureAlphaMod(screen::screenatt.ball, 0xFF);
  SDL_SetTextureColorMod(screen::screenatt.ball, 0xFF, 0xFF, 0xFF);

}
/*****************************************************************************/
void clsCannonball::setValues(double r, LOC init_place,
                              double init_vel, double init_angle, int newID) {
  /////////////////////////////////////////////////
  /// @brief Changes the values of the ball to whatever is entered.
  ///
  /// @param r = radius of the ball.
  /// @param init_place = its starting location.
  /// @param init_vel = its starting velocity.
  /// @param init_angle = its starting angle (in radians).
  /// @return void
  ///
  /////////////////////////////////////////////////

  ballID_ = newID;
  paused_ = false;

  props_.radius = r; //in meters

  // Get the width and height of the texture
  SDL_QueryTexture(screen::screenatt.ball,NULL,NULL, &screen_place_.w,
                   &screen_place_.h);

  acc_ = {0.00, global::physics::kGravity};

  spin_ = (rand() % 1000 - 500) / 500;


  place_ = init_place;
  dblLOC_ = { (double) place_.x,
              (double) place_.y };

  vel_ = { (double)(init_vel) * (cos(init_angle)),
           (double)(init_vel) * (sin(init_angle)) };
	blnstarted_ = true;

  dragCalcValues();


  forces_ = {0.00, global::physics::kGravity * props_.mass};
  blndragenabled_ = global::config.values.blnDragMode;
}
/*****************************************************************************/
LOC clsCannonball::getplace() {
  /////////////////////////////////////////////////
  /// @brief Returns the ball's place
  /////////////////////////////////////////////////
  return place_;
}
/*****************************************************************************/
dblXY clsCannonball::getVelocity() {
  /////////////////////////////////////////////////
  /// @brief Returns the ball's velocity
  /////////////////////////////////////////////////
  return vel_;
}
/*****************************************************************************/
void clsCannonball::setVelocity(dblXY newvel) {
  /////////////////////////////////////////////////
  /// @brief Sets the ball's velocity
  /////////////////////////////////////////////////
  vel_ = newvel;
}
/*****************************************************************************/
PP clsCannonball::getPhysicalProps() {
  /////////////////////////////////////////////////
  /// @brief Returns the ball's physical properties
  /////////////////////////////////////////////////
  return props_;
}
/*****************************************************************************/
void clsCannonball::setplace(LOC newplace) {
  /////////////////////////////////////////////////
  /// @brief Set the ball's place
  /////////////////////////////////////////////////
  dblLOC_.x = (double)newplace.x;
  dblLOC_.y = (double)newplace.y;
}
/*****************************************************************************/
void clsCannonball::setPhysicalProps(PP newprops) {
  /////////////////////////////////////////////////
  /// @brief Set the ball's physical properties
  ///        (only used on CollidePerfectInelastic)
  /////////////////////////////////////////////////
  props_ = newprops;
}
/*****************************************************************************/
BOX clsCannonball::getBOX() {
  /////////////////////////////////////////////////
  /// @brief Returns the ball's box
  /////////////////////////////////////////////////
  return collisionbox_;
}
/*****************************************************************************/
void clsCannonball::drawPath(LOC newplace) {
  /////////////////////////////////////////////////
  /// @brief Will draw the path the ball as taken based on kMaxNumPastPoints
  /// @param newplace = The last place the ball was at
  /// @return void
  /////////////////////////////////////////////////

  static uint updatesSinceLast;

  //If there have been enough updates since the last time the path was updated,
  //then update the path array otherwise inc updates
  SDL_SetTextureColorMod(screen::screenatt.pixel, color_.Red,
                         color_.Green, color_.Blue);
  if ( updatesSinceLast >= global::config.values.uintPastDelay ) {
    updatesSinceLast = 0;
    // put new value into array
    path_.push_back(newplace);
    //delete the oldest (first spot) array value
    path_.erase(path_.begin());
    path_.shrink_to_fit();
  } else { updatesSinceLast++; } //end if update points

  //Now draw the path
  SDL_Rect dst;
  dst.w = dst.h = 1;
  for (uint i = 0; i < path_.size(); ++i) {
    dst.y = screen::screenatt.height - (path_[i].y);
    dst.x = (path_[i].x);
    SDL_RenderCopy(screen::screenatt.ren, screen::screenatt.pixel, NULL, &dst);
  } //end for
  SDL_SetTextureColorMod(screen::screenatt.pixel, 0xFF, 0xFF, 0xFF);
}
/*****************************************************************************/
void clsCannonball::updateCollisionBox() {
  /////////////////////////////////////////////////
  /// @brief Updates the collision box
  /////////////////////////////////////////////////

	//Update the collision box for the new location
	collisionbox_.left = place_.x - floor(screen_place_.w / 2);
	collisionbox_.top = screen::screenatt.height - (place_.y + floor(screen_place_.h / 2));
	collisionbox_.bottom = collisionbox_.top + screen_place_.h;
	collisionbox_.right = collisionbox_.left + screen_place_.w;
}
/*****************************************************************************/
void clsCannonball::doFriction() {
  /////////////////////////////////////////////////
  /// @brief Updates values based on friction
  /////////////////////////////////////////////////

  // normal force is mass * gravity - buoyancy (if drag is enabled)
  double normal_force = (-1) * props_.mass * global::physics::kGravity;
  double buoyancy = (-1) * global::physics::kDensityAir *
                    global::physics::kGravity *
                    props_.volume;
  if (blndragenabled_) { normal_force -= buoyancy; }

  double friction = (double)global::physics::kKineticFriction *
                    normal_force;
  updateCollisionBox();
  //Update acc for Friction values
  if ( collisionbox_.bottom <= screen_place_.h ||
       collisionbox_.top >= screen::screenatt.height ) {
    //Ball is in contact with floor or ceiling update x acc
    forces_.x += friction * (vel_.x < 0.0 ? -1.0 : 1.0);
  } // end if touching top/bottom


  // Since there really isn't a normal force when the ball contacts
  // the edges, this section is commented out.
  /*
  if ( collisionbox_.left < (screen_place_.w /2 ) ||
       collisionbox_.right > screen::screenatt.width ) {
    // Ball is in contact with the wall update y acc.
    forces_.y += friction * (vel_.y < 0.0 ? -1.0 : 1.0);
    // add normal force
    forces_.x += normal_force * (collisionbox_.right > screen::screenatt.width) ?
                  1.0 : -1.0;
  } // end if touching side edges */
}
/*****************************************************************************/
void clsCannonball::doMagnusEffect() {
  /////////////////////////////////////////////////
  /// @brief Calculates the Magnus Effect caused by the ball's spin
  /////////////////////////////////////////////////

  double magnus_force = pow(M_PI,2) * pow(props_.radius,3) *
                        global::physics::kDensityAir;

  forces_.x -= magnus_force * vel_.y * spin_;
  forces_.y += magnus_force * vel_.x * spin_;
}
/*****************************************************************************/
void clsCannonball::addForce(dblXY newforces) {

  forces_.x += newforces.x;
  forces_.y += newforces.y;
}
/*****************************************************************************/
double clsCannonball::getSpin() {
  /////////////////////////////////////////////////
  /// @brief Returns the ball's spin
  /////////////////////////////////////////////////
  return spin_;
}
/*****************************************************************************/
void clsCannonball::setSpin(double newspin) {
  /////////////////////////////////////////////////
  /// @brief Sets the ball's spin
  /////////////////////////////////////////////////
  spin_ = newspin;
}
/*****************************************************************************/
void clsCannonball::writeInfo() {
  /////////////////////////////////////////////////
  /// @brief Writes information bout the ball to the console
  /////////////////////////////////////////////////

  printf("\n\n");
  printf("Information about Ball: %i\n",ballID_);
  printf("Color: \t \t \t (%i, %i, %i)\n",color_.Red, color_.Green, color_.Blue);
  printf("Location: \t \t (%5.5f, %5.5f)\n",dblLOC_.x,dblLOC_.y);
  printf("Velocity: \t \t (%5.5f, %5.5f)\n",vel_.x,vel_.y);
  printf("Acceleration: \t \t (%5.5f, %5.5f)\n",acc_.x,acc_.y);
  printf("Forces: \t \t (%5.5f, %5.5f)\n", forces_.x, forces_.y);
  printf("Spin: \t \t \t (%5.5f)\n\n\n", spin_);

}
/*****************************************************************************/
void clsCannonball::togglePause() {
  /////////////////////////////////////////////////
  /// @brief Enable/disable pause
  /////////////////////////////////////////////////

  paused_ = !(paused_);
}
/*****************************************************************************/
