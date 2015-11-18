/*****************************************************************************/
//General Todos
/* FIXME (GamerMan7799#1#): The balls will become stuck together for seemingly random reasons */
/* TODO (GamerMan7799#5#): Collision Method can be set in Config */
/* TODO (GamerMan7799#9#): Allow setting of some Physics Values in Config */
/*****************************************************************************/
#include "version.h"
#include "screen.h"
#include "tick.h"
/*****************************************************************************/
#define DEFINED_CANNONBALL_LIMIT 20
/**< The maximum number of cannonballs allowed. This is because the cannonballs
     are stored in an array; but later versions might allow dynamically allocated number.*/

//j#define DEFINED_USE_R2_VEL_MODDER
/**< If this is defined, then program will use unrealistic method that will
     increase the velocity of two colliding balls the closer they are together
     it will reduce the number of times that the balls stick together,
     but also causes them to get unrealistically high velocities*/
/*****************************************************************************/
/** Holds functions that deal with the balls colliding, and creating new balls
    that are currently in main.cpp. I wanted to get them out of the global scope. */
namespace Cannonballs {
    void addNew(LOC, LOC, double);
    void checkCollisons(uint);
    bool checkOverlap(BOX, BOX);
    void doCollide(uint, uint);

    clsCannonball Balls[DEFINED_CANNONBALL_LIMIT]; /**< The is the array that holds all of the cannonballs. */
}
/*****************************************************************************/
/** The method of the collision (I just wanted to play around with some options.) */
enum Collisions {
    CollideElastic = 0, /**< This is normal collision in a perfect world without
                             friction, they hit and bounce off, no energy is lost. */
    CollideInelastic, /**< This is the normal way things collide, Balls collide and
                           bounce off of each other losing some energy. */
    CollidePerfectInelastic, /**< In this method the balls collide and then
                                  stick together. */
    CollideNone /**< No collisions, balls pass through each other. */
};
/*****************************************************************************/
namespace Global {
    /** Holds if build is in debug mode, this can happen if
        DEFINED_BUILD_MODE_PRIVATE is defined in the complier */
#ifdef DEFINED_BUILD_MODE_PRIVATE
    const bool blnDebugMode = true;
#else
    const bool blnDebugMode = false;
#endif

    clsConfig Config;

    /** Holds Values that are related the the physics of the world,
        these are all based on real numbers */
    namespace Physics {
        const uint uBallDensity = 7850; /**< Density of the ball in kg/m<sup>3</sup> (7850 is steel) */
        const float fGravity = -9.81; /**< Acceleration due to gravity in m/s<sup>2</sup> */
        const float fDragCofficient = 0.47; /**< Drag coefficient in the air. \n
                                                 Value is based on that of a sphere. \n
                                                 See https://en.wikipedia.org/wiki/Drag_coefficient for more info. */
        const float fKineticFriction = 0.36; /**< Kinetic Friction values based on Concrete and Steel */
        const float fDensityAir = 1.2041; /**< Density of air (in kg/m<sup>3</sup>)  \n
                                               This value is based on air at 20ºC and 101.325 kPa \n
                                               See https://en.wikipedia.org/wiki/Density_of_air for more info */
        const float fRecoil = -0.56; /**< The multiplier of the velocity when a ball hits the walls / floor */
        const float fMinVelocity = 0.0; /**< If a ball has less velocity than the it will "die" */
        const float fCoefficientRestitution = 0.76; /**< How much total energy remains after a collision,
                                                        (see https://en.wikipedia.org/wiki/Coefficient_of_restitution for more info) */
        const uchar CollisionMethod = CollideInelastic; /**< The collision method to use (see Collisions Enum) */
    } //end Namespace Physics

