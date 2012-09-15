/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     STRING_LIT = 259,
     NUM = 260,
     FLOAT = 261,
     POUND = 262,
     COMMA = 263,
     COLON = 264,
     SEMICOLON = 265,
     LPAREN = 266,
     RPAREN = 267,
     LBRACK = 268,
     RBRACK = 269,
     LBRACE = 270,
     RBRACE = 271,
     DOT = 272,
     PLUS = 273,
     MINUS = 274,
     TIMES = 275,
     DIVIDE = 276,
     PERCENT = 277,
     EQ = 278,
     NEQ = 279,
     LT = 280,
     LE = 281,
     GT = 282,
     GE = 283,
     AND = 284,
     OR = 285,
     ASSIGN = 286,
     IF = 287,
     THEN = 288,
     ELSE = 289,
     WHILE = 290,
     FOR = 291,
     DO = 292,
     LOOP = 293,
     BREAK = 294,
     CONTINUE = 295,
     NULL_TOK = 296,
     FUNCTION = 297,
     RETURN = 298,
     QUESTION = 299,
     EXCLAMATION = 300,
     S_OR = 301,
     S_AND = 302,
     S_XOR = 303,
     PLUSPLUS = 304,
     MINUSMINUS = 305,
     DOLLAR = 306,
     POUNDPAREN = 307,
     PERCENTPAREN = 308,
     SIMULT = 309,
     PATTERN = 310,
     CODE = 311,
     TRANSPORT = 312,
     HOST = 313,
     TIME = 314,
     WHENEVER = 315,
     NEXT = 316,
     UNTIL = 317,
     EVERY = 318,
     BEFORE = 319,
     AFTER = 320,
     AT = 321,
     AT_SYM = 322,
     ATAT_SYM = 323,
     NEW = 324,
     SIZEOF = 325,
     TYPEOF = 326,
     SAME = 327,
     PLUS_CHUCK = 328,
     MINUS_CHUCK = 329,
     TIMES_CHUCK = 330,
     DIVIDE_CHUCK = 331,
     S_AND_CHUCK = 332,
     S_OR_CHUCK = 333,
     S_XOR_CHUCK = 334,
     SHIFT_RIGHT_CHUCK = 335,
     SHIFT_LEFT_CHUCK = 336,
     PERCENT_CHUCK = 337,
     SHIFT_RIGHT = 338,
     SHIFT_LEFT = 339,
     TILDA = 340,
     CHUCK = 341,
     COLONCOLON = 342,
     S_CHUCK = 343,
     AT_CHUCK = 344,
     LEFT_S_CHUCK = 345,
     UNCHUCK = 346,
     UPCHUCK = 347,
     CLASS = 348,
     INTERFACE = 349,
     EXTENDS = 350,
     IMPLEMENTS = 351,
     PUBLIC = 352,
     PROTECTED = 353,
     PRIVATE = 354,
     STATIC = 355,
     ABSTRACT = 356,
     CONST = 357,
     SPORK = 358,
     ARROW_RIGHT = 359,
     ARROW_LEFT = 360,
     L_HACK = 361,
     R_HACK = 362
   };
#endif
/* Tokens.  */
#define ID 258
#define STRING_LIT 259
#define NUM 260
#define FLOAT 261
#define POUND 262
#define COMMA 263
#define COLON 264
#define SEMICOLON 265
#define LPAREN 266
#define RPAREN 267
#define LBRACK 268
#define RBRACK 269
#define LBRACE 270
#define RBRACE 271
#define DOT 272
#define PLUS 273
#define MINUS 274
#define TIMES 275
#define DIVIDE 276
#define PERCENT 277
#define EQ 278
#define NEQ 279
#define LT 280
#define LE 281
#define GT 282
#define GE 283
#define AND 284
#define OR 285
#define ASSIGN 286
#define IF 287
#define THEN 288
#define ELSE 289
#define WHILE 290
#define FOR 291
#define DO 292
#define LOOP 293
#define BREAK 294
#define CONTINUE 295
#define NULL_TOK 296
#define FUNCTION 297
#define RETURN 298
#define QUESTION 299
#define EXCLAMATION 300
#define S_OR 301
#define S_AND 302
#define S_XOR 303
#define PLUSPLUS 304
#define MINUSMINUS 305
#define DOLLAR 306
#define POUNDPAREN 307
#define PERCENTPAREN 308
#define SIMULT 309
#define PATTERN 310
#define CODE 311
#define TRANSPORT 312
#define HOST 313
#define TIME 314
#define WHENEVER 315
#define NEXT 316
#define UNTIL 317
#define EVERY 318
#define BEFORE 319
#define AFTER 320
#define AT 321
#define AT_SYM 322
#define ATAT_SYM 323
#define NEW 324
#define SIZEOF 325
#define TYPEOF 326
#define SAME 327
#define PLUS_CHUCK 328
#define MINUS_CHUCK 329
#define TIMES_CHUCK 330
#define DIVIDE_CHUCK 331
#define S_AND_CHUCK 332
#define S_OR_CHUCK 333
#define S_XOR_CHUCK 334
#define SHIFT_RIGHT_CHUCK 335
#define SHIFT_LEFT_CHUCK 336
#define PERCENT_CHUCK 337
#define SHIFT_RIGHT 338
#define SHIFT_LEFT 339
#define TILDA 340
#define CHUCK 341
#define COLONCOLON 342
#define S_CHUCK 343
#define AT_CHUCK 344
#define LEFT_S_CHUCK 345
#define UNCHUCK 346
#define UPCHUCK 347
#define CLASS 348
#define INTERFACE 349
#define EXTENDS 350
#define IMPLEMENTS 351
#define PUBLIC 352
#define PROTECTED 353
#define PRIVATE 354
#define STATIC 355
#define ABSTRACT 356
#define CONST 357
#define SPORK 358
#define ARROW_RIGHT 359
#define ARROW_LEFT 360
#define L_HACK 361
#define R_HACK 362




/* Copy the first part of user declarations.  */
#line 1 "chuck.y"


/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: chuck.tab.c
// desc: chuck parser
//
// author: Ge Wang (gewang@cs.princeton.edu) - generated by yacc
//         Perry R. Cook (prc@cs.princeton.edu)
//
// based in part on the ansi C grammar by Jeff Lee, maintained by Jutta Degener
//
// date: Summer 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chuck_utils.h"
#include "chuck_errmsg.h"
#include "chuck_absyn.h"

// function
int yylex( void );

void yyerror( char *s )
{
    EM_error( EM_tokPos, "%s", s );
}

