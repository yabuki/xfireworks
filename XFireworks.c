#include "XFireworksP.h"

#include <signal.h>

#include "etc.h"
#include "Piece.h"
#include "AfterImage.h"

#include "xfireworks_conf.h"

/*===========================================================================*/
/* �����ǻ��Ѥ���ؿ�                                                        */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ����ե�����졼������ѤΥե������õ����̵����Хǥե���ȤΥǡ�����    */
/* �ɤ߹��ࡥ                                                                */
/*---------------------------------------------------------------------------*/

static Stream Stream_CreateFromFileOrCharacters(char * dirname,
						char * filename,
						char * default_xfireworks_conf)
{
  Stream stream;
  char * work;

  stream = Stream_CreateFromFile(filename);
  if (stream != NULL) return (stream);

  /* dirname �� "" ���ä��Ȥ��ˡ�./[filename] �Τ褦�ʥե������ */
  /* Ĵ�٤�Τǡ�1 �ǤϤʤ� 3 ��ä��Ƥ��롥                     */
  work = (char *)malloc(sizeof(char)
			* (StringLen(dirname) + StringLen(filename) + 3));

  StringCpy(work, "./");
  StringCat(work, filename);

  stream = Stream_CreateFromFile(work);
  if (stream != NULL) goto stream_return;

  StringCpy(work, dirname);
  StringCat(work, "/");
  StringCat(work, filename);

  stream = Stream_CreateFromFile(work);
  if (stream != NULL) goto stream_return;

  stream = Stream_CreateFromCharacters(default_xfireworks_conf);

stream_return:

  free(work);
  return (stream);
}

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
			     int next_number_magnification)
{
  XFireworks xfireworks;
  Stream stream;
  ColorGC color_gc;

  xfireworks = (XFireworks)malloc(sizeof(_XFireworks));
  if (!xfireworks) Error("XFireworks_Create", "Cannot allocate mamory");

  /* �������� */
  InitializeRand();

  /* �ǥ����ץ쥤����� */
  xfireworks->disp = Disp_Create(display_name, direct_draw);

  /* GC�Υǡ����١�������� */
  xfireworks->color_gc_database = ColorGCDatabase_Create(xfireworks->disp,
							 studying_flag,
							 cache_flag,
							 cache_size,
							 hash_number,
							 background_color,
							 gradation);

  /* �ǥ����ץ쥤�Υ��ꥢ */
  color_gc =
    ColorGCDatabase_GetBackgroundColorGC(xfireworks->color_gc_database);
  if (!direct_draw)
    Disp_ClearPixmap(xfireworks->disp, ColorGC_GetGC(color_gc));
  Disp_ClearDisplay(xfireworks->disp, ColorGC_GetPixel(color_gc));

  /* PieceClass ���ɤ߹��� */
  stream = 
    Stream_CreateFromFileOrCharacters(dirname, filename,
				      default_xfireworks_conf);
  xfireworks->piece_class_list =
    PieceClassList_CreateFromStream(xfireworks->disp,
				    xfireworks->color_gc_database,
				    stream,
				    fine,
				    probability_magnification,
				    size_magnification,
				    air_magnification,
				    gravity_magnification,
				    transmission_magnification,
				    after_image_magnification,
				    color_length_magnification,
				    next_power_magnification,
				    next_number_magnification);
  Stream_Destroy(stream);

  xfireworks->pieces_list            = ObjList_Create();
  xfireworks->free_pieces_list       = ObjList_Create();
  xfireworks->after_images_list      = ObjList_Create();
  xfireworks->free_after_images_list = ObjList_Create();

  /* �ϥå����������ɽ��(���塼�˥���) */
#ifdef OUTPUT_HASH_STATUS
  ColorGCDatabase_OutputHashStatus(xfireworks->color_gc_database);
#endif

  xfireworks->calculator = Calculator_Create(CALCULATOR_DEFAULT_DEGREE);

  return (xfireworks);
}

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

