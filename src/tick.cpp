/**********************************************************************************************************************************************/
#include "tick.h"
/**********************************************************************************************************************************************/
//This will hold functions related to time.
/**********************************************************************************************************************************************/
//Default Constructor
clsTick::clsTick() {
	uchrFPS = 60;

	/*
	Figure out how long the wait should be based on the our goal frames per second.

	The Sleep time will be in milliseconds and is how long each "frame" will appear on the
	the screen before the next the one is done.

	I did it this way so that if show map is on it doesn't update it as quickly as it can
	leading to the console being really hard to follow.
	The sleep allows it to look much better.

	The frames per second is also not going to be right because it doesn't account for
	all the other functions of the program and there is also the rounding. But
	it should be close enough to not matter.
	*/

	ulngSleepTime = (ulong) round(1000.0 / uchrFPS);
}
/**********************************************************************************************************************************************/
void clsTick::wait(void) {

	/*Since sleep is usually an OS specific command I made this function
	To work as a "sleep" but it doesn't function as a true sleep because the
	CPU is still being used, but whatever. It "stops" the program for a bit
	which was its point so its gonna stay until I find something better.*/


	/* Code taken from http://c-for-dummies.com/blog/?p=69 */

	ulong pause;
	clock_t now, then;

	pause = ulngSleepTime * (CLOCKS_PER_SEC/1000);

	now = then = clock();
	//printf("Wait started for %lu milliseconds.\n", ulngSleepTime);

	while ( abs(now - then) < pause) {now = clock();}
}
/**********************************************************************************************************************************************/
double clsTick::getdeltat(void) {
    return (double) (1.0 / uchrFPS);
}
/**********************************************************************************************************************************************/
