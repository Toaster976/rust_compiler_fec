#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symtab.h"
#include "type.h"
#include "rustgram.tab.h"

/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SHL = 3,                        /* SHL  */
  YYSYMBOL_SHR = 4,                        /* SHR  */
  YYSYMBOL_LE = 5,                         /* LE  */
  YYSYMBOL_EQEQ = 6,                       /* EQEQ  */
  YYSYMBOL_NE = 7,                         /* NE  */
  YYSYMBOL_GE = 8,                         /* GE  */
  YYSYMBOL_ANDAND = 9,                     /* ANDAND  */
  YYSYMBOL_OROR = 10,                      /* OROR  */
  YYSYMBOL_SHLEQ = 11,                     /* SHLEQ  */
  YYSYMBOL_SHREQ = 12,                     /* SHREQ  */
  YYSYMBOL_MINUSEQ = 13,                   /* MINUSEQ  */
  YYSYMBOL_ANDEQ = 14,                     /* ANDEQ  */
  YYSYMBOL_OREQ = 15,                      /* OREQ  */
  YYSYMBOL_PLUSEQ = 16,                    /* PLUSEQ  */
  YYSYMBOL_STAREQ = 17,                    /* STAREQ  */
  YYSYMBOL_SLASHEQ = 18,                   /* SLASHEQ  */
  YYSYMBOL_CARETEQ = 19,                   /* CARETEQ  */
  YYSYMBOL_PERCENTEQ = 20,                 /* PERCENTEQ  */
  YYSYMBOL_DOTDOT = 21,                    /* DOTDOT  */
  YYSYMBOL_DOTDOTDOT = 22,                 /* DOTDOTDOT  */
  YYSYMBOL_MOD_SEP = 23,                   /* MOD_SEP  */
  YYSYMBOL_RARROW = 24,                    /* RARROW  */
  YYSYMBOL_LARROW = 25,                    /* LARROW  */
  YYSYMBOL_FAT_ARROW = 26,                 /* FAT_ARROW  */
  YYSYMBOL_LIT_BYTE = 27,                  /* LIT_BYTE  */
  YYSYMBOL_LIT_CHAR = 28,                  /* LIT_CHAR  */
  YYSYMBOL_LIT_INTEGER = 29,               /* LIT_INTEGER  */
  YYSYMBOL_LIT_FLOAT = 30,                 /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 31,                   /* LIT_STR  */
  YYSYMBOL_LIT_STR_RAW = 32,               /* LIT_STR_RAW  */
  YYSYMBOL_LIT_BYTE_STR = 33,              /* LIT_BYTE_STR  */
  YYSYMBOL_LIT_BYTE_STR_RAW = 34,          /* LIT_BYTE_STR_RAW  */
  YYSYMBOL_IDENT = 35,                     /* IDENT  */
  YYSYMBOL_UNDERSCORE = 36,                /* UNDERSCORE  */
  YYSYMBOL_LIFETIME = 37,                  /* LIFETIME  */
  YYSYMBOL_STATIC = 38,                    /* STATIC  */
  YYSYMBOL_ALIGNOF = 39,                   /* ALIGNOF  */
  YYSYMBOL_BREAK = 40,                     /* BREAK  */
  YYSYMBOL_CATCH = 41,                     /* CATCH  */
  YYSYMBOL_ELSE = 42,                      /* ELSE  */
  YYSYMBOL_FALSE = 43,                     /* FALSE  */
  YYSYMBOL_FN = 44,                        /* FN  */
  YYSYMBOL_FOR = 45,                       /* FOR  */
  YYSYMBOL_IF = 46,                        /* IF  */
  YYSYMBOL_IN = 47,                        /* IN  */
  YYSYMBOL_LET = 48,                       /* LET  */
  YYSYMBOL_MUT = 49,                       /* MUT  */
  YYSYMBOL_OFFSETOF = 50,                  /* OFFSETOF  */
  YYSYMBOL_PURE = 51,                      /* PURE  */
  YYSYMBOL_REF = 52,                       /* REF  */
  YYSYMBOL_RETURN = 53,                    /* RETURN  */
  YYSYMBOL_SIZEOF = 54,                    /* SIZEOF  */
  YYSYMBOL_TRUE = 55,                      /* TRUE  */
  YYSYMBOL_DEFAULT = 56,                   /* DEFAULT  */
  YYSYMBOL_WHILE = 57,                     /* WHILE  */
  YYSYMBOL_PROC = 58,                      /* PROC  */
  YYSYMBOL_CONST = 59,                     /* CONST  */
  YYSYMBOL_INNER_DOC_COMMENT = 60,         /* INNER_DOC_COMMENT  */
  YYSYMBOL_OUTER_DOC_COMMENT = 61,         /* OUTER_DOC_COMMENT  */
  YYSYMBOL_SHEBANG = 62,                   /* SHEBANG  */
  YYSYMBOL_SHEBANG_LINE = 63,              /* SHEBANG_LINE  */
  YYSYMBOL_STATIC_LIFETIME = 64,           /* STATIC_LIFETIME  */
  YYSYMBOL_LAMBDA = 65,                    /* LAMBDA  */
  YYSYMBOL_SHIFTPLUS = 66,                 /* SHIFTPLUS  */
  YYSYMBOL_67_ = 67,                       /* ':'  */
  YYSYMBOL_FORTYPE = 68,                   /* FORTYPE  */
  YYSYMBOL_69_ = 69,                       /* '?'  */
  YYSYMBOL_70_ = 70,                       /* '='  */
  YYSYMBOL_71_ = 71,                       /* '<'  */
  YYSYMBOL_72_ = 72,                       /* '>'  */
  YYSYMBOL_73_ = 73,                       /* '|'  */
  YYSYMBOL_74_ = 74,                       /* '^'  */
  YYSYMBOL_75_ = 75,                       /* '&'  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '/'  */
  YYSYMBOL_80_ = 80,                       /* '%'  */
  YYSYMBOL_81_ = 81,                       /* '!'  */
  YYSYMBOL_82_ = 82,                       /* '{'  */
  YYSYMBOL_83_ = 83,                       /* '['  */
  YYSYMBOL_84_ = 84,                       /* '('  */
  YYSYMBOL_85_ = 85,                       /* '.'  */
  YYSYMBOL_RANGE = 86,                     /* RANGE  */
  YYSYMBOL_87_ = 87,                       /* '#'  */
  YYSYMBOL_88_ = 88,                       /* ']'  */
  YYSYMBOL_89_ = 89,                       /* ','  */
  YYSYMBOL_90_ = 90,                       /* ')'  */
  YYSYMBOL_91_ = 91,                       /* ';'  */
  YYSYMBOL_92_ = 92,                       /* '}'  */
  YYSYMBOL_93_ = 93,                       /* '@'  */
  YYSYMBOL_94_ = 94,                       /* '~'  */
  YYSYMBOL_95_ = 95,                       /* '$'  */
  YYSYMBOL_YYACCEPT = 96,                  /* $accept  */
  YYSYMBOL_file = 97,                      /* file  */
  YYSYMBOL_crate = 98,                     /* crate  */
  YYSYMBOL_maybe_shebang = 99,             /* maybe_shebang  */
  YYSYMBOL_maybe_inner_attrs = 100,        /* maybe_inner_attrs  */
  YYSYMBOL_inner_attrs = 101,              /* inner_attrs  */
  YYSYMBOL_inner_attr = 102,               /* inner_attr  */
  YYSYMBOL_maybe_outer_attrs = 103,        /* maybe_outer_attrs  */
  YYSYMBOL_outer_attrs = 104,              /* outer_attrs  */
  YYSYMBOL_outer_attr = 105,               /* outer_attr  */
  YYSYMBOL_meta_item = 106,                /* meta_item  */
  YYSYMBOL_meta_seq = 107,                 /* meta_seq  */
  YYSYMBOL_maybe_mod_items = 108,          /* maybe_mod_items  */
  YYSYMBOL_mod_items = 109,                /* mod_items  */
  YYSYMBOL_attrs_and_vis = 110,            /* attrs_and_vis  */
  YYSYMBOL_mod_item = 111,                 /* mod_item  */
  YYSYMBOL_item = 112,                     /* item  */
  YYSYMBOL_stmt_item = 113,                /* stmt_item  */
  YYSYMBOL_item_static = 114,              /* item_static  */
  YYSYMBOL_item_const = 115,               /* item_const  */
  YYSYMBOL_item_macro = 116,               /* item_macro  */
  YYSYMBOL_block_item = 117,               /* block_item  */
  YYSYMBOL_maybe_ty_ascription = 118,      /* maybe_ty_ascription  */
  YYSYMBOL_maybe_init_expr = 119,          /* maybe_init_expr  */
  YYSYMBOL_visibility = 120,               /* visibility  */
  YYSYMBOL_item_fn = 121,                  /* item_fn  */
  YYSYMBOL_fn_decl = 122,                  /* fn_decl  */
  YYSYMBOL_fn_params = 123,                /* fn_params  */
  YYSYMBOL_fn_anon_params = 124,           /* fn_anon_params  */
  YYSYMBOL_maybe_params = 125,             /* maybe_params  */
  YYSYMBOL_params = 126,                   /* params  */
  YYSYMBOL_param = 127,                    /* param  */
  YYSYMBOL_inferrable_params = 128,        /* inferrable_params  */
  YYSYMBOL_inferrable_param = 129,         /* inferrable_param  */
  YYSYMBOL_anon_params = 130,              /* anon_params  */
  YYSYMBOL_anon_param = 131,               /* anon_param  */
  YYSYMBOL_anon_params_allow_variadic_tail = 132, /* anon_params_allow_variadic_tail  */
  YYSYMBOL_named_arg = 133,                /* named_arg  */
  YYSYMBOL_ret_ty = 134,                   /* ret_ty  */
  YYSYMBOL_generic_params = 135,           /* generic_params  */
  YYSYMBOL_maybe_where_clause = 136,       /* maybe_where_clause  */
  YYSYMBOL_ty_params = 137,                /* ty_params  */
  YYSYMBOL_path_generic_args_without_colons = 138, /* path_generic_args_without_colons  */
  YYSYMBOL_generic_args = 139,             /* generic_args  */
  YYSYMBOL_generic_values = 140,           /* generic_values  */
  YYSYMBOL_maybe_ty_sums_and_or_bindings = 141, /* maybe_ty_sums_and_or_bindings  */
  YYSYMBOL_pat = 142,                      /* pat  */
  YYSYMBOL_binding_mode = 143,             /* binding_mode  */
  YYSYMBOL_lit_or_path = 144,              /* lit_or_path  */
  YYSYMBOL_pat_field = 145,                /* pat_field  */
  YYSYMBOL_pat_fields = 146,               /* pat_fields  */
  YYSYMBOL_pat_struct = 147,               /* pat_struct  */
  YYSYMBOL_pat_tup = 148,                  /* pat_tup  */
  YYSYMBOL_pat_tup_elts = 149,             /* pat_tup_elts  */
  YYSYMBOL_pat_vec = 150,                  /* pat_vec  */
  YYSYMBOL_pat_vec_elts = 151,             /* pat_vec_elts  */
  YYSYMBOL_ty = 152,                       /* ty  */
  YYSYMBOL_ty_prim = 153,                  /* ty_prim  */
  YYSYMBOL_ty_bare_fn = 154,               /* ty_bare_fn  */
  YYSYMBOL_ty_fn_decl = 155,               /* ty_fn_decl  */
  YYSYMBOL_ty_closure = 156,               /* ty_closure  */
  YYSYMBOL_for_in_type = 157,              /* for_in_type  */
  YYSYMBOL_for_in_type_suffix = 158,       /* for_in_type_suffix  */
  YYSYMBOL_maybe_mut = 159,                /* maybe_mut  */
  YYSYMBOL_maybe_mut_or_const = 160,       /* maybe_mut_or_const  */
  YYSYMBOL_maybe_ty_sums = 161,            /* maybe_ty_sums  */
  YYSYMBOL_ty_sums = 162,                  /* ty_sums  */
  YYSYMBOL_ty_sum = 163,                   /* ty_sum  */
  YYSYMBOL_ty_sum_elt = 164,               /* ty_sum_elt  */
  YYSYMBOL_maybe_ty_param_bounds = 165,    /* maybe_ty_param_bounds  */
  YYSYMBOL_ty_param_bounds = 166,          /* ty_param_bounds  */
  YYSYMBOL_boundseq = 167,                 /* boundseq  */
  YYSYMBOL_polybound = 168,                /* polybound  */
  YYSYMBOL_bindings = 169,                 /* bindings  */
  YYSYMBOL_binding = 170,                  /* binding  */
  YYSYMBOL_ty_param = 171,                 /* ty_param  */
  YYSYMBOL_maybe_bounds = 172,             /* maybe_bounds  */
  YYSYMBOL_bounds = 173,                   /* bounds  */
  YYSYMBOL_bound = 174,                    /* bound  */
  YYSYMBOL_maybe_ltbounds = 175,           /* maybe_ltbounds  */
  YYSYMBOL_ltbounds = 176,                 /* ltbounds  */
  YYSYMBOL_maybe_ty_default = 177,         /* maybe_ty_default  */
  YYSYMBOL_maybe_lifetimes = 178,          /* maybe_lifetimes  */
  YYSYMBOL_lifetimes = 179,                /* lifetimes  */
  YYSYMBOL_lifetime_and_bounds = 180,      /* lifetime_and_bounds  */
  YYSYMBOL_lifetime = 181,                 /* lifetime  */
  YYSYMBOL_trait_ref = 182,                /* trait_ref  */
  YYSYMBOL_inner_attrs_and_block = 183,    /* inner_attrs_and_block  */
  YYSYMBOL_block = 184,                    /* block  */
  YYSYMBOL_maybe_stmts = 185,              /* maybe_stmts  */
  YYSYMBOL_stmts = 186,                    /* stmts  */
  YYSYMBOL_stmt = 187,                     /* stmt  */
  YYSYMBOL_maybe_exprs = 188,              /* maybe_exprs  */
  YYSYMBOL_maybe_expr = 189,               /* maybe_expr  */
  YYSYMBOL_exprs = 190,                    /* exprs  */
  YYSYMBOL_path_expr = 191,                /* path_expr  */
  YYSYMBOL_path_generic_args_with_colons = 192, /* path_generic_args_with_colons  */
  YYSYMBOL_macro_expr = 193,               /* macro_expr  */
  YYSYMBOL_nonblock_expr = 194,            /* nonblock_expr  */
  YYSYMBOL_expr = 195,                     /* expr  */
  YYSYMBOL_expr_nostruct = 196,            /* expr_nostruct  */
  YYSYMBOL_nonblock_prefix_expr_nostruct = 197, /* nonblock_prefix_expr_nostruct  */
  YYSYMBOL_nonblock_prefix_expr = 198,     /* nonblock_prefix_expr  */
  YYSYMBOL_expr_qualified_path = 199,      /* expr_qualified_path  */
  YYSYMBOL_maybe_qpath_params = 200,       /* maybe_qpath_params  */
  YYSYMBOL_maybe_as_trait_ref = 201,       /* maybe_as_trait_ref  */
  YYSYMBOL_lambda_expr = 202,              /* lambda_expr  */
  YYSYMBOL_lambda_expr_no_first_bar = 203, /* lambda_expr_no_first_bar  */
  YYSYMBOL_lambda_expr_nostruct = 204,     /* lambda_expr_nostruct  */
  YYSYMBOL_lambda_expr_nostruct_no_first_bar = 205, /* lambda_expr_nostruct_no_first_bar  */
  YYSYMBOL_vec_expr = 206,                 /* vec_expr  */
  YYSYMBOL_struct_expr_fields = 207,       /* struct_expr_fields  */
  YYSYMBOL_maybe_field_inits = 208,        /* maybe_field_inits  */
  YYSYMBOL_field_inits = 209,              /* field_inits  */
  YYSYMBOL_field_init = 210,               /* field_init  */
  YYSYMBOL_default_field_init = 211,       /* default_field_init  */
  YYSYMBOL_block_expr = 212,               /* block_expr  */
  YYSYMBOL_full_block_expr = 213,          /* full_block_expr  */
  YYSYMBOL_block_expr_dot = 214,           /* block_expr_dot  */
  YYSYMBOL_expr_if = 215,                  /* expr_if  */
  YYSYMBOL_expr_if_let = 216,              /* expr_if_let  */
  YYSYMBOL_block_or_if = 217,              /* block_or_if  */
  YYSYMBOL_expr_while = 218,               /* expr_while  */
  YYSYMBOL_expr_while_let = 219,           /* expr_while_let  */
  YYSYMBOL_expr_for = 220,                 /* expr_for  */
  YYSYMBOL_maybe_label = 221,              /* maybe_label  */
  YYSYMBOL_let = 222,                      /* let  */
  YYSYMBOL_lit = 223,                      /* lit  */
  YYSYMBOL_str = 224,                      /* str  */
  YYSYMBOL_maybe_ident = 225,              /* maybe_ident  */
  YYSYMBOL_ident = 226,                    /* ident  */
  YYSYMBOL_unpaired_token = 227,           /* unpaired_token  */
  YYSYMBOL_token_trees = 228,              /* token_trees  */
  YYSYMBOL_token_tree = 229,               /* token_tree  */
  YYSYMBOL_delimited_token_trees = 230,    /* delimited_token_trees  */
  YYSYMBOL_parens_delimited_token_trees = 231, /* parens_delimited_token_trees  */
  YYSYMBOL_braces_delimited_token_trees = 232, /* braces_delimited_token_trees  */
  YYSYMBOL_brackets_delimited_token_trees = 233 /* brackets_delimited_token_trees  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;
   
struct typeinfo null_type = { NULL_TYPE };
struct typeinfo integer_type = { INT_TYPE };
struct typeinfo float_type = { FLOAT_TYPE };
struct typeinfo char_type = { CHAR_TYPE };
struct typeinfo string_type = { STRING_TYPE };
struct typeinfo func_type = { FUNC_TYPE };
struct typeinfo array_type = { ARRAY_TYPE };
struct typeinfo bool_type = { BOOL_TYPE };

typeptr null_typeptr = &null_type;
typeptr integer_typeptr = &integer_type;
typeptr float_typeptr = &float_type;
typeptr char_typeptr = &char_type;
typeptr string_typeptr = &string_type;
typeptr func_typeptr = &func_type;
typeptr array_typeptr = &array_type;
typeptr bool_typeptr = &bool_type;

int num_params = 0;
paramlist temp_param = NULL;

char *typenam[] =
   {"none", "int", "class", "list", "float", "func", "dict", "bool",
    "string", "package", "any"};

typeptr cmptype(char *return_name) {
   if (strcmp(return_name, "i8") == 0 || strcmp(return_name, "i16") == 0 || strcmp(return_name, "i32") == 0 || 
   strcmp(return_name, "i64") == 0 || strcmp(return_name, "i128") == 0) {
      return alctype(INT_TYPE);
   } else if (strcmp(return_name, "f32") == 0 || strcmp(return_name, "f64") == 0) {
      return alctype(FLOAT_TYPE);
   } else if (strcmp(return_name, "char") == 0) {
      return alctype(CHAR_TYPE);
   } else if (strcmp(return_name, "String") == 0) {
      return alctype(STRING_TYPE);
   } else if (strcmp(return_name, "bool") == 0) {
      return alctype(BOOL_TYPE);
   }

   return alctype(NULL_TYPE);
   
}

typeptr alctype(int base)
{
   typeptr rv;
   if (base == NULL_TYPE) return null_typeptr;
   else if (base == INT_TYPE) return integer_typeptr;
   else if (base == FLOAT_TYPE) return float_typeptr;
   else if (base == CHAR_TYPE) return char_typeptr;
   else if (base == STRING_TYPE) return string_typeptr;
   //else if (base == FUNC_TYPE) return func_typeptr;
   //else if (base == ARRAY_TYPE) return array_typeptr;
   else if (base == BOOL_TYPE) return bool_typeptr;

   rv = (typeptr) calloc(1, sizeof(struct typeinfo));
   if (rv == NULL) return rv;
   rv->basetype = base;
   return rv;
}

/* mebbe list size determination from a tree nodeptr is still reasonable? */
typeptr alclist()
{
   typeptr rv = alctype(LIST_TYPE);
   return rv;
}





