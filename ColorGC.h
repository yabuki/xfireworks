/*****************************************************************************/
/* ColorGC GC�ȿ��δ������䤤��碌�ѤΥ饤�֥��                            */
/*****************************************************************************/

#ifndef _XFIREWORKS_ColorGC_h_INCLUDED_
#define _XFIREWORKS_ColorGC_h_INCLUDED_

typedef struct _ColorGCDatabase * ColorGCDatabase;
typedef struct _ColorGC * ColorGC;

#include <X11/Xlib.h>

#include "Disp.h"
#include "Obj.h"

/*===========================================================================*/
/* ColorGC ��Ϣ                                                              */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Create(ColorGCDatabase database, XColor color);
ColorGC ColorGC_CreateFromColorGC(ColorGCDatabase database, ColorGC c);

/*---------------------------------------------------------------------------*/
/* RGB �ͤ����������롥                                                      */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_CreateFromRGB(ColorGCDatabase database,
			      int red, int green, int blue);

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Destroy(ColorGC color_gc);

/*===========================================================================*/
/* ʸ�����ɤ߹����Ѵؿ�                                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ����GC�Υǡ����١������� name ��Ϳ����줿̾���ο��򸡺����롥            */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_CreateFromCharacters(ColorGCDatabase database, char * name);

/*---------------------------------------------------------------------------*/
/* XColor ��¤�Τμ���                                                       */
/*---------------------------------------------------------------------------*/

XColor ColorGC_GetColor(ColorGC color_gc);

/*---------------------------------------------------------------------------*/
/* �ԥ������ͤμ���                                                          */
/*---------------------------------------------------------------------------*/

unsigned long ColorGC_GetPixel(ColorGC color_gc);

/*---------------------------------------------------------------------------*/
/* GC �μ���                                                                 */
/*---------------------------------------------------------------------------*/

GC ColorGC_GetGC(ColorGC color_gc);

/*---------------------------------------------------------------------------*/
/* �������뤵�μ���                                                          */
/*---------------------------------------------------------------------------*/

int GetBrightness(XColor color);

/*===========================================================================*/
/* GC �Υꥹ�Ȥκ���                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* GC �Υꥹ�Ȥκ���                                                         */
/* �ǡ���ʸ���󤫤� ColorGC �Υꥹ�Ȥ���������֤���                         */
/* �ǡ���ʸ����Υե����ޥåȤϰʲ��ΤȤ��ꡥ                                */
/* "[�������ȥ��顼1] [����ɥ��顼1] [Ĺ��1] [�������ȥ��顼2] ...\0"       */
/* ��) "red green 10 green blue 20 blue none 10\0"                           */
/*---------------------------------------------------------------------------*/

ObjList CreateColorGCListFromCharacters(ColorGCDatabase database,
					char * data,
					int after_image_length,
					int fine,
					int color_length_mag);

/*===========================================================================*/
/* GC �Υǡ����١���                                                         */
/*===========================================================================*/

ColorGCDatabase ColorGCDatabase_Create(Disp disp,
				       int studying_flag,
				       int cache_flag,
				       int cache_size,
				       int hash_number,
				       char * background,
				       int gradation);
ColorGCDatabase ColorGCDatabase_Destroy(ColorGCDatabase database);
ColorGC ColorGCDatabase_GetBackgroundColorGC(ColorGCDatabase database);

/*---------------------------------------------------------------------------*/
/* ���塼�˥���                                                            */
/*---------------------------------------------------------------------------*/

void ColorGCDatabase_OutputHashStatus(ColorGCDatabase database);

#endif

/*****************************************************************************/
/* End of File                                                               */
/*****************************************************************************/