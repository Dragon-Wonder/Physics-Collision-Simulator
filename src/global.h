#ifndef __GLOBAL_H_INCLUDED__
#define __GLOBAL_H_INCLUDED__
/**********************************************************************************************************************************************/
class clsConfig;
/**********************************************************************************************************************************************/
//global typedefs
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
//Global namespace
namespace Global {
    extern const bool blnDebugMode;
    extern clsConfig Config; //Hold the config file globally so configure options can be referenced everywhere
    namespace Physics { //hold physics values in one place for future reference
        extern const uint uBallDensity;
        extern const float fGravity;
        extern const float fDragCofficient;
        extern const float fDensityAir;
        extern const float fRecoil;
        extern const float fVelocityScalar;
        extern const float fMinVelocity;
        extern const float fMomentumLoss;
        extern const uchar CollisionMethod;
    }
}
/**********************************************************************************************************************************************/
#endif // __GLOBAL_H_INCLUDED__
