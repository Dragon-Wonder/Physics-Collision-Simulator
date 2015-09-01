#ifndef __CANNONBALL_HEADER__
#define __CANNONBALL_HEADER__
/**********************************************************************************************************************************************/
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include "config.h"
/**********************************************************************************************************************************************/
//typedefs because reasons
typedef unsigned int uint;
typedef unsigned char uchar;
/**********************************************************************************************************************************************/
namespace Global {
    extern const bool blnDebugMode;
    extern clsConfig Config;
}
/**********************************************************************************************************************************************/
struct stcLocation {
    uint x;
    uint y;
};

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
typedef struct stcLocation LOC;
typedef struct stcDoubleValues dblXY;
typedef struct stcPhysicalProperties PP;
/**********************************************************************************************************************************************/
namespace Global {
    extern const bool blnDebugMode;
    extern clsConfig Config;
}
/**********************************************************************************************************************************************/
class clsCannonball {
    public:
        /** Default constructor */
        clsCannonball(double, LOC, uint, double);

        void enableDrag(void);
        void setdeltat(double);
        void update(void);
        LOC getPlace(void);
    private:
        bool blnDragEnabled;

        LOC place;
        dblXY vel;
        dblXY dblLOC;
        dblXY acc;
        PP props;

        double deltat;

        void Drag_calcvalues(void);
        void Drag_updateacc(void);
};
/**********************************************************************************************************************************************/
#endif // __CANNONBALL_HEADER__
