#ifndef _XFIREWORKS_XFireworksP_h_INCLUDED_
#define _XFIREWORKS_XFireworksP_h_INCLUDED_

#include "XFireworks.h"
#include "Obj.h"
#include "Disp.h"
#include "Disp.h"
#include "ColorGC.h"
#include "Calculator.h"

/* 以下を有効にすると，ハッシュの使用度を表示することができます */
/* #define OUTPUT_HASH_STATUS */

typedef struct _XFireworks {
  ObjList piece_class_list;

  /* メインループでの malloc() の呼び出しを減らすために，                */
  /* 一度作成した Pieces, AfterImages オブジェクトは，使い終っても       */
  /* 削除せず，free のリストに追加して保存しておき，使い回すようにする． */
  ObjList pieces_list;      /* 使用中の Pieces オブジェクトのリスト */
  ObjList free_pieces_list; /* 使用してない Pieces オブジェクトのリスト */
  ObjList after_images_list;      /* 使用中の残像オブジェクトのリスト */
  ObjList free_after_images_list; /* 使用してない残像オブジェクトのリスト */

  Disp disp;
  ColorGCDatabase color_gc_database;
  Calculator calculator; /* sin, cos の計算用 */
} _XFireworks;

#endif /* _XFIREWORKS_XFireworksP_h_INCLUDED_ */
