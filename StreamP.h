/*****************************************************************************/
/* Stream Class Library Copyright (c) 1999-2000 Sakai Hiroaki.               */
/* All Rights Reserved.                                                      */
/*===========================================================================*/
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation; either version 2, or (at your option)       */
/* any later version.                                                        */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; see the file COPYING.  If not, write to          */
/* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.     */
/*****************************************************************************/

#ifndef _STREAMP_H_INCLUDED_
#define _STREAMP_H_INCLUDED_

/*****************************************************************************/
/* ��������                                                                  */
/*****************************************************************************/

#include "Stream.h"

/*****************************************************************************/
/* �إå��ե�����Υ��󥯥롼��(�ץ饤�١��Ȥʤ��)                          */
/*****************************************************************************/

/* None */

/*****************************************************************************/
/* �������(�����˸������ʤ����)                                            */
/*****************************************************************************/

/*===========================================================================*/
/* �ɤ߹��߸������ΤΥ�����                                                  */
/*===========================================================================*/

typedef enum {
  STREAM_TYPE_NONE,         /* �ɤ߹��߸���̵�� */
  STREAM_TYPE_FILE_POINTER, /* �ե�����ݥ��󥿤����ɤ߹��� */
  STREAM_TYPE_CHARACTERS    /* ʸ����(char * ��)�����ɤ߹��� */
} StreamType;

/*===========================================================================*/
/* �ɤ߹��߸�������                                                          */
/*===========================================================================*/

typedef union {
  void * dummy;        /* �ɤ߹��߸���̵�� */
  FILE * file_pointer; /* �ե�����ݥ��󥿤����ɤ߹��� */
  char * characters;   /* ʸ����(char * ��)�����ɤ߹��� */
} StreamObject;

/*===========================================================================*/
/* unget �ѥХåե�                                                          */
/*===========================================================================*/

typedef struct _UngetBuffer {
  char character;
  struct _UngetBuffer * next;
} UngetBuffer;

/*===========================================================================*/
/* Stream ���饹������                                                       */
/*===========================================================================*/

typedef struct _Stream {

  UngetBuffer * unget_buffer; /* �Хåե�(��ʸ���Ǥ� unget �Ǥ���) */

  int end_flag; /* ��λ�Υե饰(�ɤ߹����Τ�̵���Ȥ���) */

  CreateStreamFrom from;
  /* �������Υ��֥������ȤΥ�����(�ե�����̾��ʸ����ʤ�) */

  StreamType type; /* �ɤ߹��߸������ΤΥ�����(�ե����롤String ���ʤ�) */

  StreamObject object; /* �ɤ߹��߸�������(�ե����롤String ���ʤ�) */

} _Stream;

/*****************************************************************************/
/* �����ޤ�                                                                  */
/*****************************************************************************/

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