/* in order for this to make any sense, you have to pass in the subtrees
 * for the return type (r) and the parameter list (p), but the calls to
 * to this function in the example are just passing NULL at present!
 */
typeptr alcfunctype(tree_t *r, tree_t *p, SymbolTable st)
{
   typeptr rv = alctype(FUNC_TYPE);
   if (rv == NULL) return NULL;
   rv->u.f.name = "temp";
   rv->u.f.st = st;
   rv->u.f.parameters = NULL;
   rv->u.f.nparams = 0;
   rv->u.f.minparams = -1;
   rv->u.f.defined = 1;

   
   
   /* fill in return type and paramlist by traversing subtrees */
   /* rf->u.f.returntype = ... */
   if (r != NULL) {
      if (r->kids[1]->prodrule == IDENT) {
         rv->u.f.returntype = cmptype(r->kids[1]->leaf->text);
      }
   } else {
         rv->u.f.returntype = alctype(NULL_TYPE);
   }

   if (p != NULL) {
      alcfuncparamstype(rv, p, 0);
   }
   
   return rv;
}

void alcfuncparamstype(typeptr rv, tree_t *p, int depth) {
   if (p == NULL) {
      return;
   }
   
   if (p->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (p->kids[i] != NULL) {
            alcfuncparamstype(rv, p->kids[i], depth+1);
         }
      }
   }

   paramlist prev = NULL;
   paramlist param_insert = rv->u.f.parameters;

   while (param_insert != NULL) {
      prev = param_insert;

      param_insert = param_insert->next;
    }

   if (p->prodrule == YYSYMBOL_param) {
      rv->u.f.nparams++;
      paramlist param_new = malloc(sizeof(struct param));
      param_new->name = p->kids[0]->leaf->text;
      param_new->type = cmptype(p->kids[2]->leaf->text);
      param_new->next = NULL;

      

      if(prev != NULL)
         prev->next = param_new;
      else
         rv->u.f.parameters = param_new;
   }
}

