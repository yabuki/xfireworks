#include <stdio.h>
#include <stdlib.h>

#if 1
#define ONE_LINE
#endif

main()
{
  char buffer[1000];
  char * p;

  printf("static char * default_xfireworks_conf =\n");
#ifdef ONE_LINE
  printf("\"");
#endif
  while (fgets(buffer, 1000, stdin) != NULL) {
    if ((buffer[0] != '\0') && (buffer[0] != '#')) {

#ifndef ONE_LINE
      printf("\"");
#endif
      for (p = buffer; *p != '\0'; p++) {
	if      (*p == '\n') break;
	else if (*p == '\t') printf(" ");
	else if (*p == '\"') printf("\\\"");
	else if (*p == '\'') printf("\\\'");
	else printf("%c", *p);
      }
#ifdef ONE_LINE
      printf("\\n");
#else
      printf("\\n\"\n");
#endif
    }
  }
#ifdef ONE_LINE
  printf("\";\n");
#else
  printf(";\n");
#endif
  exit (0);
}

/* End of Program */
