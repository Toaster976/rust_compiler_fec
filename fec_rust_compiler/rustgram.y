%{
#include <stdio.h>
#include <string.h>
/* 
 * Adapted from from the Rust project's deleted parser-lalr.y
 * via the Wayback Machine. Since that grammar was old and
 * Rust changed incompatibly since then, you should test/check
 * everything and trust nothing.

// Copyright 2015 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

 */
	#include "tree.h"

	//#define YYERROR_VERBOSE 1
	//#define YYDEBUG 1
	//#define YYSTYPE struct node*
	extern int yylex();
	extern void yyerror(char const *error);
	extern int yychar;
    extern char *yytext;
	//extern static const char *const yytname[];
	//int yyerror(char *);
	token_t *token;
    tree_t *tree;
	tree_t *root;

%}

%union {
   struct tree *treeptr;
}

%debug

%token SHL
%token SHR
%token LE
%token EQEQ
%token NE
%token GE
%token ANDAND
%token OROR
%token SHLEQ
%token SHREQ
%token MINUSEQ
%token ANDEQ
%token OREQ
%token PLUSEQ
%token STAREQ
%token SLASHEQ
%token CARETEQ
%token PERCENTEQ
%token DOTDOT
%token <treeptr> DOTDOTDOT
%token <treeptr> MOD_SEP
%token RARROW
%token LARROW
%token <treeptr> FAT_ARROW
%token <treeptr> LIT_BYTE
%token <treeptr> LIT_CHAR
%token <treeptr> LIT_INTEGER
%token <treeptr> LIT_FLOAT
%token <treeptr> LIT_STR
%token <treeptr> LIT_STR_RAW
%token <treeptr> LIT_BYTE_STR
%token <treeptr> LIT_BYTE_STR_RAW
%token IDENT
%token <treeptr> UNDERSCORE
%token <treeptr> LIFETIME

// keywords
//%token SELF
%token <treeptr> STATIC
//%token ABSTRACT
%token <treeptr> ALIGNOF
// %token AS
//%token BECOME
%token <treeptr> BREAK
%token <treeptr> CATCH
//%token CRATE
//%token DO
%token <treeptr> ELSE
//%token ENUM
//%token EXTERN
%token <treeptr> FALSE
//%token FINAL
%token <treeptr> FN
%token <treeptr> FOR
%token <treeptr> IF  // TODO: double check this
//%token IMPL
%token <treeptr> IN
%token <treeptr> LET
//%token LOOP
//%token MACRO
//%token MATCH
//%token MOD
//%token MOVE
%token <treeptr> MUT
%token <treeptr> OFFSETOF
//%token OVERRIDE
//%token PRIV
//%token PUB
%token <treeptr> PURE
%token <treeptr> REF
%token RETURN
%token <treeptr> SIZEOF
//%token STRUCT
//%token SUPER
//%token UNION
//%token UNSIZED
%token <treeptr> TRUE
//%token TRAIT
//%token TYPE
//%token UNSAFE
//%token VIRTUAL
//%token YIELD
%token <treeptr> DEFAULT
//%token USE
%token <treeptr> WHILE
//%token CONTINUE
%token <treeptr> PROC
//%token BOX
%token <treeptr> CONST
//%token WHERE
//%token TYPEOF
%token <treeptr> INNER_DOC_COMMENT
%token <treeptr> OUTER_DOC_COMMENT

%token <treeptr> SHEBANG
%token <treeptr> SHEBANG_LINE
%token <treeptr> STATIC_LIFETIME

 /*
   Quoting from the Bison manual:

   "Finally, the resolution of conflicts works by comparing the precedence
   of the rule being considered with that of the lookahead token. If the
   token's precedence is higher, the choice is to shift. If the rule's
   precedence is higher, the choice is to reduce. If they have equal
   precedence, the choice is made based on the associativity of that
   precedence level. The verbose output file made by â€˜-vâ€™ (see Invoking
   Bison) says how each conflict was resolved"
 */

// We expect no shift/reduce or reduce/reduce conflicts in this grammar;
// all potential ambiguities are scrutinized and eliminated manually.
%expect 0

// fake-precedence symbol to cause '|' bars in lambda context to parse
// at low precedence, permit things like |x| foo = bar, where '=' is
// otherwise lower-precedence than '|'. Also used for proc() to cause
// things like proc() a + b to parse as proc() { a + b }.
%precedence <treeptr> LAMBDA

//%precedence SELF

// MUT should be lower precedence than IDENT so that in the pat rule,
// "& MUT pat" has higher precedence than "binding_mode ident [@ pat]"
%precedence MUT

// IDENT needs to be lower than '{' so that 'foo {' is shifted when
// trying to decide if we've got a struct-construction expr (esp. in
// contexts like 'if foo { .')
//
// IDENT also needs to be lower precedence than '<' so that '<' in
// 'foo:bar . <' is shifted (in a trait reference occurring in a
// bounds list), parsing as foo:(bar<baz>) rather than (foo:bar)<baz>.
%precedence <treeptr> IDENT
 // Put the weak keywords that can be used as idents here as well
%precedence CATCH
%precedence DEFAULT
//%precedence UNION

// A couple fake-precedence symbols to use in rules associated with +
// and < in trailing type contexts. These come up when you have a type
// in the RHS of operator-AS, such as "foo as bar<baz>". The "<" there
// has to be shifted so the parser keeps trying to parse a type, even
// though it might well consider reducing the type "bar" and then
// going on to "<" as a subsequent binop. The "+" case is with
// trailing type-bounds ("foo as bar:A+B"), for the same reason.
%precedence <treeptr> SHIFTPLUS

%precedence MOD_SEP
%precedence  <treeptr> RARROW ':'

// In where clauses, "for" should have greater precedence when used as
// a higher ranked constraint than when used as the beginning of a
// for_in_type (which is a ty)
%precedence <treeptr> FORTYPE
%precedence FOR

// Binops & unops, and their precedences
%precedence <treeptr> '?'
//%precedence BOX
%nonassoc <treeptr> DOTDOT

// RETURN needs to be lower-precedence than tokens that start
// prefix_exprs
%precedence <treeptr> RETURN //YIELD

%right <treeptr> '=' SHLEQ SHREQ MINUSEQ ANDEQ OREQ PLUSEQ STAREQ SLASHEQ CARETEQ PERCENTEQ
%right <treeptr> LARROW
%left <treeptr> OROR
%left <treeptr> ANDAND
%left <treeptr> EQEQ NE
%left <treeptr> '<' '>' LE GE
%left <treeptr> '|'
%left <treeptr> '^'
%left <treeptr> '&'
%left <treeptr> SHL SHR
%left <treeptr> '+' '-'
// %precedence AS
%left <treeptr> '*' '/' '%'
%precedence <treeptr> '!'

%precedence <treeptr> '{' '[' '(' '.'

%precedence <treeptr> RANGE

%token <treeptr> '#' ']' ',' ')' ';'  '}' '@' '~' '$'

%type <treeptr> file crate maybe_shebang maybe_inner_attrs inner_attrs inner_attr maybe_outer_attrs outer_attrs outer_attr meta_item meta_seq maybe_mod_items mod_items attrs_and_vis mod_item item stmt_item item_static item_const item_macro block_item maybe_ty_ascription maybe_init_expr visibility item_fn fn_decl fn_params fn_anon_params maybe_params params param inferrable_params inferrable_param anon_params anon_param anon_params_allow_variadic_tail named_arg ret_ty generic_params maybe_where_clause ty_params path_generic_args_without_colons generic_args generic_values maybe_ty_sums_and_or_bindings pat binding_mode lit_or_path pat_field pat_fields pat_struct pat_tup pat_tup_elts pat_vec pat_vec_elts ty ty_prim ty_bare_fn ty_fn_decl ty_closure for_in_type for_in_type_suffix maybe_mut maybe_mut_or_const maybe_ty_sums ty_sums ty_sum ty_sum_elt maybe_ty_param_bounds ty_param_bounds boundseq polybound bindings binding ty_param maybe_bounds bounds bound maybe_ltbounds ltbounds maybe_ty_default maybe_lifetimes lifetimes lifetime_and_bounds lifetime trait_ref inner_attrs_and_block block maybe_stmts stmts stmt maybe_exprs maybe_expr exprs path_expr path_generic_args_with_colons macro_expr nonblock_expr expr expr_nostruct nonblock_prefix_expr_nostruct nonblock_prefix_expr expr_qualified_path maybe_qpath_params maybe_as_trait_ref lambda_expr lambda_expr_no_first_bar lambda_expr_nostruct lambda_expr_nostruct_no_first_bar vec_expr struct_expr_fields maybe_field_inits field_inits field_init default_field_init block_expr full_block_expr block_expr_dot expr_if expr_if_let block_or_if expr_while expr_while_let expr_for maybe_label let lit str maybe_ident ident unpaired_token token_trees token_tree delimited_token_trees parens_delimited_token_trees braces_delimited_token_trees brackets_delimited_token_trees

%start file

%%

////////////////////////////////////////////////////////////////////////
// Part 1: Items and attributes
////////////////////////////////////////////////////////////////////////

/*test : fn | ident1 |test test;

fn : FN {print_node($1);};
para_open_test : '(' {print_node($1);};
ident1 : IDENT {print_node($1);};*/

file : crate { root=alctree(1000, "file", 1, $1); };

crate   : maybe_shebang inner_attrs maybe_mod_items  { $$=alctree(YYSYMBOL_crate, "crate", 3, $1, $2, $3); }
	| maybe_shebang maybe_mod_items  { $$=alctree(YYSYMBOL_crate, "crate", 2, $1, $2);}
	;

maybe_shebang : SHEBANG_LINE {$$ = $1;}
	| %empty	{$$ = NULL;}
	;

maybe_inner_attrs : inner_attrs	{$$ = $1;}
	| %empty                   { $$ = NULL; }
	;

inner_attrs : inner_attr           { $$=$1; }
	| inner_attrs inner_attr   { $$=alctree(YYSYMBOL_inner_attrs, "inner_attrs", 2, $1, $2); }
	;

inner_attr : SHEBANG '[' meta_item ']'   { $$=alctree(YYSYMBOL_inner_attr, "inner_attr", 4, $1, $2, $3, $4); }
	| INNER_DOC_COMMENT           { $$ = $1; }
	;

maybe_outer_attrs : outer_attrs	{$$ = $1;}
	| %empty                   { $$ = NULL; }
	;

outer_attrs : outer_attr               { $$=$1; }
	| outer_attrs outer_attr   { $$=alctree(YYSYMBOL_outer_attrs, "outer_attrs", 2, $1, $2); }
	;

