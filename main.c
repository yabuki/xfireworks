/* main */

#include "configure.h"

#include "XFireworks.h"
#include "arguments.h"

#ifndef VERSION
#define VERSION "XFireworks-unknown-version"
#endif

#ifndef XFIREWORKS_CONF_DIRECTRY
#define XFIREWORKS_CONF_DIRECTRY "/usr/X11R6/etc"
#endif

#ifndef DEFAULT_DISPLAY
#define DEFAULT_DISPLAY NULL
#endif

#ifndef DEFAULT_DIRECT_DRAW
#define DEFAULT_DIRECT_DRAW 1
#endif

#ifndef DEFAULT_BACKGROUND_COLOR
#define DEFAULT_BACKGROUND_COLOR "black"
#endif

#ifndef DEFAULT_FILENAME
#define DEFAULT_FILENAME "xfireworks.conf"
#endif

#ifndef DEFAULT_STUDYING_FLAG
#define DEFAULT_STUDYING_FLAG 0
#endif

#ifndef DEFAULT_CACHE_FLAG
#define DEFAULT_CACHE_FLAG 1
#endif

#ifndef DEFAULT_CACHE_SIZE
#define DEFAULT_CACHE_SIZE 10
#endif

#ifndef DEFAULT_HASH_NUMBER
#define DEFAULT_HASH_NUMBER 64
#endif

#ifndef DEFAULT_PIECES_MAX_NUMBER
#define DEFAULT_PIECES_MAX_NUMBER 100
#endif

#ifndef DEFAULT_FINE
#define DEFAULT_FINE 100
#endif

#ifndef DEFAULT_WAIT
#define DEFAULT_WAIT 0
#endif

#ifndef DEFAULT_GRADATION
#define DEFAULT_GRADATION 16
#endif

#ifndef DEFAULT_PROBABILITY_MAGNIFICATION
#define DEFAULT_PROBABILITY_MAGNIFICATION 100
#endif

#ifndef DEFAULT_SIZE_MAGNIFICATION
#define DEFAULT_SIZE_MAGNIFICATION 100
#endif

#ifndef DEFAULT_AIR_MAGNIFICATION
#define DEFAULT_AIR_MAGNIFICATION 100
#endif

#ifndef DEFAULT_GRAVITY_MAGNIFICATION
#define DEFAULT_GRAVITY_MAGNIFICATION 100
#endif

#ifndef DEFAULT_TRANSMISSION_MAGNIFICATION
#define DEFAULT_TRANSMISSION_MAGNIFICATION 100
#endif

#ifndef DEFAULT_AFTER_IMAGE_MAGNIFICATION
#define DEFAULT_AFTER_IMAGE_MAGNIFICATION 100
#endif

#ifndef DEFAULT_COLOR_LENGTH_MAGNIFICATION
#define DEFAULT_COLOR_LENGTH_MAGNIFICATION 100
#endif

#ifndef DEFAULT_NEXT_POWER_MAGNIFICATION
#define DEFAULT_NEXT_POWER_MAGNIFICATION 100
#endif

#ifndef DEFAULT_NEXT_NUMBER_MAGNIFICATION
#define DEFAULT_NEXT_NUMBER_MAGNIFICATION 100
#endif

static char * display_name     = DEFAULT_DISPLAY;
static int direct_draw         = DEFAULT_DIRECT_DRAW;
static char * background_color = DEFAULT_BACKGROUND_COLOR;
static char * filename         = DEFAULT_FILENAME;

static int studying_flag       = DEFAULT_STUDYING_FLAG;
static int cache_flag          = DEFAULT_CACHE_FLAG;
static int cache_size          = DEFAULT_CACHE_SIZE;
static int hash_number         = DEFAULT_HASH_NUMBER;
static int pieces_max_number   = DEFAULT_PIECES_MAX_NUMBER;
static int fine                = DEFAULT_FINE;
static int wait_time           = DEFAULT_WAIT;
static int gradation           = DEFAULT_GRADATION;

