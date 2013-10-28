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

/*****************************************************************************/
/* ��������                                                                  */
/*****************************************************************************/

#include "StreamP.h"

/*****************************************************************************/
/* �ؿ������                                                                */
/*****************************************************************************/

/*===========================================================================*/
/* ���顼����                                                                */
/*===========================================================================*/

static void Stream_ErrorExit(char * function, char * message)
{
  fprintf(stderr, "Error in %s():%s\n", function, message);
  exit(1);
}

/*===========================================================================*/
/* ���֥������Ȥ�����                                                        */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* ���֥������Ȥ�����                                                        */
/*---------------------------------------------------------------------------*/

Stream Stream_Create(CreateStreamFrom from, void * object)
{
  Stream this;

  this = (Stream)malloc(sizeof(_Stream));
  if (this == NULL)
    Stream_ErrorExit("Stream_Create", "Canot allocate memory.");

  this->unget_buffer = NULL;
  this->end_flag = 0;
  this->from = from;

  switch (this->from) {

  case CREATE_NO_STREAM :
    this->type = STREAM_TYPE_NONE;
    this->object.dummy = NULL;
    break;

  case CREATE_STREAM_FROM_FILE :
    this->type = STREAM_TYPE_FILE_POINTER;
    this->object.file_pointer = fopen((char *)object, "rt");
    if (this->object.file_pointer == NULL) {
      free(this);
      return (NULL);
    }
    break;

  case CREATE_STREAM_FROM_FILE_POINTER :
    this->type = STREAM_TYPE_FILE_POINTER;
    this->object.file_pointer = (FILE *)object;
    break;

  case CREATE_STREAM_FROM_CHARACTERS :
    this->type = STREAM_TYPE_CHARACTERS;
    this->object.characters = (char *)object;
    break;

  default :
    Stream_ErrorExit("Stream_Create", "Invalid type.");
  }

  return (this);
}

/*---------------------------------------------------------------------------*/
/* �ʤˤ�ʤ��Ȥ���������(���ߡ���)                                        */
/*---------------------------------------------------------------------------*/

Stream Stream_CreateFromNone()
{
  return (Stream_Create(CREATE_NO_STREAM, (void *)NULL));
}

/*---------------------------------------------------------------------------*/
/* �ե����뤫������                                                          */
/*---------------------------------------------------------------------------*/

Stream Stream_CreateFromFile(char * filename)
{
  return (Stream_Create(CREATE_STREAM_FROM_FILE, (void *)filename));
}
  
/*---------------------------------------------------------------------------*/
/* �ե�����ݥ��󥿤�������                                                  */
/*---------------------------------------------------------------------------*/

Stream Stream_CreateFromFilePointer(FILE * fp)
{
  return (Stream_Create(CREATE_STREAM_FROM_FILE_POINTER, (void *)fp));
}

/*---------------------------------------------------------------------------*/
/* ʸ����(char * ��)��������                                                 */
/*---------------------------------------------------------------------------*/

Stream Stream_CreateFromCharacters(char * characters)
{
  return (Stream_Create(CREATE_STREAM_FROM_CHARACTERS, (void *)characters));
}

/*===========================================================================*/
/* ���֥������Ȥξõ�                                                        */
/*===========================================================================*/

int Stream_Destroy(Stream this)
{
  UngetBuffer * p;

  if (!this) Stream_ErrorExit("Stream_Destroy", "Object is not created.");

  switch (this->from) {
  case CREATE_NO_STREAM :
    break;
  case CREATE_STREAM_FROM_FILE :
    fclose(this->object.file_pointer);
    break;
  case CREATE_STREAM_FROM_FILE_POINTER :
    break;
  case CREATE_STREAM_FROM_CHARACTERS :
    break;
  default :
    break;
  }

  while (this->unget_buffer) {
    p = this->unget_buffer->next;
    free(this->unget_buffer);
    this->unget_buffer = p;
  }

  free(this);

  return (0);
}

/*===========================================================================*/
/* ʸ����μ���                                                              */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/
/* �ɤ߹���ʸ�����ޤ����뤫�ɤ���                                            */
/*---------------------------------------------------------------------------*/

int Stream_IsEnd(Stream this)
{
  return (this->end_flag);
}

/*---------------------------------------------------------------------------*/
/* Stream ���飱ʸ�������(get)���롥                                        */
/*---------------------------------------------------------------------------*/

int Stream_GetCharacter(Stream this)
{
  UngetBuffer * p;
  int c;

  if (!this) Stream_ErrorExit("Stream_GetCharacter", "Object is not created.");

  if (this->end_flag) {
    c = EOF;
  } else if (this->unget_buffer) {
    c = this->unget_buffer->character;
    p = this->unget_buffer->next;
    free(this->unget_buffer);
    this->unget_buffer = p;
  } else {
    switch (this->type) {

    case STREAM_TYPE_NONE :
      c = EOF;
      this->end_flag = 1;
      break;

    case STREAM_TYPE_FILE_POINTER :
      c = fgetc(this->object.file_pointer);
      if (c == EOF) this->end_flag = 1;
      break;

    case STREAM_TYPE_CHARACTERS :
      c = (int)(*(this->object.characters));
      if (c == '\0') {
	c = EOF;
	this->end_flag = 1;
      } else {
	(this->object.characters)++;
      }
      break;

    default :
      Stream_ErrorExit("Stream_GetCharacter", "Invalid type.");
    }
  }
  return (c);
}

/*---------------------------------------------------------------------------*/
/* Stream �ˣ�ʸ�����ֵ�(unget)���롥                                        */
/*---------------------------------------------------------------------------*/

