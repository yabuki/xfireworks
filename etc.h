#ifndef _Etc_h_INCLUDED_
#define _Etc_h_INCLUDED_

#include <stdio.h>
#include <stdlib.h>

int Error(char * funcname, char * message);
void InitializeRand();
int Rand(int n);
double DoubleRand(double d);

int StringLen(char * str);
int StringCpy(char * dst, char * src);
int StringCat(char * dst, char * src);
int StringCmp(char * str1, char * str2);
int StringEqual(char * str1, char * str2);

#endif

/* End of Program */
