#include "AfterImageP.h"

#include "etc.h"
#include "ColorGC.h"

/*===========================================================================*/
/* メンバの取得                                                              */
/*===========================================================================*/

int AfterImages_GetArraySize(AfterImages after_images)
{
  return (after_images->array_size);
}

int AfterImages_GetNumber(AfterImages after_images)
{
  return (after_images->number);
}

XArc * AfterImages_GetArcs(AfterImages after_images)
{
  return (after_images->arcs);
}

/*===========================================================================*/
/* オブジェクトの生成と削除                                                  */
/*===========================================================================*/

int AfterImages_Initialize(AfterImages after_images,
			   int size, int number, double * x, double * y,
			   int x_min, int y_min, int x_max, int y_max,
			   ObjList list)
{
  int i, n, r;

  if (number > after_images->array_size) return (1);

  r = size / 2;
  n = 0;
  for (i = 0; i < number; i++) {

    if ((x[i] < x_min) || (x[i] > x_max) || (y[i] > y_max)) {
      continue;
    }

    after_images->arcs[n].x = x[i] - r;
    after_images->arcs[n].y = y[i] - r;
    after_images->arcs[n].width  = size;
    after_images->arcs[n].height = size;
    after_images->arcs[n].angle1 = 0;
    after_images->arcs[n].angle2 = 360*64;
    n++;
  }
  after_images->number = n;
  after_images->list = list;
  after_images->current = ObjList_GetStartEdge(after_images->list);

  return (0);
}

AfterImages AfterImages_Create(int size, int number, double * x, double * y,
			       int x_min, int y_min, int x_max, int y_max,
			       ObjList list)
{
  AfterImages after_images;

  after_images = (AfterImages)malloc(sizeof(_AfterImages));
  if (after_images == NULL)
    Error("AfterImages_Create", "Cannot allocate memory");

  after_images->array_size = number;

  /* AfterImages オブジェクトは使い回しするので，小さいサイズのものは */
  /* メモリを無駄に食ってしまうので，メモリの節約のため，あまり小さな */
  /* サイズのものは作らないようにする．                               */
  if (after_images->array_size < 30) after_images->array_size = 30;

  if (after_images->array_size) {
    after_images->arcs =
      (XArc *)malloc(sizeof(XArc) * after_images->array_size);
    if (after_images->arcs == NULL)
      Error("AfterImages_Create", "Cannot allocate memory");
  } else after_images->arcs = NULL;

  AfterImages_Initialize(after_images, size, number,
			 x, y, x_min, y_min, x_max, y_max, list);

  return (after_images);
}

AfterImages AfterImages_Destroy(AfterImages after_images)
{
  if (!after_images) return (NULL);

  if (after_images->arcs) free(after_images->arcs);

  free(after_images);

  return (NULL);
}

/*===========================================================================*/
/* 現在の色の値をGCで返し，current を一つ先に進める．(最後には NULL を返す)  */
/*===========================================================================*/

GC AfterImages_GetGC(AfterImages after_images)
{
  ColorGC color_gc;
  GC gc;

  if (after_images->current == NULL) return (NULL);
  if (ObjList_IsEnd(after_images->list, after_images->current))
    return (NULL);

  after_images->current = ObjListData_GetNext(after_images->current);

  color_gc = (ColorGC)ObjListData_GetObj(after_images->current);
  gc = ColorGC_GetGC(color_gc);

  return (gc);
}

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
