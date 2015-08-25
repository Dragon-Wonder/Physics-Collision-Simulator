#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include <time.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
//typedef because reasons
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
class clsTick {
    public:
	//private:
		//Members
		uchar uchrFPS;
		ulong ulngSleepTime;

	//public:
		//Default Constructor
		clsTick();

		//Functions
		void wait(void); //Call this to activate the wait
		double getdeltat(void);
};
/**********************************************************************************************************************************************/
#endif
