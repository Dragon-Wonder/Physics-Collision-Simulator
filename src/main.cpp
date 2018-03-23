/*****************************************************************************/
/////////////////////////////////////////////////
/// @file       Main.cpp
/// @brief      Holds all the main functions.
/// @author     GamerMan7799
/// @author     xPUREx
/// @version    1.4.0-beta.6
/// @date       2018
/// @copyright  Public Domain Unlicense.
/////////////////////////////////////////////////
/*****************************************************************************/
//General Todos
/** @bug (GamerMan7799#1#): The balls will become stuck together for seemingly random reasons */
/** @bug (GamerMan7799#9#): Balls' velocity becomes NaN quite often.
                            Fixed? (haven't seen issue in a while, will remove it
                            if I don't see it for several more updates) */
/** @todo (GamerMan7799#8#): Allow setting of some Physics Values in Config */
/** @todo (GamerMan7799#8#): Set Max/Min values for mass? */
/** @bug  (GamerMan7799#9#): Balls sometimes get stuck in walls, likely caused by the ball size increase.
                             Fixed? (haven't seen issue in a while, will remove it
                                     if I don't see it for several more updates)*/
/** @todo (GamerMan7799#2#): Add pixel to meter factor. Currently it is assumed that 1 pixel = 1
                            Which isn't accurate. Add a factor to change this.  */
/*****************************************************************************/
//#define DEFINED_USE_R2_VEL_MODDER
/**< If this is defined, then program will use unrealistic method that will
     increase the velocity of two colliding balls the closer they are together
     it will reduce the number of times that the balls stick together,
     but also causes them to get unrealistically high velocities*/
/*****************************************************************************/
//#define DEFINED_COLLISION_NORMAL_FORCE
/**< If this is defined, then program will use apply a normal force to any
     colliding balls. The forces are along the same direction as the velocities
     are. It doesn't make sense in the real world.*/
/*****************************************************************************/
#include "version.h"
#include "core.h"
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
    const double kAirDynViscosity = 2.22043 * pow(10,-5); /**< dynamic viscosity of air at 20°C */
  } //end Namespace Physics

  /** Holds Values for different equations that are not physics related */
  namespace equations {
    const float kVelocityScalar = 1; /**< Changing this effects the ratio
                                          of the line to the velocity when
                                          creating a cannonball */
    const float kTimeSizeRatio = 0.025; /**< One second of holding down
                                                the mouse button = this many
                                                meters for the ball */
    const float kMassAlphaRatio = 41.166; /**< The ratio between mass and its
                                                alpha (transparent) value. \n
                                                The equation used is Global::Equations::kMassAlphaRatio
                                                * ln( clsCannonball::props.mass ) +
                                                Global::Equations::kMassAlphaOffset */
    const float kMassAlphaOffset = 130.84; /**< The offset for the equation of
                                                mass to alpha value \n
                                                The equation used is Global::Equations::kMassAlphaRatio
                                                * ln( clsCannonball::props.mass ) +
                                                Global::Equations::kMassAlphaOffset */
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
  if ( !core::cannonwindow.getSDLStarted() ) { return 1; } //exit program if there was an error

  bool quit = false, holding = false, random_fire = false;
  double tempdeltat;
  int fire_tick  = 5000;
  SDL_Event event;

  uint ticks_since_clean = 0;
  char event_return;

  do {
    core::cannonwindow.clearscreen(); //Clear the screen so new stuff can be drawn

    //poll events
    if (SDL_PollEvent( &event ) != 0) { event_return = core::handleEvent( &event ); }
    switch (event_return) {
    case 'q':
      quit = true;
      break;
    case 'f':
      random_fire = !(random_fire);
      break;
    }

    if (random_fire) {
      fire_tick++;
      if (fire_tick > 250) {
        core::fireRandom();
        fire_tick = 0;
      }
    }

    //Update every ball
    tempdeltat = core::tick.getTimeDifference();
    for (int i = 0; i < cannonballs::ropes.size(); ++i) {
      //Loop through each rope
      cannonballs::ropes[i].update();
    } //end for loop

    for (int i = 0; i < cannonballs::balls.size(); ++i) {
      //Loop through each cannonball
      if (cannonballs::balls[i].blnstarted_) {
        //only update cannonball if it is "alive"
        //Check for collisions if no collide is not on
        if (global::physics::collisionmethod != CollideNone)
          { cannonballs::checkCollisons(i); }
        cannonballs::balls[i].update(tempdeltat);
      } //end if started
    } //end for loop

   if (core::holding && core::toolbar.getTool() == ToolFire)
    { core::cannonwindow.drawline(core::currentmouse, core::oldmouse); }

    core::toolbar.show();
    core::cannonwindow.update(); //Update the screen
    ticks_since_clean++;
    // run a clean up on dead balls if past a certain number
    if (ticks_since_clean >= 1000) {
      cannonballs::clean_up();
      ticks_since_clean = 0;
    }
  } while (!quit); //keep looping until we get a quit

  if(!cannonballs::ropes.empty()) {cannonballs::ropes = VectorRope();}
  if(global::blnDebugMode) {printf("Ropes cleared.\n");}

  if(!cannonballs::balls.empty()) {cannonballs::balls = VectorCannon();}
  if(global::blnDebugMode) {printf("Balls cleared.\n");}
  return 0;
}
/*****************************************************************************/
