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
/* ここから                                                                  */
/*****************************************************************************/

#include "Stream.h"

/*****************************************************************************/
/* ヘッダファイルのインクルード(プライベートなもの)                          */
/*****************************************************************************/

/* None */

/*****************************************************************************/
/* 型の定義(外部に公開しないもの)                                            */
/*****************************************************************************/

/*===========================================================================*/
/* 読み込み元の本体のタイプ                                                  */
/*===========================================================================*/

typedef enum {
  STREAM_TYPE_NONE,         /* 読み込み元が無い */
  STREAM_TYPE_FILE_POINTER, /* ファイルポインタから読み込む */
  STREAM_TYPE_CHARACTERS    /* 文字列(char * 型)から読み込む */
} StreamType;

/*===========================================================================*/
/* 読み込み元の本体                                                          */
/*===========================================================================*/

typedef union {
  void * dummy;        /* 読み込み元が無い */
  FILE * file_pointer; /* ファイルポインタから読み込む */
  char * characters;   /* 文字列(char * 型)から読み込む */
} StreamObject;

/*===========================================================================*/
/* unget 用バッファ                                                          */
/*===========================================================================*/

typedef struct _UngetBuffer {
  char character;
  struct _UngetBuffer * next;
} UngetBuffer;

/*===========================================================================*/
/* Stream クラスの本体                                                       */
/*===========================================================================*/

typedef struct _Stream {

  UngetBuffer * unget_buffer; /* バッファ(何文字でも unget できる) */

  int end_flag; /* 終了のフラグ(読み込むものが無いとき真) */

  CreateStreamFrom from;
  /* 作成元のオブジェクトのタイプ(ファイル名，文字列など) */

  StreamType type; /* 読み込み元の本体のタイプ(ファイル，String 型など) */

  StreamObject object; /* 読み込み元の本体(ファイル，String 型など) */

} _Stream;

/*****************************************************************************/
/* ここまで                                                                  */
/*****************************************************************************/

#endif

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
