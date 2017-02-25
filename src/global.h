#ifndef __GLOBAL_H_INCLUDED__
#define __GLOBAL_H_INCLUDED__
/*****************************************************************************/
//This file will hold the global references that are need in one place
/*****************************************************************************/
class clsConfig;
/*****************************************************************************/
//global typedefs
typedef unsigned char uchar; /**< Rename unsigned char to uchar because I'm lazy. */
typedef unsigned int uint; /**< Rename unsigned int to uint because I'm lazy. */
typedef unsigned long ulong; /**< Rename unsigned long to ulong because I'm lazy. */
/*****************************************************************************/
//Global namespace
namespace cannonballs {
  extern uint intCannonBallNum;
} //end namespace cannonballs

namespace global {
  extern const bool blnDebugMode;
  extern clsConfig config;
  namespace physics {
    extern const float kBallDensity;
    extern const float kGravity;
    extern const float kDragCofficient;
    extern const float kKineticFriction;
    extern const float kDensityAir;
    extern const float kMinVelocity;
    extern const float kCoefficientRestitution;
    extern uchar collisionmethod;
  }

  namespace equations {
    extern const float kVelocityScalar;
    extern const float kTimeSizeRatio;
    extern const float kMassAlphaRatio;
    extern const float kMassAlphaOffset;
    extern const float kAlphaMinimum;
  }
}
/*****************************************************************************/
/** The method of the collision (I just wanted to play around with some options.) */
enum Collisions {
  CollideElastic = 0, /**< This is normal collision in a perfect world without
                           friction, they hit and bounce off, no energy is lost. */
  CollideInelastic, /**< This is the normal way things collide, Balls collide and
                         bounce off of each other losing some energy. */
  CollidePerfectInelastic, /**< In this method the balls collide and then
                                stick together. */
  CollideNone /**< No collisions, balls pass through each other. */
};
/*****************************************************************************/
#endif // __GLOBAL_H_INCLUDED__
