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
/*
static VOID UpdateFunction( TOK *T )
{
  INT i;

  struct
  {
    CHAR *Name;
    FUNCS Fn; 
  } Fnc[] =
  {
    {"scan", FN_SCAN},
    {"print", FN_PRINT},
    {"sin", FN_SIN}
  };
 
  for (i = 0; i < sizeof(Fnc) / sizeof(Fnc[0]); i++)
    if (strcmp(Fnc[i].Name, T->Name) == 0)
    {
      T->Id = TOK_FN;
      T->Fn = Fnc[i].Fn; 
      return;
    }
} /* End of 'UpdateFunctions' function */

/* Scan expression function */
VOID Scanner( QUEUE *Q, CHAR *S )
{
  INT i;
  TOK T;

  while (*S != 0)
  {
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
      memset(T.Name, 0, MAX_NAME);
			memset(T.Text, 0, MAX_NAME);
      if (isalpha((UCHAR)*S) || *S == '"')
      {
				BOOL IsText, WasSpace = FALSE;

        IsText = *S == '"';
				T.Text[i++] = *S++;
				if (IsText)
          while (S != 0)					 
					  T.Text[i++] = *S++;
				else
					while (S != 0 && (isalpha(*S) || isdigit(*S) || *S == '_'))
						T.Text[i++] = *S++;

				printf("\n%s\n", T.Text);
				if (!IsText)
				{
					T.Id = TOK_VAR;
					strcpy(T.Name, T.Text);
				}
				else
				  if (T.Name[i - 1] != '"')
					  Error("Missing '\"'");
					else
						T.Id = TOK_TXT;
      }
      else
      {
        Error("Unrecognised symbol '%c'", *S);
        return;
      }
      UpdateKeyword(&T);
			/* UpdateFunction(&T); */
    }
    Put(Q, T);
  }
} /* End of 'Scanner' funstion*/
/*END OF 'SCANNER.C' FILE*/