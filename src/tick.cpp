/*****************************************************************************/
#include "tick.h"
/*****************************************************************************/
//This will hold functions related to time.
/*****************************************************************************/
//Default Constructor
clsTick::clsTick() {
    /////////////////////////////////////////////////
    /// @brief Default constructor, just sets lngTimerStart to now
    /////////////////////////////////////////////////

	lngTimerStart = clock();
}
/*****************************************************************************/
void clsTick::startTimer() {
    /////////////////////////////////////////////////
    /// @brief Sets lngTimerStart to now.
    /////////////////////////////////////////////////

    lngTimerStart = clock();
}
/*****************************************************************************/
double clsTick::getTimeDifference() {
    /////////////////////////////////////////////////
    /// @brief Returns the difference (in seconds) from now to when lngTimerStart
    /// @return Time in Seconds
    /////////////////////////////////////////////////

    double temp_time = (double)(((double)clock() - (double)lngTimerStart)
                                / (double)CLOCKS_PER_SEC);
    startTimer();
    return temp_time;
}
/*****************************************************************************/
void clsTick::startHolding() {
    /////////////////////////////////////////////////
    /// @brief Sets lngTimerStart to now.
    /////////////////////////////////////////////////
    lngHoldingStart = clock();
}
/*****************************************************************************/
double clsTick::stopHolding() {
    /////////////////////////////////////////////////
    /// @brief Returns the difference (in seconds) from now to when lngHoldingStart
    /// @return Time in Seconds
    /////////////////////////////////////////////////

    return (double)(((double)clock() - (double)lngHoldingStart)
                    / (double)CLOCKS_PER_SEC);
}
/*****************************************************************************/
