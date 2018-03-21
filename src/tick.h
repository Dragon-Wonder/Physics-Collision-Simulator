#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/*****************************************************************************/
#include <time.h>
#include <cmath>
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsTick tick.h "source/tick.h"
/// @brief This class will hold functions related to time.
/////////////////////////////////////////////////
/*****************************************************************************/
class clsTick {
  public:
    clsTick();

    void startTimer(void);
    double getTimeDifference(void);
    void startHolding(void);
    double stopHolding(void);

	private:

		clock_t lngTimerStart; /**< The time the ball positions were last updated at. */
		clock_t lngHoldingStart; /**< The time of the start of the mouse being held down. */
};
/*****************************************************************************/
#endif
