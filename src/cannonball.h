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
        bool blnstarted;
        void setValues(double, LOC, double, double);
        LOC getplace(void);

        void enableDrag(void);
        void update(double);
        void setSDLScreen(SDL_Texture*, WINATT);
    private:
        bool blnDragEnabled;

        SDL_Texture* ball;
        WINATT window;

        LOC place;
        dblXY vel;
        dblXY dblLOC;
        dblXY acc;
        PP props;

        double deltat;
        void show(void);
        void Drag_calcvalues(void);
        void Drag_updateacc(void);
};
/**********************************************************************************************************************************************/
#endif // __CANNONBALL_HEADER__
