#ifndef _XFIREWORKS_AfterImageP_h_INCLUDED_
#define _XFIREWORKS_AfterImageP_h_INCLUDED_

#include "AfterImage.h"

typedef struct _AfterImages {
  int array_size; /* 配列のサイズ */
  int number;
  /* 残像の数(メインループでの malloc() の呼び出し数を減らすため，    */
  /* AfterImage オブジェクトは使い回しするので，size != number になる */
  /* 場合がある)                                                      */

  XArc * arcs;
  /* 残像の配列．XFillArcs()でまとめて描画できるように，XArc の配列にする */

  ObjList list; /* 残像の色変化(だんだん背景色に近付いていく)のリスト*/

  ObjListData current; /* 残像の現在の色(listメンバ中の位置)を示す */
} _AfterImages;

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
