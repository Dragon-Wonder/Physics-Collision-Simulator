/**********************************************************************************************************************************************************************/
//#include <cstdio.h>
#include <cstdlib>
#include <math.h>
#include <time.h>
/**********************************************************************************************************************************************************************/
#include "version.h"
#include "screen.h"
#include "tick.h"
/**********************************************************************************************************************************************************************/
//Defines
#define PI 3.1415926535897
/**********************************************************************************************************************************************************************/
//Constants
const double Rad_Convert = PI / 180.0;
const double Drag_cofficient = 0.47;
/**********************************************************************************************************************************************************************/
//Structs and type defs
typedef unsigned char uchar;
typedef unsigned int uint;
struct stcball {
	uint x;
	uint y;
	double dbl_x;
	double dbl_y;
	double velx;
	double vely;
	double area;
	double mass;
};
typedef struct stcball cball;
/**********************************************************************************************************************************************************************/
//Gloabls
cball cannonball;
double acc_y = -9.81;
double acc_x = 0.00;
double deltat = 0.00;
/**********************************************************************************************************************************************************************/
//Protytpe Functions
namespace Ball {
	void make(void);
	void update(void);

	const uchar radius = 5;
	const uchar fire_angle = 45;
	const uint fire_velocity = 68;
};
namespace Drag {
    const bool blnDoDrag = false;
	void calc_values(void);
	void update_acc(void);
};
/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[]) {

    clsTick Tick;
    deltat = Tick.getdeltat();

	Ball::make();
    clsScreen CannonWindow; //Start the screen
	if (CannonWindow.bln_SDL_started == false) {return 1;}
	CannonWindow.updateBall(cannonball.x,cannonball.y); //Place ball at its starting point
    if (Drag::blnDoDrag) {Drag::calc_values();} //get values needed for drag calcs
	do {
        if (Drag::blnDoDrag) {Drag::update_acc();} //if drag enabled calc it.
		Ball::update(); //calc new ball values
		CannonWindow.updateBall(cannonball.x,cannonball.y); //Update ball's location
		Tick.wait();
	} while (cannonball.y > 0);
	printf("Finished.\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************************************/
void Ball::make(void) {
	cannonball.x = 0;
	cannonball.y = 5;
	cannonball.dbl_x = 0.0;
	cannonball.dbl_y = 5.0;
	cannonball.velx = (double)(Ball::fire_velocity) * (cos(Ball::fire_angle * Rad_Convert));
	cannonball.vely = (double)(Ball::fire_velocity) * (cos(Ball::fire_angle * Rad_Convert));
	printf("Ball created.\n");
}
/**********************************************************************************************************************************************************************/
void Ball::update(void) {
	cannonball.dbl_x = cannonball.dbl_x + cannonball.velx * deltat + 0.5 * acc_x * pow(deltat,2);
	cannonball.velx = (cannonball.velx + acc_x * deltat);
	cannonball.dbl_y = cannonball.dbl_y + cannonball.vely * deltat + 0.5 * acc_y * pow(deltat,2);
	cannonball.vely = (cannonball.vely + acc_y * deltat);

	cannonball.x = round(cannonball.dbl_x);
	cannonball.y = round(cannonball.dbl_y);
	printf("Ball updated, new position (%d, %d)\n",cannonball.x,cannonball.y);
}
/**********************************************************************************************************************************************************************/
void Drag::calc_values(void) {
	double density = 2.1546;
	double volume;
	cannonball.area = (double) (2.0 * PI * pow(Ball::radius,2));
	volume = (double) ((4.0/3.0) * PI * pow(Ball::radius,3));
	cannonball.mass = density * volume;
}
/**********************************************************************************************************************************************************************/
void Drag::update_acc(void) {
	double rho_air = 1.2754;
	double flow_velocity = pow(cannonball.velx,2) + pow(cannonball.vely,2);
	double Drag_Force = (double) (0.5 * rho_air * flow_velocity * Drag_cofficient * cannonball.area);
	double Drag_Acc = (double) (Drag_Force / cannonball.mass);

	double angle = atan(cannonball.vely / cannonball.velx);

	acc_x -= Drag_Acc * cos (angle);
	if (cannonball.vely < 0) {acc_y += Drag_Acc * sin (angle);}
	else {acc_y -= Drag_Acc * sin (angle);}
}
/**********************************************************************************************************************************************************************/

