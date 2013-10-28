#ifndef _XFIREWORKS_DispP_h_INCLUDED_
#define _XFIREWORKS_DispP_h_INCLUDED_

#include "Disp.h"

typedef struct _Disp {
  Display * display;
  int screen;
  Window root_window;
  Pixmap pixmap;
  Colormap colormap;
  int width, height;
  unsigned int depth;
  int direct_draw;
} _Disp;

/*****************************************************************************/
/* ここまで                                                                  */
/*****************************************************************************/

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
