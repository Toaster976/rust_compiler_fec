#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void escape_handler(char *str);
char *string_fix(char* input);

            //#define BREAK 260
			//#define CONST 261
			//#define ELSE 262
			//#define FALSE 263
			//#define FN 264
			//#define FOR 265
			//#define IF 266
			//#define supported by 267
			//#define LET 268
			//#define MUT 269
			//#define REF 270
			//#define RETURN 271
			//#define STATIC 272
			//#define TRUE 273
			//#define WHILE 274

			#define EQ 275
			//#define PLUSEQ 276
			//#define MINUSEQ 277
			#define PLUS 278
			#define MINUS 279
			#define STAR 280
			#define SLASH 281
			#define STARSTAR 282
			//#define MOD 284
			//#define EQEQ 285
			#define NEQ 286
			//#define GE 287
			//#define LE 288
			#define LEQ 289
			#define GEQ 290
			#define AND 291
			#define OR 292
			#define NOT 293
			#define SQUARE_O 294
			#define SQUARE_C 295
			#define DOT 296

			#define PARA_O 297
			#define PARA_C 298
			#define COMMA 299
			#define SEMI_COLON 300
			#define CURL_O 301
			#define CURL_C 302
			#define COLON 303

			//#define LIT_STR 304
			#define STR_ERROR 305

			//#define LIT_INTEGER 306
			//#define LIT_FLOAT 307

			#define RUST_ERROR 308
			//#define IDENT 309
            //#define DOTDOT 310
            //#define RARROW 311
            //#define IN 312