void alcarraytype(SymbolTableEntry entry, tree_t *typedecl, tree_t *eles) {
   if (eles == NULL || entry == NULL) {
      return;
   }
   
   typeptr rv = alctype(ARRAY_TYPE);

   if (typedecl != NULL) {
      if (rv->u.a.elements != NULL) {
         free_eles(rv->u.a.elements);
      }
      rv->u.a.arraytype = cmptype(typedecl->kids[1]->leaf->text);
      rv->u.a.nelements = typedecl->kids[3]->leaf->ival;
      rv->u.a.elements = NULL;
   }

   if (eles != NULL) {
      alcelementstype(rv, eles, typedecl->kids[3]->leaf);
      /*printf("%d", eles->type->basetype);
      if (eles->type->basetype != rv->u.a.arraytype->basetype) {
         error_type("Missmatched type in array", typedecl->kids[1]->leaf);
      }*/
   }

   elementlist element_insert = rv->u.a.elements;
   int num_eles = 0;

   if (typedecl != NULL) {
      while (element_insert != NULL) {
         element_insert = element_insert->next;

         num_eles++;
      }

      if (num_eles != rv->u.a.nelements) {
         error_missmatched_arraylen(typedecl->kids[3]->leaf);
      }
   }

   entry->typ = rv;
}

