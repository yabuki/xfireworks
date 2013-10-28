#include "PieceP.h"

#include "configure.h"

#include "etc.h"

#ifndef DEFAULT_PROBABILITY
#define DEFAULT_PROBABILITY 0.1
#endif

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 3
#endif

#ifndef DEFAULT_AIR
#define DEFAULT_AIR 0.05
#endif

#ifndef DEFAULT_GRAVITY
#define DEFAULT_GRAVITY 0.3
#endif

#ifndef DEFAULT_TRANSMISSION
#define DEFAULT_TRANSMISSION 60.0
#endif

#ifndef DEFAULT_AFTER_IMAGE
#define DEFAULT_AFTER_IMAGE 10
#endif

/* x, y, z などの配列を一括確保して，malloc() の数を減らし，負荷を下げる */
#if 1
#ifndef ONE_BUNDLE_ALLOCATE
#define ONE_BUNDLE_ALLOCATE
#endif
#else
#ifdef ONE_BUNDLE_ALLOCATE
#undef ONE_BUNDLE_ALLOCATE
#endif
#endif

/*===========================================================================*/
/* オブジェクトのメンバの取得                                                */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* PieceClass クラス                                                         */
/*---------------------------------------------------------------------------*/

char * PieceClass_GetName(PieceClass piece_class)
{ return (piece_class->name); }

double PieceClass_GetProbability(PieceClass piece_class)
{ return (piece_class->probability); }

ObjList PieceClass_GetGCListList(PieceClass piece_class)
{ return (piece_class->gc_list_list); }

ObjList PieceClass_GetNextList(PieceClass piece_class)
{ return (piece_class->next_list); }

/*---------------------------------------------------------------------------*/
/* PieceNext クラス                                                          */
/*---------------------------------------------------------------------------*/

PieceClass PieceNext_GetPieceClass(PieceNext piece_next)
{ return (piece_next->piece_class); }

double PieceNext_GetPower(PieceNext piece_next) { return (piece_next->power); }
int PieceNext_GetNumber(PieceNext piece_next) { return (piece_next->number); }

/*---------------------------------------------------------------------------*/
/* Pieces クラス                                                             */
/*---------------------------------------------------------------------------*/

PieceClass Pieces_GetPieceClass(Pieces pieces) {return (pieces->piece_class);}
int Pieces_GetArraySize(Pieces pieces) { return (pieces->array_size); }
int Pieces_GetNumber(Pieces pieces) { return (pieces->number); }
double * Pieces_GetX(Pieces pieces) { return (pieces->x); }
double * Pieces_GetY(Pieces pieces) { return (pieces->y); }
double * Pieces_GetZ(Pieces pieces) { return (pieces->z); }
double * Pieces_GetVx(Pieces pieces) { return (pieces->vx); }
double * Pieces_GetVy(Pieces pieces) { return (pieces->vy); }
double * Pieces_GetVz(Pieces pieces) { return (pieces->vz); }

int Pieces_GetSize(Pieces pieces) { return (pieces->piece_class->size); }

ObjList Pieces_GetGCList(Pieces pieces)
{
  return ((ObjList)ObjListData_GetObj(pieces->gc_list));
}

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
			     Disp disp)
{
  PieceClass piece_class;
  int i;

  piece_class = (PieceClass)malloc(sizeof(_PieceClass));
  if (!piece_class) Error("PieceClass_Create", "Cannot allocate memory.");

  if ((name == NULL) || (name[0] == '\0')) name = "Unknown\0";

  piece_class->name = (char *)malloc(sizeof(char) * (StringLen(name) + 1));
  if (piece_class->name == NULL)
    Error("PieceClass_Create", "Cannot allocate memory.");
  StringCpy(piece_class->name, name);

  piece_class->size               = size;
  piece_class->probability        = probability;
  piece_class->air                = air;
  piece_class->gravity            = gravity;
  piece_class->transmission       = transmission;
  piece_class->after_image_length = after_image_length;

  if (fine < 1) fine = 1;
  piece_class->fine = fine;
  piece_class->step = 100.0 / piece_class->fine;

  if (gc_list_list) piece_class->gc_list_list = gc_list_list;
  else              piece_class->gc_list_list = ObjList_Create();

  if (next_list) piece_class->next_list = next_list;
  else           piece_class->next_list = ObjList_Create();

  return (piece_class);
}

