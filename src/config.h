#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
/**********************************************************************************************************************************************/
#include "global.h"
/**********************************************************************************************************************************************/
//These are used when checking the version of the config file.
#define NEWCONFIG 'N'
#define USECONFIG 'U'
/**********************************************************************************************************************************************/
struct ConfigValues {
	uint uintScreenWidth;
	uint uintScreenHeight;
	bool blnLogging;
	bool blnDragMode;
	char* OperatingSystem;
};

typedef struct ConfigValues Configures;
/**********************************************************************************************************************************************/
class clsConfig {
    private:
        const char* FileName = "Config.ini";
        FILE* configFile;

        char verisonCheck(const char *ConfigVerison);
 		bool exists(void);
		void make(void);
		void load(void);

	public:
		Configures values;

		clsConfig();

		//Functions
		void Check(void);
};
/**********************************************************************************************************************************************/
#endif
