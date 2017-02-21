#ifndef __CANNONBALL_HEADER__
#define __CANNONBALL_HEADER__
/*****************************************************************************/
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include "screen.h"
/*****************************************************************************/
/** Holds X and Y values that are related together, the values are doubles
     (Used for location, velocity and accelerations) */
struct stcDoubleValues {
  double x; /**< X direction values */
  double y; /**< Y direction values */
};

/** Holds the physical properties of the cannonball. */
struct stcPhysicalProperties {
  double radius; /**< Radius in meters */
  double mass; /**< Mass of the ball in kilograms */
  double area; /**< HALF of Area of the ball in terms of m<sup>2</sup>, it is only half
                    because the drag equation only needs the area facing the wind,
                    and it saves me from having to divide. */
  double volume; /**< Volume of the ball in m^3 */
  double density; /**< Density of the ball in kg/m^3 see Global::Physics::uBallDensity */
};

/** Holds a box which specifics the space that the ball occupies, used to see if it is colliding with another
     ball. The box is defined with the values of it left-most edge, its right-most edge, its top-most edge, and its bottom-most edge */
struct stcBox {
  uint left; /**< Left-most edge in term of pixels from the left window edge */
  uint right; /**< Right-most edge in term of pixels from the left window edge */
  uint top; /**< Top-most edge in term of pixels from the top window edge */
  uint bottom; /**< Bottom-most edge in term of pixels from the top window edge */
};

/** The color based on red, green, and blue */
struct stcColor {
  Uint8 Red; /**< Amount of Red in Color, can vary from 0 to 255 */
  Uint8 Green; /**< Amount of Green in Color, can vary from 0 to 255 */
  Uint8 Blue;/**< Amount of Blue in Color, can vary from 0 to 255 */
};

typedef struct stcDoubleValues dblXY;
typedef struct stcPhysicalProperties PP;
typedef struct stcBox BOX;
typedef struct stcColor clr;

typedef std::vector<LOC> VPath;
/*****************************************************************************/
class clsCannonball {
  public:
    clsCannonball();
    ~clsCannonball();
    void setValues(double, LOC, double, double, int);
    LOC getplace(void);
    void setplace(LOC);

    void update(double);
    //void setSDLScreen(SDL_Texture*, SDL_Texture*, WINATT, uint);
    dblXY getVelocity(void);
    void setVelocity(dblXY);
    PP getPhysicalProps(void);
    void setPhysicalProps(PP);
    BOX getBOX(void);

    /** Whether or not the ball is "started" if it is, the program will update
        it and won't let a new ball replace it in its array spot. If it is not started
        it won't have its update called and can be replaced when addNewCannonball is called. */
    bool blnstarted_;
  private:
    /** The ball ID which is basically just its number in the array */
    uint ballID_;

    /** If Drag has been enabled or not */
    bool blndragenabled_;

    /** The color of the ball, which is randomly generated */
    clr color_;

    //** A pointer to the texture of the ball that is created in Screen.cpp,
    //    used to draw the ball on the screen */
    //SDL_Texture* ball_;

    //** A pointer to the texture of the pixel that is created in Screen.cpp,
     //   used to draw the path behind the ball when that is enabled */
    //SDL_Texture* pixel_;

    //** Hold certain values that are based on the window */
    //WINATT window_;

    /** The Box for this specific ball */
    BOX collisionbox_;

    /** The SDL Rect for the current place of the ball */
    SDL_Rect screen_place_;

    /** The ball's location in terms of X and Y */
    LOC place_;

    /** The ball's velocity in terms of X and Y */
    dblXY vel_;

    /** The ball's location in terms of X and Y in double
        (because it is more accurate then we round to get the place
        that is used to put the ball on the screen */
    dblXY dblLOC_;

    /** The ball's current acceleration values in terms of X and Y */
    dblXY acc_;

    /** The Physical properties of the ball */
    PP props_;

    /** Past locations of the ball which will be shown if enabled */
    VPath path_;

    /** The time in seconds since the last time it moved. It is variable
        based on how much time has past since the last update to keep
        the movements looking realistic. See clsTick::getTimeDifference */
    double deltat_;

    void show(void);
    void drawPath(LOC);
    void dragCalcValues(void);
    void dragUpdateAcc(void);
};
/*****************************************************************************/
#endif // __CANNONBALL_HEADER__
