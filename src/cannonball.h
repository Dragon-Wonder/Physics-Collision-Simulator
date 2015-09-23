#ifndef __CANNONBALL_HEADER__
#define __CANNONBALL_HEADER__
/**********************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <math.h>
#include <cstdio>
#include <cstdlib>
//#include <time.h>
#include "config.h"
#include "global.h"
#include "screen.h"
/**********************************************************************************************************************************************/
/** Define of PI so that I can use it other places. */
#define PI 3.1415926535897

/** This is the max number of points that are recorded and then shown if Global::Config.values.blnDrawPathOnScreen is true.
     I had issues of no more than 5 showing up, so 5 is the default value */
#define DEFINED_MAXNUMPASTPOINTS 5
/**********************************************************************************************************************************************/
struct stcDoubleValues {
    double x;
    double y;
};
/**< Holds X and Y values that are related together, the values are doubles
     (Used for location, velocity and accelerations) */

struct stcPhysicalProperties {
    double radius;
    double mass;
    double area;
    double volume;
    double density;
};
/**< Holds the physical properties of the cannonball. */

struct stcBox {
    uint left;
    uint right;
    uint top;
    uint bottom;
};
/**< Holds a box which specifics the space that the ball occupies, used to see if it is colliding with another
     ball. The box is defined with the values of it left-most edge, its right-most edge, its top-most edge, and its bottom-most edge */

struct stcColor {
    Uint8 Red;
    Uint8 Green;
    Uint8 Blue;
};
/**< The color based on red, green, and blue */

typedef struct stcDoubleValues dblXY;
typedef struct stcPhysicalProperties PP;
typedef struct stcBox BOX;
typedef struct stcColor clr;
/**********************************************************************************************************************************************/
class clsCannonball {
    public:
        clsCannonball();
        void setValues(double, LOC, double, double);
        LOC getplace(void);
        void setplace(LOC);

        void update(double);
        void setSDLScreen(SDL_Texture*, SDL_Texture*, WINATT, uint);
        dblXY getVelocity(void);
        void setVelocity(dblXY);
        PP getPhysicalProps(void);
        void setPhysicalProps(PP);
        BOX getBOX(void);

        bool blnstarted;
    private:
        /** The ball ID which is basically just its number in the array */
        uint ballID;

        /** If Drag has been enabled or not */
        bool blnDragEnabled;

        /** The color of the ball, which is randomly generated */
        clr Color;

        /** A pointer to the texture of the ball that is created in Screen.cpp, used to draw the ball on the screen */
        SDL_Texture* ball;

        /** A pointer to the texture of the pixel that is created in Screen.cpp, used to draw the path behind the ball when that is enabled */
        SDL_Texture* pixel;

        /** Hold certain values that are based on the window */
        WINATT window;

        /** The Box for this specific ball */
        BOX CollisionBox;

        /** The SDL Rect for the current place of the ball */
        SDL_Rect Screen_place;

        /** The ball's location in terms of X and Y */
        LOC place;

        /** The ball's velocity in terms of X and Y */
        dblXY vel;

        /** The ball's location in terms of X and Y in double (because it is more accurate then we round to get the place
            that is used to put the ball on the screen */
        dblXY dblLOC;

        /** The ball's current acceleration values in terms of X and Y */
        dblXY acc;

        /** The Physical properties of the ball */
        PP props;

        /** Past locations of the ball which will be shown if enabled */
        LOC path[DEFINED_MAXNUMPASTPOINTS];

        double deltat;
        void show(void);
        void drawPath(LOC);
        void Drag_calcvalues(void);
        void Drag_updateacc(void);
};
/**********************************************************************************************************************************************/
#endif // __CANNONBALL_HEADER__
