#ifndef _Calculator_h_INCLUDED_
#define _Calculator_h_INCLUDED_

typedef struct _Calculator * Calculator;

#define CALCULATOR_DEFAULT_DEGREE 360

Calculator Calculator_Create(int degree_number);
Calculator Calculator_Destroy(Calculator cal);
double Calculator_GetSin(Calculator cal, int degree);
double Calculator_GetCos(Calculator cal, int degree);

#endif

/*****************************************************************************/
/* End of Program                                                            */
/*****************************************************************************/
