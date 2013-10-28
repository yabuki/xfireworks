/*****************************************************************************/
/* ColorGC GC�ȿ��δ������䤤��碌�ѤΥ饤�֥��                            */
/*****************************************************************************/

#include "ColorGCP.h"

#include "Stream.h"
#include "etc.h"

/*===========================================================================*/
/* ColorGCInstance ��Ϣ                                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ���                                                        */
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

  /* XAllocColor �� instance->color.red �ʤɤ����Ƥ��Ѥ�äƤ��ޤ��Τǡ�   */
  /* ��Ȥ��᤹���Ǥʤ��ȡ����Ȥ�Ʊ������RGB�ͤǸ������Ƥ⡤��ä����Ȥ��� */
  /* ��ᤵ��Ƥ��ޤ��������Ǥ��ʤ��Τǡ����������¸�����̣��̵���ʤä�  */
  /* ���ޤ���                                                              */
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
/* ���֥������Ȥκ��                                                        */
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
/* ColorGCList ��Ϣ                                                          */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥκ���                                                        */
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
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

static ColorGCList ColorGCList_Destroy(ColorGCList list)
{
  if (list == NULL) return (NULL);

  if (list->list) ObjList_Destroy(list->list);

  free(list);

  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* ColorGCList ��Ǥ� ColorGCInstance �Υ������Ѥ���Ӵؿ�                   */
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
/* �ꥹ�Ȥ��� ColorGCInstance �����롥                                       */
/* GC �μ����׵���Ф��ơ�GC �Υꥹ�Ȥ򸡺������֤���                        */
/* ���� current �ǡ������Υ���������������ꤹ�롥                           */
/* ¸�ߤ��ʤ����ˤϡ��������ƥꥹ�Ȥ��ɲä��롥                            */
/* (��� RGB �ͤǥ����Ȥ��줿���֤��ɲä���)                                 */
/* ����õ���ʤΤǡ�O(n^2)���٤��ʤ롥                                        */
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

    if (cmp == 0) { /* ���Ĥ���С�������֤� */
#ifdef HIT_LIST
      fprintf(stderr, "S");
#endif
      if (list->studying_flag) ObjList_MoveObjToStart(list->list, current);
      return (instance);
    } else if (cmp > 0) {
      if (!list->studying_flag) break;
    }
  }

  /* ���Ĥ���ʤ��ä����ϡ��������ƥꥹ�Ȥ��ɲä��� */
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
/* ColorGCCache ��Ϣ                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ����å����ѥХåե�                                                      */
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
/* ����å���                                                                */
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
/* ����å��夫�� ColorGCInstance �����롥                                   */
/*---------------------------------------------------------------------------*/

