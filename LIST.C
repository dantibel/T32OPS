/* Belov Danil, DB6, 20.12.2021 */

#include <string.h>
#include <stdio.h>

#include "ops.h"

struct tagFNTB FuncTable[] =
{
  {"sin", FN_SIN},
  {"abs", FN_ABS},
  {"pi", FN_PI},
  {"time", FN_TIME},
};

/* Print one token function */
static VOID PrintTok( TOK T )
{
  INT i;

  switch (T.Id)
  {
  case TOK_NUM:
    printf("%f", T.Num);
    return;
  case TOK_OP:
    if (T.Op > 255)
      printf("%c=", T.Op - TOK_OP_ADD_EQ);
    else
      printf("%c", T.Op);
    return;
  case TOK_SYM:
    printf("%c", T.Op);
    return;
  case TOK_VAR:
    printf("%s <Variable>", T.Name);
    return;
  case TOK_TXT:
    printf("%s <Text>", T.Text);
    return;
  case TOK_FUNC:
    for (i = 0; i < sizeof(FuncTable) / sizeof(FuncTable[0]); i++)
      if (T.Func == FuncTable[i].Func)
      {
        printf(FuncTable[i].Name);
        return;
      }
    return;
  case TOK_KEYW:
    switch (T.KeyW)
    {
    case KW_IF:
      printf("if");
      break;
    case KW_ELSE:
      printf("else");
      break;
    case KW_WHILE:
      printf("while");
      break;
    case KW_SCAN:
      printf("scan");
      break;
    case KW_PRINT:
      printf("print");
      break;
    }
    return;
  default:
    printf("<Unknown token>");
    return;
  }
} /* End of 'PrintTok' function */

/* Print list function */
VOID DisplayList( LIST *L )
{
  if (L == NULL)
    printf("<No elements>\n");
  else
    while (L != NULL)
    {
      PrintTok(L->T);
      printf((L = L->Next) == NULL ? "\n" : ", ");
    }
} /* End of 'Display' function */

/* Clear list function */
VOID ClearList( LIST **L )
{
  while (*L != NULL)
  {
    LIST *Old = *L;

    *L = (*L)->Next;
    free(Old);
  }
  *L = NULL;
} /* End of 'ClearList' function */


/* Add token to queue function */
VOID Put( QUEUE *Q, TOK NewT )
{
  LIST *NewElement;

  if ((NewElement = malloc(sizeof(LIST))) == NULL)
    Error("No enough memeory");
  NewElement->T = NewT;
  NewElement->Next = NULL;

  if (Q->Head == NULL)
    Q->Head = Q->Tail = NewElement;
  else
  {
    Q->Tail->Next = NewElement;
    Q->Tail = NewElement;
  }
} /* End of 'Put' function */

/* Get token from queue function */
BOOL Get( QUEUE *Q, TOK *OldT )
{
  LIST *OldElement;

  if (Q->Head == NULL)
    return FALSE;

  OldElement = Q->Head;

  if (OldT != NULL)
    *OldT = OldElement->T;
  Q->Head = Q->Head->Next;

  free(OldElement);
  OldElement = NULL;
  return TRUE;
} /* End of 'Get' function */

/* Clear queue function */
VOID ClearQueue( QUEUE *Q )
{
  ClearList(&Q->Head);
  Q->Tail = NULL;
} /* End of 'ClearQueue' function */

/* Input queue function */
VOID DisplayQueue( QUEUE *Q )
{
  DisplayList(Q->Head);
} /* End of 'DisplayQueue' function */


/* Add token to stack function */
VOID Push( STACK *S, TOK NewT )
{
  LIST *NewElement;

  if ((NewElement = malloc(sizeof(LIST))) == NULL)
    Error("No enough memory");
  else
    NewElement->T = NewT;

  if (S->Top == NULL)
  {
    S->Top = NewElement;
    S->Top->Next = NULL;
  }
  else
  {
    NewElement->Next = S->Top;
    S->Top = NewElement; 
  }
} /* End of 'Push' function */

/* Get token from stack function */
BOOL Pop ( STACK *S, TOK *OldT )
{
  LIST *OldElement;

  if (S == NULL || S->Top == NULL)
    return FALSE;

  OldElement = S->Top;
  if (OldT != NULL)
    *OldT = OldElement->T;

  S->Top = S->Top->Next;

  free(OldElement);
  return TRUE;
} /* End of 'Pop' funtion */

/* Clear queue function */
VOID ClearStack( STACK *S )
{
  ClearList(&S->Top);
} /* End of 'ClearQueue' function */

/* Input stack function */
VOID DisplayStack( STACK *S )
{
  DisplayList(S->Top);
} /* End of 'DisplayStack' function */
/*END OF 'LISTS.C' FILE*/