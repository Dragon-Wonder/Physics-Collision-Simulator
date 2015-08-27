/**********************************************************************************************************************************************/
#include "config.h"
#include "version.h"
/**********************************************************************************************************************************************/
/*
This holds all the functions related to the config file, its loading, making, and holding the values pulled from the config.
*/
/**********************************************************************************************************************************************/
clsConfig::clsConfig() {
    //Set default values
    values.blnLogging = false;
    values.uintScreenWidth = 680;
    values.uintScreenHeight = 480;
    values.PathToImages = ".\\images\\";
}
/**********************************************************************************************************************************************/
bool clsConfig::exists(void) {
	//Returns true or false if config file exists
	FILE* pTempFile = fopen(FileName, "r");
	if (pTempFile == NULL) {return false;}
	else {return true;}
}
/**********************************************************************************************************************************************/
void clsConfig::make(void) {
	//Makes the config file
	configFile = fopen(FileName,"w");
	printf("Config File will now be created!\n");

	fprintf(configFile,"Config File for the Cannon.exe\n");
	fprintf(configFile,"%s-%s\n",DEFINED_VER_FULLVERSION_STRING, DEFINED_VER_STATUS_SHORT);
	//Write the config file with the defaults above
	//this way I only have to change the defaults in one place
    fprintf(configFile,"Screen Width: %u\n",values.uintScreenWidth);
    fprintf(configFile,"Screen Height: %u\n",values.uintScreenHeight);
    fprintf(configFile,"Log Ball's path: 0\n");
    //fprintf(configFile,"Path to image folder: %s\n",values.PathToImages);
	fclose(configFile);
}
/**********************************************************************************************************************************************/
void clsConfig::load(void) {
	//Loads all of the config values
	/* TODO (GamerMan7799#6#): Consider expanding chrTempString in case of longer folder paths */
	char chrTempString[50];
	int intTempBool, intValuesScanned;

    //Get Screen Width
    fgets(chrTempString,50,configFile);
    intValuesScanned = sscanf(chrTempString,"%*s %*s %u",&values.uintScreenWidth);
    if (intValuesScanned < 1) {printf("Error"); values.uintScreenWidth = 680;}
    if (Global::blnDebugMode) {printf("Screen Width \t %u\n", values.uintScreenWidth);}

    //Get Screen Height
    fgets(chrTempString,50,configFile);
    intValuesScanned = sscanf(chrTempString,"%*s %*s %u",&values.uintScreenHeight);
    if (intValuesScanned < 1) {printf("Error"); values.uintScreenHeight = 480;}
    if (Global::blnDebugMode) {printf("Screen Height \t %u\n", values.uintScreenHeight);}

    //Get if logging
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
	if(Global::blnDebugMode) {printf("Log Path \t %d\n",intTempBool);}
	if(intTempBool == 1) {values.blnLogging = true;}
	else {values.blnLogging = false;}

    //Get Images Path
    /*fgets(chrTempString,50,configFile);
    intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %s", &values.PathToImages);
    if (intValuesScanned < 1) {printf("ERROR!"); values.PathToImages = ".\\images\\";}
    if(Global::blnDebugMode) {printf("Images Path \t \t %s\n",values.PathToImages);}*/

	fclose(configFile);
	printf("\n\n");
}
/**********************************************************************************************************************************************/
char clsConfig::verisonCheck(const char *ConfigVerison) {
	//This checks the version number written at the top of the config file
	//against the internal version number of the program.
	//If it finds a Major revision change the config HAS to be replaced.
	//Otherwise just use the config file

	uint C_MajorNum, C_MinorNum, C_PatchNum, C_BuildNum;
    char C_SoftwareStatus, P_SoftwareStatus;
	sscanf(ConfigVerison,"%u.%u.%u.%u-%c",&C_MajorNum, &C_MinorNum, &C_PatchNum, &C_BuildNum, &C_SoftwareStatus);
	if (Global::blnDebugMode) {printf("Config: v %u %u %u %u %c \n", C_MajorNum, C_MinorNum, C_PatchNum, C_BuildNum, C_SoftwareStatus);}
	sscanf(DEFINED_VER_STATUS_SHORT,"%c",&P_SoftwareStatus);
	if (P_SoftwareStatus != C_SoftwareStatus) {return NEWCONFIG;}
	else if (DEFINED_VER_MAJOR != C_MajorNum) {return NEWCONFIG;}
	else if (DEFINED_VER_MINOR != C_MinorNum) {return USECONFIG;}
	else {return USECONFIG;}
}
/**********************************************************************************************************************************************/
void clsConfig::Check(void) {
	char chrTempString[50], chrConfigVerison;

	if (exists() != true) {
		printf("Config file was not found; creating now one\n");
		make();
	} else {
		configFile = fopen(FileName,"r");
		printf("Config file found; loading values\n");
		fgets(chrTempString,50,configFile);
		fgets(chrTempString,50,configFile);
		chrConfigVerison = verisonCheck(chrTempString);

		if (chrConfigVerison == NEWCONFIG) {
			printf("Current config file out of date. Making new one.\n");
			fclose(configFile);
			make();
		} else { load();}
	} //end if exists
}
/**********************************************************************************************************************************************/
