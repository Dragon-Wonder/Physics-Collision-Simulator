/**********************************************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************************************/
#include "version.h"
#include "screen.h"
#include "cannonball.h"
#include "tick.h"
/**********************************************************************************************************************************************************************/
//Structs and type defs
typedef unsigned char uchar;
typedef unsigned int uint;
/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[]) {
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
    Cannonball.setdeltat(Tick.getdeltat()); //Set Cannonball deltat to the one in tick

    if (tempBln == 1) {Cannonball.enableDrag();}

    clsScreen CannonWindow; //Start the screen
	if (CannonWindow.bln_SDL_started == false) {return 1;}

	CannonWindow.updateBall(tempLoc); //Place ball at its starting point
	do {
		Cannonball.update(); //calc new ball values
		tempLoc = Cannonball.getPlace();
		CannonWindow.updateBall(Cannonball.getPlace()); //Update ball's location
		Tick.wait();
	} while (tempLoc.y > 0);
	printf("Finished.\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************************************/
