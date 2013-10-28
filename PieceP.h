#ifndef _XFIREWORKS_PieceP_h_INCLUDED_
#define _XFIREWORKS_PieceP_h_INCLUDED_

#include "Piece.h"

typedef struct _PieceClass {
  char * name;            /* 名称 */
  int size;               /* 玉の大きさ(直径) */
  double probability;     /* 出現する確率 */
  double air;             /* 空気抵抗 */
  double gravity;         /* 重力加速度 */
  double transmission;    /* 爆発時に，前の玉から速度が何％伝わるか */
  int after_image_length; /* 残像の長さ */
  int fine;               /* 移動の細かさ(逆数は，玉の移動のステップ数になる)*/
  double step;            /* 玉の移動のステップ数．(fine の逆数) */

  ObjList gc_list_list; /* AfterImages の色変化用のGCのリストのリスト */
  ObjList next_list; /* PieceNext のリスト．爆発時に生成する玉のリスト */
} _PieceClass;

typedef struct _PieceNext {
  PieceClass piece_class;
  double power; /* 爆発の強さ */
  int number;   /* 爆発のときに出す玉の数 */
} _PieceNext;

typedef struct _Pieces {
  PieceClass piece_class;

  int array_size; /* 配列のサイズ */
  int number;
  /* Pieces の数(メインループでの malloc() の呼び出し数を減らすため，        */
  /* Pieces オブジェクトは使い回しするので，size != number になる場合がある) */

  double * x;  /* 位置 */
  double * y;  /* 位置 */
  double * z;  /* 位置(奥行き) */
  double * vx; /* 速度 */
  double * vy; /* 速度 */
  double * vz; /* 速度(奥行き) */

  ObjListData gc_list; /* 色の変化用．現在の色を指す */
} _Pieces;

#endif /* _XFIREWORKS_PieceP_h_INCLUDED_ */

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