PieceClass PieceClass_Destroy(PieceClass piece_class)
{
  if (!piece_class) return (NULL);

  if (piece_class->name) free(piece_class->name);

  if (piece_class->gc_list_list)
    ObjList_Destroy(piece_class->gc_list_list);

  if (piece_class->next_list)
    ObjList_Destroy(piece_class->next_list);

  free(piece_class);

  return (NULL);
}

/*===========================================================================*/
/* PieceNext オブジェクトの作成・削除用                                      */
/*===========================================================================*/

PieceNext PieceNext_Create(PieceClass piece_class, double power, int n)
{
  PieceNext next;

  next = (PieceNext)malloc(sizeof(_PieceNext));
  if (!next) Error("PieceNext_Create", "Cannot allocate memory.");

  next->piece_class = piece_class;
  next->power = power;
  next->number = n;

  return (next);
}

PieceNext PieceNext_Destroy(PieceNext next)
{
  if (!next) return (NULL);

  free(next);

  return (NULL);
}

/*===========================================================================*/
/* 内部で使用する関数                                                        */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* PieceClass の検索                                                         */
/* 見つからなかった場合には，PieceClass オブジェクトを生成し，               */
/* リストに追加する．                                                        */
/*---------------------------------------------------------------------------*/

static PieceClass PieceClassList_SearchPieceClass(ObjList list,
						  char * name,
						  Disp disp,
						  int default_size,
						  double default_air,
						  double default_gravity,
						  double default_transmission,
						  int default_after_image,
						  int fine)
{
  ObjListData current;
  PieceClass piece_class;

  if (!list) Error("PieceClassList_SearchPieceClass",
		   "PieceClassList has not created.");

  for (current = ObjList_GetStart(list);
       !ObjList_IsEndEdge(list, current);
       current = ObjListData_GetNext(current)) {
    piece_class = (PieceClass)ObjListData_GetObj(current);
    if (!StringCmp(piece_class->name, name)) {
      return (piece_class);
    }
  }

  /* 見つからなかった場合には，作成し，リストに追加する */
  piece_class = PieceClass_Create(name,
				  default_size,
				  -1.0,
				  default_air,
				  default_gravity,
				  default_transmission,
				  default_after_image,
				  fine,
				  NULL, NULL, disp);
  ObjList_InsertObjToEnd(list, piece_class, (ObjDestructor)PieceClass_Destroy);

  return (piece_class);
}

/*---------------------------------------------------------------------------*/
/* 単語の取得                                                                */
/*---------------------------------------------------------------------------*/

static char * GetWord(Stream stream)
{
  char * word;
  word = Stream_GetWord(stream, NULL, 0,
			"",     /* この文字で切り分ける */
			" \t",  /* この文字でも切り分ける */
			"!#$%", /* コメント行の先頭文字 '#' など */
			"\n",   /* 行末文字 '\n' など */
			"\"\'", /* 引用符文字 '\"' など */
			"",     /* 文字列先頭の読みとばし文字 */
			""      /* 文字列末尾の読みとばし文字 */
			);
  return (word);
}

static char * GetWord2(Stream stream)
{
  char * word;
  word = Stream_GetWord(stream, NULL, 0,
			"",     /* この文字で切り分ける */
			" \t",  /* この文字でも切り分ける */
			"",     /* コメント行の先頭文字 '#' など */
			"\n",   /* 行末文字 '\n' など */
			"\"\'", /* 引用符文字 '\"' など */
			"",     /* 文字列先頭の読みとばし文字 */
			""      /* 文字列末尾の読みとばし文字 */
			);
  return (word);
}

