#ifndef __CANNONBALL_HEADER__
#define __CANNONBALL_HEADER__
/**********************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include "config.h"
#include "global.h"
#include "screen.h"
/**********************************************************************************************************************************************/
#define PI 3.1415926535897
/**********************************************************************************************************************************************/
struct stcDoubleValues {
    double x;
    double y;
};

struct stcPhysicalProperties {
    double radius;
    double mass;
    double area;
    double volume;
    double density;
};
typedef struct stcDoubleValues dblXY;
typedef struct stcPhysicalProperties PP;
/**********************************************************************************************************************************************/
class clsCannonball {
    public:
        /** Default constructor */
        clsCannonball();
        void setValues(double, LOC, double, double);
        LOC getplace(void);
        void setplace(LOC);

        void update(double);
        void setSDLScreen(SDL_Texture*, WINATT);
        dblXY getVelocity(void);
        void setVelocity(dblXY);
        PP getPhysicalProps(void);
        void setPhysicalProps(PP);

        bool blnstarted;
    private:
        bool blnDragEnabled;

        SDL_Texture* ball;
        WINATT window;

        LOC place;
        dblXY vel;
        dblXY dblLOC;
        dblXY acc;
        PP props;

        void enableDrag(void);
        double deltat;
        void show(void);
        void Drag_calcvalues(void);
        void Drag_updateacc(void);
};
/**********************************************************************************************************************************************/
#endif // __CANNONBALL_HEADER__
