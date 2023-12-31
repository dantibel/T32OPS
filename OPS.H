/* Belov Danil, DB6, 20.12.2021 */

#ifndef __OPS_H_
#define __OPS_H_

#define VOID void

#include "commondf.h"

/* Debug memory allocation support */
#ifndef _NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>

#define MAX_FUNC 6
#define MAX_NAME 20
#define MAX_TEXT 200
#define TOK_OP_ADD_EQ 300

/* Console colors support */
#define RESET_CONSOLE() printf("\033[0m")
#define SET_CONSOLE_RED() printf("\033[1;31m")
#define SET_CONSOLE_GREEN() printf("\033[1;32m")
#define SET_CONSOLE_YELLOW() printf("\033[1;33m")
#define SET_CONSOLE_BLUE() printf("\033[1;34m")
#define SET_CONSOLE_MAGENTA() printf("\033[1;35m")
#define SET_CONSOLE_CYAN() printf("\033[1;36m")

typedef DOUBLE DBL;

/* Token identificator type */
typedef enum
{
  TOK_OP,
  TOK_NUM,
  TOK_FUNC,
  TOK_VAR,
  TOK_SYM,
  TOK_KEYW,
  TOK_TXT
} TOKID;

/* Operation identificator type */
typedef enum
{
  OP_LBR = '(',
  OP_RBR = ')',
  OP_PLUS = '+',
  OP_MINUS = '-',
  OP_AST = '*',
  OP_SLASH = '/',
  OP_MOD = '%',
  OP_POW = '^',
  OP_COMA = ',',
  OP_APPROP = '=',
  OP_LT = '<',
  OP_GT = '<',
  OP_EQ = '=' + TOK_OP_ADD_EQ,
  OP_NEQ = '!' + TOK_OP_ADD_EQ,
  OP_GE = '>' + TOK_OP_ADD_EQ,
  OP_LE = '<' + TOK_OP_ADD_EQ,
  OP_NULL = 0
} OPERS;

/* Functions identificator type */
typedef enum
{
  FN_SIN, FN_ABS, FN_PI, FN_TIME
} FUNCS;

/* Keyword identificator type */
typedef enum
{
  KW_IF, KW_ELSE, KW_WHILE, KW_PRINT, KW_SCAN
} KEYWORD;

/* Token type */
typedef struct
{
  TOKID Id;            /* identificator of token's type */
  OPERS Op;            /* operation or symbol */
  DBL Num;             /* number */
  FUNCS Func;          /* function */
  CHAR Name[MAX_NAME]; /* variable */
  KEYWORD KeyW;        /* keyword */
  CHAR Text[MAX_TEXT]; /* text */
} TOK;

extern struct tagFNTB
{
  CHAR *Name;
  FUNCS Func;
} FuncTable[4];

/* Comand identificator type */
typedef enum
{
  CMD_EXPR, CMD_IF, CMD_WHILE, CMD_SCAN, CMD_PRINT
} CMDID;

/* List type*/
typedef struct tagLIST LIST;
struct tagLIST
{
  TOK T;      /* token */
  LIST *Next; /* pointer to next element */
};

/* Comand type */
typedef struct tagCMD CMD;
struct tagCMD
{
  CMDID Id;   /* type of comand */
  LIST *Expr; /* expression */
  CMD
    *C1,      /* first comand */
    *C2,      /* second comand (for 'else') */
    *Next;    /* next comand pointer */
};

/* Queue type */
typedef struct
{
  LIST *Head, *Tail; /* pointrers to first and last elements */
} QUEUE;

/* Stack type */
typedef struct
{
  LIST *Top; /* pointer to first element */
} STACK;

extern QUEUE Queue1;
extern QUEUE TokList;
extern STACK StackEval;
extern STACK Stack2;
extern CMD *Proga;

/* Basic functions */
VOID Error (CHAR *Str, ... );
VOID GetStr( CHAR *S, INT Len );
VOID ClearList( LIST **L );

/* Variables table functions */
VOID SetValue( CHAR *Name, DBL Value );
DBL GetValue( CHAR *Name );
VOID DisplayVarTable( VOID );
VOID ClearVarTable( VOID );

/* main functions */
BOOL Scanner( QUEUE *Q, CHAR *S );
DBL Eval( LIST *Expr );
VOID ParseProgram( VOID );
VOID DoCmd( CMD *C );
VOID CmdClear( CMD **C );
VOID ClearPrinterList( VOID );

/* Queue functions */
VOID Put( QUEUE *Q, TOK NewT );
BOOL Get( QUEUE *Q, TOK *OldT );
VOID DisplayQueue( QUEUE *Q );
VOID ClearQueue( QUEUE *Q );

/* Stack functions */
VOID Push ( STACK *S, TOK NewT );
BOOL Pop (STACK *S, TOK *OldT );
VOID DisplayStack( STACK *S );
VOID ClearStack( STACK *S );

/* Print list function */
VOID DisplayList( LIST *L );

#endif /*__OPS_H_*/
/*END OF 'OPS.H' FILE*/