XFireworks XFireworks_Destroy(XFireworks xfireworks)
{
  ColorGC color_gc;

  if (!xfireworks) return (NULL);

  /* �ǥ����ץ쥤�Υ��ꥢ */
  color_gc =
    ColorGCDatabase_GetBackgroundColorGC(xfireworks->color_gc_database);
  Disp_ClearDisplay(xfireworks->disp, ColorGC_GetPixel(color_gc));

  if (xfireworks->calculator) Calculator_Destroy(xfireworks->calculator);

  if (xfireworks->free_after_images_list)
    ObjList_Destroy(xfireworks->free_after_images_list);
  if (xfireworks->after_images_list)
    ObjList_Destroy(xfireworks->after_images_list);
  if (xfireworks->free_pieces_list)
    ObjList_Destroy(xfireworks->free_pieces_list);
  if (xfireworks->pieces_list)
    ObjList_Destroy(xfireworks->pieces_list);

  if (xfireworks->piece_class_list)
    ObjList_Destroy(xfireworks->piece_class_list);

  if (xfireworks->color_gc_database)
    ColorGCDatabase_Destroy(xfireworks->color_gc_database);
  if (xfireworks->disp) Disp_Destroy(xfireworks->disp);

  free(xfireworks);

  return (NULL);
}

/*===========================================================================*/
/* �ֲФΥ�������                                                            */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* PieceClass ��ɽ��(�ǥХå���)                                             */
/*---------------------------------------------------------------------------*/

static int XFireworks_DrawAllPieceClasses(XFireworks xfireworks)
{
  PieceClass piece_class;
  ObjList gc_list_list;
  ObjList gc_list;
  ObjListData a, b, c;
  GC gc;

  int x, y;

  x = 10;
  y = 10;

  for (a = ObjList_GetStart(xfireworks->piece_class_list);
       !ObjList_IsEndEdge(xfireworks->piece_class_list, a);
       a = ObjListData_GetNext(a)) {
    piece_class = (PieceClass)ObjListData_GetObj(a);
    gc_list_list = PieceClass_GetGCListList(piece_class);

    for (b = ObjList_GetStart(gc_list_list);
	 !ObjList_IsEndEdge(gc_list_list, b);
	 b = ObjListData_GetNext(b)) {

      gc_list = ObjListData_GetObj(b);

      x = 10;
      for (c = ObjList_GetStart(gc_list);
	   !ObjList_IsEndEdge(gc_list, c);
	   c = ObjListData_GetNext(c)) {
	gc = ColorGC_GetGC(ObjListData_GetObj(c));
	Disp_DrawFilledCircle(xfireworks->disp, gc, x, y, 5);
	Disp_Flush(xfireworks->disp);
	x += 10;
      }
      y += 10;
    }
  }
  return (0);
}

/*---------------------------------------------------------------------------*/
/* �����ʥ����                                                              */
/*---------------------------------------------------------------------------*/

static int end_flag;

static void InterruptTrap(int n)
{
  end_flag = 1;
}

/*---------------------------------------------------------------------------*/
/* ̤���Ѥ� Pieces �Υꥹ�Ȥ��顤�Ȥ��� Pieces ���֥������Ȥ򸡺�����        */
/* ��������롥���Ĥ���ʤ��ä����ˤϡ�create ���롥                       */
/*---------------------------------------------------------------------------*/

static int SearchOrCreateUsablePieces(XFireworks xfireworks,
				      PieceClass piece_class, double power,
				      int number, int n,
				      double * x, double * y, double * z,
				      double * vx, double * vy, double * vz,
				      int x_min, int y_min,
				      int x_max, int y_max)
{
  Pieces pieces;
  ObjListData current;

  for (current = ObjList_GetStart(xfireworks->free_pieces_list);
       !ObjList_IsEndEdge(xfireworks->free_pieces_list, current);
       current = ObjListData_GetNext(current)) {
    pieces = (Pieces)ObjListData_GetObj(current);
    if (Pieces_GetArraySize(pieces) >= number * n) {
      Pieces_Initialize(pieces, piece_class, power, number, n,
			x, y, z, vx, vy, vz,
			x_min, y_min, x_max, y_max,
			xfireworks->calculator);
      ObjList_MoveObjToEndOfOtherList(xfireworks->free_pieces_list, current,
				      xfireworks->pieces_list);
      return (0);
    }
  }

  /* ���ѤǤ��� Pieces ���֥������Ȥ�̵���ä��顤�������������� */
  pieces = Pieces_Create(piece_class, power, number, n,
			 x, y, z, vx, vy, vz, x_min, y_min, x_max, y_max,
			 xfireworks->calculator);
  if (pieces == NULL) return (0);
  ObjList_InsertObjToEnd(xfireworks->pieces_list, pieces,
			 (ObjDestructor)Pieces_Destroy);

  return (1);
}

