#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/*****************************************************************************/
#include <cstdio>
/*****************************************************************************/
#include "global.h"
/*****************************************************************************/
/** Holds all of the config values in one place for easier reference */
struct ConfigValues {
	uint uintScreenWidth; /**< Screen Width in Pixels, defaults to 640 */
	uint uintScreenHeight; /**< Screen Height in Pixels, defaults to 480 */
	bool blnLogging; /**< If logging of balls' path to a file is enabled */
	bool blnDragMode; /**< Whether Drag is enabled or not */
	bool blnDrawPathOnScreen; /**< Whether to draw the balls' past path on the screen */
	uchar uchrCollisionMethod; /**< The Method of Collision see Collisions */
	uint uintMaxNumPastPoints; /**< Max number of past locations to keep track of for the ball. */
	uint uintPastDelay; /**< How long of a delay between recording points */
	char* OperatingSystem; /**< The OS that the program is running on, doesn't do anything */
};

typedef struct ConfigValues Configures;
/*****************************************************************************/
class clsConfig {
  private:
  /** The name of the config file that will made */
  const char* FileName_ = "Config.ini";

  /** The reference file for the config file */
  FILE* configFile_;

  char verisonCheck(const char *ConfigVerison);
  bool exists(void);
  void make(void);
  void load(void);

	public:
  /** The config values */
  Configures values;

  clsConfig();

  //Functions
  void Check(void);
};
/*****************************************************************************/
#endif
