/**********************************************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************************************/
#include "version.h"
#include "screen.h"
#include "cannonball.h"
#include "tick.h"
#include "config.h"
/**********************************************************************************************************************************************************************/
namespace Global {
#ifdef DEFINED_BUILD_MODE_PRIVATE
    const bool blnDebugMode = true;
#else
    const bool blnDebugMode = false;
#endif
    clsConfig Config;
}
/**********************************************************************************************************************************************************************/
/* TODO (GamerMan7799#1#): Allow changing starting points */
/* TODO (GamerMan7799#1#): Figure out how to detect the edges of the images */
/* TODO (GamerMan7799#9#): Multiple cannonballs ??!?? */
/* TODO (GamerMan7799#1#): Get Highest height and Distance from fire at end of run. */
/**********************************************************************************************************************************************************************/
//Structs and type defs
typedef unsigned char uchar;
typedef unsigned int uint;
/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[]) {
    Global::Config.Check(); //Load Config file and all its values.

    if (Global::blnDebugMode) {printf("OS: %s\n",Global::Config.values.OperatingSystem);}

    LOC tempLoc;
    tempLoc.x = 0;
    tempLoc.y = 5;

    uint tempradius;
    uint tempAngle;
    uint tempVel;
    uint tempBln;

    //Ask user for values.
    printf("Please enter radius for the sphere (meters): ");
    scanf("%4d",&tempradius);
    printf("Please enter initial velocity (m/s): ");
    scanf("%4d", &tempVel);
    printf("Please enter angle of fire (°): ");
    scanf("%d", &tempAngle);
    printf("Would you like to enable drag (experimental)? (1 = yes, 0 = no): ");
    scanf("%d", &tempBln);


    clsTick Tick; //create tick object
    clsCannonball Cannonball (tempradius, tempLoc, (double)tempVel, (double)tempAngle); //Create Cannonball object
    Cannonball.setdeltat(Tick.getTimeDifference());

    if (tempBln == 1) {Cannonball.enableDrag();}

    clsScreen CannonWindow(tempradius); //Start the screen
	if (CannonWindow.bln_SDL_started == false) {return 1;}

	CannonWindow.updateBall(tempLoc); //Place ball at its starting point
	do {
        Cannonball.setdeltat(Tick.getTimeDifference());
		Cannonball.update(); //calc new ball values
		tempLoc = Cannonball.getPlace();
		CannonWindow.updateBall(Cannonball.getPlace()); //Update ball's location
		//Tick.wait();
	} while (tempLoc.y > 0);
	printf("Finished.\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************************************/
