/**********************************************************************************************************************************************/
#include "cannonball.h"
#include "screen.h"
/**********************************************************************************************************************************************/
#define PI 3.1415926535897
/**********************************************************************************************************************************************/
//Constants
const double Rad_Convert = PI / 180.0;
/**********************************************************************************************************************************************/
clsCannonball::clsCannonball() {
    //Put some default values in; on the off chance I forget to set them later
    blnDragEnabled = false;
    blnstarted = false;
    deltat = (1.00 / 60.00) ;
    acc.x = 0.00;
    acc.y = Global::Physics::fGravity;

    props.radius = 5.0; //in meters
    props.density = Global::Physics::uBallDensity; //density of steel in kg/m^3

    place.x = 0;
    place.y = 0;
    dblLOC.x = (double) place.x;
    dblLOC.y = (double) place.y;

    vel.x = 0;
	vel.y = 0;

   if (Global::blnDebugMode) {printf("Ball created.\n");}
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
	double flow_velocity = sqrt(pow(vel.x,2) + pow(vel.y,2));
	double Drag_Force = (double) (0.5 * Global::Physics::fDensityAir * flow_velocity * Global::Physics::fDragCofficient * props.area);
	double Drag_Acc = (double) (Drag_Force / props.mass);

	double angle = atan(vel.y / vel.x);

	acc.x -= Drag_Acc * cos (angle);
	if (vel.y < 0) {acc.y += Drag_Acc * sin (angle);}
	else {acc.y -= Drag_Acc * sin (angle);}
}
/**********************************************************************************************************************************************/
void clsCannonball::update(double newdeltat) {
    deltat = newdeltat;
    SDL_Rect dst;
    SDL_QueryTexture(ball,NULL,NULL, &dst.w, &dst.h);

    if (blnDragEnabled) {Drag_updateacc();}

    dblLOC.x = dblLOC.x + vel.x * deltat + 0.5 * acc.x * pow(deltat,2);
	vel.x = (vel.x + acc.x * deltat);
	if (dblLOC.x <= 0.0 || dblLOC.x >= window.width - dst.w) {
            vel.x *= Global::Physics::fRecoil;
            if (dblLOC.x <= 0.0) {dblLOC.x++;}
            else {dblLOC.x--;}
    } //end if hitting x edges
	dblLOC.y = dblLOC.y + vel.y * deltat + 0.5 * acc.y * pow(deltat,2);
	vel.y = (vel.y + acc.y * deltat);
	if (dblLOC.y <= dst.h || dblLOC.y >= window.height) {
            vel.y *= Global::Physics::fRecoil;
            if (dblLOC.y <= dst.h) {dblLOC.y++;}
            else {dblLOC.y--;}
    }//end if hitting y edges

    if (dblLOC.x < 0.0) {place.x = 0;}
    else {place.x = round(dblLOC.x);}

    if (dblLOC.y < 0.0) {place.y = 0;}
    else {place.y = round(dblLOC.y);}

	//if (Global::blnDebugMode) {printf("Ball updated, new position (%f, %f)\n",dblLOC.x,dblLOC.y);}
	if (Global::Config.values.blnLogging) {
        FILE* logfile = fopen("logfile.log","a");
        fprintf(logfile,"(%.5f, %.5f)\n",dblLOC.x,dblLOC.y);
        fclose(logfile);
	}

    double total_v;
    total_v = sqrt( pow(vel.x,2) + pow(vel.y,2) );
    if (total_v < Global::Physics::fMinVelocity) {blnstarted = false;}
	show();
}
/**********************************************************************************************************************************************/
void clsCannonball::setSDLScreen(SDL_Texture* SDLball, WINATT SDLwindow) {
    ball = SDLball;
    window = SDLwindow;
}
/**********************************************************************************************************************************************/
void clsCannonball::show() {
    SDL_Rect dst;
    SDL_QueryTexture(ball,NULL,NULL, &dst.w, &dst.h);
    dst.x = place.x;
    dst.y = window.height - place.y;
    //Place the ball
    SDL_RenderCopy(window.ren,ball,NULL,&dst);
}
/**********************************************************************************************************************************************/
void clsCannonball::setValues(double r, LOC init_place, double init_vel, double init_angle) {
    props.radius = r; //in meters
    props.density = Global::Physics::uBallDensity; //density of steel in kg/m^3

    place = init_place;
    dblLOC.x = (double) place.x;
    dblLOC.y = (double) place.y;

    vel.x = (double)(init_vel) * (cos(init_angle * Rad_Convert));
	vel.y = (double)(init_vel) * (sin(init_angle * Rad_Convert));
	blnstarted = true;

	if (Global::Config.values.blnDragMode) { enableDrag(); }
}
/**********************************************************************************************************************************************/

