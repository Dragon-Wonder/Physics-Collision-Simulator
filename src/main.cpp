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

    double tempradius;
    double tempAngle;
    uint tempVel;

    //Ask user for values.
    printf("Please enter radius for the sphere (floating point): ");
    scanf("%f",&tempradius);
    printf("\nPlease enter initial velocity (decimal): ");
    scanf("%d", &tempVel);
    printf("\nPlease enter angle of fire (in degrees): ");
    scanf("%f", &tempAngle);
    printf("\n");

    clsTick Tick; //create tick object
    clsCannonball Cannonball (tempradius, tempLoc, tempVel, tempAngle); //Create Cannonball object
    Cannonball.setdeltat(Tick.getdeltat()); //Set Cannonball deltat to the one in tick

    clsScreen CannonWindow; //Start the screen
	if (CannonWindow.bln_SDL_started == false) {return 1;}

	CannonWindow.updateBall(tempLoc); //Place ball at its starting point
	do {
		Cannonball.update(); //calc new ball values
		tempLoc = Cannonball.getPlace();
		CannonWindow.updateBall(tempLoc); //Update ball's location
		Tick.wait();
	} while (tempLoc.y > 0);
	printf("Finished.\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************************************/
