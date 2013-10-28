#ifndef _XFIREWORKS_XFireworks_h_INCLUDED_
#define _XFIREWORKS_XFireworks_h_INCLUDED_

typedef struct _XFireworks * XFireworks;

#include <stdio.h>
#include <stdlib.h>

/*===========================================================================*/
/* ���֥������Ȥ������Ⱥ��                                                  */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
/*---------------------------------------------------------------------------*/

XFireworks XFireworks_Create(char * display_name,
			     int direct_draw,
			     char * background_color,
			     char * dirname,
			     char * filename,
			     int studying_flag,
			     int cache_flag,
			     int cache_size,
			     int hash_number,
			     int fine,
			     int gradation,
			     int probability_magnification,
			     int size_magnification,
			     int air_magnification,
			     int gravity_magnification,
			     int transmission_magnification,
			     int after_image_magnification,
			     int color_length_magnification,
			     int next_power_magnification,
			     int next_number_magnification);

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

XFireworks XFireworks_Destroy(XFireworks xfireworks);

/*===========================================================================*/
/* �ֲФΥ�������                                                            */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* �ֲФΥ�������                                                            */
/*---------------------------------------------------------------------------*/

int XFireworks_Start(XFireworks xfireworks,
		     int pieces_max_number,
		     int wait_time);

#endif /* _XFIREWORKS_XFireworks_h_INCLUDED_ */
