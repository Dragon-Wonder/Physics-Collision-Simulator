#ifndef __VECTOR_MATH_HEADER__
#define __VECTOR_MATH_HEADER__
/*****************************************************************************/
#include <math.h>
#include "global.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file vector_math.h
/// @brief Header for math functions for vectors
/////////////////////////////////////////////////
/*****************************************************************************/
/** @struct stcDoubleValues
    Holds X and Y values that are related together, the values are doubles
    (Used for location, velocity and accelerations) */
struct stcDoubleValues {
  double x; /**< X direction values */
  double y; /**< Y direction values */
};

typedef struct stcDoubleValues dblXY;
/*****************************************************************************/
namespace math {
  double getVectorLength(dblXY);
  double dotProduct(dblXY,dblXY);
  double crossProduct(dblXY,dblXY);
  dblXY vectorDiv(dblXY,double);
  dblXY vectorDiv(dblXY,dblXY);
  dblXY vectorAdd(dblXY,double);
  dblXY vectorAdd(dblXY,dblXY);
  dblXY vectorSub(dblXY,double);
  dblXY vectorSub(dblXY,dblXY);
  dblXY vectorMul(dblXY,double);
  dblXY vectorMul(dblXY,dblXY);
}
/*****************************************************************************/
#endif