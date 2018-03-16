#ifndef __VERSION_HEADER__
#define __VERSION_HEADER__
/*****************************************************************************/
	//Date Version Types
	#define DEFINED_VER_DATE "16"
	/**< Day of the last build */

	#define DEFINED_VER_MONTH "03"
	/**< Month of the last build */

	#define DEFINED_VER_YEAR "2018"
	/**< Year of the last build */

	#define DEFINED_VER_UBUNTU_VERSION_STYLE  "18.03"
	/**< Version styling for Ubuntu, consists of YY.MM */


	//Software Status
	#define DEFINED_VER_STATUS  "Beta"
    /**<
    Software Status can be the following:
    | Status | Short | Description |
    |-------------------|-------|-------------------------------------------------------------------------------------------------------------------------------------------------|
    | Release | r | The supported release of the program. |
    | Release Candidate | rc | The next likely release version; usually only a few bugs / todos separate this froma release. It is pretty safe to use. |
    | Beta | b | This version will compile and / or run but does not work as expected (or crashes often). |
    | Alpha | a | This version does not work at all; it usually won't compile at all or crashes more times than it works; best not to use versions in this state. |
    */

	#define DEFINED_VER_STATUS_SHORT  "B"
	/**< See table for DEFINED_VER_STATUS */

	//Standard Version Type
	#define DEFINED_VER_MAJOR  1
	/**< The major version number */

	#define DEFINED_VER_MINOR  4
	/**< The minor version number */

	#define DEFINED_VER_BUILD  0
	/**< The patch version number */

	#define DEFINED_VER_RC_FILEVERSION 1,4,0,0
	/**< The number used in the boilerplate.rc file
        Follows MAJOR,MINOR,PATCH,BUILD where build is the number of times this version has been built*/

	#define DEFINED_VER_RC_FILEVERSION_STRING "1, 4, 0, 0\0"
	/**< Is the same as DEFINED_VER_RC_FILEVERSION but is a null terminated string */

	#define DEFINED_VER_FULLVERSION_STRING  "1.4.0-Beta.1"
	/**< The full string of the version number */

/*****************************************************************************/
#endif //__VERSION_HEADER__
