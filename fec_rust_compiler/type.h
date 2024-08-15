#ifndef TYPE_H
#define TYPE_H

//#include "tree.h"
typedef struct param {
   char *name;
   struct typeinfo *type;
   struct param *next;
   } *paramlist;

typedef struct element {			/* members (fields) of structs */
   //struct type *elemtype;
   struct element *next;
} *elementlist;

/* base types. How many more base types do we need? */
#define FIRST_TYPE   1000000

#define NONE_TYPE    1000000
#define INT_TYPE     1000001
#define CLASS_TYPE   1000002
#define LIST_TYPE    1000003
#define FLOAT_TYPE   1000004
#define FUNC_TYPE    1000005
#define DICT_TYPE    1000006
#define BOOL_TYPE    1000007
#define STRING_TYPE  1000008
#define ARRAY_TYPE   1000009
#define ANY_TYPE     1000010
#define CHAR_TYPE    1000011
#define NULL_TYPE    1000012

#define LAST_TYPE    1000010

typedef struct typeinfo {
   int basetype;
   union {
      struct funcinfo {
         char *name; /* ? */
         int defined; /* 0 == prototype, 1 == not prototype */
         struct sym_table *st;
         struct typeinfo *returntype;
         int minparams;
         int nparams;
         struct param *parameters;
      }f;
      struct arrayinfo {
         struct typeinfo *arraytype;
         int nelements;
         struct element *elements;
      }a;
   } u;
   } *typeptr;

/* add constructors for other types as needed*/
typeptr cmptype(char *return_name);
typeptr alctype(int);
typeptr alclist(/*?*/);
typeptr alcfunctype(struct tree * r, struct tree * p, struct sym_table * st);
void alcfuncparamstype(typeptr rv, tree_t *p, int depth);
char *typename(typeptr t);

/*-------------------Typechecking functions----------------------*/

void init_types(tree_t *tree);
void typecheck(tree_t *tree);
int typecheck_expr(tree_t *expr);
int typecheck_expr_macro(tree_t *expr);
int typecheck_while(tree_t *expr);
int typecheck_for(tree_t *expr);
int typecheck_if(tree_t *expr);
int typecheck_fn(tree_t *expr, tree_t *stmt);
int return_search(tree_t *stmt);
int semicolon_search(tree_t *stmt);
int typecheck_let(tree_t *expr);
int typecheck_expr_rhs(tree_t *expr);
int typecheck_fn_params(SymbolTableEntry func, tree_t *params, tree_t *func_t);
paramlist typecheck_fn_params_sub(tree_t *params, paramlist list, tree_t *func_t);
paramlist compare_fn_params(tree_t *param1, paramlist param2, tree_t *func_t);
void alcarraytype(SymbolTableEntry entry, tree_t *typedecl, tree_t *eles);
void alcelementstype(typeptr rv, tree_t *eles, token_t *type);

void treeprint_types(tree_t *t, int depth);

/*-------------------free functions----------------------*/

void free_typ(typeptr type);
void free_params(paramlist param);
void free_eles(elementlist ele);

/*-------------------Error functions----------------------*/

void error_missmatched_arraylen(token_t *tok);
void error_incompatable_types(token_t *tok);
void error_type(char *message, token_t *tok);

/*-------------------Type Pointers----------------------*/

extern struct sym_table *global_table;
extern typeptr integer_typeptr;
extern typeptr double_typeptr;
extern typeptr char_typeptr;
extern typeptr null_typeptr;
extern typeptr string_typeptr;

extern char *typenam[];

#endif
