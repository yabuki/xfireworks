#include "CalculatorP.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#include "etc.h"

Calculator Calculator_Create(int degree_number)
{
  Calculator cal;
  int i;

  cal = (Calculator)malloc(sizeof(_Calculator));
  if (!cal) Error("Calculator_Create", "Cannot allocate memory.");

  cal->degree_number = degree_number;

  cal->sine = (double *)malloc(sizeof(double) * cal->degree_number);
  if (!cal->sine) Error("Calculator_Create", "Cannot allocate memory.");

  for (i = 0; i < cal->degree_number; i++) {
    cal->sine[i] = sin(i * (360.0 / cal->degree_number * M_PI / 180.0));
  }

  return (cal);
}

Calculator Calculator_Destroy(Calculator cal)
{
  if (!cal) return (NULL);
  if (cal->sine) free(cal->sine);
  free(cal);
  return (NULL);
}

double Calculator_GetSin(Calculator cal, int degree)
{
  while (degree < 0) degree += cal->degree_number;
  degree = degree % cal->degree_number;

  return (cal->sine[degree]);
}

double Calculator_GetCos(Calculator cal, int degree)
{
  return (Calculator_GetSin(cal, cal->degree_number / 4 - degree));
}

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