/*---------------------------------------------------------------------------*/
/* ̤���Ѥ� AfterImages �Υꥹ�Ȥ��顤�Ȥ��� AfterImages ���֥������Ȥ�      */
/* ����������������롥���Ĥ���ʤ��ä����ˤϡ�create ���롥               */
/*---------------------------------------------------------------------------*/

static int SearchOrCreateUsableAfterImages(XFireworks xfireworks,
					   int size, int number,
					   double * x, double * y,
					   int x_min, int y_min,
					   int x_max, int y_max,
					   ObjList list)
{
  AfterImages after_images;
  ObjListData current;

  for (current = ObjList_GetStart(xfireworks->free_after_images_list);
       !ObjList_IsEndEdge(xfireworks->free_after_images_list, current);
       current = ObjListData_GetNext(current)) {
    after_images = (AfterImages)ObjListData_GetObj(current);
    if (AfterImages_GetArraySize(after_images) >= number) {
      AfterImages_Initialize(after_images, size, number, x, y,
			     x_min, y_min, x_max, y_max, list);
      ObjList_MoveObjToEndOfOtherList(xfireworks->free_after_images_list,
				      current,
				      xfireworks->after_images_list);
      return (0);
    }
  }

  /* ���ѤǤ��� AfterImages ���֥������Ȥ�̵���ä��顤�������������� */
  after_images = AfterImages_Create(size, number, x, y,
				    x_min, y_min, x_max, y_max, list);
  if (after_images == NULL) return (0);
  ObjList_InsertObjToEnd(xfireworks->after_images_list, after_images,
			 (ObjDestructor)AfterImages_Destroy);

  return (1);
}

/*---------------------------------------------------------------------------*/
/* �ֲФΥ�������                                                            */
/*---------------------------------------------------------------------------*/