void alcelementstype(typeptr rv, tree_t *eles, token_t *type) {
   elementlist prev = NULL;
   elementlist element_insert = rv->u.a.elements;

   if (eles->nkids > 0) {
        for(int i=0; i<20; i++) {
            if (eles->kids[i] != NULL) {
               alcelementstype(rv, eles->kids[i], type);
            }
        }
   }

    // Search for symbol if it already exists and return it
    while (element_insert != NULL) {
        prev = element_insert;

        element_insert = element_insert->next;
   }

   if (eles->leaf != NULL && (eles->leaf->category == IDENT || eles->leaf->category == LIT_INTEGER || eles->leaf->category == LIT_FLOAT || eles->leaf->category == LIT_STR || eles->leaf->category == LIT_CHAR || eles->leaf->category == FALSE || eles->leaf->category == TRUE)) {
      elementlist element_new = malloc(sizeof(struct element));
      element_new->next = NULL;

      if(prev != NULL)
         prev->next = element_new;
      else
         rv->u.a.elements = element_new;
   }
}

char *typename(typeptr t)
{
   if (!t) return "(NULL)";
   else if (t->basetype < FIRST_TYPE || t->basetype > LAST_TYPE)
      return "(BOGUS)";
   else return typenam[t->basetype-1000000];
}