int Stream_UngetCharacter(Stream this, int c)
{
  UngetBuffer * p;
  char character = (char)c;

  if (!this)
    Stream_ErrorExit("Stream_UngetCharacter", "Object is not created.");

  if (c == EOF) {
    this->end_flag = 1;
    return (EOF);
  }

  p = (UngetBuffer *)malloc(sizeof(UngetBuffer));
  if (!p) Stream_ErrorExit("Stream_UngetCharacter", "Cannot allocate memory.");

  this->end_flag = 0;
  p->character = character;
  p->next = this->unget_buffer;
  this->unget_buffer = p;

  return (character);
}

/*---------------------------------------------------------------------------*/
/* Stream ��ʸ������ֵ�(unget)���롥                                        */
/*---------------------------------------------------------------------------*/

int Stream_UngetCharacters(Stream this, char * characters)
{
  int n = 0;
  int i;

  if (!this)
    Stream_ErrorExit("Stream_UngetCharacters", "Object is not created.");

  for (i = 0; characters[i] != '\0'; i++) { /* None */ }
  for(i--; i >= 0; i--) {
    Stream_UngetCharacter(this, characters[i]);
    n++;
  }

  return (n);
}

/*---------------------------------------------------------------------------*/
/* ʸ�������ʸ�������뤫�ɤ�����Ƚ��                                        */
/*---------------------------------------------------------------------------*/

static int CheckWord(char * s, int c)
{
  if (c == EOF) return (0);
  if (s == NULL) return (0);

  while (*s != '\0') {
    if (c == *s) return (1);
    s++;
  }

  return (0);
}

/*---------------------------------------------------------------------------*/
/* ʸ�����ʸ�����ɲ�                                                        */
/*---------------------------------------------------------------------------*/

static char * AddCharacter(char * buffer, int * buf_len, int * i, char c,
			   int extend)
{
  if (*i >= *buf_len - 1) {
    if (!extend) return (buffer);
    (*buf_len) *= 2;
    if (!buffer) buffer = (char *)malloc(sizeof(char) * (*buf_len));
    else buffer = (char *)realloc((void *)buffer, sizeof(char) * (*buf_len));
    if (!buffer) Stream_ErrorExit("AddCharacter", "Cannot allocate memory.");
  }

  buffer[*i] = c;
  (*i)++;
  return (buffer);
}

/*---------------------------------------------------------------------------*/
/* Stream ����ȡ�����Ƕ��ڤä�ʸ������ɤ߹��ߡ�ʸ����(String ��)���֤���  */
/* ������䥳���ȹԤβ���Ԥ���                                          */
/*---------------------------------------------------------------------------*/
/* split1 �ϡ�ʣ���ζ��ڤ�ʸ����Ϣ³������ˤ⡤�̥�ʸ���Ȥ����ڤ�ʬ���롥 */
/* split2 �ϡ�ʣ���ζ��ڤ�ʸ����Ϣ³������ˤϡ��ҤȤĤζ��ڤ�ʸ���Ȥ���   */
/* �������롥                                                                */
/*---------------------------------------------------------------------------*/

char * Stream_GetWord(Stream this,      /* �ɤ߹��߸��� Stream */
		      char * buffer,
		      int buffer_length,
		      char * split1,    /* ����ʸ�����ڤ�ʬ���� */
		      char * split2,    /* ����ʸ���Ǥ��ڤ�ʬ���� */
		      char * comment,   /* �����ȹԤ���Ƭʸ�� '#' �ʤ� */
		      char * lineend,   /* ����ʸ�� '\n' �ʤ� */
		      char * quotation, /* ������ʸ�� '\"' �ʤ� */
		      char * head_skip, /* ʸ������Ƭ���ɤߤȤФ�ʸ�� */
		      char * tail_skip  /* ʸ�����������ɤߤȤФ�ʸ�� */
		      )
{
  int c;
  int i = 0;
  int extend = 0; /* �Хåե����ĥ�Ǥ��뤫 */

  if (!this) Stream_ErrorExit("Stream_GetWord", "Object is not created.");

  do {
    c = Stream_GetCharacter(this);
    if (CheckWord(comment, c)) {
      do {
	c = Stream_GetCharacter(this);
      } while (!CheckWord(lineend, c) && (c != EOF));
    }
  } while (CheckWord(split2, c) || CheckWord(comment, c) ||
	   CheckWord(lineend, c));

  if (c == EOF) return (NULL); /* �ɤ߽Ф�ñ�줬¸�ߤ��ʤ� */

  if (!buffer) {
    extend = 1;
    buffer_length = 10;
    buffer = (char *)malloc(sizeof(char) * buffer_length);
    if (!buffer) Stream_ErrorExit("Stream_GetWord", "Cannot allocate memory.");
  }

  if (CheckWord(quotation, c)) {
    while (1) {
      c = Stream_GetCharacter(this);
      if (CheckWord(quotation, c) || (c == EOF)) break;
      buffer = AddCharacter(buffer, &buffer_length, &i, c, extend);
    }
  } else {

    while (CheckWord(head_skip, c) && (c != EOF))
      c = Stream_GetCharacter(this);

    while (1) {
      if (c == EOF) {
	break;
      }
      if (CheckWord(comment, c) || CheckWord(quotation, c) ||
	  CheckWord(split1, c) || CheckWord(split2, c) ||
	  CheckWord(lineend, c)) {
	Stream_UngetCharacter(this, c);
	break;
      }
      buffer = AddCharacter(buffer, &buffer_length, &i, c, extend);
      c = Stream_GetCharacter(this);
    }

    while ((i > 0) && (CheckWord(tail_skip, buffer[i - 1]))) i--;
  }

  if (i <= buffer_length - 1) buffer[i] = '\0';

  return (buffer);
}

/*****************************************************************************/
/* �����ޤ�                                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* End of File.                                                              */
/*****************************************************************************/