int XFireworks_Start(XFireworks xfireworks,
		     int pieces_max_number,
		     int wait_time)
{
  PieceClass piece_class;
  PieceNext piece_next;
  Pieces pieces;
  Pieces pieces2;
  ObjList next_list;
  ObjListData current;
  ObjListData current2;
  AfterImages after_images;
  double x, y, z, vx, vy, vz;
  GC gc;
  int ret;

#if 0
  XFireworks_DrawAllPieceClasses(xfireworks);
  while(1) { /* None */ }
#endif

  end_flag = 0;
  signal(SIGINT , InterruptTrap);
  signal(SIGTERM, InterruptTrap);

  while (end_flag == 0) {

    /* ������ Pieces ������ */
    if (ObjList_GetLength(xfireworks->pieces_list) < pieces_max_number) {

      for (current = ObjList_GetStart(xfireworks->piece_class_list);
	   !ObjList_IsEndEdge(xfireworks->piece_class_list, current);
	   current = ObjListData_GetNext(current)) {
	piece_class = (PieceClass)ObjListData_GetObj(current);
	if (PieceClass_GetProbability(piece_class) > 0.0) {
	  if (PieceClass_GetProbability(piece_class) > DoubleRand(100.0)) {
	    x = (double)Rand(Disp_GetWidth( xfireworks->disp));
	    y = (double)Rand(Disp_GetHeight(xfireworks->disp));
	    z = DoubleRand(400.0) - 200.0;
	    vx = 0.0;
	    vy = 0.0;
	    vz = 0.0;
	    y -= Disp_GetHeight(xfireworks->disp) * 0.25;

	    SearchOrCreateUsablePieces(xfireworks,
				       piece_class, 0.0, 1, 1,
				       &x, &y, &z, &vx, &vy, &vz,
				       0, 0,
				       Disp_GetWidth( xfireworks->disp) - 1,
				       Disp_GetHeight(xfireworks->disp) - 1);
	  }
	}
      }
    }

    /* Pieces �ν��� */
    for (current = ObjList_GetStart(xfireworks->pieces_list);
	 !ObjList_IsEndEdge(xfireworks->pieces_list, current);
	 current = ObjListData_GetNext(current)) {

      pieces = (Pieces)ObjListData_GetObj(current);
      piece_class = Pieces_GetPieceClass(pieces);

      ret = Pieces_Move(pieces, 0, 0, 
			Disp_GetWidth( xfireworks->disp) - 1,
			Disp_GetHeight(xfireworks->disp) - 1,
			Disp_GetWidth( xfireworks->disp));
      if (ret) { /* ǳ���Ԥ������ */
	next_list = PieceClass_GetNextList(piece_class);
	if ((Pieces_GetNumber(pieces) > 0) && (next_list != NULL)) {
	  for (current2 = ObjList_GetStart(next_list);
	       !ObjList_IsEndEdge(next_list, current2);
	       current2 = ObjListData_GetNext(current2)) {
	    piece_next = (PieceNext)ObjListData_GetObj(current2);

	    SearchOrCreateUsablePieces(xfireworks,
				       PieceNext_GetPieceClass(piece_next),
				       PieceNext_GetPower(piece_next),
				       PieceNext_GetNumber(piece_next),
				       Pieces_GetNumber(pieces),
				       Pieces_GetX(pieces),
				       Pieces_GetY(pieces),
				       Pieces_GetZ(pieces),
				       Pieces_GetVx(pieces),
				       Pieces_GetVy(pieces),
				       Pieces_GetVz(pieces),
				       0, 0, 
				       Disp_GetWidth( xfireworks->disp) - 1,
				       Disp_GetHeight(xfireworks->disp) - 1);
	  }
	}
	current2 = ObjListData_GetPrev(current);
	ObjList_MoveObjToStartOfOtherList(xfireworks->pieces_list,
					  current,
					  xfireworks->free_pieces_list);
	current = current2;
      } else {
	SearchOrCreateUsableAfterImages(xfireworks,
					Pieces_GetSize(pieces),
					Pieces_GetNumber(pieces),
					Pieces_GetX(pieces),
					Pieces_GetY(pieces),
					0, 0, 
					Disp_GetWidth( xfireworks->disp) - 1,
					Disp_GetHeight(xfireworks->disp) - 1,
					Pieces_GetGCList(pieces));
      }
    }

    /* �����ν��� */
    for (current = ObjList_GetStart(xfireworks->after_images_list);
	 !ObjList_IsEndEdge(xfireworks->after_images_list, current);
	 current = ObjListData_GetNext(current)) {
      after_images = (AfterImages)ObjListData_GetObj(current);

      gc = AfterImages_GetGC(after_images);

      if (gc == NULL) {
	current2 = ObjListData_GetPrev(current);
	ObjList_MoveObjToStartOfOtherList(xfireworks->after_images_list,
					  current,
					  xfireworks->free_after_images_list);
	current = current2;
      } else {
	if (AfterImages_GetNumber(after_images) > 0)
	  Disp_DrawFilledCircles(xfireworks->disp, gc,
				 AfterImages_GetArcs(after_images),
				 AfterImages_GetNumber(after_images));

	/* X �����Ф�Ϣ³����٤�Ϳ���ʤ�����Υ������� */
#if 0
	usleep(10);
#endif
      }
    }

    /* X�����Ф˻Ż��򤷤Ƥ�餤��     */
    /* X�����Ф��Ż���λ����Τ��Ԥ� */
    Disp_Sync(xfireworks->disp);

    /* usleep() �� Disp_Sync() �θ���֤����ȡ�                   */
    /* (Disp_Sync() �Ǥ�X�����ФλŻ���λ�Ԥ��ǥ֥�å�����Τǡ� */
    /*  Disp_Sync() ������ usleep() ���֤��Ƥ⤢�ޤ��̣��̵��)   */
    usleep(1000 + wait_time);
  }

  return (0);
}

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