static int probability_magnification  = DEFAULT_PROBABILITY_MAGNIFICATION;
static int size_magnification         = DEFAULT_SIZE_MAGNIFICATION;
static int air_magnification          = DEFAULT_AIR_MAGNIFICATION;
static int gravity_magnification      = DEFAULT_GRAVITY_MAGNIFICATION;
static int transmission_magnification = DEFAULT_TRANSMISSION_MAGNIFICATION;
static int after_image_magnification  = DEFAULT_AFTER_IMAGE_MAGNIFICATION;
static int color_length_magnification = DEFAULT_COLOR_LENGTH_MAGNIFICATION;
static int next_power_magnification   = DEFAULT_NEXT_POWER_MAGNIFICATION;
static int next_number_magnification  = DEFAULT_NEXT_NUMBER_MAGNIFICATION;

void help()
{
  printf("%s\n\n", VERSION);
  printf("XFireworks Copyright (c) 2000 Sakai Hiroaki.\n");
  printf("All Rights Reserved.\n\n");
  printf("Usage:\n");
  printf("        xfireworks [options]\n\n");
  printf("Options:\n");
  printf("        -h, -help\n");
  printf("        -display [displayname]\n");
  printf("        -bg, -background, -background-color [backgroundcolor]\n");
  printf("        -f, -file [filename]\n");
  printf("        -wait [micro seconds]\n");
  printf("        -fine [number]\n");
  printf("        -gradation [number]\n");
  printf("        -direct-draw, -no-direct-draw\n");
  printf("        -probability, -probability-magnification [number]\n");
  printf("        -size, -size-magnification [number]\n");
  printf("        -air, -air-magnification [number]\n");
  printf("        -gravity, -gravity-magnification [number]\n");
  printf("        -transmission, -transmission-magnification [number]\n");
  printf("        -after-image, -after-image-magnification [number]\n");
  printf("        -color-length, -color-length-magnification [number]\n");
  printf("        -next-power, -next-power-magnification [number]\n");
  printf("        -next-number, -next-number-magnification [number]\n");
  printf("\n");
  exit (0);
}

