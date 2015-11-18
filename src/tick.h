#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/*****************************************************************************/
#include <time.h>
#include <cmath>
/*****************************************************************************/
class clsTick {
	private:
		//Members
		/** The time the ball positions were last updated at. */
		clock_t lngTimerStart;

		/** The time of the start of the mouse being held down. */
		clock_t lngHoldingStart;
	public:
		//Default Constructor
		clsTick();
		//Functions
		void startTimer(void);
		double getTimeDifference(void);

		void startHolding(void);
		double stopHolding(void);
};
/*****************************************************************************/
#endif