void init_types(tree_t *tree) {
   if (tree == NULL) {
      return;
   }
   SymbolTableEntry ident = NULL;

   if (tree->leaf != NULL) {
      switch (tree->leaf->category) {
         case LIT_CHAR:
            tree->type = alctype(CHAR_TYPE);
         break;
         case LIT_STR:
            tree->type = alctype(STRING_TYPE);
         break;
         case LIT_INTEGER:
            tree->type = alctype(INT_TYPE);
         break;
         case LIT_FLOAT:
            tree->type = alctype(FLOAT_TYPE);
         break;
         case IDENT:
            ident = lookup(tree->leaf->text, tree->symtab);
            if (ident->typ != NULL && ident->typ->basetype == FUNC_TYPE && ident->typ->u.f.returntype != NULL) {
               tree->type = alctype(ident->typ->u.f.returntype->basetype);
            } else if (ident->typ != NULL && ident->typ->basetype == ARRAY_TYPE) {
               tree->type = alctype(ident->typ->u.a.arraytype->basetype);
            } else if (ident->typ != NULL) {
               tree->type = alctype(ident->typ->basetype);
            } else {
               tree->type = alctype(NULL_TYPE);
            }
         break;
         case FALSE:
            tree->type = alctype(BOOL_TYPE);
         break;
         case TRUE:
            tree->type = alctype(BOOL_TYPE);
         break;
      }
   }

   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            init_types(tree->kids[i]);
         }
      }
   }

   return;
}

void typecheck(tree_t *tree) {
   
   //SymbolTable symtab = tree->symtab;

    int i = 0;
    if (tree == NULL) {
        return;
    }

    //SymbolTableEntry sym_touse = NULL;

   //printf("%s\n", tree->symbolname);

   if (tree->nkids > 0) {
      for(i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            typecheck(tree->kids[i]);
         }
      }
   }
   //printf("%s\n", tree->symbolname);

   switch(tree->prodrule) {
      case YYSYMBOL_item_fn:
            if (tree->kids[5]->kids[2] != NULL) {
               typecheck_fn(tree, tree->kids[5]->kids[2]);
            }
      break;
      case YYSYMBOL_let:
         typecheck_let(tree);
      break;
      case YYSYMBOL_expr_for:
         typecheck_for(tree);
      break;
      case YYSYMBOL_expr_if:
         typecheck_if(tree);
      break;
      case YYSYMBOL_expr_if_let:
            
      break;
      case YYSYMBOL_expr_while:
         typecheck_while(tree);
      break;
      case YYSYMBOL_expr_while_let:
            
      break;
      case YYSYMBOL_expr:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_exprs:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_nonblock_expr:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_expr_nostruct:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_maybe_init_expr:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_nonblock_prefix_expr_nostruct:
      case YYSYMBOL_block_expr_dot:
         typecheck_expr(tree);
      break;
      case YYSYMBOL_item_macro:
      case YYSYMBOL_macro_expr:
         typecheck_expr_macro(tree);
      break;
      default:
         //printf("%d : %s\n", tree->prodrule, tree->symbolname);
      break;
   }

   return;
}

int typecheck_expr_macro(tree_t *expr) {
   SymbolTableEntry func = NULL;
   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL && expr->kids[0]->leaf->category == IDENT) {
      func = lookup(expr->kids[0]->leaf->text, expr->symtab);
      typecheck_fn_params(func, expr->kids[3], expr->kids[0]);

      expr->type = func->typ->u.f.returntype;

      return expr->type->basetype;
   }

   return 0;
}