outer_attr : '#' '[' meta_item ']'    { $$=alctree(YYSYMBOL_outer_attr, "outer_attr", 4, $1, $2, $3, $4); }
	| OUTER_DOC_COMMENT        { $$=$1; }
	;

meta_item : ident                      { $$=$1; }
	| ident '=' lit              { $$=alctree(YYSYMBOL_meta_item, "meta_item", 3, $1, $2, $3); }
	| ident '(' meta_seq ')'     { $$=alctree(YYSYMBOL_meta_item, "meta_item", 3, $1, $2, $3); }
	| ident '(' meta_seq ',' ')' { $$=alctree(YYSYMBOL_meta_item, "meta_item", 4, $1, $2, $3, $4); }
	;

meta_seq : %empty                   { $$ = NULL; }
	| meta_item                { $$=$1; }
	| meta_seq ',' meta_item   { $$=alctree(YYSYMBOL_meta_seq, "meta_seq", 3, $1, $2, $3); }
	;

maybe_mod_items : mod_items	{ $$ = $1; }
	| %empty             { $$ = NULL; }
	;

mod_items : mod_item                               { $$ = $1; }
	| mod_items mod_item                     { $$=alctree(YYSYMBOL_mod_items, "mod_items", 2, $1, $2); }
	;

attrs_and_vis : maybe_outer_attrs visibility           { $$=alctree(YYSYMBOL_attrs_and_vis, "attrs_and_vis", 2, $1, $2); }
	;

mod_item : attrs_and_vis item    { $$=alctree(YYSYMBOL_mod_item, "mod_item", 2, $1, $2); }
	;

// items that can appear outside of a fn block
item : stmt_item	{ $$ = $1; }
	| item_macro	{ $$ = $1; }
	;

// items that can appear in "stmts"
stmt_item : item_static		{$$ = $1;}
	| item_const			{$$ = $1;}
	//| item_type
	| block_item			{$$ = $1;}
	//| view_item
	;

item_static : STATIC ident ':' ty '=' expr ';'  { $$=alctree(YYSYMBOL_item_static, "item_static", 7, $1, $2, $3, $4, $5, $6, $7); }
	| STATIC MUT ident ':' ty '=' expr ';'  { $$=alctree(YYSYMBOL_item_static, "item_static", 8, $1, $2, $3, $4, $5, $6, $7, $8); }
	;

item_const : CONST ident ':' ty '=' expr ';'    { $$=alctree(YYSYMBOL_item_const, "item_const", 7, $1, $2, $3, $4, $5, $6, $7); }
	;

item_macro : path_expr '!' maybe_ident parens_delimited_token_trees ';'  { $$=alctree(YYSYMBOL_item_macro, "item_macro", 5, $1, $2, $3, $4, $5); }
	| path_expr '!' maybe_ident braces_delimited_token_trees      { $$=alctree(YYSYMBOL_item_macro, "item_macro", 4, $1, $2, $3, $4); }
	| path_expr '!' maybe_ident brackets_delimited_token_trees ';'{ $$=alctree(YYSYMBOL_item_macro, "item_macro", 4, $1, $2, $3, $4); }
	;

/*view_item : use_item
	| extern_fn_item
	//| EXTERN CRATE ident ';'                      {  }
	//| EXTERN CRATE ident AS ident ';'             {  }
	;*/

/*extern_fn_item : EXTERN maybe_abi item_fn             {  }
	;*/

/*use_item : USE view_path ';'                          {  }
	;*/

/*view_path : path_no_types_allowed                                    {  }
	| path_no_types_allowed MOD_SEP '{'                '}'     {  }
	|                       MOD_SEP '{'                '}'     {  }
	| path_no_types_allowed MOD_SEP '{' idents_or_self '}'     {  }
	|                       MOD_SEP '{' idents_or_self '}'     {  }
	| path_no_types_allowed MOD_SEP '{' idents_or_self ',' '}' {  }
	|                       MOD_SEP '{' idents_or_self ',' '}' {  }
	| path_no_types_allowed MOD_SEP '*'                        {  }
	|                       MOD_SEP '*'                        {  }
	|                               '*'                        {  }
	|                               '{'                '}'     {  }
	|                               '{' idents_or_self '}'     {  }
	|                               '{' idents_or_self ',' '}' {  }
	//| path_no_types_allowed AS ident                           {  }
	;*/

block_item : item_fn		{$$ = $1;}
	//| item_unsafe_fn
	//| item_mod
	//| item_foreign_mod          {  }
	//| item_struct
	//| item_enum
	//| item_union
	//| item_trait
	//| item_impl
	;