a_Program g_program = NULL;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 59 "chuck.y"
{
    int pos;
    int ival;
    double fval;
    c_str sval;
    
    a_Program program;
    a_Section program_section;
    a_Stmt_List stmt_list;
    a_Class_Def class_def;
    a_Class_Ext class_ext;
    a_Class_Body class_body;
    a_Stmt stmt;
    a_Exp exp;
    a_Func_Def func_def;
    a_Var_Decl_List var_decl_list;
    a_Var_Decl var_decl;
    a_Type_Decl type_decl;
    a_Arg_List arg_list;
    a_Id_List id_list;
    a_Array_Sub array_sub;
    a_Complex complex_exp;
    a_Polar polar_exp;
}
/* Line 193 of yacc.c.  */
#line 391 "chuck.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 404 "chuck.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  114
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1082

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  180
/* YYNRULES -- Number of states.  */
#define YYNSTATES  309

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   362

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    21,    29,
      36,    44,    47,    52,    55,    60,    62,    63,    65,    68,
      70,    72,    74,    77,    79,    83,    85,    89,    98,   106,
     115,   123,   125,   127,   128,   130,   132,   134,   136,   138,
     140,   141,   143,   146,   150,   155,   157,   159,   161,   164,
     167,   172,   174,   177,   179,   181,   183,   185,   187,   190,
     194,   197,   200,   206,   214,   220,   228,   235,   243,   249,
     257,   263,   266,   270,   272,   275,   277,   281,   283,   287,
     289,   293,   297,   302,   305,   309,   311,   314,   318,   321,
     325,   327,   331,   333,   336,   339,   343,   347,   349,   351,
     353,   355,   357,   359,   361,   363,   365,   367,   369,   371,
     373,   375,   377,   379,   381,   387,   389,   393,   395,   399,
     401,   405,   407,   411,   413,   417,   419,   423,   427,   429,
     433,   437,   441,   445,   447,   451,   455,   457,   461,   465,
     467,   471,   475,   479,   481,   485,   487,   491,   493,   496,
     499,   502,   505,   508,   511,   515,   519,   521,   523,   525,
     527,   529,   532,   534,   538,   540,   543,   547,   552,   556,
     559,   562,   564,   566,   568,   570,   572,   574,   576,   580,
     584
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     109,     0,    -1,   110,    -1,   110,   109,    -1,   128,    -1,
     119,    -1,   111,    -1,   120,    93,   117,    15,   113,    16,
      -1,   120,    93,   117,   112,    15,   113,    16,    -1,   120,
      94,   117,    15,   113,    16,    -1,   120,    94,   117,   116,
      15,   113,    16,    -1,    96,   117,    -1,    96,   117,    95,
     118,    -1,    95,   118,    -1,    95,   118,    96,   117,    -1,
     114,    -1,    -1,   115,    -1,   115,   114,    -1,   128,    -1,
     119,    -1,   111,    -1,    95,   117,    -1,     3,    -1,     3,
       8,   117,    -1,     3,    -1,     3,    17,   118,    -1,   121,
     122,   126,     3,    11,   127,    12,   133,    -1,   121,   122,
     126,     3,    11,    12,   133,    -1,   121,   122,   126,     3,
      11,   127,    12,    10,    -1,   121,   122,   126,     3,    11,
      12,    10,    -1,    97,    -1,    99,    -1,    -1,    42,    -1,
      97,    -1,    98,    -1,    99,    -1,   100,    -1,   101,    -1,
      -1,     3,    -1,     3,    67,    -1,    25,   118,    27,    -1,
      25,   118,    27,    67,    -1,   123,    -1,   124,    -1,   125,
      -1,   125,   139,    -1,   125,   142,    -1,   125,   142,     8,
     127,    -1,   129,    -1,   129,   128,    -1,   134,    -1,   132,
      -1,   131,    -1,   130,    -1,   133,    -1,    43,    10,    -1,
      43,   135,    10,    -1,    39,    10,    -1,    40,    10,    -1,
      32,    11,   135,    12,   129,    -1,    32,    11,   135,    12,
     129,    34,   129,    -1,    35,    11,   135,    12,   129,    -1,
      37,   129,    35,    11,   135,    12,    10,    -1,    36,    11,
     134,   134,    12,   129,    -1,    36,    11,   134,   134,   135,
      12,   129,    -1,    62,    11,   135,    12,   129,    -1,    37,
     129,    62,    11,   135,    12,    10,    -1,    38,    11,   135,
      12,   129,    -1,    15,    16,    -1,    15,   128,    16,    -1,
      10,    -1,   135,    10,    -1,   136,    -1,   136,     8,   135,
      -1,   137,    -1,   136,   145,   137,    -1,   140,    -1,   137,
     146,   140,    -1,    13,   135,    14,    -1,    13,   135,    14,
     138,    -1,    13,    14,    -1,   139,    13,    14,    -1,   147,
      -1,   125,   141,    -1,   100,   125,   141,    -1,    72,   141,
      -1,   100,    72,   141,    -1,   142,    -1,   142,     8,   141,
      -1,     3,    -1,     3,   138,    -1,     3,   139,    -1,    52,
     135,    12,    -1,    53,   135,    12,    -1,    86,    -1,    89,
      -1,    73,    -1,    74,    -1,    75,    -1,    76,    -1,    80,
      -1,    81,    -1,    82,    -1,    91,    -1,    92,    -1,    77,
      -1,    78,    -1,    79,    -1,   105,    -1,   104,    -1,   148,
      -1,   148,    44,   135,     9,   147,    -1,   149,    -1,   148,
      30,   149,    -1,   150,    -1,   149,    29,   150,    -1,   151,
      -1,   150,    46,   151,    -1,   152,    -1,   151,    48,   152,
      -1,   153,    -1,   152,    47,   153,    -1,   154,    -1,   153,
      23,   154,    -1,   153,    24,   154,    -1,   155,    -1,   154,
      25,   155,    -1,   154,    27,   155,    -1,   154,    26,   155,
      -1,   154,    28,   155,    -1,   156,    -1,   155,    84,   156,
      -1,   155,    83,   156,    -1,   157,    -1,   156,    18,   157,
      -1,   156,    19,   157,    -1,   158,    -1,   157,    20,   158,
      -1,   157,    21,   158,    -1,   157,    22,   158,    -1,   159,
      -1,   158,    85,   159,    -1,   160,    -1,   159,    51,   125,
      -1,   162,    -1,    49,   160,    -1,    50,   160,    -1,   161,
     160,    -1,    71,   160,    -1,    70,   160,    -1,    69,   125,
      -1,    69,   125,   138,    -1,   103,    85,   133,    -1,    18,
      -1,    19,    -1,    85,    -1,    45,    -1,    20,    -1,   103,
      85,    -1,   163,    -1,   162,    87,   163,    -1,   164,    -1,
     163,   138,    -1,   163,    11,    12,    -1,   163,    11,   135,
      12,    -1,   163,    17,     3,    -1,   163,    49,    -1,   163,
      50,    -1,     3,    -1,     5,    -1,     6,    -1,     4,    -1,
     138,    -1,   143,    -1,   144,    -1,   106,   135,   107,    -1,
      11,   135,    12,    -1,    11,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   177,   177,   178,   182,   183,   184,   188,   190,   192,
     194,   199,   200,   201,   202,   206,   207,   211,   212,   217,
     218,   219,   223,   227,   228,   232,   233,   237,   239,   241,
     243,   248,   249,   250,   254,   255,   256,   257,   261,   262,
     263,   267,   268,   272,   273,   282,   283,   288,   289,   293,
     294,   298,   299,   303,   304,   305,   306,   308,   312,   313,
     314,   315,   319,   321,   326,   328,   330,   332,   334,   336,
     338,   343,   344,   348,   349,   353,   354,   358,   359,   364,
     365,   370,   371,   376,   377,   381,   382,   383,   384,   385,
     389,   390,   394,   395,   396,   400,   405,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   427,   428,   432,   433,   438,   439,   444,   445,   450,
     451,   456,   457,   462,   463,   468,   469,   471,   476,   477,
     479,   481,   483,   488,   489,   491,   496,   497,   499,   504,
     505,   507,   509,   514,   515,   520,   521,   526,   527,   529,
     531,   533,   535,   537,   539,   541,   546,   547,   548,   549,
     550,   551,   556,   557,   562,   563,   565,   567,   569,   571,
     573,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING_LIT", "NUM", "FLOAT",
  "POUND", "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK",
  "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE",
  "PERCENT", "EQ", "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "ASSIGN",
  "IF", "THEN", "ELSE", "WHILE", "FOR", "DO", "LOOP", "BREAK", "CONTINUE",
  "NULL_TOK", "FUNCTION", "RETURN", "QUESTION", "EXCLAMATION", "S_OR",
  "S_AND", "S_XOR", "PLUSPLUS", "MINUSMINUS", "DOLLAR", "POUNDPAREN",
  "PERCENTPAREN", "SIMULT", "PATTERN", "CODE", "TRANSPORT", "HOST", "TIME",
  "WHENEVER", "NEXT", "UNTIL", "EVERY", "BEFORE", "AFTER", "AT", "AT_SYM",
  "ATAT_SYM", "NEW", "SIZEOF", "TYPEOF", "SAME", "PLUS_CHUCK",
  "MINUS_CHUCK", "TIMES_CHUCK", "DIVIDE_CHUCK", "S_AND_CHUCK",
  "S_OR_CHUCK", "S_XOR_CHUCK", "SHIFT_RIGHT_CHUCK", "SHIFT_LEFT_CHUCK",
  "PERCENT_CHUCK", "SHIFT_RIGHT", "SHIFT_LEFT", "TILDA", "CHUCK",
  "COLONCOLON", "S_CHUCK", "AT_CHUCK", "LEFT_S_CHUCK", "UNCHUCK",
  "UPCHUCK", "CLASS", "INTERFACE", "EXTENDS", "IMPLEMENTS", "PUBLIC",
  "PROTECTED", "PRIVATE", "STATIC", "ABSTRACT", "CONST", "SPORK",
  "ARROW_RIGHT", "ARROW_LEFT", "L_HACK", "R_HACK", "$accept", "program",
  "program_section", "class_definition", "class_ext", "class_body",
  "class_body2", "class_section", "iface_ext", "id_list", "id_dot",
  "function_definition", "class_decl", "function_decl", "static_decl",
  "type_decl_a", "type_decl_b", "type_decl", "type_decl2", "arg_list",
  "statement_list", "statement", "jump_statement", "selection_statement",
  "loop_statement", "code_segment", "expression_statement", "expression",
  "chuck_expression", "arrow_expression", "array_exp", "array_empty",
  "decl_expression", "var_decl_list", "var_decl", "complex_exp",
  "polar_exp", "chuck_operator", "arrow_operator",
  "conditional_expression", "logical_or_expression",
  "logical_and_expression", "inclusive_or_expression",
  "exclusive_or_expression", "and_expression", "equality_expression",
  "relational_expression", "shift_expression", "additive_expression",
  "multiplicative_expression", "tilda_expression", "cast_expression",
  "unary_expression", "unary_operator", "dur_expression",
  "postfix_expression", "primary_expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   110,   110,   110,   111,   111,   111,
     111,   112,   112,   112,   112,   113,   113,   114,   114,   115,
     115,   115,   116,   117,   117,   118,   118,   119,   119,   119,
     119,   120,   120,   120,   121,   121,   121,   121,   122,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   127,
     127,   128,   128,   129,   129,   129,   129,   129,   130,   130,
     130,   130,   131,   131,   132,   132,   132,   132,   132,   132,
     132,   133,   133,   134,   134,   135,   135,   136,   136,   137,
     137,   138,   138,   139,   139,   140,   140,   140,   140,   140,
     141,   141,   142,   142,   142,   143,   144,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   146,   146,   147,   147,   148,   148,   149,   149,   150,
     150,   151,   151,   152,   152,   153,   153,   153,   154,   154,
     154,   154,   154,   155,   155,   155,   156,   156,   156,   157,
     157,   157,   157,   158,   158,   159,   159,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   161,   161,   161,   161,
     161,   161,   162,   162,   163,   163,   163,   163,   163,   163,
     163,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     7,     6,
       7,     2,     4,     2,     4,     1,     0,     1,     2,     1,
       1,     1,     2,     1,     3,     1,     3,     8,     7,     8,
       7,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     3,     4,     1,     1,     1,     2,     2,
       4,     1,     2,     1,     1,     1,     1,     1,     2,     3,
       2,     2,     5,     7,     5,     7,     6,     7,     5,     7,
       5,     2,     3,     1,     2,     1,     3,     1,     3,     1,
       3,     3,     4,     2,     3,     1,     2,     3,     2,     3,
       1,     3,     1,     2,     2,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     1,     3,     1,     2,     2,
       2,     2,     2,     2,     3,     3,     1,     1,     1,     1,
       1,     2,     1,     3,     1,     2,     3,     4,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      33,   171,   174,   172,   173,    73,     0,     0,     0,   156,
     157,   160,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,    35,    36,    37,     0,     0,     0,     0,
      33,     6,     5,     0,    40,    45,    46,     0,     4,    51,
      56,    55,    54,    57,    53,     0,    75,    77,   175,    79,
     176,   177,    85,   113,   115,   117,   119,   121,   123,   125,
     128,   133,   136,   139,   143,   145,     0,   147,   162,   164,
      42,   180,     0,     0,    71,     0,    25,     0,     0,     0,
       0,     0,     0,    60,    61,    58,     0,   171,   148,   149,
       0,     0,     0,    41,   153,   152,   151,    92,    88,    90,
       0,     0,   161,     0,     1,     3,     0,     0,    38,    39,
       0,    86,    52,    74,     0,    99,   100,   101,   102,   108,
     109,   110,   103,   104,   105,    97,    98,   106,   107,     0,
     112,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   150,     0,     0,     0,   169,   170,
     165,   179,    81,    72,     0,    43,     0,     0,     0,     0,
       0,     0,    59,    95,    96,     0,   154,     0,    93,    94,
       0,    89,    87,   155,   178,    23,     0,     0,    47,     0,
      76,    78,    80,   116,     0,   118,   120,   122,   124,   126,
     127,   129,   131,   130,   132,   135,   134,   137,   138,   140,
     141,   142,   144,   146,   163,   166,     0,   168,    82,    26,
      44,     0,     0,     0,     0,     0,     0,     0,    83,     0,
      91,     0,    33,     0,     0,     0,    33,     0,     0,     0,
      48,     0,     0,   167,    62,    64,     0,     0,     0,     0,
      70,    68,    84,    24,    21,     0,    15,    33,    20,    19,
      13,    11,    33,     0,    22,    33,     0,   114,     0,    66,
       0,     0,     0,     7,    18,     0,     0,     0,     9,     0,
       0,     0,     0,    63,    67,    65,    69,    14,    12,     8,
      10,    30,    28,    49,     0,     0,    29,    27,    50
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    39,    40,   264,   245,   265,   266,   267,   248,   196,
      87,   268,    43,    44,   120,    45,    46,    47,   199,   292,
     269,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   189,    59,   108,   109,    60,    61,   139,   142,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -234