int typecheck_expr(tree_t *expr) {
   //printf("%s\n", expr->symbolname);

   int left = 0;
   int right = 0;

   //printf("%s\n", expr->symbolname);
   if (expr->kids[0] != NULL && expr->kids[0]->type != NULL) {
      left = expr->kids[0]->type->basetype;
   } else if (expr->kids[0] != NULL && expr->kids[0]->nkids > 0 && expr->kids[0]->kids[1]->leaf->category == '[') {
      left = expr->kids[0]->kids[0]->type->basetype;
   } else if (expr->kids[0] != NULL && expr->kids[0]->nkids > 0 && expr->kids[0]->kids[1]->leaf->category == '(') {
      //printf("thing\n");case THIS: case THAT: case THEOTHER:
      if (expr->kids[0]->kids[2]->leaf != NULL && expr->kids[0]->kids[2]->leaf->category == IDENT) {
         compare_fn_params(expr->kids[2]->kids[2], lookup(expr->kids[2]->kids[0]->leaf->text, expr->symtab)->typ->u.f.parameters, expr->kids[0]->kids[0]);
      } else if (expr->kids[0]->kids[2] != NULL  && expr->kids[2]->kids[2]->nkids > 1) {
         typecheck_fn_params(lookup(expr->kids[0]->kids[0]->leaf->text, expr->symtab), expr->kids[0]->kids[2], expr->kids[0]->kids[0]);
      }
      
      left = expr->kids[0]->kids[0]->type->basetype;
      //printf("left: %d", left);
      expr->type = alctype(left);
      return left;
   }

   if (expr->kids[2] != NULL && expr->kids[2]->type != NULL) {
      right = expr->kids[2]->type->basetype;
   } else if (expr->kids[2] != NULL && expr->kids[2]->nkids > 0 && expr->kids[2]->kids[1]->leaf->category == '[') {
      right = expr->kids[2]->kids[0]->type->basetype;
   } else if (expr->kids[2] != NULL && expr->kids[2]->nkids > 0 && expr->kids[2]->kids[1]->leaf->category == '(' ) {
      SymbolTableEntry func = NULL;
      if (expr->kids[2]->kids[2]->leaf != NULL && expr->kids[2]->kids[2]->leaf->category == IDENT) {
         func = lookup(expr->kids[2]->kids[0]->leaf->text, expr->symtab);
         compare_fn_params(expr->kids[2]->kids[2], func->typ->u.f.parameters, expr->kids[2]->kids[0]);
      } else if (expr->kids[2]->kids[2] != NULL && expr->kids[2]->kids[2]->nkids > 1) {
         func = lookup(expr->kids[2]->kids[0]->leaf->text, expr->symtab->parent);
         typecheck_fn_params(func, expr->kids[2]->kids[2], expr->kids[2]->kids[0]);
      }
      
      left = expr->kids[2]->kids[0]->type->basetype;
      //printf("right: %d\n", left);
      expr->type = alctype(left);
      return left;
   }

   /*if (left != right && left != 0 && right != 0) {
      printf("%d : %d\n", left, right);
      if (expr->kids[0]->leaf != NULL) {
         error_incompatable_types(expr->kids[0]->leaf);
      } else if (expr->kids[2]->leaf != NULL) {
         error_incompatable_types(expr->kids[2]->leaf);
      }
      return 0;
   }*/

   //printf("%d : %d\n", left, right);
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case '<':
         case '>':
         case LE:
         case GE:
         case NE:
         case EQEQ:
            if ((right == INT_TYPE && left == INT_TYPE) || (right == FLOAT_TYPE && left == FLOAT_TYPE)) {
               expr->type = alctype(BOOL_TYPE);
               return BOOL_TYPE;
            } else {
               error_type("Only floats and integers can be used in operation", expr->kids[1]->leaf);
            }
         break;
         case ANDAND:
         case OROR:
            if (right == BOOL_TYPE && left == BOOL_TYPE) {
               expr->type = alctype(BOOL_TYPE);
               return BOOL_TYPE;
            } else {
               error_type("Only boolean expressions can be used in operation", expr->kids[1]->leaf);
            }
         break;
         case PLUSEQ:
         case MINUSEQ:
            if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL && expr->kids[0]->leaf->category == IDENT) {
               if (lookup(expr->kids[0]->leaf->text, expr->symtab)->is_mut != 1) {
                  error_type("Assignment towards non-mutable variable", expr->kids[0]->leaf);
               }
            } else if (expr->kids[0]->kids[0] != NULL && expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == IDENT) {
               if (lookup(expr->kids[0]->kids[0]->leaf->text, expr->symtab)->is_mut != 1) {
                  error_type("Assignment towards non-mutable variable", expr->kids[0]->kids[0]->leaf);
               }
            }
         case '+':
         case '-':
         case '/':
         case '*':
            if (right == INT_TYPE && left == INT_TYPE) {
               expr->type = alctype(INT_TYPE);
               return INT_TYPE;
            } else if (right == FLOAT_TYPE && left == FLOAT_TYPE) {
               expr->type = alctype(FLOAT_TYPE);
               return FLOAT_TYPE;
            } else {
               error_type("Only floats and integers can be used in operation", expr->kids[1]->leaf);
            }
            break;
         case '%':
            if (right == INT_TYPE && left == INT_TYPE) {
               expr->type = alctype(INT_TYPE);
               return INT_TYPE;
            } else {
               error_type("Only integers can be used in operation", expr->kids[1]->leaf);
            }
         break;
         case ',':
            if (right == left) {
               expr->type = alctype(right);
               return right;
            }
         break;
         case DOTDOT:
            if (left == INT_TYPE && (right == INT_TYPE || right == 0)) {
               expr->type = alctype(left);
               return left;
            } else {
               error_incompatable_types(expr->kids[0]->leaf);
            }
         break;
         case '=':
            if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL && expr->kids[0]->leaf->category == IDENT) {
               if (lookup(expr->kids[0]->leaf->text, expr->symtab)->is_mut != 1) {
                  
                  error_type("Assignment towards non-mutable variable", expr->kids[0]->leaf);
               }

               if (right == left) {
                  expr->type = alctype(right);
                  return right;
               } else {
                  //printf("%d : %d", right, left);
                  error_type("Type mismatch in assignment", expr->kids[0]->leaf);
               }
            } 
            
            if (right != left) {
               
               error_type("Assignment towards non-variable", expr->kids[0]->leaf);
            }

            if (expr->kids[0]->kids[0] != NULL && expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == IDENT) {
               if (lookup(expr->kids[0]->kids[0]->leaf->text, expr->symtab)->is_mut != 1) {
                  
                  error_type("Assignment towards non-mutable variable", expr->kids[0]->kids[0]->leaf);
               }

               if (right == left) {
                  expr->type = alctype(right);
                  return right;
               } else {
                  error_type("Type mismatch in assignment", expr->kids[0]->kids[0]->leaf);
               }
            } else if (expr->kids[0]->kids[1] != NULL && expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == DOTDOT) {
               
            } else {
               //printf("lol\n");
               error_type("Assignment towards non-variable", expr->kids[0]->kids[0]->leaf);
            }
         break;
         case '(':
            SymbolTableEntry func = NULL;
            //printf("thing1\n");
            /*if (expr->kids[2]->leaf != NULL && expr->kids[2]->leaf->category == IDENT) {
               //printf("thing2\n");
               func = lookup(expr->kids[0]->leaf->text, expr->symtab);
               compare_fn_params(expr->kids[2], func->typ->u.f.parameters, expr->kids[0]);
            } else*/ //if (expr->kids[2] != NULL) {
               //printf("thing3\n");
               if (expr->kids[0]->leaf != NULL) {
                  func = lookup(expr->kids[0]->leaf->text, expr->symtab->parent);
                  typecheck_fn_params(func, expr->kids[2], expr->kids[0]);
               }
            //}
            
            left = expr->kids[0]->type->basetype;
            //printf("left: %d\n", left);
            expr->type = alctype(left);
            return left;
         break;
         case '.':
         if (expr->kids[2]->type != NULL) {
            expr->type = alctype(expr->kids[2]->type->basetype);
            return expr->kids[2]->type->basetype;
         } else if (expr->kids[1]->type != NULL) {
            expr->type = alctype(expr->kids[2]->type->basetype);
            return expr->kids[2]->type->basetype;
         }

         break;
      }
   }

   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) {
      switch (expr->kids[0]->leaf->category) {
         case '!':
            //printf("eingdosugdisrugsidndv\n");
            if (expr->kids[1] != NULL && expr->kids[1]->type != NULL && expr->kids[1]->type->basetype == BOOL_TYPE) {
               
               expr->type = alctype(BOOL_TYPE);
               return BOOL_TYPE;
            } else {
               error_type("Only boolean expressions can be used in operation", expr->kids[0]->leaf);
            }
         break;
         case '(':
         case '[':
         case RETURN:
         case '=':
         if (expr->kids[1] != NULL && expr->kids[1]->type != NULL) {
            expr->type = alctype(expr->kids[1]->type->basetype);
            return expr->kids[1]->type->basetype;
         }
         break;
      }
   }


   return 0;
}