maybe_ty_ascription : ':' ty_sum { $$=alctree(YYSYMBOL_maybe_ty_ascription, "maybe_ty_ascription", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

maybe_init_expr : '=' expr { $$=alctree(YYSYMBOL_maybe_init_expr, "maybe_init_expr", 2, $1, $2); }
	| %empty   { $$ = NULL; }
	;

// structs
/*item_struct : STRUCT ident generic_params maybe_where_clause struct_decl_args {  }
	| STRUCT ident generic_params struct_tuple_args maybe_where_clause ';'{  }
	| STRUCT ident generic_params maybe_where_clause ';' {  }
	;*/

/*struct_decl_args : '{' struct_decl_fields '}'         {  }
	| '{' struct_decl_fields ',' '}'              {  }
	;*/

/*struct_tuple_args : '(' struct_tuple_fields ')'       {  }
	| '(' struct_tuple_fields ',' ')'             {  }
	;*/

/*struct_decl_fields : struct_decl_field                {  }
	| struct_decl_fields ',' struct_decl_field    {  }
	| %empty                                      {  }
	;*/

/*struct_decl_field : attrs_and_vis ident ':' ty_sum    {  }
	;*/

/*struct_tuple_fields : struct_tuple_field              {  }
	| struct_tuple_fields ',' struct_tuple_field  {  }
	| %empty                                      {  }
	;*/

/*struct_tuple_field : attrs_and_vis ty_sum             {  }
	;*/

// enums
/*item_enum : ENUM ident generic_params maybe_where_clause '{' enum_defs '}' {  }
	| ENUM ident generic_params maybe_where_clause '{' enum_defs ',' '}' {  }
	;*/

/*enum_defs : enum_def             {  }
	| enum_defs ',' enum_def {  }
	| %empty                 {  }
	;*/

/*enum_def : attrs_and_vis ident enum_args {  }
	;*/

/*enum_args : '{' struct_decl_fields '}'     {  }
	| '{' struct_decl_fields ',' '}' {  }
	| '(' maybe_ty_sums ')'          {  }
	| '=' expr                       {  }
	| %empty                         {  }
	;*/

// unions
/*item_union : UNION ident generic_params maybe_where_clause '{' struct_decl_fields '}'     {  }
	| UNION ident generic_params maybe_where_clause '{' struct_decl_fields ',' '}' {  }
	;*/

/*item_mod : MOD ident ';'                                {  }
	| MOD ident '{' maybe_mod_items '}'             {  }
	| MOD ident '{' inner_attrs maybe_mod_items '}' {  }
	;*/

/*item_foreign_mod : EXTERN maybe_abi '{' maybe_foreign_items '}' {  }
	| EXTERN maybe_abi '{' inner_attrs maybe_foreign_items '}' {  }
	;*/

/*maybe_abi : str
	| %empty {  }
	;*/

/*maybe_foreign_items : foreign_items
	| %empty {  }
	;*/

/*foreign_items : foreign_item               {  }
	| foreign_items foreign_item {  }
	;*/

/*foreign_item : attrs_and_vis STATIC item_foreign_static {  }
	| attrs_and_vis item_foreign_fn            {  }
	| attrs_and_vis UNSAFE item_foreign_fn     {  }
	;*/

/*item_foreign_static
	: maybe_mut ident ':' ty ';'               {  }
	;*/

/*item_foreign_fn
	: FN ident generic_params fn_decl_allow_variadic maybe_where_clause ';'
	  {  }
	;*/

/*fn_decl_allow_variadic : fn_params_allow_variadic ret_ty {  }
	;*/

/*fn_params_allow_variadic : '(' ')'     {  }
	| '(' params ')'               {  }
	| '(' params ',' ')'           {  }
	| '(' params ',' DOTDOTDOT ')' {  }
	;*/

visibility : //PUB      {  }| 
	%empty   { $$ = NULL; }
	;

/*idents_or_self : ident_or_self                    {  }
	//| idents_or_self AS ident          {  }
	| idents_or_self ',' ident_or_self {  }
	;*/

/*ident_or_self : ident
	| SELF  {  }
	;*/

/*item_type : TYPE ident generic_params maybe_where_clause '=' ty_sum ';'  {  }
	;*/

/*for_sized : FOR '?' ident {  }
	| FOR ident '?' {  }
	| %empty        {  }
	;*/

/*item_trait : maybe_unsafe TRAIT ident generic_params for_sized
	     maybe_ty_param_bounds maybe_where_clause '{' maybe_trait_items '}' {  }
	;*/

/*maybe_trait_items : trait_items
	| %empty {  }
	;*/

/*trait_items : trait_item               {  }
	| trait_items trait_item       {  }
	;*/

/*trait_item : //trait_const
	//| trait_type
	//| trait_method| 
	maybe_outer_attrs item_macro {  }
	;*/

/*trait_const : maybe_outer_attrs CONST ident maybe_ty_ascription
	      maybe_const_default ';' {  }
	;*/

/*maybe_const_default : '=' expr {  }
	| %empty   {  }
	;*/

/*trait_type : maybe_outer_attrs TYPE ty_param ';' {  }
	;*/

/*maybe_unsafe : UNSAFE {  }
	| %empty {  }
	;*/

/*maybe_default_maybe_unsafe : DEFAULT UNSAFE {  }
	| DEFAULT        {  }
	|         UNSAFE {  }
	| %empty {  }
	;*/

/*trait_method : type_method {  }
	| method      {  }
	;*/

/*type_method : maybe_outer_attrs maybe_unsafe FN ident generic_params
	      fn_decl_with_self_allow_anon_params maybe_where_clause ';' {  }
	| maybe_outer_attrs CONST maybe_unsafe FN ident generic_params
	  fn_decl_with_self_allow_anon_params maybe_where_clause ';' {  }
	| maybe_outer_attrs maybe_unsafe EXTERN maybe_abi FN ident
	  generic_params fn_decl_with_self_allow_anon_params
	   maybe_where_clause ';' {  }
	;*/

/*method : maybe_outer_attrs maybe_unsafe FN ident generic_params
         fn_decl_with_self_allow_anon_params maybe_where_clause
	  inner_attrs_and_block {  }
	| maybe_outer_attrs CONST maybe_unsafe FN ident generic_params
	  fn_decl_with_self_allow_anon_params maybe_where_clause
	  inner_attrs_and_block {  }
	| maybe_outer_attrs maybe_unsafe EXTERN maybe_abi FN ident
	  generic_params fn_decl_with_self_allow_anon_params
	  maybe_where_clause inner_attrs_and_block {  }
	;*/

/*impl_method : attrs_and_vis maybe_default maybe_unsafe FN ident generic_params
	      fn_decl_with_self maybe_where_clause inner_attrs_and_block {  }
	| attrs_and_vis maybe_default CONST maybe_unsafe FN ident
	  generic_params fn_decl_with_self maybe_where_clause
	  inner_attrs_and_block {  }
	| attrs_and_vis maybe_default maybe_unsafe EXTERN maybe_abi FN ident
	  generic_params fn_decl_with_self maybe_where_clause
	  inner_attrs_and_block {  }
	;*/

// There are two forms of impl:
//
// impl (<...>)? TY { ... }
// impl (<...>)? TRAIT for TY { ... }
//
// Unfortunately since TY can begin with '<' itself -- as part of a
// TyQualifiedPath type -- there's an s/r conflict when we see '<' after IMPL:
// should we reduce one of the early rules of TY (such as maybe_once)
// or shall we continue shifting into the generic_params list for the
// impl?
//
// The production parser disambiguates a different case here by
// permitting / requiring the user to provide parens around types when
// they are ambiguous with traits. We do the same here, regrettably,
// by splitting ty into ty and ty_prim.
/*item_impl : maybe_default_maybe_unsafe IMPL generic_params ty_prim_sum
	     maybe_where_clause '{' maybe_inner_attrs maybe_impl_items '}' {  }
	| maybe_default_maybe_unsafe IMPL generic_params '(' ty ')'
	  maybe_where_clause '{' maybe_inner_attrs maybe_impl_items '}' {  }
	| maybe_default_maybe_unsafe IMPL generic_params trait_ref FOR ty_sum maybe_where_clause '{' maybe_inner_attrs maybe_impl_items '}' {  }
	| maybe_default_maybe_unsafe IMPL generic_params '!' trait_ref FOR
	   ty_sum maybe_where_clause '{' maybe_inner_attrs maybe_impl_items '}' {  }
	| maybe_default_maybe_unsafe IMPL generic_params trait_ref FOR DOTDOT
	  '{' '}' {  }
	| maybe_default_maybe_unsafe IMPL generic_params '!' trait_ref FOR
	  DOTDOT '{' '}' {  }
	;*/

/*maybe_impl_items : impl_items
	| %empty {  }
	;*/

/*impl_items : impl_item               {  }
	| impl_item impl_items    {  }
	;*/

/*impl_item : impl_method
	| attrs_and_vis item_macro {  }
	| impl_const
	| impl_type
	;*/

/*maybe_default : DEFAULT {  }
	| %empty {  }
	;*/

/*impl_const : attrs_and_vis maybe_default item_const {  }
	;*/

/*impl_type : attrs_and_vis maybe_default TYPE ident generic_params
	    '=' ty_sum ';'  {  }
	;*/

item_fn : FN ident generic_params fn_decl maybe_where_clause
	  inner_attrs_and_block { $$=alctree(YYSYMBOL_item_fn, "item_fn", 6, $1, $2, $3, $4, $5, $6);}
	| CONST FN ident generic_params fn_decl maybe_where_clause
	   inner_attrs_and_block { $$=alctree(YYSYMBOL_item_fn, "item_fn", 7, $1, $2, $3, $4, $5, $6, $7); }
	;

/*item_unsafe_fn : UNSAFE FN ident generic_params fn_decl maybe_where_clause
	         inner_attrs_and_block {  }
	| CONST UNSAFE FN ident generic_params fn_decl maybe_where_clause
	  inner_attrs_and_block {  }
	| UNSAFE EXTERN maybe_abi FN ident generic_params fn_decl
	   maybe_where_clause inner_attrs_and_block {  }
	;*/

fn_decl : fn_params ret_ty   { $$=alctree(YYSYMBOL_fn_decl, "fn_decl", 2, $1, $2); }
	;

/*fn_decl_with_self : fn_params_with_self ret_ty   {  }
	;*/

/*fn_decl_with_self_allow_anon_params : fn_anon_params_with_self ret_ty   {  }
	;*/

fn_params : '(' maybe_params ')'  { $$=alctree(YYSYMBOL_fn_params, "fn_params", 3, $1, $2, $3); }
	;

fn_anon_params : '(' anon_param anon_params_allow_variadic_tail ')' { $$=alctree(YYSYMBOL_fn_anon_params, "fn_anon_params", 4, $1, $2, $3, $4); }
	| '(' ')'                                            { $$=alctree(YYSYMBOL_fn_anon_params, "fn_anon_params", 2, $1, $2); }
	;

/*fn_params_with_self : '(' maybe_mut SELF maybe_ty_ascription
		       maybe_comma_params ')'              {  }
	| '(' '&' maybe_mut SELF maybe_ty_ascription maybe_comma_params ')' {
	  }
	| '(' '&' lifetime maybe_mut SELF maybe_ty_ascription
	   maybe_comma_params ')' {  }
	| '(' maybe_params ')'    {  }
	;*/

/*fn_anon_params_with_self : '(' maybe_mut SELF maybe_ty_ascription
	maybe_comma_anon_params ')'              {  }
	| '(' '&' maybe_mut SELF maybe_ty_ascription maybe_comma_anon_params
	  ')'          {  }
	| '(' '&' lifetime maybe_mut SELF maybe_ty_ascription
	  maybe_comma_anon_params ')' {  } | 
	  '(' maybe_anon_params ')'   {  }
	;*/

maybe_params : params	{$$ = $1;}
	| params ','	{ $$=alctree(YYSYMBOL_maybe_params, "maybe_params", 2, $1, $2); }
	| %empty  { $$ = NULL; }
	;

params  : param                { $$=$1; }
	| params ',' param     { $$=alctree(YYSYMBOL_params, "params", 3, $1, $2, $3); }
	;

param   : pat ':' ty_sum   { $$=alctree(YYSYMBOL_param, "param", 3, $1, $2, $3); }
	;

inferrable_params : inferrable_param                       { $$=$1; }
	| inferrable_params ',' inferrable_param { $$=alctree(YYSYMBOL_inferrable_params, "inferrable_params", 2, $1, $2, $3); }
	;

inferrable_param : pat maybe_ty_ascription { $$=alctree(YYSYMBOL_inferrable_param, "inferrable_param", 2, $1, $2); }
	;

/*maybe_comma_params : ','            {  }
	| ',' params     {  }
	| ',' params ',' {  }
	| %empty         {  }
	;*/

/*maybe_comma_anon_params : ','                 {  }
	| ',' anon_params     {  }
	| ',' anon_params ',' {  }
	| %empty              {  }
	;*/

/*maybe_anon_params : anon_params
	| anon_params ','
	| %empty      {  }
	;*/

anon_params : anon_param                 { $$=$1; }
	| anon_params ',' anon_param { $$=alctree(YYSYMBOL_anon_params, "anon_params", 3, $1, $2, $3); }
	;

// anon means it's allowed to be anonymous (type-only), but it can
// still have a name
anon_param : named_arg ':' ty   { $$=alctree(YYSYMBOL_anon_param, "anon_param", 3, $1, $2, $3); }
	| ty
	;

anon_params_allow_variadic_tail : ',' DOTDOTDOT          { $$=alctree(YYSYMBOL_anon_params_allow_variadic_tail, "anon_params_allow_variadic_tail", 2, $1, $2); }
	| ',' anon_param anon_params_allow_variadic_tail { $$=alctree(YYSYMBOL_anon_params_allow_variadic_tail, "anon_params_allow_variadic_tail", 3, $1, $2, $3); }
	| %empty                                         { $$ = NULL; }
	;

named_arg : ident		{ $$=$1; }
	| UNDERSCORE        { $$=$1; }
	| '&' ident         { $$=alctree(YYSYMBOL_named_arg, "named_arg", 2, $1, $2); }
	| '&' UNDERSCORE    { $$=alctree(YYSYMBOL_named_arg, "named_arg", 2, $1, $2); }
	| ANDAND ident      { $$=alctree(YYSYMBOL_named_arg, "named_arg", 2, $1, $2); }
	| ANDAND UNDERSCORE { $$=alctree(YYSYMBOL_named_arg, "named_arg", 2, $1, $2); }
	| MUT ident         { $$=alctree(YYSYMBOL_named_arg, "named_arg", 2, $1, $2); }
	;

ret_ty : RARROW '!'         { $$=alctree(YYSYMBOL_ret_ty, "ret_ty", 2, $1, $2); }
	| RARROW ty          { $$=alctree(YYSYMBOL_ret_ty, "ret_ty", 2, $1, $2); }
	| %prec IDENT %empty { $$ = NULL; }
	;

generic_params : '<' '>'                             { $$=alctree(YYSYMBOL_generic_params, "generic_params", 2, $1, $2); }
	| '<' lifetimes '>'                   { $$=alctree(YYSYMBOL_generic_params, "generic_params", 3, $1, $2, $3); }
	| '<' lifetimes ',' '>'               { $$=alctree(YYSYMBOL_generic_params, "generic_params", 4, $1, $2, $3, $4); }
	| '<' lifetimes SHR                   { $$=alctree(YYSYMBOL_generic_params, "generic_params", 3, $1, $2, $3); }
	| '<' lifetimes ',' SHR               { $$=alctree(YYSYMBOL_generic_params, "generic_params", 4, $1, $2, $3, $4); }
	| '<' lifetimes ',' ty_params '>'     { $$=alctree(YYSYMBOL_generic_params, "generic_params", 5, $1, $2, $3, $4, $5); }
	| '<' lifetimes ',' ty_params ',' '>' { $$=alctree(YYSYMBOL_generic_params, "generic_params", 6, $1, $2, $3, $4, $5, $6); }
	| '<' lifetimes ',' ty_params SHR     { $$=alctree(YYSYMBOL_generic_params, "generic_params", 5, $1, $2, $3, $4, $5); }
	| '<' lifetimes ',' ty_params ',' SHR { $$=alctree(YYSYMBOL_generic_params, "generic_params", 6, $1, $2, $3, $4, $5, $6); }
	| '<' ty_params '>'                   { $$=alctree(YYSYMBOL_generic_params, "generic_params", 3, $1, $2, $3); }
	| '<' ty_params ',' '>'               { $$=alctree(YYSYMBOL_generic_params, "generic_params", 4, $1, $2, $3, $4); }
	| '<' ty_params SHR                   { $$=alctree(YYSYMBOL_generic_params, "generic_params", 3, $1, $2, $3); }
	| '<' ty_params ',' SHR               { $$=alctree(YYSYMBOL_generic_params, "generic_params", 4, $1, $2, $3, $4); }
	| %empty                              { $$ = NULL; }
	;

maybe_where_clause : %empty                              { $$ = NULL; }
	//| where_clause
	;

/*where_clause : WHERE where_predicates              {  }
	| WHERE where_predicates ','          {  }
	;*/

/*where_predicates : where_predicate                      {  }
	| where_predicates ',' where_predicate {  }
	;*/

/*where_predicate : maybe_for_lifetimes lifetime ':' bounds    {  }
	| maybe_for_lifetimes ty ':' ty_param_bounds {  }
	;*/

/*maybe_for_lifetimes : FOR '<' lifetimes '>' {  }
	| %prec FORTYPE %empty  {  }
	;*/

ty_params : ty_param               { $$=$1; }
	| ty_params ',' ty_param { $$=alctree(YYSYMBOL_ty_params, "ty_params", 3, $1, $2, $3); }
	;

// A path with no type parameters; e.g. `foo::bar::Baz`
//
// These show up in 'use' view-items, because these are processed
// without respect to types.
/*path_no_types_allowed : ident                               {  }
	| MOD_SEP ident                       {  }
	| SELF                                {  }
	| MOD_SEP SELF                        {  }
	| SUPER                               {  }
	| MOD_SEP SUPER                       {  }
	| path_no_types_allowed MOD_SEP ident {  }
	;*/

// A path with a lifetime and type parameters, with no double colons
// before the type parameters; e.g. `foo::bar<'a>::Baz<T>`
//
// These show up in "trait references", the components of
// type-parameter bounds lists, as well as in the prefix of the
// path_generic_args_and_bounds rule, which is the full form of a
// named typed expression.
//
// They do not have (nor need) an extra '::' before '<' because
// unlike in expr context, there are no "less-than" type exprs to
// be ambiguous with.
path_generic_args_without_colons : %prec IDENT ident { $$=$1; }
	| %prec IDENT  ident generic_args { $$=alctree(YYSYMBOL_path_generic_args_without_colons, "path_generic_args_without_colons", 2, $1, $2); }
	| %prec IDENT ident '(' maybe_ty_sums ')' ret_ty { $$=alctree(YYSYMBOL_path_generic_args_without_colons, "path_generic_args_without_colons", 5, $1, $2, $3, $4, $5); }
	| %prec IDENT path_generic_args_without_colons MOD_SEP ident { $$=alctree(YYSYMBOL_path_generic_args_without_colons, "path_generic_args_without_colons", 3, $1, $2, $3); }
	| %prec IDENT path_generic_args_without_colons MOD_SEP ident
	  generic_args                 { $$=alctree(YYSYMBOL_path_generic_args_without_colons, "path_generic_args_without_colons", 4, $1, $2, $3, $4); }
	| %prec IDENT path_generic_args_without_colons MOD_SEP ident
	  '(' maybe_ty_sums ')' ret_ty { $$=alctree(YYSYMBOL_path_generic_args_without_colons, "path_generic_args_without_colons", 7, $1, $2, $3, $4, $5, $6, $7); }
	;

generic_args : '<' generic_values '>'   { $$=alctree(YYSYMBOL_generic_args, "generic_args", 3, $1, $2, $3); }
	| '<' generic_values SHR   { $$=alctree(YYSYMBOL_generic_args, "generic_args", 3, $1, $2, $3); }
	| '<' generic_values GE    { $$=alctree(YYSYMBOL_generic_args, "generic_args", 3, $1, $2, $3); }
	| '<' generic_values SHREQ { $$=alctree(YYSYMBOL_generic_args, "generic_args", 3, $1, $2, $3); }
// If generic_args starts with "<<", the first arg must be a
// TyQualifiedPath because that's the only type that can start with a
// '<'. This rule parses that as the first ty_sum and then continues
// with the rest of generic_values.
	//| SHL ty_qualified_path_and_generic_values '>'   {  }
	//| SHL ty_qualified_path_and_generic_values SHR   {  }
	//| SHL ty_qualified_path_and_generic_values GE    {  }
	//| SHL ty_qualified_path_and_generic_values SHREQ {  }
	;

generic_values : maybe_ty_sums_and_or_bindings { $$=$1; }
	;

maybe_ty_sums_and_or_bindings : ty_sums		{ $$=$1; }
	| ty_sums ','			{ $$=alctree(YYSYMBOL_maybe_ty_sums_and_or_bindings, "maybe_ty_sums_and_or_bindings", 2, $1, $2); }
	| ty_sums ',' bindings { $$=alctree(YYSYMBOL_maybe_ty_sums_and_or_bindings, "maybe_ty_sums_and_or_bindings", 3, $1, $2, $3); }
	| bindings				{ $$=$1; }
	| bindings ','			{ $$=alctree(YYSYMBOL_maybe_ty_sums_and_or_bindings, "maybe_ty_sums_and_or_bindings", 2, $1, $2); }
	| %empty               { $$ = NULL; }
	;

/*maybe_bindings : ',' bindings {  }
	| %empty       {  }
	;*/

////////////////////////////////////////////////////////////////////////
// Part 2: Patterns
////////////////////////////////////////////////////////////////////////

pat : UNDERSCORE                                      { $$=$1; }
	| '&' pat                                         { $$=alctree(YYSYMBOL_pat, "pat", 2, $1, $2); }
	| '&' MUT pat                                     { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| ANDAND pat                                      { $$=alctree(YYSYMBOL_pat, "pat", 2, $1, $2); }
	| '(' ')'                                         { $$=alctree(YYSYMBOL_pat, "pat", 2, $1, $2); }
	| '(' pat_tup ')'                                 { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| '[' pat_vec ']'                                 { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| lit_or_path									  { $$=$1; }
	| lit_or_path DOTDOTDOT lit_or_path               { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| path_expr '{' pat_struct '}'                    { $$=alctree(YYSYMBOL_pat, "pat", 4, $1, $2, $3, $4); }
	| path_expr '(' ')'                               { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| path_expr '(' pat_tup ')'                       { $$=alctree(YYSYMBOL_pat, "pat", 4, $1, $2, $3, $4); }
	| path_expr '!' maybe_ident delimited_token_trees { $$=alctree(YYSYMBOL_pat, "pat", 4, $1, $2, $3, $4); }
	| binding_mode ident                              { $$=alctree(YYSYMBOL_pat, "pat", 2, $1, $2); }
	|              ident '@' pat                      { $$=alctree(YYSYMBOL_pat, "pat", 3, $1, $2, $3); }
	| binding_mode ident '@' pat                      { $$=alctree(YYSYMBOL_pat, "pat", 4, $1, $2, $3, $4); }
	//| BOX pat                                         {  }
	| '<' ty_sum maybe_as_trait_ref '>' MOD_SEP ident { $$=alctree(YYSYMBOL_pat, "pat", 6, $1, $2, $3, $4, $5, $6); }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	   maybe_as_trait_ref '>' MOD_SEP ident {
		$$=alctree(YYSYMBOL_pat, "pat", 10, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10);
	  }
	;

/*pats_or : pat              {  }
	| pats_or '|' pat  {  }
	;*/

binding_mode : REF         { $$=$1; }
	| REF MUT     { $$=alctree(YYSYMBOL_binding_mode, "binding_mode", 2, $1, $2); }
	| MUT         { $$=$1; }
	;

lit_or_path : path_expr    { $$=$1; }
	| lit          { $$=$1; }
	| '-' lit      { $$=alctree(YYSYMBOL_lit_or_path, "lit_or_path", 2, $1, $2); }
	;

pat_field :                  ident        { $$=$1; }
	|     binding_mode ident        { $$=alctree(YYSYMBOL_pat_field, "pat_field", 2, $1, $2); }
	//| BOX              ident        {  }
	//| BOX binding_mode ident        {  }
	|              ident ':' pat    { $$=alctree(YYSYMBOL_pat_field, "pat_field", 3, $1, $2, $3); }
	| binding_mode ident ':' pat    { $$=alctree(YYSYMBOL_pat_field, "pat_field", 3, $1, $2, $3, $4); }
	|        LIT_INTEGER ':' pat    { $$=alctree(YYSYMBOL_pat_field, "pat_field", 3, $1, $2, $3); }
	;

pat_fields : pat_field                  { $$=$1; }
	| pat_fields ',' pat_field   { $$=alctree(YYSYMBOL_pat_fields, "pat_fields", 3, $1, $2, $3); }
	;

pat_struct : pat_fields                 { $$=$1; }
	| pat_fields ','             { $$=alctree(YYSYMBOL_pat_struct, "pat_struct", 2, $1, $2); }
	| pat_fields ',' DOTDOT      { $$=alctree(YYSYMBOL_pat_struct, "pat_struct", 3, $1, $2, $3); }
	| DOTDOT                     { $$=$1; }
	| %empty                     { $$ = NULL; }
	;

pat_tup : pat_tup_elts                                  { $$=$1; }
	| pat_tup_elts                             ','  { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 2, $1, 2); }
	| pat_tup_elts     DOTDOT                       { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 2, $1, 2); }
	| pat_tup_elts ',' DOTDOT                       { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 3, $1, $2, $3); }
	| pat_tup_elts     DOTDOT ',' pat_tup_elts      { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 4, $1, $2, $3, $4); }
	| pat_tup_elts     DOTDOT ',' pat_tup_elts ','  { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 5, $1, $2, $3, $4, $5); }
	| pat_tup_elts ',' DOTDOT ',' pat_tup_elts      { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 5, $1, $2, $3, $4, $5); }
	| pat_tup_elts ',' DOTDOT ',' pat_tup_elts ','  { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 6, $1, $2, $3, $4, $5, $6); }
	|                  DOTDOT ',' pat_tup_elts      { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 3, $1, $2, $3); }
	|                  DOTDOT ',' pat_tup_elts ','  { $$=alctree(YYSYMBOL_pat_tup, "pat_tup", 4, $1, $2, $3, $4); }
	|                  DOTDOT                       { $$=$1; }
	;

