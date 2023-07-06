/* Belov Danil, DB6, 20.12.2021 */

#include <stdio.h>
#define M_PI       3.14159265358979323846   // pi
#include <math.h>
#include <time.h>

#include "ops.h"

typedef struct tagNUMLIST NUMLIST;
struct tagNUMLIST
{
  DBL Num;
  NUMLIST *Next;
};

NUMLIST *PrinterList;

/* Clear 'PrinterList' function */
VOID ClearPrinterList( VOID )
{
  while (PrinterList != NULL)
  {
    NUMLIST *Old = PrinterList;

    PrinterList = PrinterList->Next;
    free(Old);
  }
} /* End of 'ClearPrinterList' function */

VOID Printer( LIST *Expr )
{
  DBL res = Eval(Expr);
  
  while (PrinterList != NULL)
  {
    printf("%lf ", PrinterList->Num);
    PrinterList = PrinterList->Next;
  }
  printf("%lf ", res);
  ClearPrinterList();
}

/* Do comand function */
VOID DoCmd( CMD *C )
{
  DBL Value;

  while (C != NULL)
  {
    switch (C->Id)
    {
    case CMD_EXPR:
      Eval(C->Expr);
      break;
    case CMD_IF:
      if (Eval(C->Expr))
        DoCmd(C->C1);
      else
        DoCmd(C->C2);
      break;
    case CMD_WHILE:
      while (Eval(C->Expr))
        DoCmd(C->C1);
      break;
    case CMD_PRINT:
      if (C->Expr->T.Id == TOK_TXT)
        printf("%s", C->Expr->T.Text);
      else
        printf("%lf ", Eval(C->Expr));
    break;
    case CMD_SCAN:
      while (C->Expr != NULL)
      {
        if (C->Expr->T.Id == TOK_VAR)
        {
          scanf("%lf", &Value);
          SetValue(C->Expr->T.Name, Value);
        }
        C->Expr = C->Expr->Next;
      }
      break;
    }
    C = C->Next;
  }
} /* End of 'DoCmd' function */

STACK StackEval;

/* Calculate numbers from queue function */
static DBL Eval( LIST *Expr )
{
  TOK T, A, B;
  NUMLIST **ptr = &PrinterList;

  // DisplayList(Expr);
  ClearStack(&StackEval);
  B.Num = 0;
  while (Expr != NULL)
  {
    T = Expr->T;
    Expr = Expr->Next;
    if (T.Id == TOK_NUM || T.Id == TOK_VAR)
      Push(&StackEval, T);
    else if (T.Id == TOK_OP)
    {
      if (T.Op != '@')
      {
        if (!Pop(&StackEval, &B))
        {
          Error("Can't get numbers from stack");
          return 0;
        }
        else if (B.Id == TOK_VAR)
          B.Num = GetValue(B.Name);
      }
      if (!Pop(&StackEval, &A))
      {
        Error("Can't get numbers from stack");
        return 0;
      }
      else if (A.Id == TOK_VAR)
        A.Num = GetValue(A.Name);

      switch (T.Op)
      {
      case '+':
        A.Num += B.Num;
        A.Id = TOK_NUM;
        break;
      case '-':
        A.Num -= B.Num;
        A.Id = TOK_NUM;
        break;
      case '*':
        A.Num *= B.Num;
        A.Id = TOK_NUM;
        break;
      case '/':
        if (B.Num != 0)
          A.Num /= B.Num;
        else
          Error("Evaluation error");
        A.Id = TOK_NUM;
        break;
      case '%':
        if (B.Num != 0)
          A.Num = fmod(A.Num, B.Num);
        else
          Error("Evaluation error");
        A.Id = TOK_NUM;
        break;
      case '^':
        if (A.Num < 0 && fmod(B.Num, 0.000002) == 0)
          Error("Evaluation error");
        else
          A.Num = pow(A.Num, B.Num);
        A.Id = TOK_NUM;
        break;
      case '@':
        A.Num = -A.Num;
        A.Id = TOK_NUM;
        break;
      case ',':
        A.Id = TOK_NUM;
        if ((*ptr = malloc(sizeof(NUMLIST))) == NULL)
          Error("No enough memeory");
        (*ptr)->Num = A.Num;
        ptr = &(*ptr)->Next;
        *ptr = NULL;
        A.Num = B.Num;
        break;
      case '=':
        if (A.Id != TOK_VAR)
          Error("Left operand must be left-value");
        SetValue(A.Name, B.Num);
        A.Num = B.Num;
        break;
      case '<':
        A.Num = A.Num < B.Num;
        A.Id = TOK_NUM;
        break;
      case '<' + TOK_OP_ADD_EQ:
        A.Num = A.Num <= B.Num;
        A.Id = TOK_NUM;
        break;
      case '>':
        A.Num = A.Num > B.Num;
        A.Id = TOK_NUM;
        break;
      case '>' + TOK_OP_ADD_EQ:
        A.Num = A.Num >= B.Num;
        A.Id = TOK_NUM;
        break;
      case '!' + TOK_OP_ADD_EQ:
        A.Num = A.Num != B.Num;
        A.Id = TOK_NUM;
        break;
      case '=' + TOK_OP_ADD_EQ:
        A.Num = A.Num == B.Num;
        A.Id = TOK_NUM;
        break;
      }
      Push(&StackEval, A);
    }
    else if (T.Id == TOK_FUNC)
    {
      A.Id = TOK_NUM;
      switch (T.Func)
      {
      case FN_SIN:
        if (!Pop(&StackEval, &A))
        {
          Error("Can't get numbers from stack");
          return 0;
        }
        else if (A.Id == TOK_VAR)
          A.Num = GetValue(A.Name);
        A.Num = sin(A.Num);
        A.Id = TOK_NUM;
        break;
      case FN_ABS:
        if (!Pop(&StackEval, &A))
        {
          Error("Can't get numbers from stack");
          return 0;
        }
        else if (A.Id == TOK_VAR)
          A.Num = GetValue(A.Name);
        A.Num = fabs(A.Num);
        A.Id = TOK_NUM;
        break;
      case FN_PI:
        A.Num = M_PI;
        A.Id = TOK_NUM;
        break;
      case FN_TIME:
        A.Num = clock() / (DBL)CLOCKS_PER_SEC;
        A.Id = TOK_FUNC;
        break;
      }
      Push(&StackEval, A);
    }
  }
  if (StackEval.Top == NULL)
    Error("No enough numbers in expression");
  else if (StackEval.Top->T.Id == TOK_VAR)
    return GetValue(StackEval.Top->T.Name);
  return StackEval.Top->T.Num;
} /* End of 'Eval' function */
/* END OF 'EVAL.C' FILE */
