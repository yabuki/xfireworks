#ifndef _XFIREWORKS_XFireworksP_h_INCLUDED_
#define _XFIREWORKS_XFireworksP_h_INCLUDED_

#include "XFireworks.h"
#include "Obj.h"
#include "Disp.h"
#include "Disp.h"
#include "ColorGC.h"
#include "Calculator.h"

/* �ʲ���ͭ���ˤ���ȡ��ϥå���λ����٤�ɽ�����뤳�Ȥ��Ǥ��ޤ� */
/* #define OUTPUT_HASH_STATUS */

typedef struct _XFireworks {
  ObjList piece_class_list;

  /* �ᥤ��롼�פǤ� malloc() �θƤӽФ��򸺤餹����ˡ�                */
  /* ���ٺ������� Pieces, AfterImages ���֥������Ȥϡ��Ȥ����äƤ�       */
  /* ���������free �Υꥹ�Ȥ��ɲä�����¸���Ƥ������Ȥ��󤹤褦�ˤ��롥 */
  ObjList pieces_list;      /* ������� Pieces ���֥������ȤΥꥹ�� */
  ObjList free_pieces_list; /* ���Ѥ��Ƥʤ� Pieces ���֥������ȤΥꥹ�� */
  ObjList after_images_list;      /* ������λ������֥������ȤΥꥹ�� */
  ObjList free_after_images_list; /* ���Ѥ��Ƥʤ��������֥������ȤΥꥹ�� */

  Disp disp;
  ColorGCDatabase color_gc_database;
  Calculator calculator; /* sin, cos �η׻��� */
} _XFireworks;

#endif /* _XFIREWORKS_XFireworksP_h_INCLUDED_ */