pat_tup_elts : pat                    { $$=$1; }
	| pat_tup_elts ',' pat        { $$=alctree(YYSYMBOL_pat_tup_elts, "pat_tup_elts", 3, $1, $2, $3); }
	;

pat_vec : pat_vec_elts                                  { $$=$1; }
	| pat_vec_elts                             ','  { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 2, $1, 2); }
	| pat_vec_elts     DOTDOT                       { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 2, $1, 2); }
	| pat_vec_elts ',' DOTDOT                       { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 3, $1, $2, $3); }
	| pat_vec_elts     DOTDOT ',' pat_vec_elts      { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 4, $1, $2, $3, $4); }
	| pat_vec_elts     DOTDOT ',' pat_vec_elts ','  { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 5, $1, $2, $3, $4, $5); }
	| pat_vec_elts ',' DOTDOT ',' pat_vec_elts      { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 5, $1, $2, $3, $4, $5); }
	| pat_vec_elts ',' DOTDOT ',' pat_vec_elts ','  { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 6, $1, $2, $3, $4, $5, $6); }
	|                  DOTDOT ',' pat_vec_elts      { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 3, $1, $2, $3); }
	|                  DOTDOT ',' pat_vec_elts ','  { $$=alctree(YYSYMBOL_pat_vec, "pat_vec", 4, $1, $2, $3, $4); }
	|                  DOTDOT                       { $$=$1; }
	| %empty                                        { $$ = NULL; }
	;

