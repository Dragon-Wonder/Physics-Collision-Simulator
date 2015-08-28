/**********************************************************************************************************************************************/
#include "cannonball.h"
/**********************************************************************************************************************************************/
#define PI 3.1415926535897
/**********************************************************************************************************************************************************************/
//Constants
const double Rad_Convert = PI / 180.0;
const double Drag_cofficient = 0.47;
/**********************************************************************************************************************************************/
clsCannonball::clsCannonball(double r, LOC init_place, uint init_vel, double init_angle) {
    //Put some default values in; on the off chance I forget to set them later
    blnDragEnabled = false;
    deltat = (1.00 / 60.00) ;
    acc.x = 0.00;
    acc.y = -9.81;

    props.radius = r; //in meters
    props.density = 7850; //density of steel in kg/m^3

    place = init_place;
    dblLOC.x = (double) place.x;
    dblLOC.y = (double) place.y;

    vel.x = (double)(init_vel) * (cos(init_angle * Rad_Convert));
	vel.y = (double)(init_vel) * (sin(init_angle * Rad_Convert));

   if (Global::blnDebugMode) {printf("Ball created.\n");}
}
/**********************************************************************************************************************************************/
void clsCannonball::setdeltat(double value) {
    deltat = value;
}
/**********************************************************************************************************************************************/
void clsCannonball::enableDrag(void) {
    blnDragEnabled = true;
    Drag_calcvalues();
    if (Global::blnDebugMode) {printf("Drag enabled.\n");}
}
/**********************************************************************************************************************************************/
void clsCannonball::Drag_calcvalues(void) {
	props.area = (double) (2.0 * PI * pow(props.radius,2));
	props.volume = (double) ((4.0/3.0) * PI * pow(props.radius,3));
	props.mass = props.density * props.volume;
}
/**********************************************************************************************************************************************/
void clsCannonball::Drag_updateacc(void) {
    double rho_air = 1.2754; //density of air
	double flow_velocity = sqrt(pow(vel.x,2) + pow(vel.y,2));
	double Drag_Force = (double) (0.5 * rho_air * flow_velocity * Drag_cofficient * props.area);
	double Drag_Acc = (double) (Drag_Force / props.mass);

	double angle = atan(vel.y / vel.x);

	acc.x -= Drag_Acc * cos (angle);
	if (vel.y < 0) {acc.y += Drag_Acc * sin (angle);}
	else {acc.y -= Drag_Acc * sin (angle);}
}
/**********************************************************************************************************************************************/
LOC clsCannonball::getPlace(void) {
    return place;
}
/**********************************************************************************************************************************************/
void clsCannonball::update(void) {
    if (blnDragEnabled) {Drag_updateacc();}

    dblLOC.x = dblLOC.x + vel.x * deltat + 0.5 * acc.x * pow(deltat,2);
	vel.x = (vel.x + acc.x * deltat);
	dblLOC.y = dblLOC.y + vel.y * deltat + 0.5 * acc.y * pow(deltat,2);
	vel.y = (vel.y + acc.y * deltat);

    if (dblLOC.x < 0.0) {place.x = 0;}
    else {place.x = round(dblLOC.x);}

    if (dblLOC.y < 0.0) {place.y = 0;}
    else {place.y = round(dblLOC.y);}

	if (Global::blnDebugMode) {printf("Ball updated, new position (%f, %f)\n",dblLOC.x,dblLOC.y);}
	if (Global::Config.values.blnLogging) {
        FILE* logfile = fopen("logfile.log","a");
        fprintf(logfile,"new position (%d, %d)\n",place.x,place.y);
        fclose(logfile);
	}
}
/**********************************************************************************************************************************************/