    /** Holds Values for different equations that are not physics related */
    namespace Equations {
        const float fVelocityScalar = 1; /**< Changing this effects the ratio
                                              of the line to the velocity when
                                              creating a cannonball */
        const float fTimetoSizeRatio = 0.0124; /**< One second of holding down
                                                    the mouse button = this many
                                                    meters for the ball */
        const float fMassAlphaRatio = 47.568; /**< The ratio between mass and its
                                                    alpha (transparent) value. \n
                                                    The equation used is Global::Equations::fMassAlphaRatio
                                                    * ln( clsCannonball::props.mass ) +
                                                    Global::Equations::fMassAlphaOffset */
        const float fMassAlphaOffset = 152.18; /**< The offset for the equation of
                                                    mass to alpha value \n
                                                    The equation used is Global::Equations::fMassAlphaRatio
                                                    * ln( clsCannonball::props.mass ) +
                                                    Global::Equations::fMassAlphaOffset */
        const uchar uAlphaMinimum = 30; /**< The lowest that the alpha value is allowed to be,
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


    Global::Config.Check(); //Load Config file and all its values.
    if (Global::Config.values.blnLogging) { //Open log file to clear it, if it exists
        FILE* logfile = fopen("logfile.log","w");
        fclose(logfile);
	} //end if logging

    if (Global::blnDebugMode) {printf("OS: %s\n",Global::Config.values.OperatingSystem);}
    clsTick Tick; //create tick object
    clsScreen CannonWindow; //Start the screen
	if ( !CannonWindow.getSDLStarted() ) {return 1;} //exit program if there was an error

    //Since all the Cannonballs will share the same SDL screen stuff place them all together
    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
        Cannonballs::Balls[i].setSDLScreen( CannonWindow.getBallTexture(),
                                           CannonWindow.getPixelTexture(),
                                           CannonWindow.getWindow(), i );
    } //end for each cannonball

    bool quit = false;
    bool holding = false;
    double tempdeltat;
    LOC OldMouse, CurrentMouse;
    SDL_Event event;
	do {
        CannonWindow.clearscreen(); //Clear the screen so new stuff can be drawn

        //poll events
        if (SDL_PollEvent( &event ) != 0) {
            if ( event.type == SDL_QUIT ) {quit = true;}
            else if ( event.type == SDL_MOUSEBUTTONUP ) {
                holding = false;
                if (Global::blnDebugMode) {printf("Mouse button up\n");}
                Cannonballs::addNew(CurrentMouse, OldMouse, Tick.stopHolding() );
            } else if ( event.type == SDL_MOUSEMOTION && holding) {
                //Draw the line
                SDL_GetMouseState(&CurrentMouse.x, &CurrentMouse.y );
                if (Global::blnDebugMode) {printf("Mouse found at (%d,%d)\n",CurrentMouse.x,CurrentMouse.y);}
            } else if ( event.type == SDL_MOUSEBUTTONDOWN ) {
                holding = true;
                Tick.startHolding();
                SDL_GetMouseState(&OldMouse.x, &OldMouse.y);
                CurrentMouse = OldMouse;
                if (Global::blnDebugMode) {printf("Mouse Button down at (%d,%d)\n",OldMouse.x,OldMouse.y);}
            } else if ( event.type == SDL_KEYDOWN ) {
                switch ( event.key.keysym.sym ) {
                case SDLK_k:
                    //kill all the balls
                    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++)
                        { Cannonballs::Balls[i].blnstarted = false;  }
                    break;
                case SDLK_q:
                case SDLK_ESCAPE:
                    //quit
                    quit = true;
                    break;
                case SDLK_r:
                    //stop all motion of balls
                    dblXY StopVel;
                    StopVel.x = StopVel.y = 0.0;
                    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++)
                        { Cannonballs::Balls[i].setVelocity(StopVel); }
                    break;
                } //end switch key
            } //end if event
        } //end if event
        if (holding) {CannonWindow.drawline(CurrentMouse, OldMouse);}
        //Update every ball
        tempdeltat = Tick.getTimeDifference();
		for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) { //Loop through each cannonball
            if (Cannonballs::Balls[i].blnstarted) { //only update cannonball if it is "alive"
                //Check for collisions if no collide is not on
                if (Global::Physics::CollisionMethod != CollideNone)
                    {Cannonballs::checkCollisons(i);}
                Cannonballs::Balls[i].update(tempdeltat);
            } //end if started
		} //end for loop

		CannonWindow.update(); //Update the screen
	} while (!quit); //keep looping until we get a quit
	return 0;
}
/*****************************************************************************/
void Cannonballs::addNew(LOC mouseC, LOC mouseO, double HoldTime ) {
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
    double radius = (double)Global::Equations::fTimetoSizeRatio * HoldTime;

    //Because y increasing is going down according to SDL we first negative the velocity
    fire_v = -1 * sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
    fire_v /= (double) Global::Equations::fVelocityScalar;

    //If the mouse if pointing straight up or straight down make the angle 90
    //Otherwise calculate the angle with atan.
    if (mouseC.x == mouseO.x) {
        angle = (PI / 2.0) * ( (mouseC.y > mouseO.y) ? -1.0 : 1.0 );
    } else {
        angle = (double) -1.0 * atan( (double)(mouseC.y - mouseO.y) /
                (double)(mouseC.x - mouseO.x) ) +
                (double)( (mouseC.x < mouseO.x) ? PI : 0.0 );
    } //end if x = x

    //mod mouse start, once again because the top left is 0,0 to SDL
    mouseO.y = Global::Config.values.uintScreenHeight - mouseO.y;

    //loop through array to find next available cannonball slot
    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
        if (!Cannonballs::Balls[i].blnstarted) {
            Cannonballs::Balls[i].setValues(radius, mouseO, fire_v, angle);
            return;
        } //end if not started
    } //end for cannonballs.

