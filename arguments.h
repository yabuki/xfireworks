/*****************************************************************************/
/* 実行時のコマンドラインオプションの読み込み用のライブラリ                  */
/*****************************************************************************/

#ifndef _Arguments_h_INCLUDED_
#define _Arguments_h_INCLUDED_

#include <stdio.h>
#include <stdlib.h>

#define ARGUMENTS_NONE     0
#define ARGUMENTS_FLAG     1
#define ARGUMENTS_FLAG_ON  2
#define ARGUMENTS_FLAG_OFF 3
#define ARGUMENTS_INTEGER  4
#define ARGUMENTS_STRING   5
#define ARGUMENTS_FUNCTION 6

#define ARGUMENTS_TRUE  1
#define ARGUMENTS_FALSE 0

typedef struct _Argument {
  char * name;
  short int flag;
  void * value;
} Argument;

/*===========================================================================*/
/* コマンドラインオプションの読み込み(戻り値は処理したオプションの数)        */
/*===========================================================================*/

int Arguments_Read(int * argc, char ** argv, Argument arguments[]);

#endif
