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

struct stcBox {
    uint left;
    uint right;
    uint top;
    uint bottom;
};

typedef struct stcDoubleValues dblXY;
typedef struct stcPhysicalProperties PP;
typedef struct stcBox BOX;
/**********************************************************************************************************************************************/
class clsCannonball {
    public:
        /** Default constructor */
        clsCannonball();
        void setValues(double, LOC, double, double, uint);
        LOC getplace(void);
        void setplace(LOC);

        void update(double);
        void setSDLScreen(SDL_Texture*, WINATT);
        dblXY getVelocity(void);
        void setVelocity(dblXY);
        PP getPhysicalProps(void);
        void setPhysicalProps(PP);
        BOX getBOX(void);

        bool blnstarted;
    private:
        uint ballID;
        bool blnDragEnabled;

        SDL_Texture* ball;
        WINATT window;

        BOX CollisionBox;
        SDL_Rect Screen_place;

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
