#include "etc.h"

int Error(char * funcname, char * message)
{
  fprintf(stderr, "Error in %s:%s\n", funcname, message);
  exit (1);
}

void InitializeRand()
{
  srand((unsigned)time(NULL));
}

int Rand(int n)
{
#if 1
  return (rand() % n);
#else
  return (rand() / (RAND_MAX / n + 1));
#endif
}

double DoubleRand(double d)
{
  return ((double)Rand(10001) / 10000.0 * d);
}

/* #include <string.h> なのか #include <strings.h> なのかの                  */
/* 移植性の問題がイヤだったので，strdup() や strcpy() や strcpy() を使わず， */
/* 独自に書いた．                                                            */

int StringLen(char * str)
{
  int i;
  for (i = 0; str[i] != '\0'; i++) { /* None */ }
  return (i);
}

int StringCpy(char * dst, char * src)
{
  int i;
  for (i = 0; src[i] != '\0'; i++) { dst[i] = src[i]; }
  dst[i] = '\0';
  return (i);
}

int StringCat(char * dst, char * src)
{
  return (StringCpy(dst + StringLen(dst), src));
}

int StringCmp(char * str1, char * str2)
{
  int i = 0;

  while (1) {
    if ( (str1[i] == '\0') && (str2[i] == '\0') ) return (0);
    if (str1[i] == '\0') return (-1);
    if (str2[i] == '\0') return ( 1);
    if (str1[i] > str2[i]) return ( 1);
    if (str1[i] < str2[i]) return (-1);
    i++;
  }
}

int StringEqual(char * str1, char * str2)
{
  return (!StringCmp(str1, str2));
}

/* End of Program */
