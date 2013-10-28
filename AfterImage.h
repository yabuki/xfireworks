/*****************************************************************************/
/* 花火の玉の残像を扱うクラス                                                */
/*****************************************************************************/

#ifndef _XFIREWORKS_AfterImage_h_INCLUDED_
#define _XFIREWORKS_AfterImage_h_INCLUDED_

typedef struct _AfterImages * AfterImages;

#include <X11/Xlib.h>

#include "Obj.h"

/*===========================================================================*/
/* メンバの取得                                                              */
/*===========================================================================*/

int AfterImages_GetArraySize(AfterImages after_images);
int AfterImages_GetNumber(AfterImages after_images);
XArc * AfterImages_GetArcs(AfterImages after_images);

/*===========================================================================*/
/* オブジェクトの生成と削除                                                  */
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
/* 現在の色の値をGCで返し，current を一つ先に進める．(最後には NULL を返す)  */
/*===========================================================================*/

GC AfterImages_GetGC(AfterImages after_images);

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