int typecheck_expr_rhs(tree_t *expr) {
   int type_base = 0;
   
   if (expr->nkids > 0) {
      for(int i = 0; i < 20; i++) {
         if (expr->kids[i] != NULL) {
            if (expr->kids[i]->prodrule == YYSYMBOL_expr) {
               if (type_base != 0 && expr->kids[i]->type != NULL && type_base == expr->kids[i]->type->basetype) {
                  type_base = expr->kids[i]->type->basetype;
                  expr->type = alctype(type_base);
                  return type_base;
               } else {
                  printf("missmatched types");
               }
               type_base = typecheck_expr_rhs(expr->kids[i]);
               expr->type = alctype(type_base);
            } else if (i == 2 && expr->kids[i]->leaf != NULL && expr->kids[i]->leaf->category == '[') {
               expr->type = alctype(expr->kids[0]->type->basetype);
               return expr->kids[0]->type->basetype;
            } else if (expr->kids[i]->type != NULL) {
               if (type_base != 0 && type_base == expr->kids[i]->type->basetype) {
                  expr->type = alctype(type_base);
                  return type_base;
               } else {
                  printf("missmatched types");
               }
               type_base = expr->kids[i]->type->basetype;
            }
         }
      }
   }

   return 0;
}

int typecheck_while(tree_t *expr) {
   if (expr->kids[2] != NULL && expr->kids[2]->type != NULL && expr->kids[2]->type->basetype == BOOL_TYPE) {
      return 0;
   } else {
      error_type("Invalid expression in while loop", expr->kids[1]->leaf);
   }

   return 0;
}

int typecheck_for(tree_t *expr) {
   SymbolTableEntry var = lookup(expr->kids[2]->leaf->text, expr->symtab);
   //printf("var: %s", var->s);
   
   if (expr->kids[4] != NULL && expr->kids[4]->type != NULL) {
      if (expr->kids[4]->kids[0] != NULL && expr->kids[4]->kids[0]->leaf != NULL && expr->kids[4]->kids[0]->leaf->category == '[') {
         var->typ = alctype(expr->kids[4]->type->basetype);
      } else if (expr->kids[4]->leaf != NULL && expr->kids[4]->leaf->category == IDENT && lookup(expr->kids[4]->leaf->text, expr->symtab)->typ->basetype == ARRAY_TYPE) {
         var->typ = alctype(expr->kids[4]->type->basetype);
      } else if (expr->kids[4]->kids[1] != NULL && expr->kids[4]->kids[1]->leaf != NULL && expr->kids[4]->kids[1]->leaf->category == '=') {
         var->typ = alctype(INT_TYPE);
         //expr->kids[2]->type = alctype(INT_TYPE);
      }  else if (expr->kids[4]->kids[1] != NULL && expr->kids[4]->kids[1]->leaf != NULL && expr->kids[4]->kids[1]->leaf->category == DOTDOT) {
         var->typ = alctype(INT_TYPE);
         //expr->kids[2]->type = alctype(INT_TYPE);
      } else {
         error_type("Invalid iterator in for loop", expr->kids[2]->leaf);
      }
   }

   return 0;
}

int typecheck_if(tree_t *expr) {
   if (expr->kids[1] != NULL && expr->kids[1]->type != NULL && expr->kids[1]->type->basetype == BOOL_TYPE) {
      return 0;
   } else if (expr->kids[1]->leaf != NULL) {
      error_type("Invalid expression in if loop", expr->kids[1]->leaf);
   }

   /*if (expr->kids[4] != NULL) {
      typecheck_if(expr->kids[4]);
   }*/

   return 0;
}

int typecheck_fn(tree_t *expr, tree_t *stmt) {
   SymbolTableEntry func = lookup(expr->kids[1]->leaf->text, expr->kids[1]->symtab);
   int func_ret = func->typ->u.f.returntype->basetype;
   int is_return = 0;
   int is_semicolon = 0;
   int returnblock_base = 0;


   if (stmt != NULL && stmt->prodrule == YYSYMBOL_stmts) {
      if (typecheck_fn(expr, stmt->kids[1]) == 1) {
         return 1;
      }
   } else if (stmt != NULL && stmt->prodrule == YYSYMBOL_stmt) {
      if (stmt->kids[1] != NULL && stmt->kids[1]->leaf != NULL && stmt->kids[1]->leaf->category == ';') {
         is_semicolon = 1;
      }
      if (stmt->kids[0] != NULL && stmt->kids[0]->type != NULL) {
         returnblock_base = stmt->kids[0]->type->basetype;
      } else {
         returnblock_base = NULL_TYPE;
      }

      is_return = return_search(stmt);
   } else if (stmt != NULL) {
      is_semicolon = semicolon_search(stmt);
      is_return = return_search(stmt);
      
      if (stmt->type != NULL) {
         returnblock_base = stmt->type->basetype;
      } else {
         returnblock_base = NULL_TYPE;
      }
   }

   //printf("function: %d\n", func_ret);
   //printf("block: %d\n", returnblock_base);
   //printf("semi: %d\n", is_semicolon);
   //printf("return: %d\n", is_return);

   if ((is_semicolon == 0) || (is_semicolon == 1 && is_return == 1) || func_ret == NULL_TYPE) {
      if ((returnblock_base == func_ret) || (func_ret == NULL_TYPE && is_semicolon == 1 && is_return == 0)) {
         return 1;
      } else {
         error_type("Mismatch between function return type in block and declaration", expr->kids[1]->leaf);
      }
   } else {
      error_type("Return type for function is not specified in block", expr->kids[1]->leaf);
   }

   return 0;
}

