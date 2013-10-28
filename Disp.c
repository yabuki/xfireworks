#include "DispP.h"

#include "etc.h"

Display * Disp_GetDisplay(Disp disp) { return (disp->display); }
Colormap Disp_GetColormap(Disp disp) { return (disp->colormap); }
int Disp_GetWidth( Disp disp) { return (disp->width ); }
int Disp_GetHeight(Disp disp) { return (disp->height); }

/*===========================================================================*/
/* ���μ���                                                                  */
/*===========================================================================*/

unsigned long Disp_GetPixel(Disp disp, char * color_name)
{
  XColor c0, c1;
  XAllocNamedColor(disp->display, disp->colormap, color_name, &c0, &c1);
  return (c0.pixel);
}

/*===========================================================================*/
/* GC �κ����Ȳ���                                                           */
/*===========================================================================*/

GC Disp_CreateGC(Disp disp)
{
  GC gc;
  gc = XCreateGC(disp->display, disp->root_window, 0, 0);
  return (gc);
}

int Disp_DestroyGC(Disp disp, GC gc)
{
  XFreeGC(disp->display, gc);
  return (0);
}

/*===========================================================================*/
/* ���֥������Ȥ������Ⱥ��                                                  */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
/*---------------------------------------------------------------------------*/

Disp Disp_Create(char * display_name, int direct_draw)
{
  Disp disp;
  unsigned long int pixel;

  disp = (Disp)malloc(sizeof(_Disp));
  if (!disp) Error("Disp_Create", "Cannot allocate memory.");

  disp->display = XOpenDisplay(display_name);
  if (disp->display == NULL) Error("Disp_Create", "Cannot open display.");

  disp->screen = DefaultScreen(disp->display);
  disp->root_window = RootWindow(disp->display, disp->screen);
  disp->colormap = DefaultColormap(disp->display, disp->screen);
  disp->width  = DisplayWidth( disp->display, disp->screen);
  disp->height = DisplayHeight(disp->display, disp->screen);
  disp->depth = DefaultDepth(disp->display, disp->screen);

  disp->direct_draw = direct_draw;

  if (disp->direct_draw) {
    disp->pixmap = NULL;
  } else {
    disp->pixmap = XCreatePixmap(disp->display, disp->root_window,
				 disp->width, disp->height, disp->depth);
  }

  return (disp);
}

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

Disp Disp_Destroy(Disp disp)
{
  if (!disp) return (NULL);

  if (disp->pixmap) XFreePixmap(disp->display, disp->pixmap);

  if (disp->display) XCloseDisplay(disp->display);

  free(disp);

  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* �ե�å���                                                                */
/*---------------------------------------------------------------------------*/

int Disp_Flush(Disp disp)
{
  XEvent ev;

  if (!disp->direct_draw) {
#if 0
    XSetWindowBackgroundPixmap(disp->display, disp->root_window, disp->pixmap);
#endif
    XClearWindow(disp->display, disp->root_window);
  }

  /* ���٥�ȤΥե�å��� */
  while (XPending(disp->display)) {
    XNextEvent(disp->display, &ev);
  }

  XFlush(disp->display);

  return (0);
}

/*---------------------------------------------------------------------------*/
/* X �����ФȤ� Sync��(���٥�ȥ��塼���Ѵ�����)                             */
/*---------------------------------------------------------------------------*/

int Disp_Sync(Disp disp)
{
  Disp_Flush(disp); /* �ꥯ�����ȤΥե�å��� */

#if 1
  XSync(disp->display, True); /* ���٥�ȥ��塼��Υ��٥�Ȥ��Ѵ����� */
#else
  XSync(disp->display, False); /* ���٥�ȥ��塼��Υ��٥�Ȥ��Ѵ����ʤ� */
#endif

  return (0);
}

/*---------------------------------------------------------------------------*/
/* �ԥå����ޥåפΥ��ꥢ                                                    */
/*---------------------------------------------------------------------------*/

int Disp_ClearPixmap(Disp disp, GC gc)
{
  if (disp->pixmap)
    XFillRectangle(disp->display, disp->pixmap, gc,
		   0, 0, disp->width, disp->height);
  return (0);
}

/*---------------------------------------------------------------------------*/
/* �ǥ����ץ쥤�Υ��ꥢ                                                      */
/*---------------------------------------------------------------------------*/

int Disp_ClearDisplay(Disp disp, unsigned long pixel)
{
  if (disp->direct_draw) {
    XSetWindowBackground(disp->display, disp->root_window, pixel);
  } else {
    XSetWindowBackgroundPixmap(disp->display, disp->root_window, disp->pixmap);
  }
  XClearWindow(disp->display, disp->root_window);

  Disp_Flush(disp);

  return (0);
}

/*---------------------------------------------------------------------------*/
/* �ߤ�����                                                                  */
/*---------------------------------------------------------------------------*/

int Disp_DrawFilledCircle(Disp disp, GC gc, int x, int y, int r)
{
  if (x + r < 0 || disp->width  - 1 < x - r ||
      y + r < 0 || disp->height - 1 < y - r) return (0);
  if (disp->direct_draw)
    XFillArc(disp->display, disp->root_window, gc,
	     x - r, y - r, r * 2 + 1, r * 2 + 1, 0, 360 * 64);
  else
    XFillArc(disp->display, disp->pixmap, gc,
	     x - r, y - r, r * 2 + 1, r * 2 + 1, 0, 360 * 64);
  return (0);
}

int Disp_DrawFilledCircles(Disp disp, GC gc, XArc * arcs, int n)
{
  if (n > 0) {
    if (disp->direct_draw)
      XFillArcs(disp->display, disp->root_window, gc, arcs, n);
    else
      XFillArcs(disp->display, disp->pixmap, gc, arcs, n);
  }

  return (0);
}

/*****************************************************************************/
/* �����ޤ�                                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
