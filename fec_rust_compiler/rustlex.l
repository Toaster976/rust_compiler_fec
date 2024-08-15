/* -*-bison-*- */
/* Simple wc implementation supported by Flex */
%option noyywrap
%option noinput
%option nounput
%{
	#include "ytab.h"
	#include "rustgram.tab.h"
	#include "tree.h"

	extern YYSTYPE yylval;
	char yyfilename[256];

	void yyerror(const char *error) {
		fprintf(stderr, "Syntax error: %s\nFilename:%s\nLineno:%d\ntoken: %s\n", error, yyfilename, yylineno, yytext);
		exit(2);
	}

	void yyerror_lex(const char *error) {
		fprintf(stderr, "Lexical error: %s\nFilename:%s\nLineno:%d\ntoken: %s\n", error, yyfilename, yylineno, yytext);
		exit(1);
	}
%}

%%
\/\/.* 			{/*comment*/}
[ \t\f\v]+    	{/*whitespace*/}

"break"			{return alctoken(BREAK);}
"const"			{return alctoken(CONST);}
"else"			{return alctoken(ELSE);}	
"false"			{return alctoken(FALSE);}
"fn"			{return alctoken(FN);}
"for"			{return alctoken(FOR);}
"if"			{return alctoken(IF);}
"in"			{return alctoken(IN);}
"let"			{return alctoken(LET);}
"mut"			{return alctoken(MUT);}
"ref"			{return alctoken(REF);}
"return"		{return alctoken(RETURN);}
"static"		{return alctoken(STATIC);}
"true"			{return alctoken(TRUE);}
"while"			{return alctoken(WHILE);}

"="				{return alctoken('=');}
"+="			{return alctoken(PLUSEQ);}
"-="			{return alctoken(MINUSEQ);}
"+"				{return alctoken('+');}
"-"				{return alctoken('-');}
"*"				{return alctoken('*');}
"/"				{return alctoken('/');}
"%"				{return alctoken('%');}
"=="			{return alctoken(EQEQ);}
"!="			{return alctoken(NE);}
">"				{return alctoken('>');}
"<"				{return alctoken('<');}
"<="			{return alctoken(LE);}
">="			{return alctoken(GE);}
"&&"			{return alctoken(ANDAND);}
"||"			{return alctoken(OROR);}
"!"				{return alctoken('!');}
"."				{return alctoken('.');}
".."			{return alctoken(DOTDOT);}
"["				{return alctoken('[');}
"]"				{return alctoken(']');}

"("				{return alctoken('(');}
")"				{return alctoken(')');}
","				{return alctoken(',');}
";"				{return alctoken(';');}
"{"				{return alctoken('{');}
"}"				{return alctoken('}');}
":"				{return alctoken(':');}
"->"			{return alctoken(RARROW);}

