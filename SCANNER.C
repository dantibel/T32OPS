/* Belov Danil, DB6, 20.12.2021 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "ops.h"

 /* Check name of token being keyword function */
static VOID UpdateKeyword( TOK *T )
{
  INT i;

  struct
  {
    CHAR *Name;   /* keyword string value */
    KEYWORD KeyW; /* keyword constant value */
  } Kws[] =
  {
    {"if", KW_IF},
    {"else", KW_ELSE},
    {"while", KW_WHILE},
    {"scan", KW_SCAN},
    {"print", KW_PRINT}
  };
 
  for (i = 0; i < sizeof(Kws) / sizeof(Kws[0]); i++)
    if (strcmp(Kws[i].Name, T->Name) == 0)
    {
      T->Id = TOK_KEYW;
      T->KeyW = Kws[i].KeyW; 
      return;
    }
} /* End of 'UpdateKeyword' function */

/* Check name of token being function function */
static VOID UpdateFunction( TOK *T )
{
  INT i;

  for (i = 0; i < sizeof(FuncTable) / sizeof(FuncTable[0]); i++)
    if (strcmp(FuncTable[i].Name, T->Name) == 0)
    {
      T->Id = TOK_FUNC;
      T->Func = FuncTable[i].Func;
      return;
    }
} /* End of 'UpdateFunctions' function */

/* Scan expression function */
BOOL Scanner( QUEUE *Q, CHAR *S )
{
  INT i;
  TOK T;

  while (*S != 0 )
  {
    if (*S == '/' && S[1] == '/')
      return FALSE;

    T.Op = OP_NULL;
    T.Id = 0;
    switch(*S)
    {
    case ' ':
    case '\n':
    case '\t':
      S++;
      continue;

    case '-':
    case '+':
    case '*':
    case '/':
    case '%':
    case '^':
    case '(':
    case ')':
    case '=':
    case ',':
    case '<':
    case '>':
    case '!':
      T.Id = TOK_OP;
      T.Op = *S++;
      if ((UCHAR)*S == '=' && (T.Op == '<' || T.Op == '=' || T.Op == '>' || T.Op == '!'))
        T.Op += TOK_OP_ADD_EQ, S++;
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      T.Id = TOK_NUM;
      T.Num = *S++ - '0';
      while(isdigit((UCHAR)*S))
        T.Num = T.Num * 10 + *S++ - '0';
      if (*S == '.')
      {
        DBL denum = 1;
        S++;
        while(isdigit((UCHAR)*S))
          T.Num += (*S++ - '0') / (denum *= 10);
      }
      break;

    case ';':
    case '{':
    case '}':
      T.Id = TOK_SYM;
      T.Op = *S++;
      break;

    default:
      i = 0;

      if (isalpha((UCHAR)*S) || *S == '"')
      {
        BOOL IsText;

        IsText = (*S == '\"');
        if (IsText)
        {
          CHAR Prev = 0;

          T.Id = TOK_TXT;
          memset(T.Text, 0, MAX_TEXT);
          T.Name[0] = 0;

          S++;
          while (*S != '\"' && *S != 0)
          {
            if (*S == '\\')
            {
              S++;
              T.Text[i++] = (*S == 'n') ? '\n' : (*S == '\\') ? '\\' : (*S == 't') ? '\t' : (*S == 'r') ? '\r' : (*S == 'a') ? '\a' : (*S == 'v') ? '\v' : '\0';
              S++;
            }
            else
              T.Text[i++] = *S++;
          }
          T.Text[i++] = *S++;

          if (T.Text[i - 1] != '"')
            Error("Missing '\"'");
          else
            T.Text[i - 1] = 0;
        }
        else
        {
          T.Id = TOK_VAR;
          memset(T.Name, 0, MAX_NAME);

          T.Name[i++] = *S++;
          while (S != 0 && (isalpha(*S) || isdigit(*S) || *S == '_'))
            T.Name[i++] = *S++;
        }
      }
      else
        Error("Unrecognised symbol '%c'", *S);
      UpdateKeyword(&T);
      UpdateFunction(&T);
    }
    Put(Q, T);
  }
  return TRUE;
} /* End of 'Scanner' funstion*/
/*END OF 'SCANNER.C' FILE*/