static ObjList ReadNext(ObjList piece_class_list,
			char * word,
			Disp disp,
			int default_size,
			double default_air,
			double default_gravity,
			double default_transmission,
			int default_after_image,
			int fine,
			int next_power_mag,
			int next_number_mag)
{
  ObjList list;
  Stream stream;
  char * w1;
  char * w2;
  char * w3;
  PieceClass piece_class;
  double power;
  int n;

  list = ObjList_Create();
  if (!word) return (list);
  stream = Stream_CreateFromCharacters(word);
  if (!stream) return (list);

  while ((w1 = GetWord2(stream)) != NULL) {
    w2 = w3 = NULL;
    piece_class = PieceClassList_SearchPieceClass(piece_class_list, w1, disp,
						  default_size,
						  default_air,
						  default_gravity,
						  default_transmission,
						  default_after_image,
						  fine);
    w2 = GetWord2(stream);
    if ((w3 = GetWord2(stream)) != NULL) {
      power = atof(w2) * next_power_mag * 0.01;
      n = atoi(w3);
      if (n > 0) {
	n = (n * next_number_mag + 50) / 100;
	if (n < 1) n = 1;
      }
      ObjList_InsertObjToEnd(list,
			     PieceNext_Create(piece_class, power, n),
			     (ObjDestructor)PieceNext_Destroy);
    }
    if (w1) free(w1);
    if (w2) free(w2);
    if (w3) free(w3);
  }

  Stream_Destroy(stream);

  return (list);
}

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
					int next_number_mag)
{
  ObjList piece_class_list;
  PieceClass current = NULL;
  char * word;
  char * word2;
  ObjList color_gc_list;
  double default_probability  = DEFAULT_PROBABILITY * probability_mag * 0.01;
  int default_size            = (DEFAULT_SIZE * size_mag + 50) / 100;
  double default_air          = DEFAULT_AIR * air_mag * 0.01;
  double default_gravity      = DEFAULT_GRAVITY * gravity_mag * 0.01;
  double default_transmission = DEFAULT_TRANSMISSION * transmission_mag * 0.01;
  int default_after_image     =
    (DEFAULT_AFTER_IMAGE * after_image_mag + 50) / 100;

  piece_class_list = ObjList_Create();

  while ((word = GetWord(stream)) != NULL) {
    word2 = NULL;

    if (!StringCmp(word, "Name")) {
      if ((word2 = GetWord(stream)) != NULL) {
	current =
	  PieceClassList_SearchPieceClass(piece_class_list, word2, disp,
					  default_size,
					  default_air,
					  default_gravity,
					  default_transmission,
					  default_after_image,
					  fine);
      }
    } else if (!StringCmp(word, "DefaultProbability")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_probability = atof(word2) * probability_mag * 0.01;
      }
    } else if (!StringCmp(word, "DefaultSize")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_size = (atoi(word2) * size_mag + 50) / 100;
      }
    } else if (!StringCmp(word, "DefaultAir")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_air = atof(word2) * air_mag * 0.01;
      }
    } else if (!StringCmp(word, "DefaultGravity")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_gravity = atof(word2) * gravity_mag * 0.01;
      }
    } else if (!StringCmp(word, "DefaultTransmission")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_transmission = atof(word2) * transmission_mag * 0.01;
      }
    } else if (!StringCmp(word, "DefaultAfterImage")) {
      if ((word2 = GetWord(stream)) != NULL) {
	default_after_image = (atoi(word2) * after_image_mag + 50) / 100;
      }
    } else {
      if (current) {
	if (!StringCmp(word, "Size")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->size = default_size;
	    else current->size = (atoi(word2) * size_mag + 50) / 100;
	  }
	} else if (!StringCmp(word, "Probability")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->probability = default_probability;
	    else if (!StringCmp(word2, "None") || !StringCmp(word2, "none"))
	      current->probability = -1.0;
	    else current->probability = atof(word2) * probability_mag * 0.01;
	  }
	} else if (!StringCmp(word, "Air")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->air = default_air;
	    else current->air = atof(word2) * air_mag * 0.01;
	  }
	} else if (!StringCmp(word, "Gravity")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->gravity = default_gravity;
	    else current->gravity = atof(word2) * gravity_mag * 0.01;
	  }
	} else if (!StringCmp(word, "Transmission")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->transmission = default_transmission;
	    else current->transmission = atof(word2) * transmission_mag * 0.01;
	  }
	} else if (!StringCmp(word, "AfterImage")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (!StringCmp(word2, "Default") || !StringCmp(word2, "default"))
	      current->after_image_length = default_after_image;
	    else current->after_image_length =
		   (atoi(word2) * after_image_mag + 50) / 100;
	  }
	} else if (!StringCmp(word, "Color")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (current->gc_list_list == NULL)
	      current->gc_list_list = ObjList_Create();
	    if (!StringEqual(word2, "None") && !StringEqual(word2, "none")) {
	      color_gc_list =
		CreateColorGCListFromCharacters(database,
						word2,
						current->after_image_length,
						fine,
						color_length_mag);
	      ObjList_Concatenate(current->gc_list_list, color_gc_list);
	    }
	  }
	} else if (!StringCmp(word, "Next")) {
	  if ((word2 = GetWord(stream)) != NULL) {
	    if (current->next_list == NULL)
	      current->next_list = ObjList_Create();
	    if (!StringEqual(word2, "None") && !StringEqual(word2, "none")) {
	      ObjList_Concatenate(current->next_list,
				  ReadNext(piece_class_list, word2, disp,
					   default_size,
					   default_air,
					   default_gravity,
					   default_transmission,
					   default_after_image,
					   fine,
					   next_power_mag,
					   next_number_mag));
	    }
	  }
	} else if (!StringCmp(word, "End")) {
	  if (word)  free(word);
	  if (word2) free(word2);
	  return (piece_class_list);
	} else {
	  fprintf(stderr, "Unknown command : %s\n", word);
	}
      } else {
	fprintf(stderr,
		"Cannot specify class name. Command was ignored. : %s\n",
		word);
      }
    }
    if (word)  free(word);
    if (word2) free(word2);
  }

  return (piece_class_list);
}

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
		      Calculator calculator)
{
  int i, j, a;
  int rad, z_rad;
  double p, t;

  pieces->piece_class = piece_class;
  pieces->gc_list = ObjList_GetStartEdge(pieces->piece_class->gc_list_list);

  if (n * number > pieces->array_size) return (1);

  a = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < number; j++) {
      z_rad = Rand(CALCULATOR_DEFAULT_DEGREE);
      p = Calculator_GetCos(calculator, z_rad);
      rad = Rand(CALCULATOR_DEFAULT_DEGREE);
      pieces->x[a] = x[i];
      pieces->y[a] = y[i];
      pieces->z[a] = z[i];
      t = pieces->piece_class->transmission * 0.01;
      pieces->vx[a] = Calculator_GetCos(calculator,   rad) *power*p+ vx[i] * t;
      pieces->vy[a] = Calculator_GetSin(calculator,   rad) *power*p+ vy[i] * t;
      pieces->vz[a] = Calculator_GetSin(calculator, z_rad) * power + vz[i] * t;
      if ( ((pieces->x[a] < x_min) && (pieces->vx[a] < 0.0)) ||
	   ((pieces->x[a] > x_max) && (pieces->vx[a] > 0.0)) ||
	   ((pieces->y[a] > y_max) && (pieces->vy[a] > 0.0)) ) {
	/* None */
      } else {
	a++;
      }
    }
  }
  pieces->number = a;

  return (0);
}

