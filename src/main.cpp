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
        const float fMinVelocity = 0.005; //If a ball has less velocity than the it will "die"
        const float fMomentumLoss = 1; //How much total momentum stays after a collisions
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
                CurrentMouse = OldMouse;
                if (Global::blnDebugMode) {printf("Mouse Button down at (%d,%d)\n",OldMouse.x,OldMouse.y);}
            } //end check of event
        } //end if event
        if (holding) {CannonWindow.drawline(CurrentMouse, OldMouse);}
        //Update every ball
        tempdeltat = Tick.getTimeDifference();
		for (uint i = 0; i < DEFINED_CANNONBALL_LIMIT; i++) {
            if (Cannonballs[i].blnstarted) {
                Cannonballs[i].update(tempdeltat);
                checkForCollisons(i);
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

    fire_v = -1 * sqrt( pow(mouseC.x - mouseO.x, 2) + pow(mouseC.y - mouseO.y, 2) );
    fire_v /= (double) Global::Physics::fVelocityScalar;

    if (mouseC.x == mouseO.x) {
        if (mouseC.y > mouseO.y) {angle = -90.0;}
        else if (mouseC.y < mouseO.y) {angle = 90.0;}
        else {angle = 0;}
    } else {
        angle = (double)-1.0 * atan( (mouseC.y - mouseO.y) / (mouseC.x - mouseO.x) );
        angle *= (double)(180/3.1415926535);
        if (mouseC.x < mouseO.x) {angle *= (double)-1.0;}
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
    double Amass, Bmass;
    Avel = Cannonballs[numA].getVelocity();
    Bvel = Cannonballs[numB].getVelocity();
    Amass = Cannonballs[numA].getmass();
    Bmass = Cannonballs[numB].getmass();

    double totalMomX = Amass * Avel.x - Bmass * Bvel.x;
    double totalMomY = Amass * Avel.y - Bmass * Bvel.y;
    totalMomX *= (double)Global::Physics::fMomentumLoss;
    totalMomY *= (double)Global::Physics::fMomentumLoss;

    //I know that this is not a true Momentum equation
    //I'm just trying to get something up to test things out.
    NewVel.x = totalMomX / (Amass + Bmass);
    NewVel.y = totalMomY / (Amass + Bmass);

    Cannonballs[numA].setVelocity(NewVel);
    NewVel.y *= -1;
    NewVel.x *= -1;
    Cannonballs[numB].setVelocity(NewVel);


    /*LOC Aplace = Cannonballs[numA].getplace();
    Aplace.x ++;
    Aplace.y --;
    Cannonballs[numA].setplace(Aplace);*/
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