static const yytype_int16 yypact[] =
{
     428,     9,  -234,  -234,  -234,  -234,    41,   914,   504,  -234,
    -234,  -234,    11,    12,    51,    64,   577,    76,    39,    54,
    -234,   647,  -234,   976,   976,   914,   914,   114,    25,   976,
     976,   131,  -234,   -12,  -234,    38,     7,    69,   914,   139,
     200,  -234,  -234,    44,     2,  -234,  -234,   131,  -234,   577,
    -234,  -234,  -234,  -234,  -234,   147,   962,    14,  -234,  -234,
    -234,  -234,  -234,    53,   132,   117,   116,   118,   119,    81,
      66,   127,   102,    82,   115,  -234,   976,    83,    24,  -234,
    -234,  -234,   156,   155,  -234,   157,   154,   145,   914,   914,
     717,   -19,   914,  -234,  -234,  -234,   165,  -234,  -234,  -234,
     164,   166,   914,   110,   167,  -234,  -234,   168,  -234,   171,
     131,   131,   169,    78,  -234,  -234,   183,   183,  -234,  -234,
      25,  -234,  -234,  -234,   914,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,  -234,   914,
    -234,  -234,   914,   976,   914,   976,   976,   976,   976,   976,
     976,   976,   976,   976,   976,   976,   976,   976,   976,   976,
     976,   976,   976,    25,  -234,    52,   779,   184,  -234,  -234,
    -234,  -234,   167,  -234,    11,   121,   177,   178,   717,   180,
     181,   182,  -234,  -234,  -234,   185,  -234,   841,  -234,   186,
     131,  -234,  -234,  -234,  -234,   187,     0,     5,   188,   190,
    -234,    14,  -234,   132,   189,   117,   116,   118,   119,    81,
      81,    66,    66,    66,    66,   127,   127,   102,   102,    82,
      82,    82,   115,  -234,    24,  -234,   195,  -234,  -234,  -234,
    -234,   577,   577,   903,   914,   914,   577,   577,  -234,   194,
    -234,   183,   276,    11,   183,   197,   276,   183,   199,   209,
     186,   191,   976,  -234,   175,  -234,   577,   212,   214,   215,
    -234,  -234,  -234,  -234,  -234,   217,  -234,   352,  -234,  -234,
     138,   133,   276,   228,  -234,   276,    26,  -234,   577,  -234,
     577,   236,   237,  -234,  -234,   183,    11,   235,  -234,   238,
      16,   131,   243,  -234,  -234,  -234,  -234,  -234,  -234,  -234,
    -234,  -234,  -234,   248,    62,    25,  -234,  -234,  -234
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -234,   218,  -234,    27,  -234,  -233,   -10,  -234,  -234,  -111,
    -165,    30,  -234,  -234,  -234,  -234,  -234,   -28,  -234,   -46,
      40,   -15,  -234,  -234,  -234,  -108,   -79,    -4,  -234,   122,
     -71,    68,   125,   -42,   -31,  -234,  -234,  -234,  -234,    21,
    -234,   135,   123,   128,   129,   136,     6,   -37,     4,    -6,
     -32,   113,    -5,  -234,  -234,    99,  -234
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -42
static const yytype_int16 yytable[] =
{
     104,    91,    82,    83,   193,   121,   197,   170,   111,   229,
     103,   178,   -41,   273,    86,   242,   179,    96,    98,    99,
     246,   100,   101,    88,   105,   106,   301,    41,   103,   103,
      42,     8,    12,   186,   113,   166,   188,     7,   290,   287,
      48,   167,   289,   180,     1,     2,     3,     4,    85,    93,
      12,    12,     6,    81,     7,    97,     2,     3,     4,     9,
      10,    11,    89,     6,    94,     7,    12,    41,   191,   192,
      42,   164,   306,   168,   169,    90,    80,     8,   270,   110,
      48,   -31,   -31,   143,   176,   177,    22,    92,   181,   122,
      23,    24,   198,    25,    26,   243,   244,   144,   185,   233,
     247,   228,   118,   119,    25,    26,   151,   152,   153,   154,
      28,    29,    30,    31,   211,   212,   213,   214,   140,   141,
     200,   298,   159,   160,   161,   102,    32,   219,   220,   221,
     263,   -32,   -32,   271,   107,   223,   274,   116,   117,   114,
     204,    36,   149,   150,    37,   157,   158,    38,   240,   155,
     156,   217,   218,   170,   112,   209,   210,   123,    38,   215,
     216,   145,   226,   146,   147,   148,   163,   162,   171,   172,
     165,   174,   175,   173,   297,   182,   183,    80,   184,   190,
       7,   187,   302,    83,     8,   194,   195,   227,   230,   231,
     232,   234,   235,   251,   236,   241,   307,   237,   252,   239,
      -2,   249,   276,     1,     2,     3,     4,   253,   262,   278,
       5,     6,   272,     7,   275,     8,   254,   255,     9,    10,
      11,   260,   261,   238,   280,    12,   281,   282,   286,   257,
     258,   259,    13,   283,   285,    14,    15,    16,    17,    18,
      19,   279,    20,    21,   288,    22,   295,   296,   291,    23,
      24,   299,    25,    26,   300,   304,   305,   284,   115,   308,
     303,   201,    27,   293,   224,   294,   250,   202,   205,    28,
      29,    30,    31,   277,   206,   222,   207,   291,   203,     1,
       2,     3,     4,     0,   208,    32,     5,     6,     0,     7,
       0,     8,   -16,     0,     9,    10,    11,    33,    34,    35,
      36,    12,     0,    37,     0,     0,    38,     0,    13,     0,
       0,    14,    15,    16,    17,    18,    19,     0,    20,    21,
       0,    22,     0,     0,     0,    23,    24,     0,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,     0,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     1,     2,     3,     4,     0,
       0,    32,     5,     6,     0,     7,     0,     8,   -17,     0,
       9,    10,    11,    33,    34,    35,    36,    12,     0,    37,
       0,     0,    38,     0,    13,     0,     0,    14,    15,    16,
      17,    18,    19,     0,    20,    21,     0,    22,     0,     0,
       0,    23,    24,     0,    25,    26,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
       0,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     4,     0,     0,    32,     5,     6,
       0,     7,     0,     8,     0,     0,     9,    10,    11,    33,
      34,    35,    36,    12,     0,    37,     0,     0,    38,     0,
      13,     0,     0,    14,    15,    16,    17,    18,    19,     0,
      20,    21,     0,    22,     0,     0,     0,    23,    24,     0,
      25,    26,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,     0,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     1,     2,     3,
       4,     0,     0,    32,     5,     6,     0,     7,     0,     8,
      84,     0,     9,    10,    11,    33,    34,    35,    36,    12,
       0,    37,     0,     0,    38,     0,    13,     0,     0,    14,
      15,    16,    17,    18,    19,     0,     0,    21,     0,    22,
       0,     0,     0,    23,    24,     0,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,     0,    28,    29,    30,    31,     0,     0,     0,
       1,     2,     3,     4,     0,     0,     0,     5,     6,    32,
       7,     0,     8,     0,     0,     9,    10,    11,     0,     0,
       0,     0,    12,     0,    36,     0,     0,    37,     0,    13,
      38,     0,    14,    15,    16,    17,    18,    19,     0,     0,
      21,     0,    22,     0,     0,     0,    23,    24,     0,    25,
      26,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     0,     0,     0,    28,    29,    30,    31,
       1,     2,     3,     4,     0,     0,     0,    95,     6,     0,
       7,     0,    32,     0,     0,     9,    10,    11,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    36,     0,     0,
      37,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,    23,    24,     0,    25,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,    29,    30,    31,
       1,     2,     3,     4,     0,     0,     0,     5,     6,     0,
       7,     0,    32,     0,     0,     9,    10,    11,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    36,     0,     0,
      37,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,    23,    24,     0,    25,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     2,     3,     4,    28,    29,    30,    31,
       6,   225,     7,     0,     0,     0,     0,     9,    10,    11,
       0,     0,    32,     0,    12,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
      37,     0,     0,    38,    22,     0,     0,     0,    23,    24,
       0,    25,    26,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     2,     3,     4,    28,    29,
      30,    31,     6,     0,     7,   238,     0,     0,     0,     9,
      10,    11,     0,     0,    32,     0,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,    37,     0,     0,    38,    22,     0,     0,     0,
      23,    24,     0,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     2,     3,     4,
      28,    29,    30,    31,     6,   256,     7,     1,     2,     3,
       4,     9,    10,    11,     0,     6,    32,     7,    12,     0,
       0,     0,     9,    10,    11,     0,     0,     0,     0,    12,
       0,    36,     0,     0,    37,     0,     0,    38,    22,     0,
       0,     0,    23,    24,     0,    25,    26,     0,     0,    22,
       0,     0,     0,    23,    24,     0,    25,    26,     0,     0,
     124,     0,    28,    29,    30,    31,     0,     0,     0,    97,
       2,     3,     4,    28,    29,    30,    31,     6,    32,     7,
       0,     0,     0,     0,     9,    10,    11,     0,     0,    32,
       0,     0,     0,    36,     0,     0,    37,     0,     0,    38,
       0,     0,     0,     0,    36,     0,     0,    37,     0,     0,
      38,    22,     0,     0,     0,    23,    24,     0,    25,    26,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    28,    29,    30,   135,     0,
       0,   136,     0,   137,   138,     0,     0,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,    38
};

static const yytype_int16 yycheck[] =
{
      28,    16,     6,     7,   112,    47,   117,    78,    36,   174,
       3,    90,     3,   246,     3,    15,    35,    21,    23,    24,
      15,    25,    26,    11,    29,    30,    10,     0,     3,     3,
       0,    15,    25,   104,    38,    11,   107,    13,    12,   272,
       0,    17,   275,    62,     3,     4,     5,     6,     8,    10,
      25,    25,    11,    12,    13,     3,     4,     5,     6,    18,
      19,    20,    11,    11,    10,    13,    25,    40,   110,   111,
      40,    76,    10,    49,    50,    11,    67,    15,   243,    72,
      40,    93,    94,    30,    88,    89,    45,    11,    92,    49,
      49,    50,   120,    52,    53,    95,    96,    44,   102,   178,
      95,   172,   100,   101,    52,    53,    25,    26,    27,    28,
      69,    70,    71,    72,   151,   152,   153,   154,   104,   105,
     124,   286,    20,    21,    22,    11,    85,   159,   160,   161,
     241,    93,    94,   244,     3,   163,   247,    93,    94,     0,
     144,   100,    23,    24,   103,    18,    19,   106,   190,    83,
      84,   157,   158,   224,    85,   149,   150,    10,   106,   155,
     156,    29,   166,    46,    48,    47,    51,    85,    12,    14,
      87,    17,    27,    16,   285,    10,    12,    67,    12,     8,
      13,    13,   290,   187,    15,   107,     3,     3,    67,    12,
      12,    11,    11,     3,    12,     8,   304,    12,     9,    13,
       0,    13,    11,     3,     4,     5,     6,    12,    14,    34,
      10,    11,    15,    13,    15,    15,   231,   232,    18,    19,
      20,   236,   237,    14,    12,    25,    12,    12,    95,   233,
     234,   235,    32,    16,    96,    35,    36,    37,    38,    39,
      40,   256,    42,    43,    16,    45,    10,    10,   276,    49,
      50,    16,    52,    53,    16,    12,     8,   267,    40,   305,
     291,   139,    62,   278,   165,   280,   198,   142,   145,    69,
      70,    71,    72,   252,   146,   162,   147,   305,   143,     3,
       4,     5,     6,    -1,   148,    85,    10,    11,    -1,    13,
      -1,    15,    16,    -1,    18,    19,    20,    97,    98,    99,
     100,    25,    -1,   103,    -1,    -1,   106,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    -1,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    85,    10,    11,    -1,    13,    -1,    15,    16,    -1,
      18,    19,    20,    97,    98,    99,   100,    25,    -1,   103,
      -1,    -1,   106,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    85,    10,    11,
      -1,    13,    -1,    15,    -1,    -1,    18,    19,    20,    97,
      98,    99,   100,    25,    -1,   103,    -1,    -1,   106,    -1,
      32,    -1,    -1,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    -1,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    85,    10,    11,    -1,    13,    -1,    15,
      16,    -1,    18,    19,    20,    97,    98,    99,   100,    25,
      -1,   103,    -1,    -1,   106,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    45,
      -1,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    85,
      13,    -1,    15,    -1,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,   100,    -1,    -1,   103,    -1,    32,
     106,    -1,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    45,    -1,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    -1,
      13,    -1,    85,    -1,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    -1,
      13,    -1,    85,    -1,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    69,    70,    71,    72,
      11,    12,    13,    -1,    -1,    -1,    -1,    18,    19,    20,
      -1,    -1,    85,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,
     103,    -1,    -1,   106,    45,    -1,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    69,    70,
      71,    72,    11,    -1,    13,    14,    -1,    -1,    -1,    18,
      19,    20,    -1,    -1,    85,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,   103,    -1,    -1,   106,    45,    -1,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      69,    70,    71,    72,    11,    12,    13,     3,     4,     5,
       6,    18,    19,    20,    -1,    11,    85,    13,    25,    -1,
      -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,   100,    -1,    -1,   103,    -1,    -1,   106,    45,    -1,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    -1,    45,
      -1,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    -1,
       8,    -1,    69,    70,    71,    72,    -1,    -1,    -1,     3,
       4,     5,     6,    69,    70,    71,    72,    11,    85,    13,
      -1,    -1,    -1,    -1,    18,    19,    20,    -1,    -1,    85,
      -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,    -1,   100,    -1,    -1,   103,    -1,    -1,
     106,    45,    -1,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    69,    70,    71,    86,    -1,
      -1,    89,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,   106
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,    10,    11,    13,    15,    18,
      19,    20,    25,    32,    35,    36,    37,    38,    39,    40,
      42,    43,    45,    49,    50,    52,    53,    62,    69,    70,
      71,    72,    85,    97,    98,    99,   100,   103,   106,   109,
     110,   111,   119,   120,   121,   123,   124,   125,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   140,
     143,   144,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      67,    12,   135,   135,    16,   128,     3,   118,    11,    11,
      11,   129,    11,    10,    10,    10,   135,     3,   160,   160,
     135,   135,    11,     3,   125,   160,   160,     3,   141,   142,
      72,   125,    85,   135,     0,   109,    93,    94,   100,   101,
     122,   141,   128,    10,     8,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    86,    89,    91,    92,   145,
     104,   105,   146,    30,    44,    29,    46,    48,    47,    23,
      24,    25,    26,    27,    28,    83,    84,    18,    19,    20,
      21,    22,    85,    51,   160,    87,    11,    17,    49,    50,
     138,    12,    14,    16,    17,    27,   135,   135,   134,    35,
      62,   135,    10,    12,    12,   135,   138,    13,   138,   139,
       8,   141,   141,   133,   107,     3,   117,   117,   125,   126,
     135,   137,   140,   149,   135,   150,   151,   152,   153,   154,
     154,   155,   155,   155,   155,   156,   156,   157,   157,   158,
     158,   158,   159,   125,   163,    12,   135,     3,   138,   118,
      67,    12,    12,   134,    11,    11,    12,    12,    14,    13,
     141,     8,    15,    95,    96,   112,    15,    95,   116,    13,
     139,     3,     9,    12,   129,   129,    12,   135,   135,   135,
     129,   129,    14,   117,   111,   113,   114,   115,   119,   128,
     118,   117,    15,   113,   117,    15,    11,   147,    34,   129,
      12,    12,    12,    16,   114,    96,    95,   113,    16,   113,
      12,   125,   127,   129,   129,    10,    10,   117,   118,    16,
      16,    10,   133,   142,    12,     8,    10,   133,   127
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
// BGG mm -- commented these out, not sure which proto was being used
//int yyparse (void);
#else
//int yyparse ();

// BGG mm -- this is the one I use
int yyparse (char *buf, int len);
#endif
#endif /* ! YYPARSE_PARAM */

// BGG mm -- these are declared in chuck.yy.c
extern char *thebuf;
extern int thelen;
extern int bpointer;
extern FILE *yyin;


/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

// BGG mm -- had to comment all these various yyparse()'s out,
//		not sure which one was actually defined
#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
//int
//yyparse (void *YYPARSE_PARAM)
#else
//int
//yyparse (YYPARSE_PARAM)
//    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
//int
//yyparse (void)
#else
//int
//yyparse ()

// BGG mm -- this is the one I use
int yyparse(char *buf, int len)

#endif
#endif
int yyparse(char *buf, int len)
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

// BGG mm
// these are declared in chuck.yy.c
thebuf = buf;
thelen = len;
bpointer = 0;
yyin = NULL;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 177 "chuck.y"
    { (yyval.program) = g_program = new_program( (yyvsp[(1) - (1)].program_section), EM_lineNum ); ;}
    break;

  case 3:
#line 178 "chuck.y"
    { (yyval.program) = g_program = prepend_program( (yyvsp[(1) - (2)].program_section), (yyvsp[(2) - (2)].program), EM_lineNum ); ;}
    break;

  case 4:
#line 182 "chuck.y"
    { (yyval.program_section) = new_section_stmt( (yyvsp[(1) - (1)].stmt_list), EM_lineNum ); ;}
    break;

  case 5:
#line 183 "chuck.y"
    { (yyval.program_section) = new_section_func_def( (yyvsp[(1) - (1)].func_def), EM_lineNum ); ;}
    break;

  case 6:
#line 184 "chuck.y"
    { (yyval.program_section) = new_section_class_def( (yyvsp[(1) - (1)].class_def), EM_lineNum ); ;}
    break;

  case 7:
#line 189 "chuck.y"
    { (yyval.class_def) = new_class_def( (yyvsp[(1) - (6)].ival), (yyvsp[(3) - (6)].id_list), NULL, (yyvsp[(5) - (6)].class_body), EM_lineNum ); ;}
    break;

  case 8:
#line 191 "chuck.y"
    { (yyval.class_def) = new_class_def( (yyvsp[(1) - (7)].ival), (yyvsp[(3) - (7)].id_list), (yyvsp[(4) - (7)].class_ext), (yyvsp[(6) - (7)].class_body), EM_lineNum ); ;}
    break;

  case 9:
#line 193 "chuck.y"
    { (yyval.class_def) = new_iface_def( (yyvsp[(1) - (6)].ival), (yyvsp[(3) - (6)].id_list), NULL, (yyvsp[(5) - (6)].class_body), EM_lineNum ); ;}
    break;

  case 10:
#line 195 "chuck.y"
    { (yyval.class_def) = new_iface_def( (yyvsp[(1) - (7)].ival), (yyvsp[(3) - (7)].id_list), (yyvsp[(4) - (7)].class_ext), (yyvsp[(6) - (7)].class_body), EM_lineNum ); ;}
    break;

  case 11:
#line 199 "chuck.y"
    { (yyval.class_ext) = new_class_ext( NULL, (yyvsp[(2) - (2)].id_list), EM_lineNum ); ;}
    break;

  case 12:
#line 200 "chuck.y"
    { (yyval.class_ext) = new_class_ext( (yyvsp[(4) - (4)].id_list), (yyvsp[(2) - (4)].id_list), EM_lineNum ); ;}
    break;

  case 13:
#line 201 "chuck.y"
    { (yyval.class_ext) = new_class_ext( (yyvsp[(2) - (2)].id_list), NULL, EM_lineNum ); ;}
    break;

  case 14:
#line 202 "chuck.y"
    { (yyval.class_ext) = new_class_ext( (yyvsp[(2) - (4)].id_list), (yyvsp[(4) - (4)].id_list), EM_lineNum ); ;}
    break;

  case 15:
#line 206 "chuck.y"
    { (yyval.class_body) = (yyvsp[(1) - (1)].class_body); ;}
    break;

  case 16:
#line 207 "chuck.y"
    { (yyval.class_body) = NULL; ;}
    break;

  case 17:
#line 211 "chuck.y"
    { (yyval.class_body) = new_class_body( (yyvsp[(1) - (1)].program_section), EM_lineNum ); ;}
    break;

  case 18:
#line 212 "chuck.y"
    { (yyval.class_body) = prepend_class_body( (yyvsp[(1) - (2)].program_section), (yyvsp[(2) - (2)].class_body), EM_lineNum ); ;}
    break;

  case 19:
#line 217 "chuck.y"
    { (yyval.program_section) = new_section_stmt( (yyvsp[(1) - (1)].stmt_list), EM_lineNum ); ;}
    break;

  case 20:
#line 218 "chuck.y"
    { (yyval.program_section) = new_section_func_def( (yyvsp[(1) - (1)].func_def), EM_lineNum ); ;}
    break;

  case 21:
#line 219 "chuck.y"
    { (yyval.program_section) = new_section_class_def( (yyvsp[(1) - (1)].class_def), EM_lineNum ); ;}
    break;

  case 22:
#line 223 "chuck.y"
    { (yyval.class_ext) = new_class_ext( NULL, (yyvsp[(2) - (2)].id_list), EM_lineNum ); ;}
    break;

  case 23:
#line 227 "chuck.y"
    { (yyval.id_list) = new_id_list( (yyvsp[(1) - (1)].sval), EM_lineNum ); ;}
    break;

  case 24:
#line 228 "chuck.y"
    { (yyval.id_list) = prepend_id_list( (yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].id_list), EM_lineNum ); ;}
    break;

  case 25:
#line 232 "chuck.y"
    { (yyval.id_list) = new_id_list( (yyvsp[(1) - (1)].sval), EM_lineNum ); ;}
    break;

  case 26:
#line 233 "chuck.y"
    { (yyval.id_list) = prepend_id_list( (yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].id_list), EM_lineNum ); ;}
    break;

  case 27:
