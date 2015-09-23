#ifndef __VERSION_HEADER__
#define __VERSION_HEADER__

	//Date Version Types
	#define DEFINED_VER_DATE "21"
	/**< Day of the last build */

	#define DEFINED_VER_MONTH "09"
	/**< Month of the last build */

	#define DEFINED_VER_YEAR "2015"
	/**< Year of the last build */

	#define DEFINED_VER_UBUNTU_VERSION_STYLE  "15.09"
	/**< Version styling for Ubuntu, consists of YY.MM */


	//Software Status
	#define DEFINED_VER_STATUS  "Beta"
    /**<
    Software Status can be the following:\n
    |===================|========|====================================================|\n
    | Status            | Short  | Description                                        |\n
    |===================|========|====================================================|\n
    | Release           | r      | The supported release of the program               |\n
    |===================|========|====================================================|\n
    | Release Candidate | rc     | The next likely release version;                   |\n
    |					|	     | usually only a few bugs / todos separate this from |\n
    |					|		 | a release. It is pretty safe to use.               |\n
    |===================|========|====================================================|\n
    | Beta				| b      | This version will compile and / or run but does not|\n
    |                   |        | work as expected (or crashes often)                |\n
    |===================|========|====================================================|\n
    | Alpha				| a		 | This version does not work at all; it usually won't|\n
    |                   |        | compile at all or crashes more times than it works |\n
    |                   |        | best not to use versions in this state             |\n
    |===================|========|====================================================|\n
    */

	#define DEFINED_VER_STATUS_SHORT  "b"
	/**< See table for DEFINED_VER_STATUS */

	//Standard Version Type
	#define DEFINED_VER_MAJOR  1
	/**< The major version number */

	#define DEFINED_VER_MINOR  2
	/**< The minor version number */

	#define DEFINED_VER_BUILD  5
	/**< The patch version number */

	#define DEFINED_VER_RC_FILEVERSION 1,2,5,0
	/**< The number used in the boilerplate.rc file
        Follows MAJOR,MINOR,PATCH,BUILD where build is the number of times this version has been built*/

	#define DEFINED_VER_RC_FILEVERSION_STRING "1, 2, 5, 0\0"
	/**< Is the same as DEFINED_VER_RC_FILEVERSION but is a null terminated string */

	#define DEFINED_VER_FULLVERSION_STRING  "1.2.5-Beta"
	/**< The full string of the version number */

#endif //__VERSION_HEADER__
