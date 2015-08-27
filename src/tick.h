#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include <time.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
namespace Global {extern const bool blnDebugMode;}
/**********************************************************************************************************************************************/
//typedef because reasons
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
class clsTick {
	private:
		//Members
		clock_t lngTimerStart;

	public:
		//Default Constructor
		clsTick();

		//Functions
		void startTimer(void);
		double getTimeDifference(void);
		void wait(void);
};
/**********************************************************************************************************************************************/
#endif