#line 238 "chuck.y"
    { (yyval.func_def) = new_func_def( (yyvsp[(1) - (8)].ival), (yyvsp[(2) - (8)].ival), (yyvsp[(3) - (8)].type_decl), (yyvsp[(4) - (8)].sval), (yyvsp[(6) - (8)].arg_list), (yyvsp[(8) - (8)].stmt), EM_lineNum ); ;}
    break;

  case 28:
#line 240 "chuck.y"
    { (yyval.func_def) = new_func_def( (yyvsp[(1) - (7)].ival), (yyvsp[(2) - (7)].ival), (yyvsp[(3) - (7)].type_decl), (yyvsp[(4) - (7)].sval), NULL, (yyvsp[(7) - (7)].stmt), EM_lineNum ); ;}
    break;

  case 29:
#line 242 "chuck.y"
    { (yyval.func_def) = new_func_def( (yyvsp[(1) - (8)].ival), (yyvsp[(2) - (8)].ival), (yyvsp[(3) - (8)].type_decl), (yyvsp[(4) - (8)].sval), (yyvsp[(6) - (8)].arg_list), NULL, EM_lineNum ); ;}
    break;

  case 30:
#line 244 "chuck.y"
    { (yyval.func_def) = new_func_def( (yyvsp[(1) - (7)].ival), (yyvsp[(2) - (7)].ival), (yyvsp[(3) - (7)].type_decl), (yyvsp[(4) - (7)].sval), NULL, NULL, EM_lineNum ); ;}
    break;

  case 31:
