/**********************************************************************************************************************************************/
#include "cannonball.h"
#include "screen.h"
/**********************************************************************************************************************************************/
clsCannonball::clsCannonball() {
    /////////////////////////////////////////////////
    /// @brief The default Constructor for the cannonballs. It has to call
    ///         the values as default so we can create it in the array. The values are changed
    ///         later. The default values are as follows:
    ///         * deltat = 1 / 60 (for 60 fps)
    ///         * acc.x = 0
    ///         * acc.y = Global::Physics::fGravity
    ///         * props.radius = 5
    ///         * props.density = Global::Physics::uBallDensity
    ///         * place = 0
    ///         * dblLoc = place casted as a double
    ///         * Vel = 0
    /////////////////////////////////////////////////

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
void clsCannonball::Drag_calcvalues(void) {
    /////////////////////////////////////////////////
    /// @brief This will calculate addionational properties of the cannonball based on its radius.
    ///        These include:
    ///        * area
    ///        * volume
    ///        * mass
    ///
    /// @param void
    /// @return void
    ///
    /////////////////////////////////////////////////

	props.area = (double) (2.0 * PI * pow(props.radius,2));
	props.volume = (double) ((4.0/3.0) * PI * pow(props.radius,3));
	props.mass = props.density * props.volume;
}
/**********************************************************************************************************************************************/
void clsCannonball::Drag_updateacc(void) {
    /////////////////////////////////////////////////
    /// @brief This will update the acceleration of the ball due to Friction and Drag (if enabled)
    ///
    /// @return void
    ///
    /////////////////////////////////////////////////

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
    /////////////////////////////////////////////////
    /// @brief This will do the following:
    ///        * Update ball's position
    ///        * Update ball's velocity
    ///        * Call clsCannonball::Drag_updateacc if blnDragEnabled is true
    ///        * Log the ball's location (if enabled)
    ///        * Update CollisionBox
    ///        * Set the ball's blnstarted to false (stopping future updates) if total velocity is less than Global::Physics::fMinVelocity or equals NaN
    ///
    /// @param newdeltat = the time (in seconds) that have passed since the last update (see tick.cpp)
    /// @return void
    ///
    /////////////////////////////////////////////////

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
            if ( isnan(total_v) ) {printf("Ball velocity is NaN; killing it\n");}
            else {printf("Ball moving too slow; killing it\n");}
        } //end if debug mode
    } //end if should kill
	show();
}
/**********************************************************************************************************************************************/
void clsCannonball::setSDLScreen(SDL_Texture* SDLball, SDL_Texture* SDLpxel, WINATT SDLwindow, uint newID) {
    /////////////////////////////////////////////////
    /// @brief This will set certain values for the ball based on the screen. It will also randomly generate a color for the ball.
    ///
    /// @param SDLball = Pointer to clsScreen::ball
    /// @param SDLpxel = Pointer to clsScreen::pixel
    /// @param SDLwindow = attributes of the SDL window
    /// @param newID = the ID that ball's ID will be set to
    /// @return void
    ///
    /////////////////////////////////////////////////

    ballID = newID;
    //set the color of the ball
    //Set some ID's to have specific colors, but otherwise randomize them
    Color.Red = rand() % 255;
    Color.Green = rand() % 255;
    Color.Blue = rand() % 255;

    ball = SDLball;
    pixel = SDLpxel;
    SDL_QueryTexture(ball,NULL,NULL, &Screen_place.w, &Screen_place.h);
    window = SDLwindow;
}
/**********************************************************************************************************************************************/
void clsCannonball::show() {
    /////////////////////////////////////////////////
    /// @brief Places the ball on the SDL Screen
    /// @return void
    /////////////////////////////////////////////////

    Screen_place.x = place.x;
    Screen_place.y = window.height - place.y;

    if (Global::Config.values.blnDrawPathOnScreen) { drawPath(place); }

    Uint8 alpha = 0xFF;
    double dblAlpha;

    dblAlpha = (double) Global::Equations::fMassAlphaRatio * log(props.mass) + Global::Equations::fMassAlphaOffset;
    alpha = dblAlpha < (double) Global::Equations::uAlphaMinimum ? (Uint8) Global::Equations::uAlphaMinimum : (Uint8) dblAlpha;
    alpha = dblAlpha > 255.0 ? 255 : (Uint8) dblAlpha;

    //set the ball alpha
    SDL_SetTextureAlphaMod(ball, alpha);
    SDL_SetTextureColorMod(ball, Color.Red, Color.Green, Color.Blue);

    //Place the ball
    SDL_RenderCopy(window.ren,ball,NULL,&Screen_place);

    //reset ball Alpha and color so it doesn't effect the next ball
    SDL_SetTextureAlphaMod(ball, 0xFF);
    SDL_SetTextureColorMod(ball, 0xFF, 0xFF, 0xFF);

}
/**********************************************************************************************************************************************/
void clsCannonball::setValues(double r, LOC init_place, double init_vel, double init_angle) {
    /////////////////////////////////////////////////
    /// @brief Changes the values of the ball to whatever is entered
    ///
    /// @param r = radius of the ball
    /// @param init_place = its starting location
    /// @param init_vel = its starting velocity
    /// @param init_angle = its starting angle (in radians)
    /// @return void
    ///
    /////////////////////////////////////////////////

    props.radius = r; //in meters

    acc.x = 0.00;
    acc.y = Global::Physics::fGravity;

    place = init_place;
    dblLOC.x = (double) place.x;
    dblLOC.y = (double) place.y;

    vel.x = (double)(init_vel) * (cos(init_angle));
	vel.y = (double)(init_vel) * (sin(init_angle));
	blnstarted = true;

    Drag_calcvalues();
	if (Global::Config.values.blnDragMode) {
        blnDragEnabled = true;
        if (Global::blnDebugMode) {printf("Drag enabled.\n");}
    } //end if drag mode
}
/**********************************************************************************************************************************************/
LOC clsCannonball::getplace() {
    /////////////////////////////////////////////////
    /// @brief Returns the ball's place
    /////////////////////////////////////////////////
    return place;
}
/**********************************************************************************************************************************************/
dblXY clsCannonball::getVelocity() {
    /////////////////////////////////////////////////
    /// @brief Returns the ball's velocity
    /////////////////////////////////////////////////
    return vel;
}
/**********************************************************************************************************************************************/
void clsCannonball::setVelocity(dblXY newvel) {
    /////////////////////////////////////////////////
    /// @brief Sets the ball's velocity
    /////////////////////////////////////////////////
    vel.x = newvel.x;
    vel.y = newvel.y;
}
/**********************************************************************************************************************************************/
PP clsCannonball::getPhysicalProps() {
    /////////////////////////////////////////////////
    /// @brief Returns the ball's physical properties
    /////////////////////////////////////////////////
    return props;
}
/**********************************************************************************************************************************************/
void clsCannonball::setplace(LOC newplace) {
    /////////////////////////////////////////////////
    /// @brief Set the ball's place
    /////////////////////////////////////////////////
    dblLOC.x = newplace.x;
    dblLOC.y = newplace.y;
}
/**********************************************************************************************************************************************/
void clsCannonball::setPhysicalProps(PP newprops) {
    /////////////////////////////////////////////////
    /// @brief Set the ball's physical properties (only used on CollidePerfectInelastic)
    /////////////////////////////////////////////////
    props = newprops;
}
/**********************************************************************************************************************************************/
BOX clsCannonball::getBOX() {
    /////////////////////////////////////////////////
    /// @brief Returns the ball's box
    /////////////////////////////////////////////////
    return CollisionBox;
}
/**********************************************************************************************************************************************/
void clsCannonball::drawPath(LOC newplace) {
    /////////////////////////////////////////////////
    /// @brief Will draw the path the ball as taken based on DEFINED_MAXNUMPASTPOINTS
    /// @param newplace = The last place the ball was at
    /// @return void
    /////////////////////////////////////////////////

    static uint UpdatesSinceLast;

    //If there have been enough updates since the last time the path was updated,
    //then update the path array otherwise inc updates
    SDL_SetTextureColorMod(pixel, Color.Red, Color.Green, Color.Blue);
    if ( UpdatesSinceLast >= 25 ) {
        UpdatesSinceLast = 0;
        //First move all the old locations down one spot in the array
        for (uint i = 1; i < DEFINED_MAXNUMPASTPOINTS; i++) { path[i] = path[i-1]; }
        //Now put the new location into the latest spot
        path[0] = newplace;
    } else { UpdatesSinceLast++; } //end if update points
    //Now draw the path
    SDL_Rect dst;
    dst.w = dst.h = 1;
    for (uint i = 0; i < DEFINED_MAXNUMPASTPOINTS; i++) {
        dst.y = window.height - (uint)(path[i].y - Screen_place.h / 2);
        dst.x = (uint)(path[i].x + Screen_place.w / 2);
        SDL_RenderCopy(window.ren, pixel, NULL, &dst);
    } //end for
    SDL_SetTextureColorMod(pixel, 0xFF, 0xFF, 0xFF);
}
/**********************************************************************************************************************************************/
