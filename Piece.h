#ifndef _XFIREWORKS_Piece_h_INCLUDED_
#define _XFIREWORKS_Piece_h_INCLUDED_

typedef struct _PieceClass * PieceClass;
typedef struct _PieceNext * PieceNext;
typedef struct _Pieces * Pieces;

#include "Obj.h"
#include "Stream.h"
#include "ColorGC.h"
#include "Calculator.h"

/*===========================================================================*/
/* オブジェクトのメンバの取得                                                */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* PieceClass クラス                                                         */
/*---------------------------------------------------------------------------*/

char * PieceClass_GetName(PieceClass piece_class);
double PieceClass_GetProbability(PieceClass piece_class);
ObjList PieceClass_GetGCListList(PieceClass piece_class);
ObjList PieceClass_GetNextList(PieceClass piece_class);

/*---------------------------------------------------------------------------*/
/* PieceNext クラス                                                          */
/*---------------------------------------------------------------------------*/

PieceClass PieceNext_GetPieceClass(PieceNext piece_next);
double PieceNext_GetPower(PieceNext piece_next);
int PieceNext_GetNumber(PieceNext piece_next);

/*---------------------------------------------------------------------------*/
/* Pieces クラス                                                             */
/*---------------------------------------------------------------------------*/

PieceClass Pieces_GetPieceClass(Pieces pieces);
int Pieces_GetArraySize(Pieces pieces);
int Pieces_GetNumber(Pieces pieces);
double * Pieces_GetX(Pieces pieces);
double * Pieces_GetY(Pieces pieces);
double * Pieces_GetZ(Pieces pieces);
double * Pieces_GetVx(Pieces pieces);
double * Pieces_GetVy(Pieces pieces);
double * Pieces_GetVz(Pieces pieces);
int Pieces_GetSize(Pieces pieces);
ObjList Pieces_GetGCList(Pieces pieces);

/*===========================================================================*/
/* PieceClass オブジェクトの作成・削除用                                     */
/*===========================================================================*/

PieceClass PieceClass_Create(char * name,
			     int size,
			     double probability,
			     double air,
			     double gravity,
			     double transmission,
			     int after_image_length,
			     int fine,
			     ObjList gc_list_list,
			     ObjList next_list,
			     Disp disp);
PieceClass PieceClass_Destroy(PieceClass piece_class);

/*===========================================================================*/
/* PieceNext オブジェクトの作成・削除用                                      */
/*===========================================================================*/

PieceNext PieceNext_Create(PieceClass piece_class, double power, int n);
PieceNext PieceNext_Destroy(PieceNext next);

/*===========================================================================*/
/* データの読み込み                                                          */
/*===========================================================================*/

ObjList PieceClassList_CreateFromStream(Disp disp,
					ColorGCDatabase database,
					Stream stream,
					int fine,
					int probability_mag,
					int size_mag,
					int air_mag,
					int gravity_mag,
					int transmission_mag,
					int after_image_mag,
					int color_length_mag,
					int next_power_mag,
					int next_number_mag);

/*===========================================================================*/
/* Pieces オブジェクトの操作                                                 */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* Pieces オブジェクトの初期化                                               */
/*---------------------------------------------------------------------------*/

int Pieces_Initialize(Pieces pieces,
		      PieceClass piece_class, double power,
		      int number, int n,
		      double * x, double * y, double * z,
		      double * vx, double * vy, double * vz,
		      int x_min, int y_min, int x_max, int y_max,
		      Calculator calculator);

/*---------------------------------------------------------------------------*/
/* Pieces オブジェクトの生成                                                 */
/*---------------------------------------------------------------------------*/

Pieces Pieces_Create(PieceClass piece_class, double power, int number, int n,
		     double * x, double * y, double * z,
		     double * vx, double * vy, double * vz,
		     int x_min, int y_min, int x_max, int y_max,
		     Calculator calculator);

/*---------------------------------------------------------------------------*/
/* Piece オブジェクトの削除                                                  */
/*---------------------------------------------------------------------------*/

Pieces Pieces_Destroy(Pieces pieces);

/*---------------------------------------------------------------------------*/
/* Pieces オブジェクトの移動                                                 */
/*---------------------------------------------------------------------------*/

int Pieces_Move(Pieces pieces,
		int x_min, int y_min, int x_max, int y_max, int size);

#endif

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
