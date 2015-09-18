/**********************************************************************************************************************************************/
#include "cannonball.h"
#include "screen.h"
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
    acc.x = 0.0;
    acc.y = Global::Physics::fGravity;
    if (vel.x != 0.0 && vel.y != 0.0 && props.mass != 0.0) {
        double flow_velocity = sqrt(pow(vel.x,2) + pow(vel.y,2));
        double Drag_Force = (double) (0.5 * Global::Physics::fDensityAir * flow_velocity * Global::Physics::fDragCofficient * props.area);
        double Drag_Acc = (double) (Drag_Force / props.mass);
        double angle;

        angle = (vel.x != 0.0) ? atan(vel.y / vel.x) : PI / 2.0;
        angle += vel.x < 0.0 ? PI : 0.0;

        acc.x += Drag_Acc * cos (angle);
        acc.y += Drag_Acc * sin (angle);
        //Please recall fGravity is negative
        double Friction = (double)Global::Physics::fKineticFriction * (double)Global::Physics::fGravity;

        //Update acc for Friction values
        if ( dblLOC.y <= Screen_place.h || dblLOC.y >= window.height ) {
            //Ball is in contact with floor or ceiling update x acc
            acc.x += Friction * (vel.x < 0.0 ? 1.0 : -1.0);
        }

        if ( dblLOC.x <= 0.0 || dblLOC.x >= window.width - Screen_place.w ) {
            //Ball is in contact with the wall update y acc.
            acc.y += Friction * (vel.y < 0.0 ? 1.0 : -1.0);
        }
    } //end if things don't equal 0
}
/**********************************************************************************************************************************************/
void clsCannonball::update(double newdeltat) {
    deltat = newdeltat;

    if (blnDragEnabled) {Drag_updateacc();}

    dblLOC.x = dblLOC.x + vel.x * deltat + 0.5 * acc.x * pow(deltat,2);
	vel.x = (vel.x + acc.x * deltat);
	if (dblLOC.x <= 0.0 || dblLOC.x >= window.width - Screen_place.w) {
        vel.x *= Global::Physics::fRecoil;
        dblLOC.x += dblLOC.x <= 0.0 ? 1.0 : -1.0;
    } //end if hitting x edges

	dblLOC.y = dblLOC.y + vel.y * deltat + 0.5 * acc.y * pow(deltat,2);
	vel.y = (vel.y + acc.y * deltat);
	if (dblLOC.y <= Screen_place.h || dblLOC.y >= window.height) {
        vel.y *= Global::Physics::fRecoil;
        dblLOC.y += dblLOC.y <= Screen_place.y ? 1.0 : -1.0;
    }//end if hitting y edges

    place.x = dblLOC.x < 0.0 ? 0 : round(dblLOC.x);
    place.y = dblLOC.y < 0.0 ? 0 : round(dblLOC.y);

	if (Global::Config.values.blnLogging) {
        FILE* logfile = fopen("logfile.log","a");
        fprintf(logfile,"Ball %3u \t (%.3f, %.3f)\n",ballID, dblLOC.x,dblLOC.y);
        fclose(logfile);
	}

	//Update the collision box for the new location
	CollisionBox.left = place.x;
	CollisionBox.top = window.height - place.y;
	CollisionBox.right = CollisionBox.left + Screen_place.w;
	CollisionBox.bottom = CollisionBox.top + Screen_place.h;

    double total_v;
    total_v = sqrt( pow(vel.x,2) + pow(vel.y,2) );
    if (total_v < Global::Physics::fMinVelocity || isnan(total_v) ) {
        blnstarted = false;
        if (Global::blnDebugMode) {
            if ( isnan(total_v) ) {printf("Ball vel is NaN; killing it\n");}
            else {printf("Ball moving too slow; killing it\n");}
        } //end if debug mode
    } //end if should kill
	show();
}
/**********************************************************************************************************************************************/
void clsCannonball::setSDLScreen(SDL_Texture* SDLball, WINATT SDLwindow) {
    ball = SDLball;
    SDL_QueryTexture(ball,NULL,NULL, &Screen_place.w, &Screen_place.h);
    window = SDLwindow;
}
/**********************************************************************************************************************************************/
void clsCannonball::show() {
    Screen_place.x = place.x;
    Screen_place.y = window.height - place.y;
    //Place the ball
    SDL_RenderCopy(window.ren,ball,NULL,&Screen_place);
}
/**********************************************************************************************************************************************/
void clsCannonball::setValues(double r, LOC init_place, double init_vel, double init_angle, uint ID) {
    props.radius = r; //in meters

    acc.x = 0.00;
    acc.y = Global::Physics::fGravity;

    place = init_place;
    dblLOC.x = (double) place.x;
    dblLOC.y = (double) place.y;

    vel.x = (double)(init_vel) * (cos(init_angle));
	vel.y = (double)(init_vel) * (sin(init_angle));

	ballID = ID;
	blnstarted = true;

    Drag_calcvalues();
	if (Global::Config.values.blnDragMode) { enableDrag(); }
}
/**********************************************************************************************************************************************/
LOC clsCannonball::getplace() {
    return place;
}
/**********************************************************************************************************************************************/
dblXY clsCannonball::getVelocity() {
    return vel;
}
/**********************************************************************************************************************************************/
void clsCannonball::setVelocity(dblXY newvel) {
    vel.x = newvel.x;
    vel.y = newvel.y;
}
/**********************************************************************************************************************************************/
PP clsCannonball::getPhysicalProps() {
    return props;
}
/**********************************************************************************************************************************************/
void clsCannonball::setplace(LOC newplace) {
    dblLOC.x = newplace.x;
    dblLOC.y = newplace.y;
}
/**********************************************************************************************************************************************/
void clsCannonball::setPhysicalProps(PP newprops) {
    props = newprops;
}
/**********************************************************************************************************************************************/
BOX clsCannonball::getBOX() {
    return CollisionBox;
}
/**********************************************************************************************************************************************/
