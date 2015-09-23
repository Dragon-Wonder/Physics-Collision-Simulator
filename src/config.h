#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
/**********************************************************************************************************************************************/
#include "global.h"
/**********************************************************************************************************************************************/
/** This is a define of a possible return that will cause the program to make a new config file */
#define NEWCONFIG 'N'

/** This is a define of a possible return that will cause the program to use an existing config file */
#define USECONFIG 'U'
/**********************************************************************************************************************************************/
/** Holds all of the config values in one place for easier reference */
struct ConfigValues {
	uint uintScreenWidth;
	uint uintScreenHeight;
	bool blnLogging;
	bool blnDragMode;
	bool blnDrawPathOnScreen;
	char* OperatingSystem;
};

typedef struct ConfigValues Configures;
/**********************************************************************************************************************************************/
class clsConfig {
    private:
        /** The name of the config file that will made */
        const char* FileName = "Config.ini";

        /** The reference file for the config file */
        FILE* configFile;

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
/**********************************************************************************************************************************************/
#endif
