/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************************************/
#include "version.h"
#include "global.h"
#include "screen.h"
#include "cannonball.h"
#include "tick.h"
#include "config.h"
/**********************************************************************************************************************************************************************/
struct stcBox {
    uint left;
    uint right;
    uint top;
    uint bottom;
};
typedef struct stcBox BOX;
/**********************************************************************************************************************************************************************/
void addNewCannonball(LOC, LOC);
void checkForCollisons(uint);
bool checkCollide(BOX, BOX);
void doCollision(uint, uint);
BOX boxMaker(LOC);
/**********************************************************************************************************************************************************************/
enum Collisions { //The method of the collision (I just wanted to play around with some options.
    CollideElastic = 0, //This is the normal way things collide, they hit and bounce off, no energy is lost
    CollideInelastic, //balls collide and bounce off of each other losing some energy.
    CollidePerfectInelastic, //In this method the balls collide and then stick together
    CollideNone //No collisions
};
/**********************************************************************************************************************************************************************/
namespace Global {
#ifdef DEFINED_BUILD_MODE_PRIVATE
    const bool blnDebugMode = true;
#else
    const bool blnDebugMode = false;
#endif
    clsConfig Config;
    namespace Physics { //hold physics values in one place for future reference
        const uint uBallDensity = 7850; //Density of the ball in kg/m^3 (7850 is steel)
        const float fGravity = -9.81;
        const float fDragCofficient = 0.47;
        const float fDensityAir = 1.2754; //Density of air
        const float fRecoil = -0.56;
        const float fVelocityScalar = 1; //Changing this effects the fire velocity
        const float fMinVelocity = 0.5; //If a ball has less velocity than the it will "die"
        const float fMomentumLoss = 0.76; //How much total momentum stays after a collisions
        const uchar CollisionMethod = CollideElastic;
    }
}
/**********************************************************************************************************************************************************************/
//This is the maximum number of cannonballs which can be "alive" at a time
#define DEFINED_CANNONBALL_LIMIT 10
/**********************************************************************************************************************************************************************/
clsCannonball Cannonballs[DEFINED_CANNONBALL_LIMIT];
WINATT tempwin;
SDL_Texture* tempball;
/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[]) {
    Global::Config.Check(); //Load Config file and all its values.

    if (Global::blnDebugMode) {printf("OS: %s\n",Global::Config.values.OperatingSystem);}
    clsTick Tick; //create tick object
    clsScreen CannonWindow; //Start the screen
	if ( !CannonWindow.getSDLStarted() ) {return 1;} //exit program if there was an error
	//Place get needed values from screen
    tempwin = CannonWindow.getWindow();
    tempball = CannonWindow.getBallTexture();

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
                addNewCannonball(CurrentMouse, OldMouse);
            } else if ( event.type == SDL_MOUSEMOTION && holding) {
                //Draw the line
                SDL_GetMouseState(&CurrentMouse.x, &CurrentMouse.y );
                if (Global::blnDebugMode) {printf("Mouse found at (%d,%d)\n",CurrentMouse.x,CurrentMouse.y);}
            } else if ( event.type == SDL_MOUSEBUTTONDOWN ) {
                holding = true;
                SDL_GetMouseState(&OldMouse.x, &OldMouse.y);
                CurrentMouse = OldMouse;
                if (Global::blnDebugMode) {printf("Mouse Button down at (%d,%d)\n",OldMouse.x,OldMouse.y);}
            } //end check of event
        } //end if event
        if (holding) {CannonWindow.drawline(CurrentMouse, OldMouse);}
        //Update every ball
        tempdeltat = Tick.getTimeDifference();

		for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) { //Loop through each cannonball
            if (Cannonballs[i].blnstarted) { //only update cannonball if it is "alive"
                Cannonballs[i].update(tempdeltat);
                //Check for collisions if no collide is not on
                if (Global::Physics::CollisionMethod != CollideNone) {checkForCollisons(i);}
            } //end if started
		} //end for loop

		CannonWindow.update(); //Update the screen
	} while (!quit);
	return 0;
}
/**********************************************************************************************************************************************************************/
void addNewCannonball(LOC mouseC, LOC mouseO ) {
    //Get location, vel, and angle
    double fire_v;
    double angle;

    //Because y increasing is going down according to SDL we first negative the velocity
    fire_v = -1 * sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
    fire_v /= (double) Global::Physics::fVelocityScalar;

    //if the mouse if pointing straight up or straight down make the angle 90
    //Other calculate the angle with atan.
    if (mouseC.x == mouseO.x) {
        if (mouseC.y > mouseO.y) {angle = -90.0;}
        else if (mouseC.y < mouseO.y) {angle = 90.0;}
        else {angle = 0;}
    } else {
        angle = (double)-1.0 * atan( (mouseC.y - mouseO.y) / (mouseC.x - mouseO.x) );
        if (mouseC.x < mouseO.x) {angle += (double)PI;}
        angle *= (double)(180/PI);
    } //end if x = x

    //mod mouse start, once again because the top left is 0,0 to SDL
    mouseO.y = tempwin.height - mouseO.y;

    //loop through array to find next available cannonball slot
    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
        if (!Cannonballs[i].blnstarted) {
            Cannonballs[i].setValues(2.0, mouseO, fire_v, angle);
            Cannonballs[i].setSDLScreen(tempball, tempwin);
            return;
        } //end if not started
    } //end for cannonballs.

    //since the program would have exited this function if a spot was open
    //below we tell the use they have to wait
    printf("You cannot add more cannonballs, please wait for some to decay.\n");
}
/**********************************************************************************************************************************************************************/
void checkForCollisons(uint j) { //Checks every cannonball against every other cannonball to see if they collide
    BOX A, B;
    A = boxMaker( Cannonballs[j].getplace() );
    for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
        if (Cannonballs[i].blnstarted && i != j) {
            B = boxMaker( Cannonballs[i].getplace() );
            if ( checkCollide(A, B) ) { doCollision(j, i); }
        } //end if started and not same ball
    } //end for loop inner
}
/**********************************************************************************************************************************************************************/
bool checkCollide(BOX A, BOX B) { //checks if two objects (made with the BOXES collide)
    if( A.bottom <= B.top ){return false;}
    if( A.top >= B.bottom ){return false;}
    if( A.right <= B.left ){return false;}
    if( A.left >= B.right ){return false;}
    //If none of the sides from A are outside B
    return true;
}
/**********************************************************************************************************************************************************************/
void doCollision(uint numA, uint numB) {
    dblXY Avel, Bvel, NewVel;
    PP Aprops, Bprops;

    Avel = Cannonballs[numA].getVelocity();
    Bvel = Cannonballs[numB].getVelocity();
    Aprops = Cannonballs[numA].getPhysicalProps();
    Bprops = Cannonballs[numB].getPhysicalProps();

    double Aangle, Bangle, ContactAngle;
    double Atotal_v, Btotal_v;

    dblXY TotalAMomentum, TotalBMomentum;

    if ( Global::Physics::CollisionMethod != CollidePerfectInelastic ) {
        //The equations for Perfect Inelastic is much simpler than if not
        //so I am handling those a bit differently to speed up that method.

        //Equations used can be found and explained here: https://en.wikipedia.org/wiki/Elastic_collision
        Atotal_v = sqrt( pow(Avel.x,2) + pow(Avel.y,2) );
        Btotal_v = sqrt( pow(Bvel.x,2) + pow(Bvel.y,2) );

        //get the angle for both A and B
        if (Avel.x != 0.0) {Aangle = atan(Avel.y/Avel.x);}
        else {
            if (Avel.y > 0.0) {Aangle = PI / 2.0;}
            else if (Avel.y < 0.0) {Aangle = - PI / 2.0;}
            else {Aangle = 0.0;}
        }

        if (Bvel.x != 0.0) {Bangle = atan(Bvel.y/Bvel.x);}
        else {
            if (Bvel.y > 0.0) {Bangle = PI / 2.0;}
            else if (Bvel.y < 0.0) {Bangle = - PI / 2.0;}
            else {Bangle = 0.0;}
        }

        //Adjust the angle to be the right one.
        //Since atan will only yield a number between -PI/2 and PI/2 we
        //have to adjust it if xvel is negative.
        if (Avel.x < 0.0) {Aangle += PI;}
        if (Bvel.x < 0.0) {Bangle += PI;}
        ContactAngle = Aangle + Bangle;

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
        Cannonballs[numB].blnstarted = false;
        Cannonballs[numA].setPhysicalProps(Aprops);
        Cannonballs[numA].setVelocity(Avel);
        break;
    case CollideInelastic:
        //uses the same equations as below but some energy is lost.
        TotalAMomentum.x *= (double)Global::Physics::fMomentumLoss;
        TotalAMomentum.y *= (double)Global::Physics::fMomentumLoss;
        TotalBMomentum.x *= (double)Global::Physics::fMomentumLoss;
        TotalBMomentum.y *= (double)Global::Physics::fMomentumLoss;

    case CollideElastic:
        //The balls collide and bounce away from each other

        //All of the heavy lifting is handled above.
        Cannonballs[numA].setVelocity(TotalAMomentum);
        Cannonballs[numB].setVelocity(TotalBMomentum);
        break;
    default: //the catch all and CollideNone
        //Nothing Happens!
        break;
    } //end switch collide method
}
/**********************************************************************************************************************************************************************/
BOX boxMaker(LOC place) {
    BOX tempBOX;
    tempBOX.left = place.x;
    tempBOX.top = place.y;
    tempBOX.bottom = tempBOX.top + 10;
    tempBOX.right = tempBOX.left + 10;
    return tempBOX;
}
/**********************************************************************************************************************************************************************/
