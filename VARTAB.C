/* Belov Danil, DB6, 20.12.2021 */

#include <stdio.h>
#include <string.h>

#include "ops.h"

/* Binary tree struct type */
typedef struct tagTREE TREE;
struct tagTREE
{
  CHAR Name[MAX_NAME];
  DBL Value;
  TREE *More, *Less;
} *VarTable = {NULL};

/* Set variable to tree function */
static TREE * AccessTree( CHAR *Name )
{
  INT n;
  TREE **T = &VarTable;

  while (*T != NULL && (n = strcmp((*T)->Name, Name)) != 0)
    if (n > 0)
      T = &(*T)->More;
    else
      T = &(*T)->Less;

  if (*T == NULL)
  {
    if ((*T = malloc(sizeof(TREE))) == NULL)
      Error("No enough memory");
    strcpy((*T)->Name, Name);
    (*T)->Less = (*T)->More = NULL;
  }
  return *T;
} /* End of 'SetValue' function */

/* Set value of variable function */
VOID SetValue( CHAR *Name, DBL Value )
{
  AccessTree(Name)->Value = Value;
} /* End of 'SetTree' function */

/* Get value of variable function */
DBL GetValue( CHAR *Name )
{
  return AccessTree(Name)->Value;
} /* End of 'GetValue' function */

/* Output tree function */
static VOID PutTree( TREE *T )
{
  if (T != NULL)
  {
    PutTree(T->Less);
    printf("%s: %lf ", T->Name, T->Value);
    PutTree(T->More);
  }
} /* End of 'PutTree' function */

/* Output variables table function */
VOID DisplayVarTable( VOID )
{
  if (&VarTable->Name == NULL)
  {
    printf("<No variables>");
  }
  else
  {
    PutTree(VarTable->Less);
		printf("%s: %lf ", VarTable->Name, VarTable->Value);
    PutTree(VarTable->More);
  }
} /* End of 'DisplayVarTable' functoin */

/* Clear tree function */
static VOID ClearTree( TREE **T )
{
  if (*T != NULL)
  {
    ClearTree(&(*T)->Less);
    ClearTree(&(*T)->More);
    free(*T);
    *T = NULL;
  }
} /* End of 'ClearTree' function */

/* Clear variable tree function */
VOID ClearVarTable( VOID )
{
  if (&VarTable->Name != NULL)
  {
    ClearTree(&VarTable->Less);
    ClearTree(&VarTable->More);
    free(VarTable);
    VarTable = NULL;
  }
} /* end of 'ClearVarTable' function */
/* END OF 'VARTAB.C' FILE*/