/*---------------------------------------------------------------------------*/
/* Pieces オブジェクトの生成                                                 */
/*---------------------------------------------------------------------------*/

Pieces Pieces_Create(PieceClass piece_class, double power, int number, int n,
		     double * x, double * y, double * z,
		     double * vx, double * vy, double * vz,
		     int x_min, int y_min, int x_max, int y_max,
		     Calculator calculator)
{
  Pieces pieces;

  pieces = (Pieces)malloc(sizeof(_Pieces));
  if (!pieces) Error("Pieces_Create", "Cannot allocate memory");

  pieces->array_size = number * n;

  /* Pieces オブジェクトは使い回しするので，小さいサイズのものは      */
  /* メモリを無駄に食ってしまうので，メモリの節約のため，あまり小さな */
  /* サイズのものは作らないようにする．                               */
  if (pieces->array_size < 30) pieces->array_size = 30;

  if (pieces->array_size) {

#ifdef ONE_BUNDLE_ALLOCATE
    pieces->x = (double *)malloc(sizeof(double) * pieces->array_size * 6);
    if (pieces->x == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->y  = pieces->x + pieces->array_size * 1;
    pieces->z  = pieces->x + pieces->array_size * 2;
    pieces->vx = pieces->x + pieces->array_size * 3;
    pieces->vy = pieces->x + pieces->array_size * 4;
    pieces->vz = pieces->x + pieces->array_size * 5;
#else
    pieces->x  = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->x  == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->y  = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->y  == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->z  = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->z  == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->vx = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->vx == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->vy = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->vy == NULL) Error("Pieces_Create", "Cannot allocate memory");
    pieces->vz = (double *)malloc(sizeof(double) * pieces->array_size);
    if (pieces->vz == NULL) Error("Pieces_Create", "Cannot allocate memory");
#endif
  } else {
    pieces->x  = NULL;
    pieces->y  = NULL;
    pieces->z  = NULL;
    pieces->vx = NULL;
    pieces->vy = NULL;
    pieces->vz = NULL;
  }

  Pieces_Initialize(pieces, piece_class, power, number, n,
		    x, y, z, vx, vy, vz, x_min, y_min, x_max, y_max,
		    calculator);

  return (pieces);
}

