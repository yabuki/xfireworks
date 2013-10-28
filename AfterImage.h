/*****************************************************************************/
/* �ֲФζ̤λ����򰷤����饹                                                */
/*****************************************************************************/

#ifndef _XFIREWORKS_AfterImage_h_INCLUDED_
#define _XFIREWORKS_AfterImage_h_INCLUDED_

typedef struct _AfterImages * AfterImages;

#include <X11/Xlib.h>

#include "Obj.h"

/*===========================================================================*/
/* ���Фμ���                                                              */
/*===========================================================================*/

int AfterImages_GetArraySize(AfterImages after_images);
int AfterImages_GetNumber(AfterImages after_images);
XArc * AfterImages_GetArcs(AfterImages after_images);

/*===========================================================================*/
/* ���֥������Ȥ������Ⱥ��                                                  */
/*===========================================================================*/

int AfterImages_Initialize(AfterImages after_images,
			   int size, int number, double * x, double * y,
			   int x_min, int y_min, int x_max, int y_max,
			   ObjList list);
AfterImages AfterImages_Create(int size, int number, double * x, double * y,
			       int x_min, int y_min, int x_max, int y_max,
			       ObjList list);
AfterImages AfterImages_Destroy(AfterImages after_images);

/*===========================================================================*/
/* ���ߤο����ͤ�GC���֤���current ������˿ʤ�롥(�Ǹ�ˤ� NULL ���֤�)  */
/*===========================================================================*/

GC AfterImages_GetGC(AfterImages after_images);

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