pat_vec_elts : pat                    { $$=$1; }
	| pat_vec_elts ',' pat   { $$=alctree(YYSYMBOL_pat_vec_elts, "pat_vec_elts", 3, $1, $2, $3); }
	;

////////////////////////////////////////////////////////////////////////
// Part 3: Types
////////////////////////////////////////////////////////////////////////

ty : ty_prim
	| ty_closure
	| '<' ty_sum maybe_as_trait_ref '>' MOD_SEP ident { $$=alctree(YYSYMBOL_ty, "ty", 6, $1, $2, $3, $4, $5, $6); }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	  maybe_as_trait_ref '>' MOD_SEP ident				{ $$=alctree(YYSYMBOL_ty, "ty", 10, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10); }
	| '(' ty_sums ')'                                 { $$=alctree(YYSYMBOL_ty, "ty", 3, $1, $2, $3); }
	| '(' ty_sums ',' ')'                             { $$=alctree(YYSYMBOL_ty, "ty", 4, $1, $2, $3, $4); }
	| '(' ')'                                         { $$=alctree(YYSYMBOL_ty, "ty", 2, $1, $2); }
	;

ty_prim : %prec IDENT path_generic_args_without_colons    { $$=$1; }
	| %prec IDENT MOD_SEP path_generic_args_without_colons { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 2, $1, $2); }
	//| %prec IDENT SELF MOD_SEP path_generic_args_without_colons {  }
	| %prec IDENT path_generic_args_without_colons '!' maybe_ident
	  delimited_token_trees         { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 4, $1, $2, $3, $4); }
	| %prec IDENT MOD_SEP path_generic_args_without_colons '!'
	  maybe_ident delimited_token_trees { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 3, $2, $4, $5); }
	//| BOX ty                                                    {  }
	| '*' maybe_mut_or_const ty                                 { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 3, $1, $2, $3); }
	| '&' ty                                                    { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 2, $1, $2); }
	| '&' MUT ty                                                { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 3, $1, $2, $3); }
	| ANDAND ty                                                 { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 2, $1, $2); }
	| ANDAND MUT ty                                             { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 3, $1, $2, $3); }
	| '&' lifetime maybe_mut ty                                 { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 4, $1, $2, $3, $4); }
	| ANDAND lifetime maybe_mut ty                              { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 4, $1, $2, $3, $4); }
	| '[' ty ']'                                                { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 3, $1, $2, $3); }
	| '[' ty ',' DOTDOT expr ']'                                { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 5, $1, $2, $3, $4, $5); }
	| '[' ty ';' expr ']'                                       { $$=alctree(YYSYMBOL_ty_prim, "ty_prim", 5, $1, $2, $3, $4, $5); }
	//| TYPEOF '(' expr ')'                                       {  }
	| UNDERSCORE                                                { $$ = $1; }
	| ty_bare_fn												{ $$=$1; }
	| for_in_type												{ $$=$1; }
	;

ty_bare_fn :                      FN ty_fn_decl { $$=alctree(YYSYMBOL_ty_bare_fn, "ty_bare_fn", 2, $1, $2); }
	//| UNSAFE                  FN ty_fn_decl {  }
	//|        EXTERN maybe_abi FN ty_fn_decl {  }
	//| UNSAFE EXTERN maybe_abi FN ty_fn_decl {  }
	;

ty_fn_decl : generic_params fn_anon_params ret_ty { $$=alctree(YYSYMBOL_ty_fn_decl, "ty_fn_decl", 3, $1, $2, $3); }
	;

ty_closure : //UNSAFE '|' anon_params '|' maybe_bounds ret_ty {  }|
           '|' anon_params '|' maybe_bounds ret_ty { $$=alctree(YYSYMBOL_ty_closure, "ty_closure", 5, $1, $2, $3, $4, $5); }
	//|    UNSAFE OROR maybe_bounds ret_ty                {  }
	|           OROR maybe_bounds ret_ty                { $$=alctree(YYSYMBOL_ty_closure, "ty_closure", 3, $1, $2, $3); }
	;

for_in_type : FOR '<' maybe_lifetimes '>' for_in_type_suffix { $$=alctree(YYSYMBOL_for_in_type, "for_in_type", 5, $1, $2, $3, $4, $5); }
	;

for_in_type_suffix : ty_bare_fn		{ $$=$1; }
	| trait_ref						{ $$=$1; }
	| ty_closure					{ $$=$1; }
	;

maybe_mut : MUT              { $$ = $1; }
	| %prec MUT %empty { $$ = NULL; }
	;

maybe_mut_or_const : MUT    { $$ = $1; }
	| CONST  { $$ = $1; }
	| %empty { $$ = NULL; }
	;

/*ty_qualified_path_and_generic_values : ty_qualified_path maybe_bindings {  }
	| ty_qualified_path ',' ty_sums maybe_bindings {  }
	;*/

/*ty_qualified_path : ty_sum AS trait_ref '>' MOD_SEP ident           {  }
	| ty_sum AS trait_ref '>' MOD_SEP ident '+' ty_param_bounds {  }
	;*/

