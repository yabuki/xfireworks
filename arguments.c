#include "arguments.h"
#include "etc.h"

static void DeleteArgument(int * argc, char ** argv, int i)
{
  (*argc)--;
  for (; i < (*argc); i++)
    argv[i] = argv[i + 1];
}

static int CheckArguments(int * argc, char ** argv, char * s)
{
  int i, r = 0;

  for (i = 1; i < *argc; i++) {
    if (!StringCmp(s, argv[i])) {
      DeleteArgument(argc, argv, i);
      r = 1;
    }
  }
  return (r);
}

static char * GetStringFromArguments(int * argc, char ** argv, char * s)
{
  int i;
  char * p = (char *)NULL;

  for (i = 1; i < *argc; i++) {
    if (!StringCmp(s, argv[i])) {
      if (i == *argc - 1) {
        DeleteArgument(argc, argv, i);
      } else {
        DeleteArgument(argc, argv, i);
        p = argv[i];
        DeleteArgument(argc, argv, i);
      }
    }
  }
  return (p);
}

int Arguments_Read(int * argc, char ** argv, Argument arguments[])
{
  int i;
  char * p;
  void (*funcp)();
  int n = 0;

  for (i = 0; arguments[i].name != NULL; i++) {
    if (arguments[i].flag == ARGUMENTS_NONE) {
      /* None */
    } else if (arguments[i].flag == ARGUMENTS_FUNCTION) {
      if (CheckArguments(argc, argv, arguments[i].name)) {
        funcp = (void (*)())(arguments[i].value);
        (*funcp)();
	n++;
      }
    } else if (arguments[i].flag == ARGUMENTS_FLAG) {
      if ((p = GetStringFromArguments(argc,argv,arguments[i].name)) != NULL) {
        if        ( (!StringCmp(p, "off")) || (!StringCmp(p, "OFF")) ) {
	  *((short int *)(arguments[i].value)) = ARGUMENTS_FALSE;
	  n++;
	} else if ( (!StringCmp(p, "on" )) || (!StringCmp(p, "ON" )) ) {
	  *((short int *)(arguments[i].value)) = ARGUMENTS_TRUE;
	  n++;
	}
      }
    } else if (arguments[i].flag == ARGUMENTS_FLAG_ON) {
      if (CheckArguments(argc, argv, arguments[i].name)) {
        *((short int *)(arguments[i].value)) = ARGUMENTS_TRUE;
	n++;
      }
    } else if (arguments[i].flag == ARGUMENTS_FLAG_OFF) {
      if (CheckArguments(argc, argv, arguments[i].name)) {
        *((short int *)(arguments[i].value)) = ARGUMENTS_FALSE;
	n++;
      }
    } else if (arguments[i].flag == ARGUMENTS_INTEGER) {
      if ((p=GetStringFromArguments(argc, argv, arguments[i].name)) != NULL) {
        *((int *)(arguments[i].value)) = atoi(p);
	n++;
      }
    } else if (arguments[i].flag == ARGUMENTS_STRING) {
      if ((p=GetStringFromArguments(argc, argv, arguments[i].name)) != NULL) {
        *((char **)(arguments[i].value)) = p;
	n++;
      }
    }
  }

  while (*argc > 1) {
    fprintf(stderr, "ERROR : Unknown Argument : %s\n", argv[1]);
    DeleteArgument(argc, argv, 1);
  }

  return (n);
}

/*****************************************************************************/
/* End of Program.                                                           */
/*****************************************************************************/