Argument arguments[] = {
  {"-h",                ARGUMENTS_FUNCTION, (void *)(&help)},
  {"-help",             ARGUMENTS_FUNCTION, (void *)(&help)},
  {"-display",          ARGUMENTS_STRING,   (void *)(&display_name)},
  {"-bg",               ARGUMENTS_STRING,   (void *)(&background_color)},
  {"-background",       ARGUMENTS_STRING,   (void *)(&background_color)},
  {"-background-color", ARGUMENTS_STRING,   (void *)(&background_color)},
  {"-f",                ARGUMENTS_STRING,   (void *)(&filename)},
  {"-file",             ARGUMENTS_STRING,   (void *)(&filename)},
  {"-wait",             ARGUMENTS_INTEGER,  (void *)(&wait_time)},
  {"-fine",             ARGUMENTS_INTEGER,  (void *)(&fine)},
  {"-gradation",        ARGUMENTS_INTEGER,  (void *)(&gradation)},
  {"-direct-draw",      ARGUMENTS_FLAG_ON,  (void *)(&direct_draw)},
  {"-no-direct-draw",   ARGUMENTS_FLAG_OFF, (void *)(&direct_draw)},

  {"-probability",
   ARGUMENTS_INTEGER, (void *)(&probability_magnification)},
  {"-probability-magnification",
   ARGUMENTS_INTEGER, (void *)(&probability_magnification)},

  {"-size",               ARGUMENTS_INTEGER, (void *)(&size_magnification)},
  {"-size-magnification", ARGUMENTS_INTEGER, (void *)(&size_magnification)},

  {"-air",                ARGUMENTS_INTEGER, (void *)(&air_magnification)},
  {"-air-magnification",  ARGUMENTS_INTEGER, (void *)(&air_magnification)},

  {"-gravity",
   ARGUMENTS_INTEGER, (void *)(&gravity_magnification)},
  {"-gravity-magnification",
   ARGUMENTS_INTEGER, (void *)(&gravity_magnification)},

  {"-transmission", ARGUMENTS_INTEGER, (void *)(&transmission_magnification)},
  {"-transmission-magnification",
   ARGUMENTS_INTEGER, (void *)(&transmission_magnification)},

  {"-after-image", ARGUMENTS_INTEGER, (void *)(&after_image_magnification)},
  {"-after-image-magnification",
   ARGUMENTS_INTEGER, (void *)(&after_image_magnification)},

  {"-color-length", ARGUMENTS_INTEGER, (void *)(&color_length_magnification)},
  {"-color-length-magnification",
   ARGUMENTS_INTEGER, (void *)(&color_length_magnification)},

  {"-next-power", ARGUMENTS_INTEGER, (void *)(&next_power_magnification)},
  {"-next-power-magnification",
   ARGUMENTS_INTEGER, (void *)(&next_power_magnification)},

  {"-next-number", ARGUMENTS_INTEGER, (void *)(&next_number_magnification)},
  {"-next-number-magnification",
   ARGUMENTS_INTEGER, (void *)(&next_number_magnification)},

  /* ここから下はチューニング用の隠しオプションです */

  {"-pieces-max-number",ARGUMENTS_INTEGER,  (void *)(&pieces_max_number)},

  /* 学習機能を利用したい場合には，以下を有効にします．                      */
  /* 学習機能とキャッシュは併用できますが，併用してもあまり意味はありません．*/
  /* 学習機能を利用すると，検索した instance をリストの先頭に持って          */
  /* くるようになります．これはこれで高速．                                  */
  /* 学習機能が無効だと，色をソートして格納するので，検索するときに，        */
  /* 色が格納されていなかったばあいに，すぐに検索を打ち切ることができるように*/
  /* なります．これはこれで高速．                                            */
  {"-studying",         ARGUMENTS_FLAG_ON,  (void *)(&studying_flag)},
  {"-no-studying",      ARGUMENTS_FLAG_OFF, (void *)(&studying_flag)},

  /* 以下を有効にすると，キャッシュを使用するようになります． */

  {"-cache",            ARGUMENTS_FLAG_ON,  (void *)(&cache_flag)},
  {"-no-cache",         ARGUMENTS_FLAG_OFF, (void *)(&cache_flag)},

  /* キャッシュのサイズを指定します． */
  {"-cache-size",       ARGUMENTS_INTEGER,  (void *)(&cache_size)},

  /* ハッシュのサイズを指定します． */
  {"-hash-number",      ARGUMENTS_INTEGER,  (void *)(&hash_number)},

  {NULL,                ARGUMENTS_NONE,     NULL}
};

int main(int argc, char * argv[])
{
  XFireworks xfireworks;

  Arguments_Read(&argc, argv, arguments);

  if (cache_size < 1) cache_size = 1;
  if (hash_number < 1) hash_number = 1;
  if (fine < 1) fine = 1;
  if (gradation < 1) gradation = 1;
  if (pieces_max_number < 1) pieces_max_number = 1;
  if (wait_time < 0) wait_time = 0;

  if (probability_magnification  < 0) probability_magnification  = 0;
  if (size_magnification         < 0) size_magnification         = 0;
  if (air_magnification          < 0) air_magnification          = 0;
  if (gravity_magnification      < 0) gravity_magnification      = 0;
  if (transmission_magnification < 0) transmission_magnification = 0;
  if (after_image_magnification  < 0) after_image_magnification  = 0;
  if (color_length_magnification < 0) color_length_magnification = 0;
  if (next_power_magnification   < 0) next_power_magnification   = 0;
  if (next_number_magnification  < 0) next_number_magnification  = 0;

  xfireworks = XFireworks_Create(display_name,
				 direct_draw,
				 background_color,
				 XFIREWORKS_CONF_DIRECTRY,
				 filename,
				 studying_flag,
				 cache_flag,
				 cache_size,
				 hash_number,
				 fine,
				 gradation,
				 probability_magnification,
				 size_magnification,
				 air_magnification,
				 gravity_magnification,
				 transmission_magnification,
				 after_image_magnification,
				 color_length_magnification,
				 next_power_magnification,
				 next_number_magnification);
  XFireworks_Start(xfireworks, pieces_max_number, wait_time);
  XFireworks_Destroy(xfireworks);

  exit (0);
}

/* End of Program */