int return_search(tree_t *stmt) {
   int is_return = 0;
   
   if (stmt->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (stmt->kids[i] != NULL) {
            if (return_search(stmt->kids[i]) == 1) {
               is_return = 1;
            }
         }
      }
   } else {
      if (stmt->leaf->category == RETURN) {
         is_return = 1;
      }
   }

   return is_return;
}

int semicolon_search(tree_t *stmt) {
   int is_semicolon = 0;
   
   if (stmt->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (stmt->kids[i] != NULL) {
            if (semicolon_search(stmt->kids[i]) == 1) {
               is_semicolon = 1;
            }
         }
      }
   } else {
      if (stmt->leaf->category == ';') {
         is_semicolon = 1;
      }
   }

   return is_semicolon;
}

int typecheck_fn_params(SymbolTableEntry func, tree_t *params, tree_t *func_t) { 
   num_params = 0;
   //struct param *temp_param = func->typ->u.f.parameters;

   typecheck_fn_params_sub(params, func->typ->u.f.parameters, func_t);

   //printf("%d: %d\n", num_params, func->typ->u.f.nparams);
   if (func->typ->u.f.nparams == num_params && func->typ->u.f.minparams == -1) {
      return 0;
   } else if (func->typ->u.f.minparams > -1 && num_params >= func->typ->u.f.minparams) {
      return 0;
   } else {
      error_type("Missmatched number of function parameters", func_t->leaf);
   }

   return 0;
}

paramlist typecheck_fn_params_sub(tree_t *params, paramlist list, tree_t *func_t) {
   //printf("rthjdonodn\n");
   if (params == NULL) {
      return NULL;
   }
   //printf("xczoidvjsd %s\n", params->symbolname);
   
   for(int i=0; i<20; i++) {
      if (params->kids[i] != NULL && (params->prodrule == YYSYMBOL_exprs || params->prodrule == YYSYMBOL_token_trees || params->prodrule == YYSYMBOL_parens_delimited_token_trees)) {
         list = typecheck_fn_params_sub(params->kids[i], temp_param, func_t);
         //printf("%s\n", params->symbolname);
      }
   }
   if (params->type != NULL && params->prodrule != YYSYMBOL_exprs && params->prodrule != YYSYMBOL_token_trees) {
      list = compare_fn_params(params, list, func_t);
   }

   return list;
}

paramlist compare_fn_params(tree_t *param1, paramlist param2, tree_t *func_t) {
   num_params++;
   //printf("rthjdonodn%d:%d\n", param1->type->basetype, param2->type->basetype);
   if (temp_param == NULL || param1->type->basetype == param2->type->basetype) {
      if (temp_param == NULL || temp_param->next == NULL) {
         temp_param = NULL;
         return NULL;
      } else {
         temp_param = temp_param->next;
         return temp_param->next;
      }
   } else {
      if (param1->leaf != NULL) {
         error_type("Missmatched types for function parameters", param1->leaf);
      } else {
         error_type("Missmatched types for function parameters", func_t->leaf);
      }
   }

   return NULL;
}

int typecheck_let(tree_t *expr) {
   int right = 0;
   int left = 0;

   if (expr->kids[1] != NULL && expr->kids[1]->type != NULL) {
      left = expr->kids[1]->type->basetype;
   } else if (expr->kids[1] != NULL && expr->kids[1]->kids[1] != NULL && expr->kids[1]->kids[1]->type != NULL) {
      left = expr->kids[1]->kids[1]->type->basetype;
   }

   if (expr->kids[3] != NULL && expr->kids[3]->type != NULL) {
      right = expr->kids[3]->type->basetype;
   }

   if (left == right || right == 0) {
      expr->type = alctype(left);
      return left;
   } else {
      printf("%d : %d\n", left, right);
      error_type("Mismatched types in declaration", expr->kids[0]->leaf);
   }

   return 0;
}

void treeprint_types(tree_t *t, int depth)
{
    int i;
    
   if(t->symbolname != NULL && t->nkids > 0) {
      printf("%*s BRANCH-%s: %d", depth*2, " ", t->symbolname, t->nkids);
      if (t->type != NULL) {
         printf(" type: %d\n", t->type->basetype);
      } else {
         printf("\n");
      }
   }

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
               treeprint_types(t->kids[i], depth+1);
            }
        }
    } else {
         if (t->leaf != NULL && t->symbolname != NULL) {
            printf("%*s LEAF-%s: %s-%d", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
            if (t->type != NULL) {
               printf(" type: %d\n", t->type->basetype);
            } else {
               printf("\n");
            }
         }
    }
   
}

void free_typ(typeptr type) {
   if (type == NULL) {
      return;
   }
   free_params(type->u.f.parameters);
   //free(type->u.f.parameters);
   free_typ(type->u.f.returntype);
   if (type->basetype == FUNC_TYPE) {
      free(type);
   } else if (type->basetype == ARRAY_TYPE) {
      free_eles(type->u.a.elements);
      free(type);
   }

   if (type != NULL) {
      //free(type);
   }
}

void free_params(paramlist param) {
   if (param == NULL) {
      return;
   }

   paramlist prev = param;
   paramlist param_free = param;

   while (param_free != NULL) {
      prev = param_free;

      param_free = param_free->next;
      free_typ(prev->type);
      free(prev);
   }

   return;
}

void free_eles(elementlist ele) {
   if (ele == NULL) {
      return;
   }

   elementlist prev = ele;
   elementlist ele_free = ele;

   while (ele_free != NULL) {
      prev = ele_free;

      ele_free = ele_free->next;
      free(prev);
   }

   return;
}

void error_missmatched_arraylen(token_t *tok) {
   error_semantic("Mismatched array length", tok->filename, tok->lineno, tok->text);
}

void error_incompatable_types(token_t *tok) {
   error_semantic("Incompatable types in expression", tok->filename, tok->lineno, tok->text);
}

void error_type(char *message, token_t *tok) {
   error_semantic(message, tok->filename, tok->lineno, tok->text);
}