    //Since the program would have exited this function if a spot was open
    //below we tell the use they have to wait
    printf("You cannot add more cannonballs, please wait for some to decay.\n");
}
/*****************************************************************************/
void Cannonballs::checkCollisons(uint j) {

    /////////////////////////////////////////////////
    /// @brief Checks ball number j is colliding with another ball and then do collisions.
    ///
    /// @param  j = the number in the array that ball we are checking is.
    /// @return void (all changes if they are colliding is handled in this function).
    ///
    /////////////////////////////////////////////////

    BOX A, B;
    A = Cannonballs::Balls[j].getBOX();
    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
        if (Cannonballs::Balls[i].blnstarted && i != j) {
            B = Cannonballs::Balls[i].getBOX();
            if ( Cannonballs::checkOverlap(A, B) ) { Cannonballs::doCollide(j, i); }
        } //end if started and not same ball
    } //end for loop inner
}
/*****************************************************************************/
bool Cannonballs::checkOverlap(BOX A, BOX B) {
    /////////////////////////////////////////////////
    /// @brief Checks if two boxes overlap
    ///
    /// @param A = Box for ball A
    /// @param B = Box for ball B
    /// @return TRUE / FALSE if they overlap and therefore collide
    ///
    /////////////////////////////////////////////////

    if( A.bottom <= B.top ){return false;}
    if( A.top >= B.bottom ){return false;}
    if( A.right <= B.left ){return false;}
    if( A.left >= B.right ){return false;}
    //If none of the sides from A are outside B
    return true;
}
/*****************************************************************************/
void Cannonballs::doCollide(uint numA, uint numB) {
    /////////////////////////////////////////////////
    /// @brief Will calculate the new velocities of two balls that are colliding
    ///
    /// @param numA = the number in the array ball A is
    /// @param numB = the number in the array ball B is
    /// @return void (everything is handled inside the function)
    ///
    /////////////////////////////////////////////////

    dblXY Avel, Bvel;
    PP Aprops, Bprops;

    Avel = Cannonballs::Balls[numA].getVelocity();
    Bvel = Cannonballs::Balls[numB].getVelocity();
    Aprops = Cannonballs::Balls[numA].getPhysicalProps();
    Bprops = Cannonballs::Balls[numB].getPhysicalProps();

#ifdef DEFINED_USE_R2_VEL_MODDER
    //This part here has no actual basis on real life,
    //it is just my attempt at preventing the cannonballs from sticking together
    LOC CenterA, CenterB, DeltaCenters;
    double VelModder;
    CenterA = Cannonballs::Balls[numA].getplace();
    CenterB = Cannonballs::Balls[numB].getplace();

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

    if ( Global::Physics::CollisionMethod != CollidePerfectInelastic ) {
        //The equations for Perfect Inelastic is much simpler than if not
        //so I am handling those a bit differently to speed up that method.

        //Equations used can be found and explained here:
        //https://en.wikipedia.org/wiki/Elastic_collision
        Atotal_v = sqrt( pow(Avel.x,2) + pow(Avel.y,2) );
        Btotal_v = sqrt( pow(Bvel.x,2) + pow(Bvel.y,2) );

        //get the angle for both A and B
        if (Avel.x != 0.0) {Aangle = atan(Avel.y/Avel.x);}
        else {
            Aangle = (Avel.y >= 0.0) ? PI / 2.0 : -PI / 2.0;
        }

        if (Bvel.x != 0.0) {Bangle = atan(Bvel.y/Bvel.x);}
        else {
            Bangle = (Bvel.y >= 0.0) ? PI / 2.0 : -PI / 2.0;
        }

        //Adjust the angle to be the right one.
        //Since atan will only yield a number between -PI/2 and PI/2 we
        //have to adjust it if xvel is negative.
        Aangle += Avel.x < 0.0 ? PI : 0;
        Bangle += Bvel.x < 0.0 ? PI : 0;
        //The contact angle has to be the difference between the two angles but
        //since sometimes one or the other is negative, we'll use abs to ensure the right number
        ContactAngle = abs ( abs(Aangle) - abs(Bangle) );

        TotalAMomentum.x = Atotal_v * cos(Aangle - ContactAngle) * (Aprops.mass - Bprops.mass);
        TotalAMomentum.x += 2.0 * Bprops.mass * Btotal_v * cos(Bangle - ContactAngle);
        TotalAMomentum.x /= (Aprops.mass + Bprops.mass);
        //x and y formulas are the same until this point
        TotalAMomentum.y = TotalAMomentum.x;
        TotalAMomentum.x *= cos(ContactAngle);
        TotalAMomentum.y *= sin(ContactAngle);
        TotalAMomentum.x += Atotal_v * sin(Aangle - ContactAngle) * cos(ContactAngle + (PI/2.0) );
        TotalAMomentum.y += Atotal_v * cos(Aangle - ContactAngle) * cos(ContactAngle + (PI/2.0) );

        //Now do B
        TotalBMomentum.x = Btotal_v * cos(Bangle - ContactAngle) * (Bprops.mass - Aprops.mass);
        TotalBMomentum.x += 2.0 * Aprops.mass * Atotal_v * cos(Aangle - ContactAngle);
        TotalBMomentum.x /= (Aprops.mass + Bprops.mass);
        //x and y formulas are the same until this point
        TotalBMomentum.y = TotalBMomentum.x;
        TotalBMomentum.x *= cos(ContactAngle);
        TotalBMomentum.y *= sin(ContactAngle);
        TotalBMomentum.x += Btotal_v * sin(Bangle - ContactAngle) * cos(ContactAngle + (PI/2.0) );
        TotalBMomentum.y += Btotal_v * cos(Bangle - ContactAngle) * cos(ContactAngle + (PI/2.0) );
    } else {
        TotalAMomentum.x = Aprops.mass * Avel.x + Bprops.mass * Bvel.x;
        TotalAMomentum.y = Aprops.mass * Avel.y + Bprops.mass * Bvel.y;
    } //end if Perfect Inelastic or not

    switch (Global::Physics::CollisionMethod) { //figure out what to do based on the collision method
    case CollidePerfectInelastic:
        //The balls collide and stick together
        //Get the new mass.
        Aprops.mass += Bprops.mass;
        //Now we have to calculate the new radius, volume, and area so that drag will reflect the new size
        //we are of course assuming that density stays the same.
        Aprops.volume = (Aprops.mass / (double)Global::Physics::uBallDensity);
        //cbrt = cube root
        Aprops.radius = cbrt( (double) (3.0*Aprops.volume) / (double) (4.0*PI) );
        Aprops.area = (double) (2.0 * PI * pow(Aprops.radius, 2) );
        //Now calculate the new velocity
        Avel.x = TotalAMomentum.x / Aprops.mass;
        Avel.y = TotalAMomentum.y / Aprops.mass;
        //now "kill" cannonball B and update ball A
        Cannonballs::Balls[numB].blnstarted = false;
        Cannonballs::Balls[numA].setPhysicalProps(Aprops);
        Cannonballs::Balls[numA].setVelocity(Avel);
        break;
    case CollideInelastic:
        //uses the same equations as below but some energy is lost.
        TotalAMomentum.x *= (double)Global::Physics::fCoefficientRestitution;
        TotalAMomentum.y *= (double)Global::Physics::fCoefficientRestitution;
        TotalBMomentum.x *= (double)Global::Physics::fCoefficientRestitution;
        TotalBMomentum.y *= (double)Global::Physics::fCoefficientRestitution;

    case CollideElastic:
        //The balls collide and bounce away from each other

        //All of the heavy lifting is handled above.
        Cannonballs::Balls[numA].setVelocity(TotalAMomentum);
        Cannonballs::Balls[numB].setVelocity(TotalBMomentum);
        break;
    default: //the catch all and CollideNone
        //Nothing Happens!
        break;
    } //end switch collide method
} //end do Collide
/*****************************************************************************/