#line 248 "chuck.y"
    { (yyval.ival) = ae_key_public; ;}
    break;

  case 32:
#line 249 "chuck.y"
    { (yyval.ival) = ae_key_private; ;}
    break;

  case 33:
#line 250 "chuck.y"
    { (yyval.ival) = ae_key_private; ;}
    break;

  case 34:
#line 254 "chuck.y"
    { (yyval.ival) = ae_key_func; ;}
    break;

  case 35:
#line 255 "chuck.y"
    { (yyval.ival) = ae_key_public; ;}
    break;

  case 36:
#line 256 "chuck.y"
    { (yyval.ival) = ae_key_protected; ;}
    break;

  case 37:
#line 257 "chuck.y"
    { (yyval.ival) = ae_key_private; ;}
    break;

  case 38:
#line 261 "chuck.y"
    { (yyval.ival) = ae_key_static; ;}
    break;

  case 39:
#line 262 "chuck.y"
    { (yyval.ival) = ae_key_abstract; ;}
    break;

  case 40:
#line 263 "chuck.y"
    { (yyval.ival) = ae_key_instance; ;}
    break;

  case 41:
#line 267 "chuck.y"
    { (yyval.type_decl) = new_type_decl( new_id_list( (yyvsp[(1) - (1)].sval), EM_lineNum ), 0, EM_lineNum ); ;}
    break;

  case 42:
