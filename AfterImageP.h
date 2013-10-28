#ifndef _XFIREWORKS_AfterImageP_h_INCLUDED_
#define _XFIREWORKS_AfterImageP_h_INCLUDED_

#include "AfterImage.h"

typedef struct _AfterImages {
  int array_size; /* ����Υ����� */
  int number;
  /* �����ο�(�ᥤ��롼�פǤ� malloc() �θƤӽФ����򸺤餹���ᡤ    */
  /* AfterImage ���֥������ȤϻȤ��󤷤���Τǡ�size != number �ˤʤ� */
  /* ��礬����)                                                      */

  XArc * arcs;
  /* ����������XFillArcs()�ǤޤȤ������Ǥ���褦�ˡ�XArc ������ˤ��� */

  ObjList list; /* �����ο��Ѳ�(��������طʿ��˶��դ��Ƥ���)�Υꥹ��*/

  ObjListData current; /* �����θ��ߤο�(list������ΰ���)�򼨤� */
} _AfterImages;

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
