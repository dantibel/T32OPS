/* Belov Danil, DB6, 20.12.2021 */

#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <setjmp.h>

#include "ops.h"

#define STR_MAX 100

/* Input string function */
VOID GetStr( CHAR *S, INT Len )
{
  INT i = 0;
  CHAR c;

  while ((c = getchar()) != '\n')
    if (S != NULL && i < Len - 1)
      S[i++] = c;

  if (S != NULL && i < Len - 1)
    S[i] = 0;
} /* End of 'GetStr' function */

jmp_buf ExprJumpBuf;

/* Error function */
VOID Error( CHAR *Str, ... )
{
  va_list ap;

  SET_CONSOLE_RED();
  printf("ERROR: ");
  va_start(ap, Str);
  vprintf(Str, ap);
  va_end(ap);
  RESET_CONSOLE();
  printf("\n");

  longjmp(ExprJumpBuf, 1);
} /* End of 'Error' function */

CMD *Proga;

/* Program main function */
VOID main ( VOID )
{
  FILE *F;
  static CHAR Buf[10000];

  SetDbgMemHooks();

  if (setjmp(ExprJumpBuf))
  {
    ClearQueue(&Queue1);
    ClearStack(&StackEval);
    ClearQueue(&TokList);
    ClearStack(&Stack2);
    ClearVarTable();
    CmdClear(&Proga);
    ClearPrinterList();
    return;
  }

  if ((F = fopen("Code.txt", "r")) == NULL)
    Error("Can't open file");

  SET_CONSOLE_CYAN();
  printf("Script:\n");
  RESET_CONSOLE();
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (!Scanner(&TokList, Buf))
    {
      SET_CONSOLE_YELLOW();
      printf("%s", Buf);
      RESET_CONSOLE();
    }
    else
      printf("%s", Buf);
  }
  fclose(F);

  SET_CONSOLE_CYAN();
  // printf("\n\nScanned:\n");
  RESET_CONSOLE();
  // DisplayQueue(&TokList);

  SET_CONSOLE_CYAN();
  printf("\nOutput:\n");
  RESET_CONSOLE();
  ParseProgram();
  DoCmd(Proga);
  SET_CONSOLE_CYAN();
  printf("\n\nVariables:\n");
  RESET_CONSOLE();
  DisplayVarTable();
  printf("\n");

  printf("Press any button to exit\n");
  _getch();
  Error("Safe exit of program");
} /* End of 'main' function*/