#line 268 "chuck.y"
    { (yyval.type_decl) = new_type_decl( new_id_list( (yyvsp[(1) - (2)].sval), EM_lineNum ), 1, EM_lineNum ); ;}
    break;

  case 43:
#line 272 "chuck.y"
    { (yyval.type_decl) = new_type_decl( (yyvsp[(2) - (3)].id_list), 0, EM_lineNum ); ;}
    break;

  case 44:
#line 273 "chuck.y"
    { (yyval.type_decl) = new_type_decl( (yyvsp[(2) - (4)].id_list), 1, EM_lineNum ); ;}
    break;

  case 45:
#line 282 "chuck.y"
    { (yyval.type_decl) = (yyvsp[(1) - (1)].type_decl); ;}
    break;

  case 46:
#line 283 "chuck.y"
    { (yyval.type_decl) = (yyvsp[(1) - (1)].type_decl); ;}
    break;

  case 47:
#line 288 "chuck.y"
    { (yyval.type_decl) = (yyvsp[(1) - (1)].type_decl); ;}
    break;

  case 48:
#line 289 "chuck.y"
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[(1) - (2)].type_decl), (yyvsp[(2) - (2)].array_sub), EM_lineNum ); ;}
    break;

  case 49:
#line 293 "chuck.y"
    { (yyval.arg_list) = new_arg_list( (yyvsp[(1) - (2)].type_decl), (yyvsp[(2) - (2)].var_decl), EM_lineNum ); ;}
    break;

  case 50:
#line 294 "chuck.y"
    { (yyval.arg_list) = prepend_arg_list( (yyvsp[(1) - (4)].type_decl), (yyvsp[(2) - (4)].var_decl), (yyvsp[(4) - (4)].arg_list), EM_lineNum ); ;}
    break;

  case 51:
