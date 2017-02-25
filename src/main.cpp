/*****************************************************************************/
//General Todos
/* FIXME (GamerMan7799#1#): The balls will become stuck together for seemingly random reasons */
/* TODO (GamerMan7799#9#): Allow setting of some Physics Values in Config */
/* TODO (GamerMan7799#9#): Add spin of balls to calculations */
/* TODO (GamerMan7799#9#): Add different shapes (cube, cylinder, etc...) */
/*****************************************************************************/
#include "version.h"
#include "screen.h"
#include "cannonball.h"
#include "tick.h"
/*****************************************************************************/
#include <vector>
typedef std::vector<clsCannonball> VectorCannon;
/*****************************************************************************/
//#define DEFINED_USE_R2_VEL_MODDER
/**< If this is defined, then program will use unrealistic method that will
     increase the velocity of two colliding balls the closer they are together
     it will reduce the number of times that the balls stick together,
     but also causes them to get unrealistically high velocities*/
/*****************************************************************************/
/** Holds functions that deal with the balls colliding, and creating new balls
    that are currently in main.cpp. I wanted to get them out of the global scope. */
namespace cannonballs {
  void addNew(LOC, LOC, double);
  void clean_up(void);
  void checkCollisons(uint);
  bool checkOverlap(BOX, BOX);
  void doCollide(uint, uint);
  void fireRandom(void);
  uint intCannonBallNum = 0;
  VectorCannon balls;
}
/*****************************************************************************/
namespace global {
  /** Holds if build is in debug mode, this can happen if
      DEFINED_BUILD_MODE_PRIVATE is defined in the complier */
#ifdef DEFINED_BUILD_MODE_PRIVATE
  const bool blnDebugMode = true;
#else
  const bool blnDebugMode = false;
#endif

  clsConfig config;

  /** Holds Values that are related the the physics of the world,
      these are all based on real numbers */
  namespace physics {
    const float kBallDensity = 7850; /**< Density of the ball in kg/m<sup>3</sup> (7850 is steel) */
    const float kGravity = -9.81; /**< Acceleration due to gravity in m/s<sup>2</sup> */
    const float kDragCofficient = 0.47; /**< Drag coefficient in the air. \n
                                             Value is based on that of a sphere. \n
                                             See https://en.wikipedia.org/wiki/Drag_coefficient for more info. */
    const float kKineticFriction = 0.36; /**< Kinetic Friction values based on Concrete and Steel */
    const float kDensityAir = 1.2041; /**< Density of air (in kg/m<sup>3</sup>)  \n
                                           This value is based on air at 20ºC and 101.325 kPa \n
                                           See https://en.wikipedia.org/wiki/Density_of_air for more info */
    const float kMinVelocity = 0.0; /**< If a ball has less velocity than the it will "die" */
    const float kCoefficientRestitution = 0.76; /**< How much total energy remains after a collision,
                                                    (see https://en.wikipedia.org/wiki/Coefficient_of_restitution for more info) */
    uchar collisionmethod = CollideInelastic; /**< The collision method to use (see Collisions Enum) */
  } //end Namespace Physics

