/* Belov Danil, DB6, 20.12.2021 */

#include <stdio.h>

#include "ops.h"

#define NextTok() \
  (IsEOTL ? Error("Unexpected end-of-data") : \
    Get(&TokList, &TokCurrent) ?  (VOID)0 : (VOID)(IsEOTL = TRUE))

#define IsTokAnyOp() \
  (!IsEOTL && TokCurrent.Id == TOK_OP)
#define IsTokAnySym() \
  (!IsEOTL && TokCurrent.Id == TOK_SYM)
#define IsTokAnyNum() \
  (!IsEOTL && TokCurrent.Id == TOK_NUM)
#define IsTokAnyVar() \
  (!IsEOTL && TokCurrent.Id == TOK_VAR)
#define IsTokAnyFn() \
  (!IsEOTL && TokCurrent.Id == TOK_FN)
#define IsTokTxt() \
  (!IsEOTL && TokCurrent.Id == TOK_TXT)

#define IsTokOp(C) \
  (IsTokAnyOp() && TokCurrent.Op == (C))
#define IsTokSym(C) \
  (IsTokAnySym() && TokCurrent.Op == (C))
#define IsTokKw(Keyword) \
  (!IsEOTL && TokCurrent.Id == TOK_KEYW && TokCurrent.KeyW == KW_ ## Keyword)
#define IsTokFn(Func) \
  (!IsEOTL && TokCurrent.Id == TOK_FN && TokCurrent.Func == FN_ ## Func)


#define ParseAnyTok() \
  NextTok()
#define ParseAnyNum() \
  (IsTokAnyNum() ? NextTok() : Error("Expected any number"))
#define ParseAnyOp() \
  (IsTokAnyOp() ? NextTok() : Error("Expected any operator"))
#define ParseOp(C) \
  (IsTokOp(C) ? NextTok() : Error("Expected operator: %c", (C)))
#define ParseSym(C) \
  (IsTokSym(C) ? NextTok() : Error("Expected symbol: %c", (C)))
#define ParseNum(X) \
	(IsTokAnyNum() ? (*(X) = TokCurrent.Num, NextTok()) : Error("Expected number: %lf", (X)))
#define ParseKw(Keyword) \
  (IsTokKw(Keyword) ? NextTok() : Error("Expected keyword: \"" #Keyword "\""))
#define ParseFn(Func) \
  (IsTokFn(Func) ? NextTok() : Error("Expected function: \"" #Func "\""))

QUEUE Queue1;
QUEUE TokList;
STACK Stack2;
TOK TokCurrent;
BOOL IsEOTL;

/* Get priotity of operation function */
static INT GetPrior( INT Op )
{
  switch(Op)
  {
  case '(':
    return 0;
  case ')':
    return 1;
  case ',':
    return 2;
  case '<':
  case '<' + TOK_OP_ADD_EQ:
  case '>':
  case '>' + TOK_OP_ADD_EQ:
  case '=':
  case '=' + TOK_OP_ADD_EQ:
  case '!' + TOK_OP_ADD_EQ:
    return 3;
  case '+':
  case '-':
    return 4;
  case '*':
  case '/':
  case '%':
    return 5;
  case '^':
  case '@':
    return 6;
  default:
		Error("Unrecognised operation: %c", Op);
		return -1;
  }
} /* End of 'GetPrior' function */

/* Move orerations from */
static VOID DropOpers( CHAR Op )
{
  while (Stack2.Top != NULL && GetPrior((INT)Stack2.Top->T.Op) >= GetPrior((INT)Op))
  {
    TOK T;

    Pop(&Stack2, &T);
    Put(&Queue1, T);
  }
} /* End of 'DropOpers' function */

/* Convert expression function */
static VOID ParseExpr( LIST **Expr )
{
	TOK T;
  enum
  {
    STATE_PREFIX,
    STATE_SUFFIX,
    STATE_DONE,
    STATE_END
  } state = STATE_PREFIX;

  TokList.Head = *Expr;
	Queue1.Head = Queue1.Tail = NULL;

	NextTok();
  while (!IsEOTL && state != STATE_END)
  {
    switch (state)
    {
    case STATE_PREFIX:
      if (IsTokAnyNum() || IsTokAnyVar())
      {
        Put(&Queue1, TokCurrent);
        state = STATE_SUFFIX;
      }
      else if (IsTokAnyOp())
      {
        if (IsTokOp('('))
        {
          Push(&Stack2, TokCurrent);
        }
        else if (IsTokOp('-'))
        {
          TokCurrent.Op = '@';
          Push(&Stack2, TokCurrent);
        }
      }
			else if (IsTokTxt())
				if (TokList.Head != NULL)
					state = STATE_END;
				else
          Error("Using text in expression");
			ParseAnyTok();
      break;

    case STATE_SUFFIX:
      if (IsTokAnyOp())
      {
        DropOpers(TokCurrent.Op);
        if (!IsTokOp(')'))
        {
          Push(&Stack2, TokCurrent);
          state = STATE_PREFIX;
          ParseAnyOp();
        }
        else if (!Pop(&Stack2, &T))
          ParseAnyOp();
      }
			else 
				state = STATE_DONE;
      break;

    case STATE_DONE:
      DropOpers(')');
      state = STATE_END;
      break;
    }
  }

  if (Stack2.Top != NULL && !IsEOTL)
    Error("Missing ')'");
	*Expr = Queue1.Head;
} /* End of 'Parser' function */

QUEUE OldTokList;

/* Get expression from 'TokList' function */
static VOID ReadExpr( LIST **Expr)
{
	INT Brackets = 0;
  LIST **list = Expr;

	if (IsTokOp('('))
		Brackets++;

	while (!IsTokSym(';') && Brackets >= 0)
		{
			if ((*list = malloc(sizeof(LIST))) == NULL)
			  Error("No memory");
			(*list)->T = TokCurrent;
			list = &(*list)->Next;
			
			NextTok();
			if (IsTokOp('('))
				Brackets++;
			else if (IsTokOp(')'))
				Brackets--; 
		}
	
	if ((*list = malloc(sizeof(LIST))) == NULL)
		Error("No memory");
	(*list)->T = TokCurrent;
	(*list)->Next = NULL;

	OldTokList = TokList;
	printf("TOKLIST: ");
	DisplayQueue(&TokList);
} /* End of 'ReadExpr' function */

/* Initialise comand function */
static VOID CmdInit( CMD **C )
{
  if ((*C = malloc(sizeof(CMD))) == NULL)
    Error("No memory");
  if (((*C)->Expr = malloc(sizeof(LIST))) == NULL)
		Error("No memory");
  
  (*C)->Id = -1;
	
	(*C)->Expr = NULL;
	(*C)->C1 = (*C)->C2 = (*C)->Next = NULL;
} /* End of 'CmdInit' function */

/* Parse comand fuction */
VOID ParseCmd( CMD **C )
{
  *C = NULL;
  if (IsTokKw(IF))
  {
    CmdInit(C);
    (*C)->Id = CMD_IF;

    ParseKw(IF);
    ParseOp('(');

		ReadExpr(&(*C)->Expr);
		ParseExpr(&(*C)->Expr);

    TokList = OldTokList;
		IsEOTL = TokList.Head == NULL;
		ParseOp(')');

    ParseCmd(&(*C)->C1);
    if (IsTokKw(ELSE))
    {
      ParseKw(ELSE);
      ParseCmd(&(*C)->C2);
    }
  }
  else if (IsTokKw(WHILE))
  {
    CmdInit(C);
    (*C)->Id = CMD_WHILE;

		ParseKw(WHILE);
		ParseOp('(');

		ReadExpr(&(*C)->Expr);
    ParseExpr(&(*C)->Expr);
    
		TokList = OldTokList;
		IsEOTL = TokList.Head == NULL;
		ParseOp(')');
		
    ParseCmd(&(*C)->C1);
  }
	else if (IsTokKw(PRINT))
	{
    CmdInit(C);
		(*C)->Id = CMD_PRINT;

		ParseKw(PRINT);
		ParseOp('(');

		ReadExpr(&(*C)->Expr);
		ParseExpr(&(*C)->Expr);

		TokList = OldTokList;
		IsEOTL = TokList.Head == NULL;
		ParseOp(')');
		if (!IsEOTL)
			ParseSym(';');
		CmdInit(&(*C)->Next);
	}
	else if (IsTokKw(SCAN))
	{
    CmdInit(C);
		(*C)->Id = CMD_SCAN;
		ParseKw(SCAN);
		ParseOp('(');
		ReadExpr(&(*C)->Expr);
		
		TokList = OldTokList;
		IsEOTL = TokList.Head == NULL;
		ParseOp(')');
		if (!IsEOTL)
			ParseSym(';');
		CmdInit(&(*C)->Next);
	}
  else if (IsTokSym('{'))
  {
    ParseSym('{');
    while (!IsTokSym('}'))
    {
      ParseCmd(C);
      C = &(*C)->Next;
    }
    ParseSym('}');
  }
  else
  {
    CmdInit(C);
    (*C)->Id = CMD_EXPR;

		ReadExpr(&(*C)->Expr);
    ParseExpr(&(*C)->Expr);
	
		TokList = OldTokList;
		IsEOTL = TokList.Head == NULL;;
		if (!IsEOTL)
			ParseSym(';');
		CmdInit(&(*C)->Next);
  }
} /* End of 'ParseCmd' function */

/* Parsee full program function */
VOID ParseProgram( VOID )
{
  CMD **cmd = &Proga;

	Proga = NULL;
  IsEOTL = 0;
  
  NextTok();

  while (!IsEOTL)
  {
    ParseCmd(cmd);
    cmd = &(*cmd)->Next;
  }
} /* End of 'ParseProgram' function */

/* Clear comand function */
VOID CmdClear( CMD **C )
{
	while (*C != NULL)
	{
		ClearList(&(*C)->Expr);
		CmdClear(&(*C)->C1);
	  CmdClear(&(*C)->C2);
	  C = &(*C)->Next;
	}
} /* End of 'CmdClear' function */
/* END OF 'PARSER.C' FILE */