#line 298 "chuck.y"
    { (yyval.stmt_list) = new_stmt_list( (yyvsp[(1) - (1)].stmt), EM_lineNum ); ;}
    break;

  case 52:
#line 299 "chuck.y"
    { (yyval.stmt_list) = prepend_stmt_list( (yyvsp[(1) - (2)].stmt), (yyvsp[(2) - (2)].stmt_list), EM_lineNum ); ;}
    break;

  case 53:
#line 303 "chuck.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 54:
#line 304 "chuck.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 55:
#line 305 "chuck.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 56:
#line 306 "chuck.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 57:
#line 308 "chuck.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 58:
#line 312 "chuck.y"
    { (yyval.stmt) = new_stmt_from_return( NULL, EM_lineNum ); ;}
    break;

  case 59:
#line 313 "chuck.y"
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[(2) - (3)].exp), EM_lineNum ); ;}
    break;

  case 60:
#line 314 "chuck.y"
    { (yyval.stmt) = new_stmt_from_break( EM_lineNum ); ;}
    break;

  case 61:
#line 315 "chuck.y"
    { (yyval.stmt) = new_stmt_from_continue( EM_lineNum ); ;}
    break;

  case 62:
#line 320 "chuck.y"
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[(3) - (5)].exp), (yyvsp[(5) - (5)].stmt), NULL, EM_lineNum ); ;}
    break;

  case 63:
#line 322 "chuck.y"
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[(3) - (7)].exp), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt), EM_lineNum ); ;}
    break;

  case 64:
#line 327 "chuck.y"
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[(3) - (5)].exp), (yyvsp[(5) - (5)].stmt), EM_lineNum ); ;}
    break;

  case 65:
#line 329 "chuck.y"
    { (yyval.stmt) = new_stmt_from_do_while( (yyvsp[(5) - (7)].exp), (yyvsp[(2) - (7)].stmt), EM_lineNum ); ;}
    break;

  case 66:
#line 331 "chuck.y"
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[(3) - (6)].stmt), (yyvsp[(4) - (6)].stmt), NULL, (yyvsp[(6) - (6)].stmt), EM_lineNum ); ;}
    break;

  case 67:
#line 333 "chuck.y"
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[(3) - (7)].stmt), (yyvsp[(4) - (7)].stmt), (yyvsp[(5) - (7)].exp), (yyvsp[(7) - (7)].stmt), EM_lineNum ); ;}
    break;

  case 68:
#line 335 "chuck.y"
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[(3) - (5)].exp), (yyvsp[(5) - (5)].stmt), EM_lineNum ); ;}
    break;

  case 69:
#line 337 "chuck.y"
    { (yyval.stmt) = new_stmt_from_do_until( (yyvsp[(5) - (7)].exp), (yyvsp[(2) - (7)].stmt), EM_lineNum ); ;}
    break;

  case 70:
#line 339 "chuck.y"
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[(3) - (5)].exp), (yyvsp[(5) - (5)].stmt), EM_lineNum ); ;}
    break;

  case 71:
#line 343 "chuck.y"
    { (yyval.stmt) = new_stmt_from_code( NULL, EM_lineNum ); ;}
    break;

  case 72:
#line 344 "chuck.y"
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[(2) - (3)].stmt_list), EM_lineNum ); ;}
    break;

  case 73:
#line 348 "chuck.y"
    { (yyval.stmt) = NULL; ;}
    break;

  case 74:
#line 349 "chuck.y"
    { (yyval.stmt) = new_stmt_from_expression( (yyvsp[(1) - (2)].exp), EM_lineNum ); ;}
    break;

  case 75:
#line 353 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 76:
#line 354 "chuck.y"
    { (yyval.exp) = prepend_expression( (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 77:
#line 358 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 78:
#line 360 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 79:
#line 364 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 80:
#line 366 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 81:
#line 370 "chuck.y"
    { (yyval.array_sub) = new_array_sub( (yyvsp[(2) - (3)].exp), EM_lineNum ); ;}
    break;

  case 82:
#line 372 "chuck.y"
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[(4) - (4)].array_sub), (yyvsp[(2) - (4)].exp), EM_lineNum ); ;}
    break;

  case 83:
#line 376 "chuck.y"
    { (yyval.array_sub) = new_array_sub( NULL, EM_lineNum ); ;}
    break;

  case 84:
#line 377 "chuck.y"
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[(1) - (3)].array_sub), NULL, EM_lineNum ); ;}
    break;

  case 85:
#line 381 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 86:
#line 382 "chuck.y"
    { (yyval.exp) = new_exp_decl( (yyvsp[(1) - (2)].type_decl), (yyvsp[(2) - (2)].var_decl_list), 0, EM_lineNum ); ;}
    break;

  case 87:
#line 383 "chuck.y"
    { (yyval.exp) = new_exp_decl( (yyvsp[(2) - (3)].type_decl), (yyvsp[(3) - (3)].var_decl_list), 1, EM_lineNum ); ;}
    break;

  case 88:
#line 384 "chuck.y"
    { (yyval.exp) = new_exp_decl( NULL, (yyvsp[(2) - (2)].var_decl_list), 0, EM_lineNum ); ;}
    break;

  case 89:
#line 385 "chuck.y"
    { (yyval.exp) = new_exp_decl( NULL, (yyvsp[(3) - (3)].var_decl_list), 1, EM_lineNum ); ;}
    break;

  case 90:
#line 389 "chuck.y"
    { (yyval.var_decl_list) = new_var_decl_list( (yyvsp[(1) - (1)].var_decl), EM_lineNum ); ;}
    break;

  case 91:
#line 390 "chuck.y"
    { (yyval.var_decl_list) = prepend_var_decl_list( (yyvsp[(1) - (3)].var_decl), (yyvsp[(3) - (3)].var_decl_list), EM_lineNum ); ;}
    break;

  case 92:
#line 394 "chuck.y"
    { (yyval.var_decl) = new_var_decl( (yyvsp[(1) - (1)].sval), NULL, EM_lineNum ); ;}
    break;

  case 93:
#line 395 "chuck.y"
    { (yyval.var_decl) = new_var_decl( (yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].array_sub), EM_lineNum ); ;}
    break;

  case 94:
#line 396 "chuck.y"
    { (yyval.var_decl) = new_var_decl( (yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].array_sub), EM_lineNum ); ;}
    break;

  case 95:
#line 401 "chuck.y"
    { (yyval.complex_exp) = new_complex( (yyvsp[(2) - (3)].exp), EM_lineNum ); ;}
    break;

  case 96:
#line 406 "chuck.y"
    { (yyval.polar_exp) = new_polar( (yyvsp[(2) - (3)].exp), EM_lineNum ); ;}
    break;

  case 97:
#line 410 "chuck.y"
    { (yyval.ival) = ae_op_chuck; ;}
    break;

  case 98:
#line 411 "chuck.y"
    { (yyval.ival) = ae_op_at_chuck; ;}
    break;

  case 99:
#line 412 "chuck.y"
    { (yyval.ival) = ae_op_plus_chuck; ;}
    break;

  case 100:
#line 413 "chuck.y"
    { (yyval.ival) = ae_op_minus_chuck; ;}
    break;

  case 101:
#line 414 "chuck.y"
    { (yyval.ival) = ae_op_times_chuck; ;}
    break;

  case 102:
#line 415 "chuck.y"
    { (yyval.ival) = ae_op_divide_chuck; ;}
    break;

  case 103:
#line 416 "chuck.y"
    { (yyval.ival) = ae_op_shift_right_chuck; ;}
    break;

  case 104:
#line 417 "chuck.y"
    { (yyval.ival) = ae_op_shift_left_chuck; ;}
    break;

  case 105:
#line 418 "chuck.y"
    { (yyval.ival) = ae_op_percent_chuck; ;}
    break;

  case 106:
#line 419 "chuck.y"
    { (yyval.ival) = ae_op_unchuck; ;}
    break;

  case 107:
#line 420 "chuck.y"
    { (yyval.ival) = ae_op_upchuck; ;}
    break;

  case 108:
