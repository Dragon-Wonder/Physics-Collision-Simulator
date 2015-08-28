/**********************************************************************************************************************************************/
#include "tick.h"
/**********************************************************************************************************************************************/
//This will hold functions related to time.
/**********************************************************************************************************************************************/
//Default Constructor
clsTick::clsTick() {
	lngTimerStart = clock();
}
/**********************************************************************************************************************************************/
void clsTick::startTimer(void) {
    lngTimerStart = clock();
}
/**********************************************************************************************************************************************/
double clsTick::getTimeDifference(void) {
    double temp_time = (double)(((double)clock() - (double)lngTimerStart) / (double)CLOCKS_PER_SEC);
    if (Global::blnDebugMode) {
        FILE* something = fopen("time-log.log","a");
        fprintf(something,"%f\n",temp_time);
        fclose(something);
    }
    startTimer();
    return temp_time;
}
/**********************************************************************************************************************************************/
void clsTick::wait(void) {
	/* Code taken from http://c-for-dummies.com/blog/?p=69 */
	ulong pause;
	clock_t now, then;
	pause = 5 * (CLOCKS_PER_SEC/1000); //pause equals 1 millisecond
    now = then = clock();
	while (abs(now - then) < pause) {now = clock();}
}
/**********************************************************************************************************************************************/