/*---------------------------------------------------------------------------*/
/* Piece オブジェクトの削除                                                  */
/*---------------------------------------------------------------------------*/

Pieces Pieces_Destroy(Pieces pieces)
{
  if (!pieces) return (NULL);

#ifdef ONE_BUNDLE_ALLOCATE
  if (pieces->x ) free(pieces->x );
#else
  if (pieces->x ) free(pieces->x );
  if (pieces->y ) free(pieces->y );
  if (pieces->z ) free(pieces->z );
  if (pieces->vx) free(pieces->vx);
  if (pieces->vy) free(pieces->vy);
  if (pieces->vz) free(pieces->vz);
#endif

  free(pieces);

  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* Pieces オブジェクトの移動                                                 */
/*---------------------------------------------------------------------------*/

int Pieces_Move(Pieces pieces,
		int x_min, int y_min, int x_max, int y_max, int size)
{
  int length;
  int i, a;
  double z;
  double step;
  double air, g;
  double tmp1, tmp2;

  if (ObjList_IsEnd(pieces->piece_class->gc_list_list, pieces->gc_list))
    return (1);

  step = pieces->piece_class->step;
  air = 1.0 - pieces->piece_class->air * step;
  g = pieces->piece_class->gravity * step;
  tmp1 = step * size * 0.001;

  a = 0;
  for (i = 0; i < pieces->number; i++) {
    if (a) {
      pieces->x[ i] = pieces->x[ i + a];
      pieces->y[ i] = pieces->y[ i + a];
      pieces->z[ i] = pieces->z[ i + a];
      pieces->vx[i] = pieces->vx[i + a];
      pieces->vy[i] = pieces->vy[i + a];
      pieces->vz[i] = pieces->vz[i + a];
    }

    pieces->vy[i] += g;

    /* 空気抵抗の計算 */
    pieces->vx[i] *= air;
    pieces->vy[i] *= air;
    pieces->vz[i] *= air;

    /* 奥行きの計算 */
    pieces->z[i] += pieces->vz[i] * step;
    z = 1.0 - pieces->z[i] * (1.0 / 1000.0);
    if (z < 0.01) z = 0.01;

    tmp2 = z * tmp1;
    pieces->x[i] += pieces->vx[i] * tmp2;
    pieces->y[i] += pieces->vy[i] * tmp2;

    if (((pieces->x[i] < x_min) && (pieces->vx[i] < 0.0)) ||
	((pieces->x[i] > x_max) && (pieces->vx[i] > 0.0)) ||
	(pieces->y[i] > y_max)) {
      i--;
      a++;
      pieces->number--;
    }
  }

  if (pieces->number == 0) return (1);

  pieces->gc_list = ObjListData_GetNext(pieces->gc_list);

  return (0);
}

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