#line 421 "chuck.y"
    { (yyval.ival) = ae_op_s_and_chuck; ;}
    break;

  case 109:
#line 422 "chuck.y"
    { (yyval.ival) = ae_op_s_or_chuck; ;}
    break;

  case 110:
#line 423 "chuck.y"
    { (yyval.ival) = ae_op_s_xor_chuck; ;}
    break;

  case 111:
#line 427 "chuck.y"
    { (yyval.ival) = ae_op_arrow_left; ;}
    break;

  case 112:
#line 428 "chuck.y"
    { (yyval.ival) = ae_op_arrow_right; ;}
    break;

  case 113:
#line 432 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 114:
#line 434 "chuck.y"
    { (yyval.exp) = new_exp_from_if( (yyvsp[(1) - (5)].exp), (yyvsp[(3) - (5)].exp), (yyvsp[(5) - (5)].exp), EM_lineNum ); ;}
    break;

  case 115:
#line 438 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 116:
#line 440 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_or, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 117:
#line 444 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 118:
#line 446 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_and, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 119:
#line 450 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 120:
#line 452 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_s_or, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 121:
#line 456 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 122:
#line 458 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_s_xor, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 123:
#line 462 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 124:
#line 464 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_s_and, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 125:
#line 468 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 126:
#line 470 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_eq, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 127:
#line 472 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_neq, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 128:
#line 476 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 129:
#line 478 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_lt, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 130:
#line 480 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_gt, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 131:
#line 482 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_le, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 132:
#line 484 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_ge, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 133:
#line 488 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 134:
#line 490 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_shift_left, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 135:
#line 492 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_shift_right, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 136:
#line 496 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 137:
#line 498 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_plus, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 138:
#line 500 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_minus, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 139:
#line 504 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 140:
#line 506 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_times, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 141:
#line 508 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_divide, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 142:
#line 510 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_percent, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 143:
#line 514 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 144:
#line 516 "chuck.y"
    { (yyval.exp) = new_exp_from_binary( (yyvsp[(1) - (3)].exp), ae_op_tilda, (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 145:
#line 520 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 146:
#line 522 "chuck.y"
    { (yyval.exp) = new_exp_from_cast( (yyvsp[(3) - (3)].type_decl), (yyvsp[(1) - (3)].exp), EM_lineNum ); ;}
    break;

  case 147:
#line 526 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 148:
#line 528 "chuck.y"
    { (yyval.exp) = new_exp_from_unary( ae_op_plusplus, (yyvsp[(2) - (2)].exp), EM_lineNum ); ;}
    break;

  case 149:
#line 530 "chuck.y"
    { (yyval.exp) = new_exp_from_unary( ae_op_minusminus, (yyvsp[(2) - (2)].exp), EM_lineNum ); ;}
    break;

  case 150:
#line 532 "chuck.y"
    { (yyval.exp) = new_exp_from_unary( (yyvsp[(1) - (2)].ival), (yyvsp[(2) - (2)].exp), EM_lineNum ); ;}
    break;

  case 151:
#line 534 "chuck.y"
    { (yyval.exp) = new_exp_from_unary( ae_op_typeof, (yyvsp[(2) - (2)].exp), EM_lineNum ); ;}
    break;

  case 152:
#line 536 "chuck.y"
    { (yyval.exp) = new_exp_from_unary( ae_op_sizeof, (yyvsp[(2) - (2)].exp), EM_lineNum ); ;}
    break;

  case 153:
#line 538 "chuck.y"
    { (yyval.exp) = new_exp_from_unary2( ae_op_new, (yyvsp[(2) - (2)].type_decl), NULL, EM_lineNum ); ;}
    break;

  case 154:
#line 540 "chuck.y"
    { (yyval.exp) = new_exp_from_unary2( ae_op_new, (yyvsp[(2) - (3)].type_decl), (yyvsp[(3) - (3)].array_sub), EM_lineNum ); ;}
    break;

  case 155:
#line 542 "chuck.y"
    { (yyval.exp) = new_exp_from_unary3( ae_op_spork, (yyvsp[(3) - (3)].stmt), EM_lineNum ); ;}
    break;

  case 156:
#line 546 "chuck.y"
    { (yyval.ival) = ae_op_plus; ;}
    break;

  case 157:
#line 547 "chuck.y"
    { (yyval.ival) = ae_op_minus; ;}
    break;

  case 158:
#line 548 "chuck.y"
    { (yyval.ival) = ae_op_tilda; ;}
    break;

  case 159:
#line 549 "chuck.y"
    { (yyval.ival) = ae_op_exclamation; ;}
    break;

  case 160:
#line 550 "chuck.y"
    { (yyval.ival) = ae_op_times; ;}
    break;

  case 161:
#line 551 "chuck.y"
    { (yyval.ival) = ae_op_spork; ;}
    break;

  case 163:
#line 558 "chuck.y"
    { (yyval.exp) = new_exp_from_dur( (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp), EM_lineNum ); ;}
    break;

  case 164:
#line 562 "chuck.y"
    { (yyval.exp) = (yyvsp[(1) - (1)].exp); ;}
    break;

  case 165:
#line 564 "chuck.y"
    { (yyval.exp) = new_exp_from_array( (yyvsp[(1) - (2)].exp), (yyvsp[(2) - (2)].array_sub), EM_lineNum ); ;}
    break;

  case 166:
#line 566 "chuck.y"
    { (yyval.exp) = new_exp_from_func_call( (yyvsp[(1) - (3)].exp), NULL, EM_lineNum ); ;}
    break;

  case 167:
#line 568 "chuck.y"
    { (yyval.exp) = new_exp_from_func_call( (yyvsp[(1) - (4)].exp), (yyvsp[(3) - (4)].exp), EM_lineNum ); ;}
    break;

  case 168:
#line 570 "chuck.y"
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].sval), EM_lineNum ); ;}
    break;

  case 169:
#line 572 "chuck.y"
    { (yyval.exp) = new_exp_from_postfix( (yyvsp[(1) - (2)].exp), ae_op_plusplus, EM_lineNum ); ;}
    break;

  case 170:
#line 574 "chuck.y"
    { (yyval.exp) = new_exp_from_postfix( (yyvsp[(1) - (2)].exp), ae_op_minusminus, EM_lineNum ); ;}
    break;

  case 171:
#line 578 "chuck.y"
    { (yyval.exp) = new_exp_from_id( (yyvsp[(1) - (1)].sval), EM_lineNum ); ;}
    break;

  case 172:
#line 579 "chuck.y"
    { (yyval.exp) = new_exp_from_int( (yyvsp[(1) - (1)].ival), EM_lineNum ); ;}
    break;

  case 173:
#line 580 "chuck.y"
    { (yyval.exp) = new_exp_from_float( (yyvsp[(1) - (1)].fval), EM_lineNum ); ;}
    break;

  case 174:
#line 581 "chuck.y"
    { (yyval.exp) = new_exp_from_str( (yyvsp[(1) - (1)].sval), EM_lineNum ); ;}
    break;

  case 175:
#line 582 "chuck.y"
    { (yyval.exp) = new_exp_from_array_lit( (yyvsp[(1) - (1)].array_sub), EM_lineNum ); ;}
    break;

  case 176:
#line 583 "chuck.y"
    { (yyval.exp) = new_exp_from_complex( (yyvsp[(1) - (1)].complex_exp), EM_lineNum ); ;}
    break;

  case 177:
#line 584 "chuck.y"
    { (yyval.exp) = new_exp_from_polar( (yyvsp[(1) - (1)].polar_exp), EM_lineNum ); ;}
    break;

  case 178:
#line 585 "chuck.y"
    { (yyval.exp) = new_exp_from_hack( (yyvsp[(2) - (3)].exp), EM_lineNum ); ;}
    break;

  case 179:
#line 586 "chuck.y"
    { (yyval.exp) = (yyvsp[(2) - (3)].exp); ;}
    break;

  case 180:
#line 587 "chuck.y"
    { (yyval.exp) = new_exp_from_nil( EM_lineNum ); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2971 "chuck.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