"as"			{yyerror_lex("\'as\' rust reserved word is not supported by irony");}
"continue"			{yyerror_lex("\'continue\' rust reserved word is not supported by irony");}
"crate"			{yyerror_lex("\'crate\' rust reserved word is not supported by irony");}
"enum"		{yyerror_lex("\'enum\' rust reserved word is not supported by irony");}
"extern"		{yyerror_lex("\'extern\' rust reserved word is not supported by irony");}
"impl"			{yyerror_lex("\'impl\' rust reserved word is not supported by irony");}
"loop"			{yyerror_lex("\'loop\' rust reserved word is not supported by irony");}
"match"			{yyerror_lex("\'match\' rust reserved word is not supported by irony");}
"mod"			{yyerror_lex("\'mod\' rust reserved word is not supported by irony");}
"move"		{yyerror_lex("\'move\' rust reserved word is not supported by irony");}
"pub"			{yyerror_lex("\'pub\' rust reserved word is not supported by irony");}
"self"		{yyerror_lex("\'self\' rust reserved word is not supported by irony");}
"Self"		{yyerror_lex("\'Self\' rust reserved word is not supported by irony");}
"struct"		{yyerror_lex("\'struct\' rust reserved word is not supported by irony");}
"super"			{yyerror_lex("\'super\' rust reserved word is not supported by irony");}
"trait"			{yyerror_lex("\'trait\' rust reserved word is not supported by irony");}
"type"	{yyerror_lex("\'type\' rust reserved word is not supported by irony");}
"unsafe"			{yyerror_lex("\'unsafe\' rust reserved word is not supported by irony");}
"use"			{yyerror_lex("\'use\' rust reserved word is not supported by irony");}
"where"			{yyerror_lex("\'where\' rust reserved word is not supported by irony");}
"async"			{yyerror_lex("\'async\' rust reserved word is not supported by irony");}
"await"			{yyerror_lex("\'await\' rust reserved word is not supported by irony");}
"dyn"			{yyerror_lex("\'dyn\' rust reserved word is not supported by irony");}
"abstract"		{yyerror_lex("\'abstract\' rust reserved word is not supported by irony");}
"become"			{yyerror_lex("\'become\' rust reserved word is not supported by irony");}
"box"			{yyerror_lex("\'\' rust reserved word is not supported by irony");}
"do"			{yyerror_lex("\'do\' rust reserved word is not supported by irony");}
"final"		{yyerror_lex("\'final\' rust reserved word is not supported by irony");}
"macro"			{yyerror_lex("\'macro\' rust reserved word is not supported by irony");}
"override"			{yyerror_lex("\'override\' rust reserved word is not supported by irony");}
"priv"			{yyerror_lex("\'priv\' rust reserved word is not supported by irony");}
"typeof"			{yyerror_lex("\'typeof\' rust reserved word is not supported by irony");}
"unsized"		{yyerror_lex("\'unsized\' rust reserved word is not supported by irony");}
"virtual"		{yyerror_lex("\'virtual\' rust reserved word is not supported by irony");}
"yield"			{yyerror_lex("\'yield\' rust reserved word is not supported by irony");}
"try"			{yyerror_lex("\'try\' rust reserved word is not supported by irony");}
"macro_rules"			{yyerror_lex("\'macro_rules\' rust reserved word is not supported by irony");}
"union"			{yyerror_lex("\'union\' rust reserved word is not supported by irony");}

"~"				{yyerror_lex("\'~\' rust operation is not supported by irony");}
"<<"				{yyerror_lex("\'<<\' rust operation is not supported by irony");}
">>"				{yyerror_lex("\'>>\' rust operation is not supported by irony");}
"&="				{yyerror_lex("\'&=\' rust operation is not supported by irony");}
"|="				{yyerror_lex("\'!=\' rust operation is not supported by irony");}
"^="				{yyerror_lex("\'^=\' rust operation is not supported by irony");}
">>="				{yyerror_lex("\'>>=\' rust operation is not supported by irony");}
"<<="				{yyerror_lex("\'<<=\' rust operation is not supported by irony");}
"&"				{yyerror_lex("\'&\' rust operation is not supported by irony");}
"|"				{yyerror_lex("\'|\' rust operation is not supported by irony");}
"^"				{yyerror_lex("\'^\' rust operation is not supported by irony");}
"*="				{yyerror_lex("\'*=\' rust operation is not supported by irony");}
"/="				{yyerror_lex("\'/=\' rust operation is not supported by irony");}
"%="				{yyerror_lex("\'%=\' rust operation is not supported by irony");}

\"([^\"\\]|\\[\"\'\\ntrxu])*\"	{return alctoken(LIT_STR);}	
\"([^\"\\]|\\[\"\'\\ntrxu])*$   {yyerror("Missing terminating \" in string");}
\'([A-Za-z_0-9]|\\n|\\r|\\t|\\\'|\\\"|\\\\)\'	{return alctoken(LIT_CHAR);}
\'([A-Za-z_0-9]|\\n|\\r|\\t|\\\'|\\\"|\\\\)   {yyerror("Missing terminating \' in char");}

0b[01]+							{yyerror_lex("Binary literals are not supported by irony");}
0x[0-9a-fA-F]+					{yyerror_lex("Hex literals are not supported by irony");}
[iu](8|16|32|64|128)		{return alctoken(IDENT);}
"f"(32|64)					{return alctoken(IDENT);}
"bool"						{return alctoken(IDENT);}
[0-9]+						{return alctoken(LIT_INTEGER);}
[0-9]*[.][0-9]+				{return alctoken(LIT_FLOAT);}
[A-Za-z_][A-Za-z_0-9]*		{return alctoken(IDENT);}

(\n|\r\n)		        		{ yylineno++;}

.		        		{yyerror_lex("Unkown character");}
%%