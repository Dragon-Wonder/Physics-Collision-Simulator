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
void addNewCannonball(LOC, LOC);
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
        const float fVelocityScalar = 1;
        const float fMinVelocity = 15; //If a ball has less velocity than the it will "die"
        const float fMomentumLoss = 0.89; //How much total momentum stays after a collisions
    }
}
/**********************************************************************************************************************************************************************/
/* TODO (GamerMan7799#1#): Figure out how to detect the edges of the images */
/* TODO (GamerMan7799#1#): Get Highest height and Distance from fire at end of run. */
/**********************************************************************************************************************************************************************/
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
        CannonWindow.clearscreen();
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
                if (Global::blnDebugMode) {printf("Mouse Button down at (%d,%d)\n",OldMouse.x,OldMouse.y);}
            } //end check of event
        } //end if event
        if (holding) {CannonWindow.drawline(CurrentMouse, OldMouse);}
        //Update every ball
        tempdeltat = Tick.getTimeDifference();
		for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
            if (Cannonballs[i].blnstarted) {
                Cannonballs[i].update(tempdeltat);
            } //end if started
		} //end for loop

		CannonWindow.update();
	} while (!quit);
	return 0;
}
/**********************************************************************************************************************************************************************/
void addNewCannonball(LOC mouseC, LOC mouseO ) {
    //Get location, vel, and angle
    double fire_v;
    double angle;

    fire_v = sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
    fire_v /= Global::Physics::fVelocityScalar;

    if (mouseC.x == mouseO.x) {
        if (mouseC.y > mouseO.y) {angle = -90.0;}
        else if (mouseC.y < mouseO.y) {angle = 90.0;}
        else {angle = 0;}
    } else {
        angle = atan( (mouseC.y - mouseO.y) / (mouseC.x - mouseO.x) );
        if (mouseC.x < mouseO.x) {angle = -1 * angle;}
        angle *= (180/3.1415926535);
    } //end if x = x

    //mod mouse start
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
void checkCollide(LOC place1, LOC place2) {

}
/**********************************************************************************************************************************************************************/
void doCollision(uint num1, uint num2) {

}
/**********************************************************************************************************************************************************************/
