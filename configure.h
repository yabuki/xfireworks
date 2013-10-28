/* configure.h for configuration of xfireworks */

#define DEFAULT_DISPLAY NULL
/* #define DEFAULT_DISPLAY ":0.0" */
#define DEFAULT_DIRECT_DRAW 1

#define DEFAULT_BACKGROUND_COLOR "black"
#define DEFAULT_FILENAME "xfireworks.conf"

/* Parameters for tuning hash and cache of colors. */

#define DEFAULT_STUDYING_FLAG       0
#define DEFAULT_CACHE_FLAG          1
#define DEFAULT_CACHE_SIZE          5
#define DEFAULT_HASH_NUMBER        130

/* Other parameters. */

#define DEFAULT_PIECES_MAX_NUMBER 100
#define DEFAULT_FINE              100
/* #define DEFAULT_FINE  75 */ /* for slow machine. */
/* #define DEFAULT_FINE 150 */ /* for fast machine. */
#define DEFAULT_WAIT 0

/* If color gradation is a little, colors are shared, memory of        */
/* X server and xfireworks are saved, and xfireworks starts up speedy. */

#define DEFAULT_GRADATION 16
/* #define DEFAULT_GRADATION  4 */ /* for less color and slow machine. */
/* #define DEFAULT_GRADATION 64 */ /* for high color and fast machine. */

/* Some parameters are magnified by these values. */

#define DEFAULT_PROBABILITY_MAGNIFICATION  100
#define DEFAULT_SIZE_MAGNIFICATION         100
#define DEFAULT_AIR_MAGNIFICATION          100
#define DEFAULT_GRAVITY_MAGNIFICATION      100
#define DEFAULT_TRANSMISSION_MAGNIFICATION 100
#define DEFAULT_AFTER_IMAGE_MAGNIFICATION  100
#define DEFAULT_COLOR_LENGTH_MAGNIFICATION 100
#define DEFAULT_NEXT_POWER_MAGNIFICATION   100
#define DEFAULT_NEXT_NUMBER_MAGNIFICATION  100

/* Default values. If default values are not defined in xfireworks.conf,    */
/* these values are used.                                                   */
/* (If default values are defined in xfireworks.conf, they are used first.) */

#define DEFAULT_PROBABILITY   0.1
#define DEFAULT_SIZE          3
#define DEFAULT_AIR           0.05
#define DEFAULT_GRAVITY       0.3
#define DEFAULT_TRANSMISSION 60.0
#define DEFAULT_AFTER_IMAGE  10

/* End of configure.h */