maybe_ty_sums : ty_sums		{ $$=$1; }
	| ty_sums ','			{ $$=alctree(YYSYMBOL_maybe_ty_sums, "maybe_ty_sums", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

ty_sums : ty_sum             { $$ = $1; }
	| ty_sums ',' ty_sum { $$=alctree(YYSYMBOL_ty_sums, "ty_sums", 3, $1, $2, $3); }
	;

ty_sum : ty_sum_elt            { $$ = $1; }
	| ty_sum '+' ty_sum_elt { $$=alctree(YYSYMBOL_ty_sum, "ty_sum", 3, $1, $2, $3); }
	;

ty_sum_elt : ty		{ $$=$1; }
	| lifetime		{ $$=$1; }
	;

/*ty_prim_sum : ty_prim_sum_elt                 {  }
	| ty_prim_sum '+' ty_prim_sum_elt {  }
	;*/

/*ty_prim_sum_elt : ty_prim
	| lifetime
	;*/

maybe_ty_param_bounds : ':' ty_param_bounds { $$=alctree(YYSYMBOL_maybe_ty_param_bounds, "maybe_ty_param_bounds", 2, $1, $2); }
	| %empty              { $$ = NULL; }
	;

ty_param_bounds : boundseq		{ $$=$1; }
	| %empty { $$ = NULL; }
	;

boundseq : polybound		{ $$=$1; }
	| boundseq '+' polybound { $$=alctree(YYSYMBOL_boundseq, "boundseq", 3, $1, $2, $3); }
	;

polybound : FOR '<' maybe_lifetimes '>' bound { $$=alctree(YYSYMBOL_polybound, "polybound", 5, $1, $2, $3, $4, $5); }
	| bound
	| '?' FOR '<' maybe_lifetimes '>' bound { $$=alctree(YYSYMBOL_polybound, "polybound", 6, $1, $2, $3, $4, $5, $6); }
	| '?' bound { $$=alctree(YYSYMBOL_polybound, "polybound", 2, $1, $2); }
	;

bindings : binding              { $$=$1; }
	| bindings ',' binding { $$=alctree(YYSYMBOL_bindings, "bindings", 3, $1, $2, $3); }
	;

binding : ident '=' ty { $$=alctree(YYSYMBOL_binding, "binding", 3, $1, $2, $3); }
	;

ty_param : ident maybe_ty_param_bounds maybe_ty_default           { $$=alctree(YYSYMBOL_ty_param, "ty_param", 3, $1, $2, $3); }
	| ident '?' ident maybe_ty_param_bounds maybe_ty_default { $$=alctree(YYSYMBOL_ty_param, "ty_param", 5, $1, $2, $3, $4, $5); }
	;

maybe_bounds : %prec SHIFTPLUS ':' bounds             { $$=alctree(YYSYMBOL_maybe_bounds, "maybe_bounds", 2, $1, $2); }
	| %prec SHIFTPLUS %empty { $$ = NULL; }
	;

bounds : bound            { $$=$1; }
	| bounds '+' bound { $$=alctree(YYSYMBOL_bounds, "bounds", 3, $1, $2, $3); }
	;

bound : lifetime		{ $$=$1; }
	| trait_ref			{ $$=$1; }
	;

maybe_ltbounds : %prec SHIFTPLUS ':' ltbounds       { $$=alctree(YYSYMBOL_maybe_ltbounds, "maybe_ltbounds", 2, $1, $2); }
	| %empty             { $$ = NULL; }
	;

ltbounds : lifetime              { $$=$1; }
	| ltbounds '+' lifetime { $$=alctree(YYSYMBOL_ltbounds, "ltbounds", 3, $1, $2, $3); }
	;

maybe_ty_default : '=' ty_sum { $$=alctree(YYSYMBOL_maybe_ty_default, "maybe_ty_default", 2, $1, $2); }
	| %empty     { $$ = NULL; }
	;

maybe_lifetimes : lifetimes		{ $$=$1; }
	| lifetimes ','				{ $$=alctree(YYSYMBOL_maybe_lifetimes, "maybe_lifetimes", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

lifetimes : lifetime_and_bounds               { $$=$1; }
	| lifetimes ',' lifetime_and_bounds { $$=alctree(YYSYMBOL_lifetimes, "lifetimes", 3, $1, $2, $3); }
	;

lifetime_and_bounds : LIFETIME maybe_ltbounds         { $$=alctree(YYSYMBOL_lifetime_and_bounds, "lifetime_and_bounds", 2, $1, $2);  }
	| STATIC_LIFETIME                 { $$=$1; }
	;

lifetime : LIFETIME         { $$=$1; }
	| STATIC_LIFETIME  { $$=$1; }
	;

trait_ref : %prec IDENT path_generic_args_without_colons
	| %prec IDENT MOD_SEP path_generic_args_without_colons { $$=alctree(YYSYMBOL_trait_ref, "trait_ref", 2, $1, $2); }
	;

////////////////////////////////////////////////////////////////////////
// Part 4: Blocks, statements, and expressions
////////////////////////////////////////////////////////////////////////

inner_attrs_and_block : '{' maybe_inner_attrs maybe_stmts '}'        { $$=alctree(YYSYMBOL_inner_attrs_and_block, "inner_attrs_and_block", 4, $1, $2, $3, $4); }
	;

block : '{' maybe_stmts '}'                          { $$=alctree(YYSYMBOL_block, "block", 3, $1, $2, $3); }
	;

maybe_stmts : stmts			{ $$=$1; }
	| stmts nonblock_expr { $$=alctree(YYSYMBOL_maybe_stmts, "maybe_st$$=$1;mts", 2, $1, $2); }
	| nonblock_expr			{ $$=$1; }
	| %empty              { $$ = NULL; }
	;

// There are two sub-grammars within a "stmts: exprs" derivation
// depending on whether each stmt-expr is a block-expr form; this is to
// handle the "semicolon rule" for stmt sequencing that permits
// writing
//
//     if foo { bar } 10
//
// as a sequence of two stmts (one if-expr stmt, one lit-10-expr
// stmt). Unfortunately by permitting juxtaposition of exprs in
// sequence like that, the non-block expr grammar has to have a
// second limited sub-grammar that excludes the prefix exprs that
// are ambiguous with binops. That is to say:
//
//     {10} - 1
//
// should parse as (progn (progn 10) (- 1)) not (- (progn 10) 1), that
// is to say, two statements rather than one, at least according to
// the mainline rust parser.
//
// So we wind up with a 3-way split in exprs that occur in stmt lists:
// block, nonblock-prefix, and nonblock-nonprefix.
//
// In non-stmts contexts, expr can relax this trichotomy.

stmts : stmt           { $$=$1; }
	| stmts stmt     { $$=alctree(YYSYMBOL_stmts, "stmts", 2, $1, $2); }
	;

stmt : maybe_outer_attrs let     { $$=alctree(YYSYMBOL_stmt, "stmt", 2, $1, $2); }
	|                 stmt_item		{ $$=$1; }
	//|             PUB stmt_item {  }
	| outer_attrs     stmt_item { $$=alctree(YYSYMBOL_stmt, "stmt", 2, $1, $2); }
	//| outer_attrs PUB stmt_item {  }
	| full_block_expr			{ $$=$1; }
	| maybe_outer_attrs block   { $$=alctree(YYSYMBOL_stmt, "stmt", 2, $1, $2); }
	|             nonblock_expr ';'	{ $$=alctree(YYSYMBOL_stmt, "stmt", 2, $1, $2); }
	| outer_attrs nonblock_expr ';' { $$=alctree(YYSYMBOL_stmt, "stmt", 3, $1, $2, $3); }
	| ';'                   { $$=$1; }
	;

maybe_exprs : exprs		{ $$=$1; }
	| exprs ','			{ $$=alctree(YYSYMBOL_maybe_exprs, "maybe_exprs", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

maybe_expr : expr		{ $$=$1; }
	| %empty { $$ = NULL; }
	;

exprs : expr                                         { $$=$1; }
	| exprs ',' expr                             { $$=alctree(YYSYMBOL_exprs, "exprs", 3, $1, $2, $3); }
	;

path_expr : path_generic_args_with_colons			{ $$=$1; }
	| MOD_SEP path_generic_args_with_colons      { $$=alctree(YYSYMBOL_path_expr, "path_expr", 2, $1, $2); }
	//| SELF MOD_SEP path_generic_args_with_colons {  }
	;

// A path with a lifetime and type parameters with double colons before
// the type parameters; e.g. `foo::bar::<'a>::Baz::<T>`
//
// These show up in expr context, in order to disambiguate from "less-than"
// expressions.
path_generic_args_with_colons : ident                        { $$=$1; }
	//| SUPER                                              {  }
	| path_generic_args_with_colons MOD_SEP ident        { $$=alctree(YYSYMBOL_path_generic_args_with_colons, "path_generic_args_with_colons", 3, $1, $2, $3); }
	//| path_generic_args_with_colons MOD_SEP SUPER        {  }
	| path_generic_args_with_colons MOD_SEP generic_args { $$=alctree(YYSYMBOL_path_generic_args_with_colons, "path_generic_args_with_colons", 3, $1, $2, $3); }
	;

// the braces-delimited macro is a block_expr so it doesn't appear here
macro_expr : path_expr '!' maybe_ident parens_delimited_token_trees   { $$=alctree(YYSYMBOL_macro_expr, "macro_expr", 4, $1, $2, $3, $4); }
	| path_expr '!' maybe_ident brackets_delimited_token_trees { $$=alctree(YYSYMBOL_macro_expr, "macro_expr", 4, $1, $2, $3, $4); }
	;

nonblock_expr : lit                                                     { $$=$1; }
	| %prec IDENT path_expr                                         { $$=$1; }
	//| SELF                                                          {  }
	| macro_expr                                                    { $$=$1; }
	| path_expr '{' struct_expr_fields '}'                          { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 4, $1, $2, $3, $4); }
	| nonblock_expr '?'                                             { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $2); }
	| nonblock_expr '.' path_generic_args_with_colons               { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '.' LIT_INTEGER                                 { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '[' maybe_expr ']'                              { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 4, $1, $2, $3, $4); }
	| nonblock_expr '(' maybe_exprs ')'                             { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 4, $1, $2, $3, $4); }
	| '[' vec_expr ']'                                              { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| '(' maybe_exprs ')'                                           { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	//| CONTINUE                                                      {  }
	//| CONTINUE lifetime                                             {  }
	| RETURN                                                        { $$=$1; }
	| RETURN expr                                                   { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $2); }
	| BREAK                                                         { $$=$1; }
	| BREAK lifetime                                                { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $2); }
	//| YIELD                                                         {  }
	//| YIELD expr                                                    {  }
	| nonblock_expr '=' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr SHLEQ expr                                      { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr SHREQ expr                                      { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr MINUSEQ expr                                    { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr ANDEQ expr                                      { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr OREQ expr                                       { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr PLUSEQ expr                                     { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr STAREQ expr                                     { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr SLASHEQ expr                                    { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr CARETEQ expr                                    { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr PERCENTEQ expr                                  { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr OROR expr                                       { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr ANDAND expr                                     { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr EQEQ expr                                       { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr NE expr                                         { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '<' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '>' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr LE expr                                         { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr GE expr                                         { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '|' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '^' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '&' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr SHL expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr SHR expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '+' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '-' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '*' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '/' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr '%' expr                                        { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	| nonblock_expr DOTDOT                                          { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $2); }
	| nonblock_expr DOTDOT expr                                     { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 3, $1, $2, $3); }
	|               DOTDOT expr                                     { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $2); }
	|               DOTDOT                                          { $$ = $1; }
	//| nonblock_expr AS ty                                           {  }
	| nonblock_expr ':' ty                                          { $$=alctree(YYSYMBOL_nonblock_expr, "nonblock_expr", 2, $1, $3); }
	//| BOX expr                                                      {  }
	| expr_qualified_path											{ $$=$1; }
	| nonblock_prefix_expr											{ $$=$1; }
	;

expr : lit                                                 { $$=$1; }
     | %prec IDENT path_expr                               { $$=$1; }
     //| SELF                                                {  }
     | macro_expr                                          { $$=$1; }
     | path_expr '{' struct_expr_fields '}'                { $$=alctree(YYSYMBOL_expr, "expr", 4, $1, $2, $3, $4); }
     | expr '?'                                            { $$=alctree(YYSYMBOL_expr, "expr", 2, $1, $2); }
     | expr '.' path_generic_args_with_colons              { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '.' LIT_INTEGER                                { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '[' maybe_expr ']'                             { $$=alctree(YYSYMBOL_expr, "expr", 4, $1, $2, $3, $4); }
     | expr '(' maybe_exprs ')'                            { $$=alctree(YYSYMBOL_expr, "expr", 4, $1, $2, $3, $4); }
     | '(' maybe_exprs ')'                                 { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | '[' vec_expr ']'                                    { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     //| CONTINUE                                            {  }
     //| CONTINUE ident                                      {  }
     | RETURN                                              { $$=$1; }
     | RETURN expr                                         { $$=alctree(YYSYMBOL_expr, "expr", 2, $1, $2); }
     | BREAK                                               { $$=$1; }
     | BREAK ident                                         { $$=alctree(YYSYMBOL_expr, "expr", 2, $1, $2); }
     //| YIELD                                               {  }
     //| YIELD expr                                          {  }
     | expr '=' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr SHLEQ expr                                     { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr SHREQ expr                                     { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr MINUSEQ expr                                   { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr ANDEQ expr                                     { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr OREQ expr                                      { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr PLUSEQ expr                                    { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr STAREQ expr                                    { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr SLASHEQ expr                                   { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr CARETEQ expr                                   { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr PERCENTEQ expr                                 { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr OROR expr                                      { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr ANDAND expr                                    { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr EQEQ expr                                      { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr NE expr                                        { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '<' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '>' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr LE expr                                        { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr GE expr                                        { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '|' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '^' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '&' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr SHL expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr SHR expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '+' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '-' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '*' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '/' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr '%' expr                                       { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     | expr DOTDOT                                         { $$=alctree(YYSYMBOL_expr, "expr", 2, $1, $2); }
     | expr DOTDOT expr                                    { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     |      DOTDOT expr                                    { $$=alctree(YYSYMBOL_expr, "expr", 2, $1, $2); }
     |      DOTDOT                                         { $$=$1; }
     //| expr AS ty                                          {  }
     | expr ':' ty                                         { $$=alctree(YYSYMBOL_expr, "expr", 3, $1, $2, $3); }
     //| BOX expr                                            {  }
     | expr_qualified_path									{ $$=$1; }
     | block_expr											{ $$=$1; }
     | block												{ $$=$1; }
     | nonblock_prefix_expr									{ $$=$1; }
     ;

expr_nostruct : lit                                                 { $$=$1; }
	| %prec IDENT path_expr                                     { $$=$1; }
	//| SELF                                                {  }
	| macro_expr                                          { $$=$1; }
	| expr_nostruct '?'                                   { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 2, $1, $2); }
	| expr_nostruct '.' path_generic_args_with_colons     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '.' LIT_INTEGER                       { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '[' maybe_expr ']'                    { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 4, $1, $2, $3, $4); }
	| expr_nostruct '(' maybe_exprs ')'                   { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 4, $1, $2, $3, $4); }
	| '[' vec_expr ']'                                    { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| '(' maybe_exprs ')'                                 { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	//| CONTINUE                                            {  }
	//| CONTINUE ident                                      {  }
	| RETURN                                              { $$=$1; }
	| RETURN expr                                         { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 2, $1, $2); }
	| BREAK                                               { $$=$1; }
	| BREAK ident                                         { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 2, $1, $2); }
	//| YIELD                                               {  }
	//| YIELD expr                    '('                      {  }
	| expr_nostruct '=' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct SHLEQ expr_nostruct                   { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct SHREQ expr_nostruct                   { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct MINUSEQ expr_nostruct                 { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct ANDEQ expr_nostruct                   { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct OREQ expr_nostruct                    { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct PLUSEQ expr_nostruct                  { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct STAREQ expr_nostruct                  { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct SLASHEQ expr_nostruct                 { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct CARETEQ expr_nostruct                 { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct PERCENTEQ expr_nostruct               { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct OROR expr_nostruct                    { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct ANDAND expr_nostruct                  { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct EQEQ expr_nostruct                    { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct NE expr_nostruct                      { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '<' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '>' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct LE expr_nostruct                      { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct GE expr_nostruct                      { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '|' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '^' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '&' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct SHL expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct SHR expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '+' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '-' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '*' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '/' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct '%' expr_nostruct                     { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	| expr_nostruct DOTDOT               %prec RANGE      { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 2, $1, $2); }
	| expr_nostruct DOTDOT expr_nostruct                  { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	|               DOTDOT expr_nostruct                  { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 2, $1, $2); }
	|               DOTDOT                                { $$=$1; }
	//| expr_nostruct AS ty                                 {  }
	| expr_nostruct ':' ty                                { $$=alctree(YYSYMBOL_expr_nostruct, "expr_nostruct", 3, $1, $2, $3); }
	//| BOX expr                                            {  }
	| expr_qualified_path									{ $$=$1; }
	| block_expr											{ $$=$1; }
	| block													{ $$=$1; }
	| nonblock_prefix_expr_nostruct							{ $$=$1; }
	;

nonblock_prefix_expr_nostruct : '-' expr_nostruct                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr_nostruct, "nonblock_prefix_expr_nostruct", 2, $1, $2); }
	| '!' expr_nostruct                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr_nostruct, "nonblock_prefix_expr_nostruct", 2, $1, $2); }
	| '*' expr_nostruct                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr_nostruct, "nonblock_prefix_expr_nostruct", 2, $1, $2); }
	| '&' maybe_mut expr_nostruct               { $$=alctree(YYSYMBOL_nonblock_prefix_expr_nostruct, "nonblock_prefix_expr_nostruct", 3, $1, $2, $3); }
	| ANDAND maybe_mut expr_nostruct            { $$=alctree(YYSYMBOL_nonblock_prefix_expr_nostruct, "nonblock_prefix_expr_nostruct", 3, $1, $2, $3); }
	| lambda_expr_nostruct						{ $$=$1; }
	//| MOVE lambda_expr_nostruct                 {  }
	;

nonblock_prefix_expr : '-' expr                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr, "nonblock_prefix_expr", 2, $1, $2); }
	| '!' expr                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr, "nonblock_prefix_expr", 2, $1, $2); }
	| '*' expr                         { $$=alctree(YYSYMBOL_nonblock_prefix_expr, "nonblock_prefix_expr", 2, $1, $2); }
	| '&' maybe_mut expr               { $$=alctree(YYSYMBOL_nonblock_prefix_expr, "nonblock_prefix_expr", 3, $1, $2, $3); }
	| ANDAND maybe_mut expr            { $$=alctree(YYSYMBOL_nonblock_prefix_expr, "nonblock_prefix_expr", 3, $1, $2, $3); }
	| lambda_expr						{ $$=$1; }
	//| MOVE lambda_expr                 {  }
	;

expr_qualified_path : '<' ty_sum maybe_as_trait_ref '>' MOD_SEP ident
		       maybe_qpath_params {
				$$=alctree(YYSYMBOL_expr_qualified_path, "expr_qualified_path", 7, $1, $2, $3, $4, $5, $6, $7);
	  }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	  maybe_as_trait_ref '>' MOD_SEP ident {
		$$=alctree(YYSYMBOL_expr_qualified_path, "expr_qualified_path", 10, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10);
	  }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	  generic_args maybe_as_trait_ref '>' MOD_SEP ident {
		$$=alctree(YYSYMBOL_expr_qualified_path, "expr_qualified_path", 11, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
	  }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	  maybe_as_trait_ref '>' MOD_SEP ident generic_args {
		$$=alctree(YYSYMBOL_expr_qualified_path, "expr_qualified_path", 11, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
	  }
	| SHL ty_sum maybe_as_trait_ref '>' MOD_SEP ident
	  generic_args maybe_as_trait_ref '>' MOD_SEP ident generic_args {
		$$=alctree(YYSYMBOL_expr_qualified_path, "expr_qualified_path", 12, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12);
	  }
	;

maybe_qpath_params : MOD_SEP generic_args { $$=alctree(YYSYMBOL_maybe_qpath_params, "maybe_qpath_params", 2, $1, $2); }
	| %empty               { $$ = NULL; }
	;

maybe_as_trait_ref : //AS trait_ref {  }
	%empty       { $$ = NULL; }
	;

lambda_expr : %prec LAMBDA OROR ret_ty expr                          { $$=alctree(YYSYMBOL_lambda_expr, "lambda_expr", 3, $1, $2, $3); }
	| %prec LAMBDA '|' '|' ret_ty expr                           { $$=alctree(YYSYMBOL_lambda_expr, "lambda_expr", 3, $1, $2, $3); }
	| %prec LAMBDA '|' inferrable_params '|' ret_ty expr         { $$=alctree(YYSYMBOL_lambda_expr, "lambda_expr", 4, $1, $2, $3, $4); }
	| %prec LAMBDA '|' inferrable_params OROR lambda_expr_no_first_bar { $$=alctree(YYSYMBOL_lambda_expr, "lambda_expr", 4, $1, $2, $3, $4); }
	;

lambda_expr_no_first_bar : %prec LAMBDA '|' ret_ty expr                { $$=alctree(YYSYMBOL_lambda_expr_no_first_bar, "lambda_expr_no_first_bar", 1, $2); }
	| %prec LAMBDA inferrable_params '|' ret_ty expr               { $$=alctree(YYSYMBOL_lambda_expr_no_first_bar, "lambda_expr_no_first_bar", 3, $1, $2, $3); }
	| %prec LAMBDA inferrable_params OROR lambda_expr_no_first_bar { $$=alctree(YYSYMBOL_lambda_expr_no_first_bar, "lambda_expr_no_first_bar", 3, $1, $2, $3); }
	;

lambda_expr_nostruct : %prec LAMBDA OROR expr_nostruct                 { $$=alctree(YYSYMBOL_lambda_expr_nostruct, "lambda_expr_nostruct", 2, $1, $2); }
	| %prec LAMBDA '|' '|' ret_ty expr_nostruct                    { $$=alctree(YYSYMBOL_lambda_expr_nostruct, "lambda_expr_nostruct", 4, $1, $2, $3, $4); }
	| %prec LAMBDA '|' inferrable_params '|' expr_nostruct         { $$=alctree(YYSYMBOL_lambda_expr_nostruct, "lambda_expr_nostruct", 4, $1, $2, $3, $4); }
	| %prec LAMBDA '|' inferrable_params OROR
	  lambda_expr_nostruct_no_first_bar { $$=alctree(YYSYMBOL_lambda_expr_nostruct, "lambda_expr_nostruct", 4, $1, $2, $3, $4); }
	;

lambda_expr_nostruct_no_first_bar : %prec LAMBDA '|' ret_ty expr_nostruct { $$=alctree(YYSYMBOL_lambda_expr_nostruct_no_first_bar, "lambda_expr_nostruct_no_first_bar", 3, $1, $2, $3); } //todo
	| %prec LAMBDA inferrable_params '|' ret_ty expr_nostruct         { $$=alctree(YYSYMBOL_lambda_expr_nostruct_no_first_bar, "lambda_expr_nostruct_no_first_bar", 4, $1, $2, $3, $4); } //todo
	| %prec LAMBDA inferrable_params OROR
	  lambda_expr_nostruct_no_first_bar { $$=$1; }
	;

vec_expr : maybe_exprs	{ $$=$1; }
	| exprs ';' expr { $$=alctree(YYSYMBOL_vec_expr, "vec_expr", 3, $1, $2, $3); }
	;

struct_expr_fields : field_inits	{ $$=$1; }
	| field_inits ','				{ $$=alctree(YYSYMBOL_struct_expr_fields, "struct_expr_fields", 2, $1, $2); }
	| maybe_field_inits default_field_init { $$=alctree(YYSYMBOL_struct_expr_fields, "struct_expr_fields", 2, $1, $2); }
	| %empty                               { $$ = NULL; }
	;

maybe_field_inits : field_inits		{ $$=$1; }
	| field_inits ','				{ $$=alctree(YYSYMBOL_maybe_field_inits, "maybe_field_inits", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

field_inits : field_init                 { $$=$1; }
	| field_inits ',' field_init { $$=alctree(YYSYMBOL_field_inits, "field_inits", 3, $1, $2, $3); }
	;

field_init : ident                { $$=$1; }
	| ident ':' expr       { $$=alctree(YYSYMBOL_field_init, "field_init", 3, $1, $2, $3); }
	| LIT_INTEGER ':' expr { $$=alctree(YYSYMBOL_field_init, "field_init", 3, $1, $2, $3); }
	;

default_field_init : DOTDOT expr   { $$=alctree(YYSYMBOL_default_field_init, "default_field_init", 2, $1, $2); }
	;

block_expr : //expr_match| 
	expr_if				{ $$=$1; }
	| expr_if_let		{ $$=$1; }
	| expr_while		{ $$=$1; }
	| expr_while_let	{ $$=$1; }
	//| expr_loop
	| expr_for			{ $$=$1; }
	//| UNSAFE block                                           {  }
	| path_expr '!' maybe_ident braces_delimited_token_trees { $$=alctree(YYSYMBOL_block_expr, "block_expr", 4, $1, $2, $3, $4); }
	;

full_block_expr : block_expr		{ $$=$1; }
	| block_expr_dot				{ $$=$1; }
	;

block_expr_dot : block_expr '.' path_generic_args_with_colons %prec IDENT { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 3, $1, $2, $3); }
	| block_expr_dot '.' path_generic_args_with_colons %prec IDENT    { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 3, $1, $2, $3); }
	| block_expr     '.' path_generic_args_with_colons '[' maybe_expr ']' { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 6, $1, $2, $3, $4, $5, $6); }
	| block_expr_dot '.' path_generic_args_with_colons '[' maybe_expr ']' { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 6, $1, $2, $3, $4, $5, $6); }
	| block_expr    '.' path_generic_args_with_colons '(' maybe_exprs ')' { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 6, $1, $2, $3, $4, $5, $6); }
	| block_expr_dot '.' path_generic_args_with_colons '(' maybe_exprs ')' { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 6, $1, $2, $3, $4, $5, $6); }
	| block_expr     '.' LIT_INTEGER                                  { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 3, $1, $2, $3); }
	| block_expr_dot '.' LIT_INTEGER                                  { $$=alctree(YYSYMBOL_block_expr_dot, "block_expr_dot", 3, $1, $2, $3); }
	;

/*expr_match : MATCH expr_nostruct '{' '}'                                  {  }
	| MATCH expr_nostruct '{' match_clauses                       '}' {  }
	| MATCH expr_nostruct '{' match_clauses nonblock_match_clause '}' {  }
	| MATCH expr_nostruct '{'               nonblock_match_clause '}' {  }
	;*/

/*match_clauses : match_clause               {  }
	| match_clauses match_clause {  }
	;*/

/*match_clause : nonblock_match_clause ','
	| block_match_clause
	| block_match_clause ','
	;*/

/*nonblock_match_clause : maybe_outer_attrs pats_or maybe_guard FAT_ARROW
		        nonblock_expr  {  }
	| maybe_outer_attrs pats_or maybe_guard FAT_ARROW block_expr_dot {  }
	;*/

/*block_match_clause : maybe_outer_attrs pats_or maybe_guard FAT_ARROW block {  }
	| maybe_outer_attrs pats_or maybe_guard FAT_ARROW block_expr {  }
	;*/

/*maybe_guard : IF expr_nostruct           {  }
	| %empty                     {  }
	;*/

expr_if : IF expr_nostruct block                              { $$=alctree(YYSYMBOL_expr_if, "expr_if", 3, $1, $2, $3); }
	| IF expr_nostruct block ELSE block_or_if             { $$=alctree(YYSYMBOL_expr_if, "expr_if", 5, $1, $2, $3, $4, $5); }
	;

expr_if_let : IF LET pat '=' expr_nostruct block                  { $$=alctree(YYSYMBOL_expr_if_let, "expr_if_let", 5, $1, $2, $3, $4, $5); }
	| IF LET pat '=' expr_nostruct block ELSE block_or_if { $$=alctree(YYSYMBOL_expr_if_let, "expr_if_let", 8, $1, $2, $3, $4, $5, $6, $7, $8); }
	;

block_or_if : block			{ $$=$1; }
	| expr_if				{ $$=$1; }
	| expr_if_let			{ $$=$1; }
	;	

expr_while : maybe_label WHILE expr_nostruct block               { $$=alctree(YYSYMBOL_expr_while, "expr_while", 4, $1, $2, $3, $4); }
	;

expr_while_let : maybe_label WHILE LET pat '=' expr_nostruct block   { $$=alctree(YYSYMBOL_expr_while_let, "expr_while_let", 6, $1, $2, $3, $4, $5, $6); }
	;

/*expr_loop : maybe_label LOOP block                              {  }
	;*/

expr_for : maybe_label FOR pat IN expr_nostruct block          { $$=alctree(YYSYMBOL_expr_for, "expr_for", 6, $1, $2, $3, $4, $5, $6); }
	;

maybe_label : lifetime ':'	{ $$=alctree(YYSYMBOL_maybe_label, "maybe_label", 2, $1, $2); }
	| %empty { $$ = NULL; }
	;

let : LET pat maybe_ty_ascription maybe_init_expr ';' { $$=alctree(YYSYMBOL_let, "let", 5, $1, $2, $3, $4, $5); }
	;

////////////////////////////////////////////////////////////////////////
// Part 5: Macros and misc. rules
////////////////////////////////////////////////////////////////////////

lit : LIT_BYTE                   { $$=$1; }
    | LIT_CHAR                   { $$=$1; }
    | LIT_INTEGER                { $$=$1; }
    | LIT_FLOAT                  { $$=$1; }
    | TRUE                       { $$=$1; }
    | FALSE                      { $$=$1; }
    | str						 { $$=$1; }
    ;

str : LIT_STR                    { $$=$1; }
    | LIT_STR_RAW                { $$=$1; }
    | LIT_BYTE_STR               { $$=$1; }
    | LIT_BYTE_STR_RAW           { $$=$1; }
    ;

maybe_ident : %empty { $$ = NULL; }
	| ident						 { $$=$1; }
	;

ident : IDENT                      { $$=$1; }
// Weak keywords that can be used as identifiers.  Boo! Not in Irony!
	| CATCH                      { $$=$1; }
	| DEFAULT                    { $$=$1; }
	//| UNION                      {  }
	;

unpaired_token : SHL                        { $$=$1; }
	| SHR                        { $$=$1; }
	| LE                         { $$=$1; }
	| EQEQ                       { $$=$1; }
	| NE                         { $$=$1; }
	| GE                         { $$=$1; }
	| ANDAND                     { $$=$1; }
	| OROR                       { $$=$1; }
	| LARROW                     { $$=$1; }
	| SHLEQ                      { $$=$1; }
	| SHREQ                      { $$=$1; }
	| MINUSEQ                    { $$=$1; }
	| ANDEQ                      { $$=$1; }
	| OREQ                       { $$=$1; }
	| PLUSEQ                     { $$=$1; }
	| STAREQ                     { $$=$1; }
	| SLASHEQ                    { $$=$1; }
	| CARETEQ                    { $$=$1; }
	| PERCENTEQ                  { $$=$1; }
	| DOTDOT                     { $$=$1; }
	| DOTDOTDOT                  { $$=$1; }
	| MOD_SEP                    { $$=$1; }
	| RARROW                     { $$=$1; }
	| FAT_ARROW                  { $$=$1; }
	| LIT_BYTE                   { $$=$1; }
	| LIT_CHAR                   { $$=$1; }
	| LIT_INTEGER                { $$=$1; }
	| LIT_FLOAT                  { $$=$1; }
	| LIT_STR                    { $$=$1; }
	| LIT_STR_RAW                { $$=$1; }
	| LIT_BYTE_STR               { $$=$1; }
	| LIT_BYTE_STR_RAW           { $$=$1; }
	| IDENT                      { $$=$1; }
	| UNDERSCORE                 { $$=$1; }
	| LIFETIME                   { $$=$1; }
	//| SELF                       {  }
	| STATIC                     { $$=$1; }
	//| ABSTRACT                   {  }
	| ALIGNOF                    { $$=$1; }
	//| AS                         {  }
	//| BECOME                     {  }
	| BREAK                      { $$=$1; }
	| CATCH                      { $$=$1; }
	//| CRATE                      {  }
	| DEFAULT                    { $$=$1; }
	//| DO                         {  }
	| ELSE                       { $$=$1; }
	//| ENUM                       {  }
	//| EXTERN                     {  }
	| FALSE                      { $$=$1; }
	//| FINAL                      {  }
	| FN                         { $$=$1; }
	| FOR                        { $$=$1; }
	| IF                         { $$=$1; }
	//| IMPL                       {  }
	| IN                         { $$=$1; }
	| LET                        { $$=$1; }
	//| LOOP                       {  }
	//| MACRO                      {  }
	//| MATCH                      {  }
	//| MOD                        {  }
	//| MOVE                       {  }
	| MUT                        { $$=$1; }
	| OFFSETOF                   { $$=$1; }
	//| OVERRIDE                   {  }
	//| PRIV                       {  }
	//| PUB                        {  }
	| PURE                       { $$=$1; }
	| REF                        { $$=$1; }
	| RETURN                     { $$=$1; }
	//| STRUCT                     {  }
	| SIZEOF                     { $$=$1; }
	//| SUPER                      {  }
	| TRUE                       { $$=$1; }
	//| TRAIT                      {  }
	//| TYPE                       {  }
	//| UNION                      {  }
	//| UNSAFE                     {  }
	//| UNSIZED                    {  }
	//| USE                        {  }
	//| VIRTUAL                    {  }
	| WHILE                      { $$=$1; }
	//| YIELD                      {  }
	//| CONTINUE                   {  }
	| PROC                       { $$=$1; }
	//| BOX                        {  }
	| CONST                      { $$=$1; }
	//| WHERE                      {  }
	//| TYPEOF                     {  }
	| INNER_DOC_COMMENT          { $$=$1; }
	| OUTER_DOC_COMMENT          { $$=$1; }
	| SHEBANG                    { $$=$1; }
	| STATIC_LIFETIME            { $$=$1; }
	| ';'                        { $$=$1; }
	| ','                        { $$=$1; }
	| '.'                        { $$=$1; }
	| '@'                        { $$=$1; }
	| '#'                        { $$=$1; }
	| '~'                        { $$=$1; }
	| ':'                        { $$=$1; }
	| '$'                        { $$=$1; }
	| '='                        { $$=$1; }
	| '?'                        { $$=$1; }
	| '!'                        { $$=$1; }
	| '<'                        { $$=$1; }
	| '>'                        { $$=$1; }
	| '-'                        { $$=$1; }
	| '&'                        { $$=$1; }
	| '|'                        { $$=$1; }
	| '+'                        { $$=$1; }
	| '*'                        { $$=$1; }
	| '/'                        { $$=$1; }
	| '^'                        { $$=$1; }
	| '%'                        { $$=$1; }
	;

token_trees : %empty                     { $$ = NULL; }
	| token_trees token_tree     { $$=alctree(YYSYMBOL_token_trees, "token_trees", 2, $1, $2); }
	;

token_tree : delimited_token_trees	{ $$=$1; }
	| unpaired_token         { $$=$1; }
	;

delimited_token_trees : parens_delimited_token_trees	{ $$=$1; }
	| braces_delimited_token_trees						{ $$=$1; }
	| brackets_delimited_token_trees					{ $$=$1; }
	;

parens_delimited_token_trees : '(' token_trees ')' { $$=alctree(YYSYMBOL_parens_delimited_token_trees, "parens_delimited_token_trees", 3, $1, $2, $3);  }
	;

braces_delimited_token_trees : '{' token_trees '}' { $$=alctree(YYSYMBOL_braces_delimited_token_trees, "braces_delimited_token_trees", 3, $1, $2, $3); }
	;

brackets_delimited_token_trees : '[' token_trees ']' { $$=alctree(YYSYMBOL_parens_delimited_token_trees, "brackets_delimited_token_trees", 3, $1, $2, $3); }
	;
%%

/*const char * yyname(int sym)
{
	char * s = malloc(100*sizeof(char));
    strcpy(s, yytname[sym-258+3]);
	return s;
}*/