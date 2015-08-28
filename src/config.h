#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
/**********************************************************************************************************************************************/
//These are used when checking the version of the config file.
#define NEWCONFIG 'N'
#define USECONFIG 'U'
/**********************************************************************************************************************************************/
typedef unsigned int uint;
typedef unsigned char uchar;
/**********************************************************************************************************************************************/
namespace Global {
    extern const bool blnDebugMode;
}
/**********************************************************************************************************************************************/
struct ConfigValues {
	uint uintScreenWidth;
	uint uintScreenHeight;
	bool blnLogging;
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