  /** Holds Values for different equations that are not physics related */
  namespace equations {
    const float kVelocityScalar = 1; /**< Changing this effects the ratio
                                          of the line to the velocity when
                                          creating a cannonball */
    const float kTimeSizeRatio = 0.025; /**< One second of holding down
                                                the mouse button = this many
                                                meters for the ball */
    const float kMassAlphaRatio = 20.583; /**< The ratio between mass and its
                                                alpha (transparent) value. \n
                                                The equation used is Global::Equations::fMassAlphaRatio
                                                * ln( clsCannonball::props.mass ) +
                                                Global::Equations::fMassAlphaOffset */
    const float kMassAlphaOffset = 117.14; /**< The offset for the equation of
                                                mass to alpha value \n
                                                The equation used is Global::Equations::fMassAlphaRatio
                                                * ln( clsCannonball::props.mass ) +
                                                Global::Equations::fMassAlphaOffset */
    const float kAlphaMinimum = 50; /**< The lowest that the alpha value is allowed to be,
                                         for reference 0 is completely transparent and 255 is completely opaque */
  } //end namespace Equations
} //end namespace Global
/*****************************************************************************/
int main(int argc, char *argv[]) {
  /////////////////////////////////////////////////
  /// @brief The main function
  ///
  /// @param argc = Something required by SDL
  /// @param argv = Something required by SDL
  /// @return 0 for successfully ran, or 1 for an error happened.
  ///
  /////////////////////////////////////////////////
  global::config.Check(); //Load Config file and all its values.
  if (global::config.values.blnLogging) { //Open log file to clear it, if it exists
    FILE* logfile = fopen("logfile.log","w");
    fclose(logfile);
  } //end if logging

  if (global::blnDebugMode) { printf("OS: %s\n",global::config.values.OperatingSystem); }
  clsTick tick; //create tick object
  clsScreen cannonwindow; //Start the screen
  if ( !cannonwindow.getSDLStarted() ) { return 1; } //exit program if there was an error

  bool quit = false, holding = false, random_fire = false;
  double tempdeltat;
  int fire_tick  = 5000;
  LOC oldmouse, currentmouse;
  SDL_Event event;

  uint ticks_since_clean = 0;
  do {
    cannonwindow.clearscreen(); //Clear the screen so new stuff can be drawn

    //poll events
    if (SDL_PollEvent( &event ) != 0) {
      if ( event.type == SDL_QUIT ) { quit = true; }
      else if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        holding = true;
        tick.startHolding();
        SDL_GetMouseState(&oldmouse.x, &oldmouse.y);
        currentmouse = oldmouse;
      } else if ( event.type == SDL_MOUSEMOTION && holding ) {
        //Draw the line
        SDL_GetMouseState(&currentmouse.x, &currentmouse.y );
      } else if ( event.type == SDL_MOUSEBUTTONUP ) {
        holding = false;
        cannonballs::addNew(currentmouse, oldmouse, tick.stopHolding() );
      } else if ( event.type == SDL_KEYDOWN ) {
        switch ( event.key.keysym.sym ) {
        case SDLK_k:
          //kill all the balls
          for (VectorCannon::iterator i = cannonballs::balls.begin();
               i != cannonballs::balls.end(); ++i) { b->blnstarted_ = false;  }
          break;
        case SDLK_q:
        case SDLK_ESCAPE:
          //quit
          quit = true;
          break;
        case SDLK_r:
          //stop all motion of balls
          /* TODO (GamerMan7799#9#): Figure out how to count i by element internal to the vector */
          for (VectorCannon::iterator i = cannonballs::balls.begin();
               i != cannonballs::balls.end(); ++i) { i->setVelocity({0.0,0.0}); }

          break;
        case SDLK_f:
          if(global::blnDebugMode) { printf("Random fire triggered\n"); }
          random_fire = !random_fire;
          break;
        } //end switch key
      } //end if event
    } //end if event

    if (holding) { cannonwindow.drawline(currentmouse, oldmouse); }
    if (random_fire) {
      fire_tick++;
      if (fire_tick > 250) {
        cannonballs::fireRandom();
        fire_tick = 0;
      }
    }

    //Update every ball
    tempdeltat = tick.getTimeDifference();
    for (uint i = 0; i < cannonballs::intCannonBallNum; ++i) {
      //Loop through each cannonball
      if (cannonballs::balls[i].blnstarted_) {
        //only update cannonball if it is "alive"
        //Check for collisions if no collide is not on
        if (global::physics::collisionmethod != CollideNone)
          { cannonballs::checkCollisons(i); }
        cannonballs::balls[i].update(tempdeltat);
      } //end if started
    } //end for loop

    cannonwindow.update(); //Update the screen
    ticks_since_clean++;
    // run a clean up on dead balls if past a certain number
    if (ticks_since_clean >= 1000) {
      cannonballs::clean_up();
      ticks_since_clean = 0;
    }

  } while (!quit); //keep looping until we get a quit

  // clear the cannonball vector

  //cannonballs::balls.clear();
  //cannonballs::balls.shrink_to_fit();
  cannonballs::balls = VectorCannon();

  return 0;
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
  double radius = (double)global::equations::kTimeSizeRatio * HoldTime;

  fire_v = -1 * sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
  fire_v *= (double) global::equations::kVelocityScalar;

  //If the mouse if pointing straight up or straight down make the angle 90
  //Otherwise calculate the angle with atan.
  if (mouseC.x == mouseO.x) {
    angle = (M_PI / 2.0) * ( (mouseC.y > mouseO.y) ? -1.0 : 1.0 );
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
    for (uint i = 0; i < intCannonBallNum; ++i) {
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

#ifdef DEFINED_USE_R2_VEL_MODDER
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
    else { Aangle = (Avel.y >= 0.0) ? M_PI / 2.0 : -M_PI / 2.0; }

    if (Bvel.x != 0.0) { Bangle = atan(Bvel.y/Bvel.x); }
    else { Bangle = (Bvel.y >= 0.0) ? M_PI / 2.0 : -M_PI / 2.0; }

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
                        cos(ContactAngle + (M_PI/2.0) );
    TotalAMomentum.y += Atotal_v * sin(Aangle - ContactAngle) *
                        sin(ContactAngle + (M_PI/2.0) );

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
                        cos(ContactAngle + (M_PI/2.0) );
    TotalBMomentum.y += Btotal_v * sin(Bangle - ContactAngle) *
                        sin(ContactAngle + (M_PI/2.0) );
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
    break;
  default: //the catch all and CollideNone
    //Nothing Happens!
    break;
  } //end switch collide method
} //end do Collide
/*****************************************************************************/
void cannonballs::clean_up() {
  /////////////////////////////////////////////////
  /// @brief Removes any "dead" balls from the balls vector and shrinks it
  ///        to reduce memory usage
  /////////////////////////////////////////////////

  uint new_cannon_num = 0; // keeps track of the number of valid balls found

  for(uint i = 0; i < intCannonBallNum; ++i) {
    if ( !(balls[i].blnstarted_) ) {
      balls.erase(balls.begin()+i);
    } else { new_cannon_num++; }
  }

  if (global::blnDebugMode) {
    printf("Clean up has been run.\n");
    printf("%3i entities have been cleared\n",intCannonBallNum-new_cannon_num);
    printf("%3i entities remain\n",new_cannon_num);
  }
  intCannonBallNum = new_cannon_num;
  balls.shrink_to_fit();
}
/*****************************************************************************/
void cannonballs::fireRandom() {
  /////////////////////////////////////////////////
  /// @brief Causes cannonballs to be fired randomly
  /////////////////////////////////////////////////
  Configures cnfg = global::config.values;
  LOC mouseo, mousec;
  // time delay can be anywhere from 0 to 10 seconds
  double time_delay = (rand() % 10000) / 1000;

  mouseo.x = rand() % cnfg.uintScreenWidth;
  mouseo.y = rand() % cnfg.uintScreenHeight;

  mousec.x = rand() % cnfg.uintScreenWidth;
  mousec.y = rand() % cnfg.uintScreenHeight;

  addNew(mousec, mouseo, time_delay);

}
/*****************************************************************************/