static ColorGCInstance ColorGCCache_GetColorGCInstance(ColorGCCache cache,
						       XColor color)
{
  ObjListData current;
  ColorGCCacheBuffer buffer;
  ColorGCInstance instance;
  int cmp;

  /* ����å������򸡺� */
  for (current = ObjList_GetStart(cache->buffer_list);
       !ObjList_IsEndEdge(cache->buffer_list, current);
       current = ObjListData_GetNext(current)) {

    buffer = (ColorGCCacheBuffer)ObjListData_GetObj(current);
    cmp = ColorGCInstance_CmpToColor(buffer->instance, color);

    if (cmp == 0) { /* ����å�����˸��Ĥ������ */
#ifdef HIT_CACHE
      fprintf(stderr, "H"); /* Hit! */
#endif
      ObjList_MoveObjToStart(cache->buffer_list, current);
      return (buffer->instance);
    }
  }

  /* ����å�����˸��Ĥ����ʤ��ä���� */

  /* �ꥹ�Ȥ��鸡������ */
  instance = ColorGCList_GetColorGCInstance(cache->color_gc_list, color);

  /* ����å�����ɲä��� */
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
/* ColorGCHash ��Ϣ                                                          */
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
/* �ϥå���ؿ�                                                              */
/* ���򸺿����Ƥ����硤����ΰ��֤������ˤ˻��Ѥ��줿�ꤷ�ʤ��褦����ա�  */
/* (���Ȥ��С�                                                               */
/*  ((int)color.red*3 + (int)color.green*2 + (int)color.blue) % hash->number */
/*  �Τ褦�ʥϥå���ؿ����ȡ�16��Ĵ�˸��������Ȥ��ˡ�4096 ���ܿ��ΰ��֤���  */
/*  ���ˤ˻��Ѥ���Ƥ��ޤ���                                                 */
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
/* �ϥå��夫�� ColorGCInstance �����롥                                     */
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
/* ColorName ��Ϣ                                                            */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
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
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

static ColorName ColorName_Destroy(ColorName color_name)
{
  if (color_name == NULL) return (NULL);
  if (color_name->name) free(color_name->name);
  free(color_name);
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
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
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

static ColorNameList ColorNameList_Destroy(ColorNameList list)
{
  if (list == NULL) return (NULL);
  if (list->list) ObjList_Destroy(list->list);
  free(list);
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/* ʸ�����Ϳ����줿��̾���顤RGB�ͤ򸡺����롥                             */
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
/* ColorGC ��Ϣ                                                              */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Create(ColorGCDatabase database, XColor color)
{
  ColorGC color_gc;

  color_gc = (ColorGC)malloc(sizeof(_ColorGC));

  /* �ϥå��夫�鿧��GC�򸡺� */
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
/* RGB �ͤ����������롥                                                      */
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
/* ���֥������Ȥκ��                                                        */
/*---------------------------------------------------------------------------*/

ColorGC ColorGC_Destroy(ColorGC color_gc)
{
  if (!color_gc) return (NULL);

  free(color_gc);

  return (NULL);
}

/*===========================================================================*/
/* ʸ�����ɤ߹����Ѵؿ�                                                      */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ��������Τ���Ρ������Ѥδؿ���                                          */
/* ��������ȡ����ζ�ͭ�����ʤߡ�����å��󥰤θ��̤��礭���ʤ롥            */
/* ����Ƥ��ɸ������ʤ��ȡ�������ͭ�Ǥ��ʤ��Τǡ�̵�̤�¿���ʤ롥            */
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
/* ����GC�Υǡ����١������� name ��Ϳ����줿̾���ο��򸡺����롥            */
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

  /* ��������Τ��ᡤ�������� */
  color = DecreaseColor(color, database->gradation);

  return (ColorGC_Create(database, color));
}

/*---------------------------------------------------------------------------*/
/* XColor ��¤�Τμ���                                                       */
/*---------------------------------------------------------------------------*/

XColor ColorGC_GetColor(ColorGC color_gc)
{
  return (color_gc->instance->color);
}

/*---------------------------------------------------------------------------*/
/* �ԥ������ͤμ���                                                          */
/*---------------------------------------------------------------------------*/

unsigned long ColorGC_GetPixel(ColorGC color_gc)
{
  return (color_gc->instance->color.pixel);
}

/*---------------------------------------------------------------------------*/
/* GC �μ���                                                                 */
/*---------------------------------------------------------------------------*/

GC ColorGC_GetGC(ColorGC color_gc)
{
  return (color_gc->instance->gc);
}

/*---------------------------------------------------------------------------*/
/* �������뤵�μ���                                                          */
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
/* GC �Υꥹ�Ȥκ���                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ñ����ɤ߹����ѡ�(Stream �饤�֥�����)                                 */
/*---------------------------------------------------------------------------*/

static char * GetWord(Stream stream)
{
  char * word;
  word = Stream_GetWord(stream, NULL, 0,
			"",     /* ����ʸ�����ڤ�ʬ���� */
			" \t",  /* ����ʸ���Ǥ��ڤ�ʬ���� */
			"",     /* �����ȹԤ���Ƭʸ�� '#' �ʤ� */
			"\n",   /* ����ʸ�� '\n' �ʤ� */
			"",     /* ������ʸ�� '\"' �ʤ� */
			"",     /* ʸ������Ƭ���ɤߤȤФ�ʸ�� */
			""      /* ʸ�����������ɤߤȤФ�ʸ�� */
			);
  return (word);
}

/*---------------------------------------------------------------------------*/
/* color1 �� color2 ��Ϳ����줿������ֿ���׻�����                         */
/* per �ǥѡ�����ơ�����Ϳ���롥                                            */
/* 0% �ΤȤ���color1�ˤʤ롥100%�ΤȤ���color2 �ˤʤ�                        */
/* 0% ̤������100%����礭�� per �ΤȤ��ˤϡ�flags ���Ф�0�����ꤷ��       */
/* �֤�������ʳ��ΤȤ��ϡ�flags ���Ф� DoRed | DoGreen | DoBlue �ˤʤ롥  */
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

  /* ��ֿ��η׻� */
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
/* GC �Υꥹ�Ȥκ���                                                         */
/* "red" "black" 10                                                          */
/* �Τ褦�ʰ������顤GC�Υꥹ�Ȥ����������֤���                              */
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
    /* ��������Τ��ᡤ�������� */
    middle_color = DecreaseColor(middle_color, database->gradation);

    color_gc = ColorGC_Create(database, middle_color);

    ObjList_InsertObjToEnd(gc_list, color_gc, (ObjDestructor)ColorGC_Destroy);
  }

  return (gc_list);
}

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
#if 0 /* ������ɬ�פʤ� */
      /* ��������Τ��ᡤ�������� */
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
/* GC �Υǡ����١���                                                         */
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

  /* background �� "none" �ξ��ˤ�����ư���褦�ˡ�               */
  /* ColorGC_CreateFromCharacters()��ƤӽФ����ˡ�NULL �ǽ�������롥 */
  database->background_color_gc = NULL;

  /* ������ database ������ƸƤӽФ��Τǡ�ɬ���Ǹ���֤����� */
  database->background_color_gc =
    ColorGC_CreateFromCharacters(database, background);
  /* ����ľ��� return() �����褦�ˤ��뤳�� */

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
/* ���塼�˥���                                                            */
/*---------------------------------------------------------------------------*/

void ColorGCDatabase_OutputHashStatus(ColorGCDatabase database)
{
  ColorGCHash_OutputHashStatus(database->hash);
}

/*****************************************************************************/
/* End of File                                                               */
/*****************************************************************************/
