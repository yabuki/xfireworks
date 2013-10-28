#ifndef _XFIREWORKS_PieceP_h_INCLUDED_
#define _XFIREWORKS_PieceP_h_INCLUDED_

#include "Piece.h"

typedef struct _PieceClass {
  char * name;            /* ̾�� */
  int size;               /* �̤��礭��(ľ��) */
  double probability;     /* �и������Ψ */
  double air;             /* ������ */
  double gravity;         /* ���ϲ�®�� */
  double transmission;    /* ��ȯ���ˡ����ζ̤���®�٤���������뤫 */
  int after_image_length; /* ������Ĺ�� */
  int fine;               /* ��ư�κ٤���(�տ��ϡ��̤ΰ�ư�Υ��ƥå׿��ˤʤ�)*/
  double step;            /* �̤ΰ�ư�Υ��ƥå׿���(fine �εտ�) */

  ObjList gc_list_list; /* AfterImages �ο��Ѳ��Ѥ�GC�Υꥹ�ȤΥꥹ�� */
  ObjList next_list; /* PieceNext �Υꥹ�ȡ���ȯ������������̤Υꥹ�� */
} _PieceClass;

typedef struct _PieceNext {
  PieceClass piece_class;
  double power; /* ��ȯ�ζ��� */
  int number;   /* ��ȯ�ΤȤ��˽Ф��̤ο� */
} _PieceNext;

typedef struct _Pieces {
  PieceClass piece_class;

  int array_size; /* ����Υ����� */
  int number;
  /* Pieces �ο�(�ᥤ��롼�פǤ� malloc() �θƤӽФ����򸺤餹���ᡤ        */
  /* Pieces ���֥������ȤϻȤ��󤷤���Τǡ�size != number �ˤʤ��礬����) */

  double * x;  /* ���� */
  double * y;  /* ���� */
  double * z;  /* ����(���Ԥ�) */
  double * vx; /* ®�� */
  double * vy; /* ®�� */
  double * vz; /* ®��(���Ԥ�) */

  ObjListData gc_list; /* �����Ѳ��ѡ����ߤο���ؤ� */
} _Pieces;

#endif /* _XFIREWORKS_PieceP_h_INCLUDED_ */

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
