/*****************************************************************************/
/* ColorGC GCと色の管理・問い合わせ用のライブラリ                            */
/*****************************************************************************/

#include "ColorGCP.h"

#include "Stream.h"
#include "etc.h"

/*===========================================================================*/
/* ColorGCInstance 関連                                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* オブジェクトの作成                                                        */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCInstance_Create(Disp disp, XColor color)
{
  ColorGCInstance instance;

  instance = (ColorGCInstance)malloc(sizeof(_ColorGCInstance));
  if (instance == NULL)
    Error("ColorGCInstance_Create", "Cannot allocate memory");

  instance->disp = disp;
  instance->color = color;

  XAllocColor(Disp_GetDisplay(instance->disp),
	      Disp_GetColormap(instance->disp),
	      &(instance->color));

  /* XAllocColor で instance->color.red などの内容が変わってしまうので，   */
  /* もとに戻す．でないと，あとで同じ色をRGB値で検索しても，違った色として */
  /* 解釈されてしまい，検索できないので，色情報を保存する意味が無くなって  */
  /* しまう．                                                              */
  instance->color.red   = color.red;
  instance->color.green = color.green;
  instance->color.blue  = color.blue;

  instance->gc = Disp_CreateGC(instance->disp);

  XSetForeground(Disp_GetDisplay(instance->disp), instance->gc,
		 instance->color.pixel);
  XSetBackground(Disp_GetDisplay(instance->disp), instance->gc,
		 instance->color.pixel);

  return (instance);
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの削除                                                        */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCInstance_Destroy(ColorGCInstance instance)
{
  unsigned long pixel;

  if (instance == NULL) return (NULL);
  if (instance->gc) Disp_DestroyGC(instance->disp, instance->gc);
  pixel = instance->color.pixel;
  XFreeColors(Disp_GetDisplay(instance->disp),
	      Disp_GetColormap(instance->disp),
	      &pixel, 1, 0);
  free(instance);
  return (NULL);
}

/*===========================================================================*/
/* ColorGCList 関連                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* オブジェクトの作成                                                        */
/*---------------------------------------------------------------------------*/

static ColorGCList ColorGCList_Create(Disp disp, int studying_flag)
{
  ColorGCList list;

  list = (ColorGCList)malloc(sizeof(_ColorGCList));
  if (list == NULL) Error("ColorGCList_Create", "Cannot allocate memory");

  list->disp = disp;
  list->studying_flag = studying_flag;
  list->list = ObjList_Create();

  return (list);
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの削除                                                        */
/*---------------------------------------------------------------------------*/

static ColorGCList ColorGCList_Destroy(ColorGCList list)
{
  if (list == NULL) return (NULL);

  if (list->list) ObjList_Destroy(list->list);

  free(list);

  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* ColorGCList 中での ColorGCInstance のソート用の比較関数                   */
/*---------------------------------------------------------------------------*/

static int ColorGCInstance_CmpToColor(ColorGCInstance gci, XColor color)
{
  if (gci->color.red   > color.red  ) return ( 1);
  if (gci->color.red   < color.red  ) return (-1);
  if (gci->color.green > color.green) return ( 1);
  if (gci->color.green < color.green) return (-1);
  if (gci->color.blue  > color.blue ) return ( 1);
  if (gci->color.blue  < color.blue ) return (-1);
  return (0);
}

/*---------------------------------------------------------------------------*/
/* リストから ColorGCInstance を得る．                                       */
/* GC の取得要求に対して，GC のリストを検索して返す．                        */
/* 引数 current で，検索のスタート地点を指定する．                           */
/* 存在しない場合には，作成してリストに追加する．                            */
/* (常に RGB 値でソートされた状態で追加する)                                 */
/* 線形探索なので，O(n^2)で遅くなる．                                        */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCList_GetColorGCInstance(ColorGCList list,
						      XColor color)
{
  ColorGCInstance instance;
  ObjListData current;
  int cmp;

  for (current = ObjList_GetStart(list->list);
       !ObjList_IsEndEdge(list->list, current);
       current = ObjListData_GetNext(current)) {

    instance = (ColorGCInstance)ObjListData_GetObj(current);
    cmp = ColorGCInstance_CmpToColor(instance, color);

    if (cmp == 0) { /* 見つかれば，それを返す */
#ifdef HIT_LIST
      fprintf(stderr, "S");
#endif
      if (list->studying_flag) ObjList_MoveObjToStart(list->list, current);
      return (instance);
    } else if (cmp > 0) {
      if (!list->studying_flag) break;
    }
  }

  /* 見つからなかった場合は，作成してリストに追加する */
  instance = ColorGCInstance_Create(list->disp, color);

  if (list->studying_flag)
    ObjList_InsertObjToStart(list->list, instance,
			     (ObjDestructor)ColorGCInstance_Destroy);
  else
    ObjList_InsertObjToPrev(list->list, current, instance,
			    (ObjDestructor)ColorGCInstance_Destroy);

#ifdef HIT_LIST
  fprintf(stderr, "A");
#endif

  return (instance);
}

/*===========================================================================*/
/* ColorGCCache 関連                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* キャッシュ用バッファ                                                      */
/*---------------------------------------------------------------------------*/

static ColorGCCacheBuffer ColorGCCacheBuffer_Create(ColorGCInstance instance)
{
  ColorGCCacheBuffer buffer;
  buffer = (ColorGCCacheBuffer)malloc(sizeof(_ColorGCCacheBuffer));
  if (buffer == NULL) Error("ColorGCCacheBuffer_Create",
			    "Cannot allocate memory.");
  buffer->instance = instance;
  return (buffer);
}

static ColorGCCacheBuffer ColorGCCacheBuffer_Destroy(ColorGCCacheBuffer buffer)
{
  if (buffer == NULL) return (NULL);
  free(buffer);
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* キャッシュ                                                                */
/*---------------------------------------------------------------------------*/

static ColorGCCache ColorGCCache_Create(ColorGCList color_gc_list, int size)
{
  ColorGCCache cache;

  cache = (ColorGCCache)malloc(sizeof(_ColorGCCache));
  if (cache == NULL) Error("ColorGCCache_Create",
			    "Cannot allocate memory.");

  cache->size = size;
  cache->color_gc_list = color_gc_list;
  cache->buffer_list = ObjList_Create();

  return (cache);
}

static ColorGCCache ColorGCCache_Destroy(ColorGCCache cache)
{
  if (cache == NULL) return (NULL);

  if (cache->buffer_list) ObjList_Destroy(cache->buffer_list);
  free(cache);

  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* キャッシュから ColorGCInstance を得る．                                   */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCCache_GetColorGCInstance(ColorGCCache cache,
						       XColor color)
{
  ObjListData current;
  ColorGCCacheBuffer buffer;
  ColorGCInstance instance;
  int cmp;

  /* キャッシュの中を検索 */
  for (current = ObjList_GetStart(cache->buffer_list);
       !ObjList_IsEndEdge(cache->buffer_list, current);
       current = ObjListData_GetNext(current)) {

    buffer = (ColorGCCacheBuffer)ObjListData_GetObj(current);
    cmp = ColorGCInstance_CmpToColor(buffer->instance, color);

    if (cmp == 0) { /* キャッシュ中に見つけた場合 */
#ifdef HIT_CACHE
      fprintf(stderr, "H"); /* Hit! */
#endif
      ObjList_MoveObjToStart(cache->buffer_list, current);
      return (buffer->instance);
    }
  }

  /* キャッシュ中に見つけられなかった場合 */

  /* リストから検索する */
  instance = ColorGCList_GetColorGCInstance(cache->color_gc_list, color);

  /* キャッシュに追加する */
  if (ObjList_GetLength(cache->buffer_list) < cache->size) {
    buffer = ColorGCCacheBuffer_Create(instance);
    ObjList_InsertObjToStart(cache->buffer_list, buffer,
			     (ObjDestructor)ColorGCCacheBuffer_Destroy);
  } else {
    current = ObjList_GetEnd(cache->buffer_list);
    buffer = (ColorGCCacheBuffer)ObjListData_GetObj(current);
    buffer->instance = instance;
    ObjList_MoveObjToStart(cache->buffer_list, current);
  }

#ifdef HIT_CACHE
  fprintf(stderr, "F"); /* False! */
#endif

  return (instance);
}

/*===========================================================================*/
/* ColorGCHash 関連                                                          */
/*===========================================================================*/

static ColorGCHash ColorGCHash_Create(Disp disp,
				      int studying_flag,
				      int cache_flag,
				      int cache_size,
				      int hash_number)
{
  ColorGCHash hash;
  int i;

  hash = (ColorGCHash)malloc(sizeof(_ColorGCHash));
  if (hash == NULL) Error("ColorGCHash_Create", "Cannot allocate memory.");

  hash->number = hash_number;
  hash->cache_flag = cache_flag;

  if (cache_flag) {
    hash->color_gc_cache =
      (ColorGCCache *)malloc(sizeof(ColorGCCache) * hash->number);
    if (hash->color_gc_cache == NULL) Error("ColorGCHash_Create",
					    "Cannot allocate memory.");
  } else {
    hash->color_gc_cache = NULL;
  }

  hash->color_gc_list =
    (ColorGCList *)malloc(sizeof(ColorGCList) * hash->number);
  if (hash->color_gc_list == NULL) Error("ColorGCHash_Create",
					 "Cannot allocate memory.");

  for (i = 0; i < hash->number; i++) {
    hash->color_gc_list[i] = ColorGCList_Create(disp, studying_flag);
    if (cache_flag) {
      hash->color_gc_cache[i] = ColorGCCache_Create(hash->color_gc_list[i],
						    cache_size);
    }
  }

  return (hash);
}

static ColorGCHash ColorGCHash_Destroy(ColorGCHash hash)
{
  int i;

  if (hash == NULL) return (NULL);

  if (hash->color_gc_cache) {
    for (i = 0; i < hash->number; i++) {
      if (hash->color_gc_cache[i])
	ColorGCCache_Destroy(hash->color_gc_cache[i]);
    }
    free(hash->color_gc_cache);
  }

  if (hash->color_gc_list) {
    for (i = 0; i < hash->number; i++) {
      if (hash->color_gc_list[i])
	ColorGCList_Destroy(hash->color_gc_list[i]);
    }
    free(hash->color_gc_list);
  }

  free(hash);

  return (NULL);
}

static void ColorGCHash_OutputHashStatus(ColorGCHash hash)
{
  int i;
  printf ("\nHash :");
  for (i = 0; i < hash->number; i++) {
    printf("%d ", ObjList_GetLength(hash->color_gc_list[i]->list));
  }
  printf ("\n");
}

/*---------------------------------------------------------------------------*/
/* ハッシュ関数                                                              */
/* 色を減色している場合，特定の位置だけ頻繁に使用されたりしないように注意．  */
/* (たとえば，                                                               */
/*  ((int)color.red*3 + (int)color.green*2 + (int)color.blue) % hash->number */
/*  のようなハッシュ関数だと，16階調に減色したときに，4096 の倍数の位置だけ  */
/*  頻繁に使用されてしまう．                                                 */
/*---------------------------------------------------------------------------*/

static int HashFunction(ColorGCHash hash, XColor color)
{
  return ((
	   (((int)color.red)   / 3000) * 11 +
	   (((int)color.green) % 3000) / 7 +
	   (((int)color.blue)  % 1000) / 3
	   ) % hash->number);
}

/*---------------------------------------------------------------------------*/
/* ハッシュから ColorGCInstance を得る．                                     */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCHash_GetColorGCInstance(ColorGCHash hash,
						      XColor color)
{
  int n;

  n = HashFunction(hash, color);

  if (hash->cache_flag)
    return (ColorGCCache_GetColorGCInstance(hash->color_gc_cache[n], color));
  else
    return (ColorGCList_GetColorGCInstance(hash->color_gc_list[n], color));
}

/*===========================================================================*/
/* ColorName 関連                                                            */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* オブジェクトの生成                                                        */
/*---------------------------------------------------------------------------*/

static ColorName ColorName_Create(Disp disp, char * name)
{
  ColorName color_name;

  color_name = (ColorName)malloc(sizeof(_ColorName));
  if (color_name == NULL) Error("ColorName_Create", "Cannot allocate memory");

  color_name->name = malloc(sizeof(char) * (StringLen(name) + 1));
  if (color_name->name == NULL)
    Error("ColorName_Create", "Cannot allocate memory");

  StringCpy(color_name->name, name);

  XParseColor(Disp_GetDisplay(disp), Disp_GetColormap(disp),
	      color_name->name, &(color_name->color));

  return (color_name);
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの削除                                                        */
/*---------------------------------------------------------------------------*/

static ColorName ColorName_Destroy(ColorName color_name)
{
  if (color_name == NULL) return (NULL);
  if (color_name->name) free(color_name->name);
  free(color_name);
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの生成                                                        */
/*---------------------------------------------------------------------------*/

static ColorNameList ColorNameList_Create(Disp disp)
{
  ColorNameList list;

  list = (ColorNameList)malloc(sizeof(_ColorNameList));
  if (list == NULL) Error("ColorNameList_Create", "Cannot allocate memory");

  list->disp = disp;
  list->list = ObjList_Create();

  return (list);
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの削除                                                        */
/*---------------------------------------------------------------------------*/

static ColorNameList ColorNameList_Destroy(ColorNameList list)
{
  if (list == NULL) return (NULL);
  if (list->list) ObjList_Destroy(list->list);
  free(list);
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* 文字列で与えられた色名から，RGB値を検索する．                             */
/*---------------------------------------------------------------------------*/

static XColor ColorNameList_GetColor(ColorNameList list, char * name)
{
  ObjListData current;
  ColorName color_name;

  for (current = ObjList_GetStart(list->list);
       !ObjList_IsEndEdge(list->list, current);
       current = ObjListData_GetNext(current)) {
    color_name = (ColorName)ObjListData_GetObj(current);
    if (StringEqual(color_name->name, name)) {
      ObjList_MoveObjToStart(list->list, current);
      return (color_name->color);
    }
  }

  color_name = ColorName_Create(list->disp, name);
  if (color_name == NULL)
    Error("ColorNameList_GetColor", "Cannot create ColorName");

  ObjList_InsertObjToStart(list->list, color_name,
			   (ObjDestructor)ColorName_Destroy);

  return (color_name->color);
}

/*===========================================================================*/
/* ColorGC 関連                                                              */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* オブジェクトの生成                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Create(ColorGCDatabase database, XColor color)
{
  ColorGC color_gc;

  color_gc = (ColorGC)malloc(sizeof(_ColorGC));

  /* ハッシュから色とGCを検索 */
  color_gc->instance = ColorGCHash_GetColorGCInstance(database->hash, color);

  return (color_gc);
}

ColorGC ColorGC_CreateFromColorGC(ColorGCDatabase database, ColorGC c)
{
  ColorGC color_gc;

  color_gc = (ColorGC)malloc(sizeof(_ColorGC));

  color_gc->instance = c->instance;

  return (color_gc);
}

/*---------------------------------------------------------------------------*/
/* RGB 値から生成する．                                                      */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_CreateFromRGB(ColorGCDatabase database,
			      int red, int green, int blue)
{
  XColor color;

  color.red   = red;
  color.green = green;
  color.blue  = blue;
  color.flags = DoRed | DoGreen | DoBlue;

  return (ColorGC_Create(database, color));
}

/*---------------------------------------------------------------------------*/
/* オブジェクトの削除                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Destroy(ColorGC color_gc)
{
  if (!color_gc) return (NULL);

  free(color_gc);

  return (NULL);
}

/*===========================================================================*/
/* 文字列読み込み用関数                                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* 色の節約のための，減色用の関数．                                          */
/* 減色すると，色の共有化が進み，キャッシングの効果が大きくなる．            */
/* あるていど減色しないと，色が共有できないので，無駄が多くなる．            */
/*---------------------------------------------------------------------------*/

static XColor DecreaseColor(XColor color, int gradation)
{
  int div = RGB_MAX_VALUE / gradation + 1;

  color.red   /= div;
  if (color.red   >= gradation - 1) color.red   = RGB_MAX_VALUE;
  else color.red   *= div;

  color.green /= div;
  if (color.green >= gradation - 1) color.green = RGB_MAX_VALUE;
  else color.green *= div;

  color.blue  /= div;
  if (color.blue  >= gradation - 1) color.blue  = RGB_MAX_VALUE;
  else color.blue  *= div;

  return (color);
}

/*---------------------------------------------------------------------------*/
/* 色とGCのデータベースから name で与えられた名前の色を検索する．            */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_CreateFromCharacters(ColorGCDatabase database, char * name)
{
  XColor color;

  if (!StringCmp(name, "none") ||
      !StringCmp(name, "None") ||
      !StringCmp(name, "NONE") ||
      !StringCmp(name, "back") ||
      !StringCmp(name, "Back") ||
      !StringCmp(name, "BACK") ||
      !StringCmp(name, "background") ||
      !StringCmp(name, "Background") ||
      !StringCmp(name, "BACKGROUND")) {
    if (database->background_color_gc)
      return (ColorGC_CreateFromColorGC(database,
					database->background_color_gc));
    else
#if 1
      name = "none";
#else
      name = "black";
#endif
  }

  color = ColorNameList_GetColor(database->color_name_list, name);

  /* 色の節約のため，減色する */
  color = DecreaseColor(color, database->gradation);

  return (ColorGC_Create(database, color));
}

/*---------------------------------------------------------------------------*/
/* XColor 構造体の取得                                                       */
/*---------------------------------------------------------------------------*/

XColor ColorGC_GetColor(ColorGC color_gc)
{
  return (color_gc->instance->color);
}

/*---------------------------------------------------------------------------*/
/* ピクセル値の取得                                                          */
/*---------------------------------------------------------------------------*/

unsigned long ColorGC_GetPixel(ColorGC color_gc)
{
  return (color_gc->instance->color.pixel);
}

/*---------------------------------------------------------------------------*/
/* GC の取得                                                                 */
/*---------------------------------------------------------------------------*/

GC ColorGC_GetGC(ColorGC color_gc)
{
  return (color_gc->instance->gc);
}

/*---------------------------------------------------------------------------*/
/* 色の明るさの取得                                                          */
/*---------------------------------------------------------------------------*/

int GetBrightness(XColor color)
{
  long int br;

#if 0
  br = color.red > color.green ? color.red : color.green;
  br = br > color.blue ? br : color.blue;
  br = br * 100 / RGB_MAX_VALUE;
#else
  br = color.red + color.green + color.blue;
  br = br * 100 / RGB_MAX_VALUE;
#endif

  if (br > 100) br = 100;

  return ((int)br);
}

/*===========================================================================*/
/* GC のリストの作成                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* 単語の読み込み用．(Stream ライブラリ使用)                                 */
/*---------------------------------------------------------------------------*/

static char * GetWord(Stream stream)
{
  char * word;
  word = Stream_GetWord(stream, NULL, 0,
			"",     /* この文字で切り分ける */
			" \t",  /* この文字でも切り分ける */
			"",     /* コメント行の先頭文字 '#' など */
			"\n",   /* 行末文字 '\n' など */
			"",     /* 引用符文字 '\"' など */
			"",     /* 文字列先頭の読みとばし文字 */
			""      /* 文字列末尾の読みとばし文字 */
			);
  return (word);
}

/*---------------------------------------------------------------------------*/
/* color1 と color2 で与えられた色の中間色を計算する                         */
/* per でパーセンテージを与える．                                            */
/* 0% のとき，color1になる．100%のとき，color2 になる                        */
/* 0% 未満か，100%より大きな per のときには，flags メンバに0を設定して       */
/* 返す．それ以外のときは，flags メンバは DoRed | DoGreen | DoBlue になる．  */
/*---------------------------------------------------------------------------*/

static XColor CalMiddleColor(XColor color1, XColor color2, int per)
{
  XColor color;

  color.pixel = 0;

  if ((per < 0) || (per > 100)) {
    color.flags = 0;
    color.red   = 0;
    color.green = 0;
    color.blue  = 0;
    return (color);
  }

  /* 中間色の計算 */
  color.flags = DoRed | DoGreen | DoBlue;
  color.red   =
    color1.red   + (int)(((color2.red   - color1.red  ) * per + 50) / 100);
  color.green =
    color1.green + (int)(((color2.green - color1.green) * per + 50) / 100);
  color.blue  =
    color1.blue  + (int)(((color2.blue  - color1.blue ) * per + 50) / 100);

  return (color);
}

/*---------------------------------------------------------------------------*/
/* GC のリストの作成                                                         */
/* "red" "black" 10                                                          */
/* のような引数から，GCのリストを生成して返す．                              */
/*---------------------------------------------------------------------------*/

static ObjList CreateGCList(ColorGCDatabase database,
			    XColor start_color, XColor end_color, int length)
{
  ColorGC color_gc;
  ObjList gc_list;
  XColor middle_color;
  int i, per;

  gc_list = ObjList_Create();

  for (i = 0; i < length; i++) {

    if (length == 1) per = 100;
    else per = i * 100 / (length - 1) ;

    middle_color = CalMiddleColor(start_color, end_color, per);
    /* 色の節約のため，減色する */
    middle_color = DecreaseColor(middle_color, database->gradation);

    color_gc = ColorGC_Create(database, middle_color);

    ObjList_InsertObjToEnd(gc_list, color_gc, (ObjDestructor)ColorGC_Destroy);
  }

  return (gc_list);
}

/*---------------------------------------------------------------------------*/
/* GC のリストの作成                                                         */
/* データ文字列から ColorGC のリストを作成し，返す．                         */
/* データ文字列のフォーマットは以下のとおり．                                */
/* "[スタートカラー1] [エンドカラー1] [長さ1] [スタートカラー2] ...\0"       */
/* 例) "red green 10 green blue 20 blue none 10\0"                           */
/*---------------------------------------------------------------------------*/

ObjList CreateColorGCListFromCharacters(ColorGCDatabase database,
					char * data,
					int after_image_length,
					int fine,
					int color_length_mag)
{
  Stream stream;
  char * start;
  char * end;
  char * len;
  int length;
  int length2;
  ObjList gc_list;
  ObjList list;
  ColorGC start_color_gc;
  ColorGC end_color_gc;
  XColor middle_color;
  int i, per;
  int br;

  list = ObjList_Create();

  if (!data) return (list);

  stream = Stream_CreateFromCharacters(data);
  if (!stream) return (list);

  while ((start = GetWord(stream)) != NULL) {
    end = GetWord(stream); if (end == NULL) end = "none";
    len = GetWord(stream); if (len == NULL) len = "1";

    start_color_gc = ColorGC_CreateFromCharacters(database, start);
    end_color_gc   = ColorGC_CreateFromCharacters(database, end  );
    length = (atoi(len) * color_length_mag + 50) / 100;
    if (length > 0) {
      length = (length * fine + 50) / 100;
#if 0
      if (length < 1) length = 1;
#endif
    }

    for (i = 0; i < length; i++) {

      if (length == 1) per = 100;
      else per = i * 100 / (length - 1) ;

      middle_color = CalMiddleColor(start_color_gc->instance->color,
				    end_color_gc->instance->color, per);
#if 0 /* 減色の必要なし */
      /* 色の節約のため，減色する */
      middle_color = DecreaseColor(middle_color, database->gradation);
#endif

      br = GetBrightness(middle_color);

      length2 = after_image_length;
      if (length2 > 0) {
	length2 = (length2 * fine + 50) / 100;
#if 0
	if (length2 < 1) length2 = 1;
#endif
      }
      length2 = (length2 * br + 50) / 100;

      gc_list = CreateGCList(database, middle_color,
			     database->background_color_gc->instance->color,
			     length2);

      ObjList_InsertObjToEnd(list, gc_list, (ObjDestructor)ObjList_Destroy);
    }

    ColorGC_Destroy(start_color_gc);
    ColorGC_Destroy(end_color_gc);

    if (start) free(start);
    if (end)   free(end);
    if (len)   free(len);
  }

  Stream_Destroy(stream);

  return (list);
}

/*===========================================================================*/
/* GC のデータベース                                                         */
/*===========================================================================*/

ColorGCDatabase ColorGCDatabase_Create(Disp disp,
				       int studying_flag,
				       int cache_flag,
				       int cache_size,
				       int hash_number,
				       char * background,
				       int gradation)
{
  ColorGCDatabase database;

  database = (ColorGCDatabase)malloc(sizeof(_ColorGCDatabase));
  if (database == NULL) Error("ColorGCDatabase_Create",
			      "Cannot allocate memory.");

  database->disp = disp;
  database->gradation = gradation;

  database->hash = ColorGCHash_Create(database->disp,
				      studying_flag,
				      cache_flag,
				      cache_size,
				      hash_number);
  database->color_name_list = ColorNameList_Create(database->disp);

  /* background が "none" の場合にも正常動作するように，               */
  /* ColorGC_CreateFromCharacters()を呼び出す前に，NULL で初期化する． */
  database->background_color_gc = NULL;

  /* 引数に database を入れて呼び出すので，必ず最後に置くこと */
  database->background_color_gc =
    ColorGC_CreateFromCharacters(database, background);
  /* この直後に return() が来るようにすること */

  return (database);
}

ColorGCDatabase ColorGCDatabase_Destroy(ColorGCDatabase database)
{
  if (database == NULL) return (NULL);

  if (database->background_color_gc)
    ColorGC_Destroy(database->background_color_gc);

  if (database->color_name_list)
    ColorNameList_Destroy(database->color_name_list);
  if (database->hash) ColorGCHash_Destroy(database->hash);
  free(database);

  return (NULL);
}

ColorGC ColorGCDatabase_GetBackgroundColorGC(ColorGCDatabase database)
{
  return (database->background_color_gc);
}

/*---------------------------------------------------------------------------*/
/* チューニング用                                                            */
/*---------------------------------------------------------------------------*/

void ColorGCDatabase_OutputHashStatus(ColorGCDatabase database)
{
  ColorGCHash_OutputHashStatus(database->hash);
}

/*****************************************************************************/
/* End of File                                                               */
/*****************************************************************************/
