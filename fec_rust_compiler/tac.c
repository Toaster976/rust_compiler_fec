/*
 * Three Address Code - skeleton for CS 423
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ytab.h"
#include "tree.h"
#include "rustgram.tab.h"
#include "symtab.h"
#include "type.h"
#include "tac.h"

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
   

char *regionnames[] = {"global","loc", "class", "lab", "const", "", "none", "str", "float"};
char *regionname(int i) { return regionnames[i-R_GLOBAL]; }
char *opcodenames[] = {
   "ADD","SUB", "MUL", "DIV", "NEG", "ASN", "ADDR", "LCONT", "SCONT", "GOTO",
   "BLT", "BLE", "BGT", "BGE", "BEQ", "BNE", "BIF", "BNIF", "PARM", "CALL",
   "RETURN", "OR", "AND", "OFFASN", "MOD", "BNEQ"
   };
char *opcodename(int i) { return opcodenames[i-O_ADD]; }
char *pseudonames[] = {
   "glob","proc", "loc", "lab", "end", "prot"
   };
char *pseudoname(int i) { return pseudonames[i-D_GLOB]; }

int labelcounter;
int stringcounter;
int num_strings = 0;
int label_counter = 0;

struct addr *genlabel()
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = R_LABEL;
   a->u.offset = labelcounter;
   labelcounter += 8;
   //printf("generated a label %d\n", a->u.offset);
   return a;
}

struct instr *gen(int op, struct addr *a1, struct addr *a2, struct addr *a3)
{
  struct instr *rv = malloc(sizeof (struct instr));
  if (rv == NULL) {
     fprintf(stderr, "out of memory\n");
     exit(4);
     }
  rv->opcode = op;
  rv->dest = a1;
  rv->src1 = a2;
  rv->src2 = a3;
  rv->next = NULL;
  return rv;
}

struct addr *copyaddr(struct addr *a)
{
   if (a == NULL) return NULL;
   struct addr *copy = NULL;
   if (a->region == R_NONE) {
      copy = create_addr_name(R_NONE, "");
   } else if (a->region == R_CONST) {
      copy = create_addr_ival(R_CONST, a->val_i);
      //free(a);
   } else if (a->region == R_FLOAT) {
      copy = create_addr_dval(R_FLOAT, a->val_d);
      //free(a);
   } else if (a->region == R_LOCAL) {
      copy = create_addr_spec(R_LOCAL, a->u.offset);
   } else if (a->region == R_NAME) {
      copy = create_addr_name(R_NAME, a->u.name);
   } else if (a->region == R_STR) {
      copy = create_addr_spec(R_STR, a->u.offset);
   } else if (a->region == R_GLOBAL) {
      copy = create_addr_spec(R_GLOBAL, a->u.offset);
   } else if (a->region == R_LABEL) {
      copy = create_addr_spec(R_LABEL, a->u.offset);
   }
   copy->og = 0;
   
   
   return copy;
}

struct instr *copylist(struct instr *l)
{
   if (l == NULL) return NULL;
   struct instr *lcopy = gen(l->opcode, l->dest, l->src1, l->src2);
   lcopy->next = copylist(l->next);
   free(l);
   return lcopy;
}

struct instr *append(struct instr *l1, struct instr *l2)
{
   if (l1 == NULL) return l2;
   struct instr *ltmp = l1;
   while(ltmp->next != NULL) ltmp = ltmp->next;
   ltmp->next = l2;
   return l1;
}

struct instr *concat(struct instr *l1, struct instr *l2)
{
   return append(copylist(l1), copylist(l2));
}

struct addr *create_addr_name(int region, char* name)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;
   
   a->u.name = name;
   a->og = 1;
   //printf("generated addr %s\n", a->u.name);
   return a;
}

struct addr *create_addr_ival(int region, int ival)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = ival;
   a->val_i = ival;
   a->val_d = ival;
   a->og = 1;
   //printf("generated addr %d\n", a->val_i);
   return a;
}

struct addr *create_addr_dval(int region, double dval)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;
   
   a->val_d = dval;
   a->val_i = (int)dval;
   a->og = 1;
   //printf("generated addr %f\n", a->val_d);
   return a;
}

struct addr *create_addr_offset(int region)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = labelcounter;
   a->val_d = labelcounter;
   a->val_i = labelcounter;
   labelcounter += 8;
   a->og = 1;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_label(int region)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = label_counter;
   a->val_d = label_counter;
   a->val_i = label_counter;
   label_counter += 1;
   a->og = 1;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_offset_spec(int region, int counter)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = labelcounter;
   a->val_d = labelcounter;
   a->val_i = labelcounter;
   labelcounter += counter;
   a->og = 0;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_string(int region, int counter)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = stringcounter;
   a->val_d = stringcounter;
   a->val_i = stringcounter;
   stringcounter += counter;
   a->og = 0;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_spec(int region, int counter)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = counter;
   a->val_d = counter;
   a->val_i = counter;
   a->og = 0;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_offset_ival(int region, int ival)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = labelcounter;
   a->val_d = ival;
   a->val_i = ival;
   labelcounter += 8;
   a->og = 1;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

struct addr *create_addr_offset_dval(int region, double dval)
{
   struct addr *a = malloc(sizeof(struct addr));
   a->region = region;

   a->u.offset = labelcounter;
   a->val_d = dval;
   a->val_i = (int)dval;
   labelcounter += 8;
   a->og = 1;
   //printf("generated addr %d\n", a->u.offset);
   return a;
}

void assign_first(struct tree *t)
{  
   int i;
   if (t->nkids > 0) {
      for(i=0; i<20; i++) {
         if (t->kids[i] != NULL) {
            assign_first(t->kids[i]);
         }
      }
   }
   switch(t->prodrule) {
     /* all switch cases that DO need a label: */
      case YYSYMBOL_item_fn:
         if (t->kids[1]->leaf != NULL && t->kids[1]->leaf->category == IDENT) {
            t->first = t->kids[1]->first;
         }
      break;
      case YYSYMBOL_let:
         SymbolTableEntry entry = NULL;
         if (t->kids[1]->nkids > 0) {
            entry = lookup(t->kids[1]->kids[1]->leaf->text, t->symtab);
         } else {
            entry = lookup(t->kids[1]->leaf->text, t->symtab);
         }

         t->first = t->kids[1]->first;
         //t->address = t->kids[1]->first;
         if (t->kids[3] != NULL && t->kids[3]->kids[1] != NULL && t->kids[3]->kids[1]->kids[0] != NULL && t->kids[3]->kids[1]->kids[0]->leaf != NULL && t->kids[3]->kids[1]->kids[0]->leaf->category == '[') {
            //printf("bsifbwfbsuiiasiau\n");
            t->kids[1]->first = t->kids[3]->first;
            t->kids[1]->address = t->kids[3]->first;
            entry->address = t->kids[3]->first;

            t->first = t->kids[1]->first;
            t->address = t->kids[1]->first;
         } else if (t->kids[3] != NULL) {
            /*if (t->type->basetype == FLOAT_TYPE) {
               t->first->val_d = t->kids[3]->address->val_d;
            } else if (t->type->basetype == INT_TYPE) {
               t->first->val_i = t->kids[3]->address->val_i;
            } else if (t->type->basetype == BOOL_TYPE) {
               t->first->val_i = t->kids[3]->address->val_i;
            }*/
            //t->first = t->kids[3]->first;

            /*else if (t->type->basetype == STRING_TYPE || t->type->basetype == CHAR_TYPE) {
               t->first->val_i = t->kids[3]->address->val_i;
            } */
         }
         
      break;
      case YYSYMBOL_expr_for:

      break;
      case YYSYMBOL_expr_if:
         t->first = t->kids[1]->first;
      break;
      case YYSYMBOL_expr_while:
         t->first = t->kids[2]->first;
      break;
      case YYSYMBOL_nonblock_prefix_expr_nostruct:
      case YYSYMBOL_exprs:
      case YYSYMBOL_expr:
      case YYSYMBOL_nonblock_expr:
      case YYSYMBOL_expr_nostruct:
      case YYSYMBOL_block_expr_dot: 
      case YYSYMBOL_nonblock_prefix_expr:
         gen_expr_first(t);
      break;
      case YYSYMBOL_maybe_init_expr:
         gen_expr_first(t);
         //t->address = t->kids[1]->address;
      break;
      case YYSYMBOL_item_macro:

      break;
      case YYSYMBOL_macro_expr:
         if (t->kids[0]->leaf != NULL && t->kids[0]->leaf->category == IDENT) {
            t->address = create_addr_offset(R_LOCAL);
            t->kids[0]->first = t->kids[0]->address;
            //t->address = t->kids[0]->address;
            t->first = t->address;
         }
      break;
      case YYSYMBOL_pat:
         //printf("tenriosidfns\n");
         if (t->kids[1]->leaf != NULL && t->kids[1]->leaf->category == IDENT) {
            t->first = t->kids[1]->address;
            //t->address = t->kids[1]->address;
         }
         
         //t->first = create_addr(R_GLOBAL, "thing");
         //printf("hi: %s\n", t->symbolname);
      break;
      case YYSYMBOL_stmt:
         if (t->kids[0] != NULL && t->kids[0]->first != NULL) {
            t->first = t->kids[0]->first;
            //t->address = t->kids[0]->address;
         } else if (t->kids[1] != NULL && t->kids[1]->first != NULL) {
            t->first = t->kids[1]->first;
            //t->address = t->kids[1]->address;
         }
      break;
      case YYSYMBOL_stmts:
         t->first = t->kids[0]->first;
      break;
      case YYSYMBOL_inner_attrs_and_block:
         t->first = t->kids[2]->first;
         //printf("fffffffffff: %d\n", t->first->region);
      break;
      case YYSYMBOL_block:
         if (t->kids[1] != NULL) {
            t->first = t->kids[1]->first;
         }
      break;
      case YYSYMBOL_token_trees:
         //printf("fffffffffff");
         if (t->kids[0] == NULL) {
            t->first = t->kids[1]->first;
         } else {
            t->first = t->kids[0]->first;
         }
         //printf("fffffffffff %d\n", t->first->region);
      break;
   }
   if (t->leaf != NULL && strcmp(t->leaf->text, "i64") != 0 && strcmp(t->leaf->text, "f64") != 0 && strcmp(t->leaf->text, "char") != 0 && strcmp(t->leaf->text, "String") != 0 && strcmp(t->leaf->text, "bool") != 0 ) {
      switch (t->leaf->category) {
         case LIT_CHAR:
            //t->first = create_addr(R_STR, t->leaf->text);
            t->address = create_addr_offset_spec(R_STR, 8);
            t->first = t->address;
            break;
         case LIT_STR:
            //t->first = create_addr(R_STR, t->leaf->text);
            t->address = create_addr_string(R_STR, strlen(t->leaf->sval) - (strlen(t->leaf->sval) % 8) + 8);
            t->first = t->address;
            num_strings++;
            break;
         case LIT_INTEGER:
            //t->first = create_addr(R_CONST, t->leaf->text);
            t->address = create_addr_ival(R_CONST, t->leaf->ival);
            t->first = t->address;
            break;
         case LIT_FLOAT:
            //t->first = create_addr(R_CONST, t->leaf->text);
            t->address = create_addr_dval(R_CONST, t->leaf->dval);
            t->first = t->address;
            break;
         case FALSE:
            //t->first = create_addr(R_CONST, "0");
            t->address = create_addr_ival(R_CONST, 0);
            t->first = t->address;
            break;
         case TRUE:
            //t->first = create_addr(R_CONST, "1");
            t->address = create_addr_ival(R_CONST, 1);
            t->first = t->address;
            break;
         break;
         case IDENT:
            SymbolTableEntry var = lookup(t->leaf->text, t->symtab);

            if (var->typ != NULL && var->typ->basetype == ARRAY_TYPE) {
               if (var->address != NULL) {
                  //t->first = genlabel();
                  t->address = var->address;
                  t->first = t->address;
               }
            } else {
               if (var != NULL && var->address == NULL && (var->typ == NULL || var->typ->basetype != FUNC_TYPE)) {
                  //printf("%s\n", var->s);
                  
                  t->address = create_addr_offset(R_LOCAL);
                  //t->first = create_addr(R_LOCAL, t->leaf->text);
                  var->address = t->address;
                  t->first = t->address;
               } else if (var->address != NULL) {
                  //t->first = genlabel();
                  //printf("sdjniseubwief\n");
                  t->address = create_addr_spec(var->address->region, var->address->u.offset);
                  t->first = t->address;
               }
            }

            
         break;
         case BREAK:
            t->address = create_addr_name(R_NONE, "");
            t->first = t->address;
         break;
      }
   }
   
   //printf("%s\n", t->symbolname);
}

int gen_expr_first(tree_t *expr) {

   //printf("%d : %d\n", left, right);
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case '<':
         case '>':
         case LE:
         case GE:
         case NE:
         case EQEQ:
            expr->address = create_addr_offset(R_LOCAL);
            /*if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i - expr->kids[2]->address->val_i;
            } else if (expr->type->basetype == FLOAT_TYPE) {
               expr->address->val_d = expr->kids[0]->address->val_d - expr->kids[2]->address->val_d;
            }*/

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
         break;
         case ANDAND:
         case OROR:
            expr->address = create_addr_offset(R_LOCAL);
            expr->first = expr->address;
            /*if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }*/
            if (expr->kids[0]->first->region == R_LOCAL) {
               expr->first = expr->kids[0]->first;
            } else if (expr->kids[2]->first->region == R_LOCAL) {
               expr->first = expr->kids[2]->first;
            }

         break;
         case PLUSEQ:
         case MINUSEQ:
            expr->first = expr->kids[2]->first;
            //expr->address = expr->kids[0]->address;
         break;
         case '+':
            expr->address = create_addr_offset(R_LOCAL);
            /*if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i + expr->kids[2]->address->val_i;
            } else if (expr->type->basetype == FLOAT_TYPE) {
               expr->address->val_d = expr->kids[0]->address->val_d + expr->kids[2]->address->val_d;
            }*/

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
         break;
         case '-':
            expr->address = create_addr_offset(R_LOCAL);
            if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i - expr->kids[2]->address->val_i;
            } else if (expr->type->basetype == FLOAT_TYPE) {
               expr->address->val_d = expr->kids[0]->address->val_d - expr->kids[2]->address->val_d;
            }

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
         break;
         case '/':
            expr->address = create_addr_offset(R_LOCAL);
            if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i / expr->kids[2]->address->val_i;
            } else if (expr->type->basetype == FLOAT_TYPE) {
               expr->address->val_d = expr->kids[0]->address->val_d / expr->kids[2]->address->val_d;
            }

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
         break;
         case '*':
            expr->address = create_addr_offset(R_LOCAL);
            if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i * expr->kids[2]->address->val_i;
            } else if (expr->type->basetype == FLOAT_TYPE) {
               expr->address->val_d = expr->kids[0]->address->val_d * expr->kids[2]->address->val_d;
            }

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
         break;
         case '%':
            expr->address = create_addr_offset(R_LOCAL);
            if (expr->type->basetype == INT_TYPE) {
               expr->address->val_i = expr->kids[0]->address->val_i % expr->kids[2]->address->val_i;
            }

            expr->first = expr->address;
            if (expr->nkids > 0) {
               for(int i=19; i>=0; i--) {
                  if (expr->kids[i] != NULL && expr->kids[i]->first != NULL) {
                     expr->first = expr->kids[i]->first;
                  }
               }
            }
            
         break;
         case ',':
            /*if (expr->kids[0]->type->basetype == INT_TYPE && expr->kids[0]->prodrule != YYSYMBOL_exprs) {
               expr->kids[0]->address = create_addr_offset_ival(R_LOCAL, expr->kids[0]->leaf->ival);
               expr->first = expr->kids[0]->address;
            } else if (expr->kids[0]->type->basetype == FLOAT_TYPE && expr->kids[0]->prodrule != YYSYMBOL_exprs) {
               expr->kids[0]->address = create_addr_offset_dval(R_LOCAL, expr->kids[0]->leaf->dval);
               expr->first = expr->kids[0]->address;
            }

            if (expr->kids[2]->type->basetype == INT_TYPE && expr->kids[2]->prodrule != YYSYMBOL_exprs) {
               expr->kids[2]->address = create_addr_offset_ival(R_LOCAL, expr->kids[2]->leaf->ival);
               expr->first = expr->kids[2]->address;
            } else if (expr->kids[2]->type->basetype == FLOAT_TYPE && expr->kids[2]->prodrule != YYSYMBOL_exprs) {
               expr->kids[2]->address = create_addr_offset_dval(R_LOCAL, expr->kids[2]->leaf->dval);
               expr->first = expr->kids[2]->address;
            }*/
            /*if (expr->kids[0]->leaf != NULL) {
               if (expr->kids[0]->address != NULL) {
                  free(expr->kids[0]->address);
                  expr->kids[0]->address = NULL;
               }
               expr->kids[0]->address = create_addr_offset(R_LOCAL);
               expr->kids[0]->first = expr->kids[0]->address;
            }

            if (expr->kids[2]->leaf != NULL) {
               if (expr->kids[2]->address != NULL) {
                  free(expr->kids[2]->address);
                  expr->kids[2]->address = NULL;
               }
               expr->kids[2]->address = create_addr_offset(R_LOCAL);
               expr->kids[2]->first = expr->kids[2]->address;
            }
            expr->first = expr->kids[0]->first;*/
            //expr->address = expr->first;
         break;
         case DOTDOT:
            
         break;
         case '=':
            expr->first = expr->kids[0]->first;
            //expr->address = expr->kids[0]->address;
         break;
         case '(':
            if (expr->kids[0]->leaf != NULL && expr->kids[0]->leaf->category == IDENT) {
               expr->address = create_addr_offset(R_LOCAL);
               //expr->kids[0]->first = expr->kids[0]->address;
               //t->address = t->kids[0]->address;
               expr->first = expr->kids[2]->first;
               
            }
         break;
         case '[':
            //printf("%d\n", expr->kids[0]->address->u.offset + (expr->kids[2]->leaf->ival * 8));
            expr->first = create_addr_spec(R_LOCAL, expr->kids[0]->address->u.offset + (expr->kids[2]->leaf->ival * 8));
            expr->address = expr->first;
            //printf("%d\n", expr->kids[0]->address->u.offset + (expr->kids[2]->leaf->ival * 8));
            
            //expr->first = expr->kids[0]->first;
            //expr->first = expr->address;
            
         break;
      }
   }

   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) {
      switch (expr->kids[0]->leaf->category) {
         case '!':
            /*if (expr->kids[1]->leaf != NULL && expr->kids[1]->leaf->category == IDENT) {

            } else {

            }*/
            expr->address = create_addr_offset(R_LOCAL);
            expr->first = expr->kids[1]->first;
         break;
         case '(':
            expr->first = expr->kids[1]->first;
            expr->address = copyaddr(expr->kids[1]->address);
         break;
         case '[':
            gen_array_first(expr->kids[1]);
            expr->first = expr->kids[1]->first;
         break;
         case RETURN:
            expr->first = expr->kids[1]->first;
            //expr->address = expr->kids[1]->address;
         break;
         case '=':
            expr->first = expr->kids[1]->first;
            expr->address = expr->kids[1]->address;
         break;
         case '-':
            expr->first = expr->kids[1]->first;
            expr->address = create_addr_offset(R_LOCAL);
         break;
      }
   }


   return 0;
}

void gen_array_first(tree_t *t) {
   int i;
   if (t->nkids > 0) {
      for(i=0; i<20; i++) {
         if (t->kids[i] != NULL) {
            gen_array_first(t->kids[i]);
         }
      }
   }
   if (t->kids[1] != NULL &&  t->kids[1]->leaf != NULL) {
      switch (t->kids[1]->leaf->category) {
         case ',':
            if (t->kids[0]->leaf != NULL) {
               if (t->kids[0]->address != NULL) {
                  free(t->kids[0]->address);
                  t->kids[0]->address = NULL;
               }
               t->kids[0]->address = create_addr_offset(R_LOCAL);
               t->kids[0]->first = t->kids[0]->address;
            }

            if (t->kids[2]->leaf != NULL) {
               if (t->kids[2]->address != NULL) {
                  free(t->kids[2]->address);
                  t->kids[2]->address = NULL;
               }
               t->kids[2]->address = create_addr_offset(R_LOCAL);
               t->kids[2]->first = t->kids[2]->address;
            }
            t->first = t->kids[0]->first;
            //expr->address = expr->first;
         break;
      }
   }
}

/*void gen_funcparams_first(tree_t *t) {
   int i;
   if (t->nkids > 0) {
      for(i=0; i<20; i++) {
         if (t->kids[i] != NULL) {
            gen_funcparams_first(t->kids[i]);
         }
      }
   }
}*/

void assign_follow(struct tree *t)
{  
   int i;
   switch(t->prodrule) {

      /*case StmtSeqFromStatement1Statement2Prodrule: {
         t->kids[0].follow = t->kids[1].first;
      }

      case IfThenStmtProdrule: {
         t->kids[4].follow = t->follow;
      }*/
      /* ??? */
      /* all switch cases that DO need a label: */
      //case YYSYMBOL_item_fn:
      case YYSYMBOL_let:
      break;
      case YYSYMBOL_expr_for:
         //t.follow = t->kids[5].first;
      break;
      case YYSYMBOL_expr_if:
         if (t->kids[4] != NULL && (t->kids[4]->prodrule == YYSYMBOL_expr_if || t->kids[4]->prodrule == YYSYMBOL_block)) {
            t->kids[4]->follow = t->follow;
            t->follow = t->kids[4]->first;
         }
      break;
      //case YYSYMBOL_expr_if_let:
      case YYSYMBOL_expr_while:

      break;

      //case YYSYMBOL_expr_while_let:
      case YYSYMBOL_expr:
      case YYSYMBOL_nonblock_expr:
         gen_expr_follow(t);
      break;
      //case YYSYMBOL_exprs:
      //case YYSYMBOL_expr_nostruct:
      //case YYSYMBOL_maybe_init_expr:
      //case YYSYMBOL_block_expr_dot: 
      //case YYSYMBOL_item_macro:
      //case YYSYMBOL_macro_expr:
      case YYSYMBOL_stmt:
         if (t->kids[0] != NULL) {
            t->kids[0]->follow = t->follow;
         } else {
            t->kids[1]->follow = t->follow;
         }
      break;
      case YYSYMBOL_stmts:
         if (t->kids[0]->prodrule == YYSYMBOL_stmt) {
            //printf("dfgbdirugbeignd\n");
            t->kids[0]->follow = t->kids[1]->first;
            //printf("%d\n", t->kids[0]->follow->u.offset);
         } else if (t->kids[0]->prodrule == YYSYMBOL_stmts) {
            t->kids[0]->kids[1]->follow = t->kids[1]->first;
            //printf("thingy: %d\n", t->kids[0]->kids[1]->follow->u.offset);
         }
      break;
      case YYSYMBOL_inner_attrs_and_block:
         //t->first = t->kids[2]->first;

         //printf("fffffffffff: %d\n", t->first->region);
      break;
      case YYSYMBOL_block:
         t->follow = t->kids[1]->first;
      break;
         
         //t->first = create_addr(R_GLOBAL, "thing");
         //printf("hi: %s\n", t->symbolname);
      break;
   }
   
   if (t->nkids > 0) {
      for(i=0; i<20; i++) {
         if (t->kids[i] != NULL) {
            assign_follow(t->kids[i]);
         }
      }
   }

}

int gen_expr_follow(tree_t *expr) {

   //printf("%d : %d\n", left, right);
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case '<':
         case '>':
         case LE:
         case GE:
         case NE:
         case EQEQ:
            
         break;
         case ANDAND:
         case OROR:
            
         break;
         case PLUSEQ:
         case MINUSEQ:
         case '=':
            expr->kids[2]->follow = expr->follow;
         break;
         case '+':
         case '-':
         case '/':
         case '*':
         case '%':

            if (expr->address->u.offset == expr->first->u.offset) {

            } else if (expr->kids[0] != NULL && expr->kids[0]->nkids > 0) {
               expr->kids[0]->follow = expr->address;
            }
            //expr->icode = concat()
            //expr->first = create_addr_offset(R_LOCAL);
            
         break;
         case ',':
            
         break;
         case DOTDOT:
            
         break;
         case '(':
            
         break;
         case '.':
         

         break;
      }
   }

   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) {
      switch (expr->kids[0]->leaf->category) {
         case '!':
         
         break;
         case '(':
         case '[':
         case RETURN:
         case '=':
         
         break;
      }
   }


   return 0;
}

void assign_truefalse(struct tree *t)
{  
   int i;
   switch(t->prodrule) {

      /*case StmtSeqFromStatement1Statement2Prodrule: {
         t->kids[0].follow = t->kids[1].first;
      }

      case IfThenStmtProdrule: {
         t->kids[4].follow = t->follow;
      }*/
      /* ??? */
      /* all switch cases that DO need a label: */
      //case YYSYMBOL_item_fn:
      case YYSYMBOL_let:
      break;
      case YYSYMBOL_expr_for:
         //t.follow = t->kids[5].first;
      break;
      case YYSYMBOL_expr_if:
         //printf("ergdsgwesdfs\n");
         if (t->kids[2] != NULL) {
            t->kids[1]->onTrue = t->kids[2]->first;
         }
         if (t->follow != NULL) {
            t->kids[1]->onFalse = t->follow;
         } else {
            t->kids[1]->onFalse = create_addr_name(R_NONE, "");
         }
      break;
      //case YYSYMBOL_expr_if_let:
      case YYSYMBOL_expr_while:
         if (t->kids[3] != NULL) {
            if (t->kids[2]->address != NULL) {
               t->kids[2]->onTrue = t->kids[3]->first;
            } else {
               t->kids[2]->onTrue = t->kids[2]->first;
            }
         }
         if (t->follow != NULL) {
            t->kids[2]->onFalse = t->follow;
         } else {
            t->kids[2]->onFalse = create_addr_name(R_NONE, "");
         }
      break;

      //case YYSYMBOL_expr_while_let:
      case YYSYMBOL_expr:
      case YYSYMBOL_nonblock_expr:
         gen_expr_truefalse(t);
      break;
      //case YYSYMBOL_exprs:
      //case YYSYMBOL_expr_nostruct:
      //case YYSYMBOL_maybe_init_expr:
      //case YYSYMBOL_block_expr_dot: 
      //case YYSYMBOL_item_macro:
      //case YYSYMBOL_macro_expr:
      case YYSYMBOL_stmt:

      break;
      case YYSYMBOL_stmts:

      break;
      case YYSYMBOL_inner_attrs_and_block:
         //t->first = t->kids[2]->first;

         //printf("fffffffffff: %d\n", t->first->region);
      break;
      case YYSYMBOL_block:

      break;
         
         //t->first = create_addr(R_GLOBAL, "thing");
         //printf("hi: %s\n", t->symbolname);
      break;
   }
   
   if (t->nkids > 0) {
      for(i=0; i<20; i++) {
         if (t->kids[i] != NULL) {
            assign_truefalse(t->kids[i]);
         }
      }
   }

}

int gen_expr_truefalse(tree_t *expr) {

   //printf("%d : %d\n", left, right);
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case '<':
         case '>':
         case LE:
         case GE:
         case NE:
         case EQEQ:
            
         break;
         case ANDAND:
         case OROR:
            
         break;
         case PLUSEQ:
         case MINUSEQ:
         case '=':
            
         break;
         case '+':
         case '-':
         case '/':
         case '*':
         case '%':

            
         break;
         case ',':
            
         break;
         case DOTDOT:
            
         break;
         case '(':
            
         break;
         case '.':
         

         break;
      }
   }

   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) {
      switch (expr->kids[0]->leaf->category) {
         case '!':
         
         break;
         case '(':
         case '[':
         case RETURN:
         case '=':
         
         break;
      }
   }


   return 0;
}

void gencode_break(tree_t *tree) {
   int i = 0;
    if (tree == NULL) {
      return;
   }

    //SymbolTableEntry sym_touse = NULL;

   //printf("%s\n", tree->symbolname);

   if (tree->nkids > 0) {
      for(i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            gencode_break(tree->kids[i]);
         }
      }
   }

   switch(tree->prodrule) {
      case YYSYMBOL_expr_for:
         take_a_break(tree->kids[5], tree->follow);
      break;
      case YYSYMBOL_expr_while:
         take_a_break(tree->kids[3], tree->follow);
      break;
      case YYSYMBOL_expr_if:
         take_a_break(tree->kids[2], tree->follow);
      break;
   }

}

void gencode(tree_t *tree) {
   
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
            gencode(tree->kids[i]);
         }
      }
   }
   //printf("%s\n", tree->symbolname);

   switch(tree->prodrule) {
      case YYSYMBOL_item_fn:
         
         SymbolTableEntry func = lookup(tree->kids[1]->leaf->text, tree->symtab);

         if (tree->kids[3]->kids[0] != NULL) {
            //func->paramcode = tree->kids[3]->kids[0]->icode;
            tree->kids[5]->icode = concat(tree->kids[3]->kids[0]->icode, tree->kids[5]->icode);
         }

         if (tree->kids[5]->icode != NULL) {
            //printf("got the code!!!!!!%d\n", tree->symtab);
            tree->icode = tree->kids[5]->icode;
            func->icode = tree->kids[5]->icode;
         }
         if (func->typ->u.f.returntype->basetype != NULL_TYPE) {
            
            struct addr *last_addr = NULL;

            if (tree->kids[5]->kids[2]->prodrule == YYSYMBOL_stmt) {
               last_addr = tree->kids[5]->kids[2]->address;
            } else if (tree->kids[5]->kids[2]->prodrule == YYSYMBOL_stmts) {
               last_addr = tree->kids[5]->kids[2]->kids[1]->address;
            } else {
               last_addr = tree->kids[5]->kids[2]->address;
            }

            if (last_addr != NULL) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            }

            /*if (func->typ->u.f.returntype->basetype == STRING_TYPE) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else if (func->typ->u.f.returntype->basetype == CHAR_TYPE) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else if (func->typ->u.f.returntype->basetype == FLOAT_TYPE) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else if (func->typ->u.f.returntype->basetype == INT_TYPE) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else if (func->typ->u.f.returntype->basetype == BOOL_TYPE) {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else {
               tree->icode = concat(tree->icode, gen(O_RET, last_addr, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            }*/



            func->icode = tree->icode;
         } else {
            tree->icode = concat(tree->icode, gen(O_RET, create_addr_ival(R_CONST, 0), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            func->icode = tree->icode;
         }
      break;
      case YYSYMBOL_let:
         if (tree->kids[3] != NULL) {
            struct instr *g = NULL;
            SymbolTableEntry entry = NULL;
            if (tree->kids[1]->nkids > 0) {
               entry = lookup(tree->kids[1]->kids[1]->leaf->text, tree->symtab);
            } else {
               entry = lookup(tree->kids[1]->leaf->text, tree->symtab);
            }

            

            if (entry->typ->basetype == ARRAY_TYPE) {
               //printf("entry: %d\n", entry->typ->basetype);
               //g = gen(O_ASN, *tree->kids[1]->first, *tree->kids[3]->kids[1]->address, *create_addr_name(R_NONE, ""));
               //expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               //tree->icode = concat(tree->icode, g);

               tree->icode = concat(tree->icode, tree->kids[3]->icode);
               //tacprint(tree->kids[3]->icode);
               //printf("\n");
            } else {
               if (tree->kids[3]->icode != NULL) {
                  tree->icode = concat(tree->icode, tree->kids[3]->icode);
               }

               if (tree->kids[3] != NULL) {
                  struct addr *a2;

                  if ((tree->kids[3]->kids[1] != NULL && tree->kids[3]->kids[1]->leaf != NULL) || 
                  (tree->kids[3]->kids[1] != NULL && tree->kids[3]->kids[1]->kids[1] != NULL && 
                  ((tree->kids[3]->kids[1]->kids[1]->leaf != NULL && tree->kids[3]->kids[1]->kids[1]->leaf->category == '[') ||
                  (tree->kids[3]->kids[1]->kids[0]->leaf != NULL && tree->kids[3]->kids[1]->kids[0]->leaf->category == '(')))) {
                     //printf("sefueygwuefw\n");
                     a2 = tree->kids[3]->address;
                  } else {
                     a2 = copyaddr(tree->kids[3]->address);
                  }
                  g = gen(O_ASN, tree->kids[1]->first, a2, create_addr_name(R_NONE, ""));
                  tree->icode = concat(tree->icode, g);
               
               }
               //g = gen(O_ASN, tree->kids[1]->first, tree->kids[3]->kids[1]->address, create_addr_name(R_NONE, ""));
               //tree->icode = concat(tree->icode, g);
            }
         }
      break;
      case YYSYMBOL_item_static:
         if (tree->kids[3] != NULL) {
            struct instr *g = NULL;
            SymbolTableEntry entry = NULL;
            entry = lookup(tree->kids[1]->leaf->text, tree->symtab);

            

            if (entry->typ->basetype == ARRAY_TYPE) {
               //printf("entry: %d\n", entry->typ->basetype);
               //g = gen(O_ASN, *tree->kids[1]->first, *tree->kids[3]->kids[1]->address, *create_addr_name(R_NONE, ""));
               //expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               //tree->icode = concat(tree->icode, g);

               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               //tacprint(tree->kids[3]->icode);
               //printf("\n");
            } else {
               g = gen(O_ASN, tree->kids[1]->first, tree->kids[5]->address, create_addr_name(R_NONE, ""));
               //expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, g);
            }
         }
      break;
      case YYSYMBOL_expr_for:
         if (tree->kids[2] != NULL) {
            struct addr *label1 = create_addr_label(R_LABEL);
            struct addr *label2 = create_addr_label(R_LABEL);
            struct addr *label3 = create_addr_label(R_LABEL);
            if (tree->kids[4]->leaf != NULL) {
               /*SymbolTableEntry array = lookup(tree->kids[4]->leaf->text, tree->symtab); 
               struct addr *counter = create_addr_offset_spec(R_LOCAL, tree->kids[4]->address->u.offset);
               tree->icode = concat(tree->icode, gen(O_ASN, *tree->kids[2]->address, *counter, *create_addr_name(R_NONE, "")));
               struct addr *expr = create_addr_offset(R_LOCAL);
               tree->icode = concat(tree->icode, gen(O_BLT, *expr, *create_addr_ival(R_CONST, counter->u.offset), *create_addr_ival(R_CONST, tree->kids[4]->address->u.offset + (array->typ->u.a.nelements * 8))));
               tree->icode = concat(tree->icode, gen(O_BIF, *expr, *create_addr_name(R_NONE, ""), *create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, gen(O_ADD, *counter, *counter, *create_addr_ival(R_CONST, 8)));
               tree->icode = concat(tree->icode, gen(O_GOTO, *tree->kids[2]->first, *create_addr_name(R_NONE, ""), *create_addr_name(R_NONE, "")));*/
               SymbolTableEntry array = lookup(tree->kids[4]->leaf->text, tree->symtab); 
               //struct addr *counter = create_addr_offset_spec(R_LOCAL, tree->kids[4]->address->u.offset);
               struct addr *counter = create_addr_offset(R_LOCAL);
               tree->icode = concat(tree->icode, gen(O_ASN, counter, create_addr_ival(R_CONST, tree->kids[4]->address->u.offset), create_addr_name(R_NONE, "")));
               struct addr *expr = create_addr_offset(R_LOCAL);

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_BLE, expr, create_addr_spec(counter->region, counter->u.offset), create_addr_ival(R_CONST, tree->kids[4]->address->u.offset + (array->typ->u.a.nelements * 8) - 8)));
               tree->icode = concat(tree->icode, gen(O_OFFASN, tree->kids[2]->address, create_addr_spec(counter->region, counter->u.offset), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BIF, create_addr_spec(expr->region, expr->u.offset), label2, create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BNIF, create_addr_spec(expr->region, expr->u.offset), label3, create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, gen(O_ADD, create_addr_spec(counter->region, counter->u.offset), create_addr_spec(counter->region, counter->u.offset), create_addr_ival(R_CONST, 8)));
               tree->icode = concat(tree->icode, gen(O_GOTO, label1, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));



            } else if (tree->kids[4]->kids[1] != NULL && tree->kids[4]->kids[1]->leaf != NULL && tree->kids[4]->kids[1]->leaf->category == DOTDOT) {
               tree->icode = concat(tree->icode, gen(O_ASN, tree->kids[2]->address, create_addr_ival(R_CONST, tree->kids[4]->kids[0]->leaf->ival), create_addr_name(R_NONE, "")));
               struct addr *expr = create_addr_offset(R_LOCAL);
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_BLT, expr, create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_ival(R_CONST, tree->kids[4]->kids[2]->leaf->ival)));
               tree->icode = concat(tree->icode, gen(O_BIF, create_addr_spec(expr->region, expr->u.offset), label2, create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BNIF, create_addr_spec(expr->region, expr->u.offset), label3, create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, gen(O_ADD, create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_ival(R_CONST, 1)));
               tree->icode = concat(tree->icode, gen(O_GOTO, label1, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            
            } else if (tree->kids[4]->kids[1] != NULL && tree->kids[4]->kids[1]->leaf != NULL && tree->kids[4]->kids[1]->leaf->category == '=') {
               tree->icode = concat(tree->icode, gen(O_ASN, tree->kids[2]->address, create_addr_ival(R_CONST, tree->kids[4]->kids[0]->kids[0]->leaf->ival), create_addr_name(R_NONE, "")));
               struct addr *expr = create_addr_offset(R_LOCAL);
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_BLE, expr, create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_ival(R_CONST, tree->kids[4]->kids[2]->leaf->ival)));
               tree->icode = concat(tree->icode, gen(O_BIF, create_addr_spec(expr->region, expr->u.offset), label2, create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BNIF, create_addr_spec(expr->region, expr->u.offset), label3, create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, gen(O_ADD, create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_spec(tree->kids[2]->address->region, tree->kids[2]->address->u.offset), create_addr_ival(R_CONST, 1)));
               tree->icode = concat(tree->icode, gen(O_GOTO, label1, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            } else if (tree->kids[4]->kids[1] != NULL && tree->kids[4]->kids[1]->prodrule == YYSYMBOL_exprs) {
               //printf("sjdfsiebgwueifbw\n");
               int num_eles = len_eles(tree->kids[4]->kids[1]);
               //printf("%d\n\n", num_eles);
               //tree->icode = concat(tree->icode, gen(O_ADDR, tree->kids[2]->address, create_addr_spec(tree->kids[4]->first->region, tree->kids[4]->first->u.offset), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[4]->icode);

               struct addr *counter = create_addr_offset(R_LOCAL);
               tree->icode = concat(tree->icode, gen(O_ASN, counter, create_addr_ival(R_CONST, tree->kids[4]->first->u.offset), create_addr_name(R_NONE, "")));
               struct addr *expr = create_addr_offset(R_LOCAL);

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BLT, expr, create_addr_spec(counter->region, counter->u.offset), create_addr_ival(R_CONST, tree->kids[4]->first->u.offset + (num_eles * 8))));
               tree->icode = concat(tree->icode, gen(O_OFFASN, tree->kids[2]->address, create_addr_spec(counter->region, counter->u.offset), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BIF, create_addr_spec(expr->region, expr->u.offset), label2, create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_BNIF, create_addr_spec(expr->region, expr->u.offset), label3, create_addr_name(R_NONE, "")));

               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, tree->kids[5]->icode);
               tree->icode = concat(tree->icode, gen(O_ADD, create_addr_spec(counter->region, counter->u.offset), create_addr_spec(counter->region, counter->u.offset), create_addr_ival(R_CONST, 8)));
               tree->icode = concat(tree->icode, gen(O_GOTO, label1, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
               tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            }
            
         }
         //printf("break search\n");
      break;
      case YYSYMBOL_expr_if:
         if (tree->kids[4]->icode != NULL) {
            struct addr *a = NULL;
            struct addr *label1 = create_addr_label(R_LABEL);
            struct addr *label2 = create_addr_label(R_LABEL);
            struct addr *label3 = create_addr_label(R_LABEL);

            if ((tree->kids[1] != NULL && tree->kids[1]->leaf != NULL) || 
               (tree->kids[1] != NULL && tree->kids[1]->kids[1] != NULL && 
               ((tree->kids[1]->kids[1]->leaf != NULL && tree->kids[1]->kids[1]->leaf->category == '[') ||
               (tree->kids[1]->kids[0]->leaf != NULL && tree->kids[1]->kids[0]->leaf->category == '('))) ||
               (tree->kids[1] != NULL && tree->kids[1]->address != NULL)) {
               //printf("siufsieygsuef\n");
               a = tree->kids[1]->address;
               //printf("%d\n", a->u.offset);
            } else {
               a = copyaddr(tree->kids[1]->address);
            }

            //gen(O_BNIF, *create_addr_name(R_NONE, ""), *tree->kids[1]->first, *create_addr_name(R_NONE, ""));
            if (tree->kids[1]->icode != NULL) {
               tree->icode = concat(tree->icode, tree->kids[1]->icode);
            }
            //tree->icode = concat(tree->icode, gen(O_BIF, a, create_addr_spec(tree->kids[1]->onTrue->region, tree->kids[1]->onTrue->u.offset), create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_BIF, a, label1, create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), label2, create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, tree->kids[2]->icode);
            


            if (tree->kids[1]->onFalse != NULL && tree->kids[1]->onFalse->region == R_NONE) {
               //tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), tree->kids[1]->onFalse, create_addr_name(R_NONE, "")));
            } else {
               //printf("11111111\n");
               //tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), create_addr_spec(tree->kids[1]->onFalse->region, tree->kids[1]->onFalse->u.offset), create_addr_name(R_NONE, "")));
            }

            tree->icode = concat(tree->icode, gen(O_GOTO, label3, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            
            if (tree->kids[4] != NULL) {
               tree->icode = concat(tree->icode, tree->kids[4]->icode);
            }

            tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
         }
      break;
      case YYSYMBOL_expr_if_let:
            
      break;
      case YYSYMBOL_expr_while:
         //printf("while\n");
         if (tree->kids[3] != NULL) {
            //gen(O_BNIF, *create_addr_name(R_NONE, ""), *tree->kids[1]->first, *create_addr_name(R_NONE, ""));
            struct addr *a = NULL;
            //struct addr *f = NULL;
            struct addr *label1 = create_addr_label(R_LABEL);
            struct addr *label2 = create_addr_label(R_LABEL);
            struct addr *label3 = create_addr_label(R_LABEL);


            if ((tree->kids[2] != NULL && tree->kids[2]->leaf != NULL) || 
               (tree->kids[2] != NULL && tree->kids[2]->kids[1] != NULL && 
               ((tree->kids[2]->kids[1]->leaf != NULL && tree->kids[2]->kids[1]->leaf->category == '[') ||
               (tree->kids[2]->kids[0]->leaf != NULL && tree->kids[2]->kids[0]->leaf->category == '('))) ||
               (tree->kids[1] != NULL && tree->kids[1]->address != NULL)) {
               //printf("siufsieygsuef\n");
               a = tree->kids[2]->address;
               //f = copyaddr(tree->kids[2]->address);
            } else {
               a = copyaddr(tree->kids[2]->address);
               //f = copyaddr(tree->kids[2]->first);
            }



            tree->icode = concat(tree->icode, gen(O_LCONT, label3, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            if (tree->kids[2]->icode != NULL) {
               tree->icode = concat(tree->icode, tree->kids[2]->icode);
            }

            tree->icode = concat(tree->icode, gen(O_BIF, a, label1, create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), label2, create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label1), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, tree->kids[3]->icode);
            tree->icode = concat(tree->icode, gen(O_GOTO, copyaddr(label3), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
            tree->icode = concat(tree->icode, gen(O_LCONT, copyaddr(label2), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));

            //tree->icode = concat(tree->icode, gen(O_GOTO, f, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));

            /*if (tree->kids[2]->onFalse->region != R_NONE) {
               tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), create_addr_spec(tree->kids[2]->onFalse->region, tree->kids[2]->onFalse->u.offset), create_addr_name(R_NONE, "")));
            } else {
               tree->icode = concat(tree->icode, gen(O_BNIF, copyaddr(a), tree->kids[2]->onFalse, create_addr_name(R_NONE, "")));
            }*/
         }
      break;
      case YYSYMBOL_expr_while_let:
            
      break;
      case YYSYMBOL_nonblock_prefix_expr_nostruct:
      case YYSYMBOL_maybe_init_expr:
      case YYSYMBOL_exprs:
      case YYSYMBOL_expr_nostruct:
      case YYSYMBOL_expr:
      case YYSYMBOL_nonblock_expr:
      case YYSYMBOL_nonblock_prefix_expr:
         gencode_expr(tree);
         //tacprint(tree->icode);
         //printf("\n");
         
      break;
      case YYSYMBOL_block_expr_dot:
         
      break;
      case YYSYMBOL_item_macro:
      case YYSYMBOL_macro_expr:
         tree->icode = concat(tree->icode, tree->kids[3]->icode);
         //tacprint(tree->kids[3]->icode);
         //printf("\n");
         tree->icode = concat(tree->icode, gencode_println(tree, tree->address, tree->kids[0]->leaf->text));
      break;
      case YYSYMBOL_stmt:
         if (tree->kids[0] != NULL && tree->kids[0]->icode != NULL) {
            
            tree->icode = concat(tree->icode, tree->kids[0]->icode);
         } else if (tree->kids[1] != NULL && tree->kids[1]->icode != NULL) {
            
            tree->icode = concat(tree->icode, tree->kids[1]->icode);
         }
         //tacprint(tree->icode);
         //printf("\n\n");
      break;
      case YYSYMBOL_stmts:
         tree->icode = concat(tree->kids[0]->icode, tree->kids[1]->icode);
      break;
      case YYSYMBOL_param:
         if (tree->kids[0] != NULL && tree->kids[0]->address != NULL) {
            tree->icode = gen(O_ADDR, tree->kids[0]->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, ""));
         }
         //tacprint(tree->icode);
         //printf("\n\n");
      break;
      case YYSYMBOL_params:
         tree->icode = concat(tree->kids[0]->icode, tree->kids[2]->icode);
      break;
      case YYSYMBOL_fn_params:
         if (tree->kids[1] != NULL) {
            tree->icode = concat(tree->icode, tree->kids[1]->icode);
         }
      break;
      case YYSYMBOL_inner_attrs_and_block:
         tree->icode = concat(tree->icode, tree->kids[2]->icode);
         //printf("ffergergergf: %d\n", tree->first->region);
      break;
      case YYSYMBOL_block:
         if (tree->kids[1] != NULL) {
            tree->icode = concat(tree->icode, tree->kids[1]->icode);
         }
      break;
      case YYSYMBOL_token_trees:
         if (tree->kids[0] != NULL && tree->kids[1] != NULL) {
            tree->icode = concat(tree->kids[0]->icode, tree->kids[1]->icode);
         } else if (tree->kids[0] == NULL && tree->kids[1] != NULL) {
            tree->icode = concat(tree->icode, tree->kids[1]->icode);
         } else if (tree->kids[0] != NULL && tree->kids[1] == NULL) {
            tree->icode = concat(tree->icode, tree->kids[0]->icode);
         }
      break;
      case YYSYMBOL_parens_delimited_token_trees:
         if (tree->kids[1] != NULL) {
            tree->icode = concat(tree->icode, tree->kids[1]->icode);
         }
      break;
      case YYSYMBOL_ty_prim:
         if (tree->kids[3] != NULL && tree->kids[3]->address != NULL) {
            free(tree->kids[3]->address);
         }
      break;
      default:
         //printf("%d : %s\n", tree->prodrule, tree->symbolname);
      break;
   }

   if (tree->leaf != NULL && strcmp(tree->leaf->text, "i64") != 0 && strcmp(tree->leaf->text, "f64") != 0 && strcmp(tree->leaf->text, "char") != 0 && strcmp(tree->leaf->text, "String") != 0 && strcmp(tree->leaf->text, "bool") != 0 ) {
      switch (tree->leaf->category) {
         case LIT_STR:
            //t->first = create_addr(R_STR, t->leaf->text);
            //t->address = create_addr_name(R_STR, tree->leaf->sval);
            //t->first = t->address;
            tree->icode = gen(O_ADDR, create_addr_spec(R_LOCAL, tree->address->u.offset), tree->address, create_addr_name(R_NONE, ""));
            break;
      }
   }

   return;
}

int gencode_expr(tree_t *expr) {
   struct instr *g = NULL;
   struct addr *a1 = NULL;
   struct addr *a2 = NULL;
   //printf("%d : %d\n", left, right);
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case '<':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BLT, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case '>':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BGT, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case LE:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BLE, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case GE:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BGE, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case NE:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }

            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BNEQ, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case EQEQ:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_BEQ, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case ANDAND:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }

            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_AND, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case OROR:
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }

            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_OR, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case PLUSEQ:
            if (expr->kids[0]->first != NULL) {
               if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
                  expr->icode = concat(expr->icode, expr->kids[0]->icode);
               }
               
               if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
                  (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
                  ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
                  (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
                  a2 = expr->kids[2]->address;
               } else {
                  a2 = copyaddr(expr->kids[2]->address);
               }
               g = gen(O_ADD, expr->kids[0]->first, create_addr_spec(expr->kids[0]->first->region, expr->kids[0]->first->u.offset), a2);
               expr->icode = concat(expr->icode, g);
            }
         break;
         case MINUSEQ:
            if (expr->kids[0]->first != NULL) {
               if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
                  expr->icode = concat(expr->icode, expr->kids[0]->icode);
               }

               if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
                  (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
                  ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
                  (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
                  a2 = expr->kids[2]->address;
               } else {
                  a2 = copyaddr(expr->kids[2]->address);
               }
               g = gen(O_SUB, expr->kids[0]->first, create_addr_spec(expr->kids[0]->first->region, expr->kids[0]->first->u.offset), a2);
               expr->icode = concat(expr->icode, g);
            }
         break;
         case '=':
            if (expr->kids[0]->first != NULL) {
               if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
                  expr->icode = concat(expr->icode, expr->kids[2]->icode);
               } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
                  expr->icode = concat(expr->icode, expr->kids[0]->icode);
               }
               
               if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
                  (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
                  ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
                  (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
                  a2 = expr->kids[2]->address;
               } else {
                  a2 = copyaddr(expr->kids[2]->address);
               }
               g = gen(O_ASN, expr->kids[0]->first, a2, create_addr_name(R_NONE, ""));
               expr->icode = concat(expr->icode, g);


               
            }
         break;
         case '+':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }

            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_ADD, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);


            /*if (expr->address->u.offset == expr->first->u.offset) {
               g = gen(O_ADD, expr->address, expr->kids[0]->address, expr->kids[2]->address);
               expr->icode = concat(expr->icode, g);
            } else {
               //g = gen(O_ADD, expr->address, create_addr_offset_spec(expr->kids[0]->address->region, expr->kids[0]->address->u.offset), create_addr_offset_spec(expr->kids[2]->address->region, expr->kids[2]->address->u.offset));
               g = gen(O_ADD, expr->address, copyaddr(expr->kids[0]->address), copyaddr(expr->kids[2]->address));
               expr->icode = concat(expr->icode, g);
            }*/
            //g = gen(O_ADD, expr->address, create_addr_offset_spec(expr->kids[0]->address->region, expr->kids[0]->address->u.offset), create_addr_offset_spec(expr->kids[2]->address->region, expr->kids[2]->address->u.offset));
         break;
         case '-':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_SUB, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case '/':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_DIV, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case '*':
            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_MUL, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
         break;
         case '%':

            if (expr->kids[0]->icode != NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->kids[0]->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode == NULL && expr->kids[2]->icode != NULL) {
               expr->icode = concat(expr->icode, expr->kids[2]->icode);
            } else if (expr->kids[0]->icode != NULL && expr->kids[2]->icode == NULL) {
               expr->icode = concat(expr->icode, expr->kids[0]->icode);
            }
            if ((expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) || 
               (expr->kids[0] != NULL && expr->kids[0]->kids[1] != NULL && 
               ((expr->kids[0]->kids[1]->leaf != NULL && expr->kids[0]->kids[1]->leaf->category == '[') ||
               (expr->kids[0]->kids[0]->leaf != NULL && expr->kids[0]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[0]->address;
            } else {
               a1 = copyaddr(expr->kids[0]->address);
            }

            if ((expr->kids[2] != NULL && expr->kids[2]->leaf != NULL) || 
               (expr->kids[2] != NULL && expr->kids[2]->kids[1] != NULL && 
               ((expr->kids[2]->kids[1]->leaf != NULL && expr->kids[2]->kids[1]->leaf->category == '[') ||
               (expr->kids[2]->kids[0]->leaf != NULL && expr->kids[2]->kids[0]->leaf->category == '(')))) {
               a2 = expr->kids[2]->address;
            } else {
               a2 = copyaddr(expr->kids[2]->address);
            }

            g = gen(O_MOD, expr->address, a1, a2);
            expr->icode = concat(expr->icode, g);
            
         break;
         case ',':
            //if (expr->type->basetype == INT_TYPE) {
               /*if (expr->kids[0]->prodrule == YYSYMBOL_exprs) {
                  if (expr->kids[2]->type->basetype == INT_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == FLOAT_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_dval(R_CONST, expr->kids[2]->leaf->dval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == STRING_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == CHAR_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == BOOL_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  }
               } else if (expr->kids[0]->nkids < 2) {
                  if (expr->kids[2]->type->basetype == INT_TYPE && expr->kids[0]->type->basetype == INT_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_ival(R_CONST, expr->kids[0]->leaf->ival), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == FLOAT_TYPE && expr->kids[0]->type->basetype == FLOAT_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_dval(R_CONST, expr->kids[0]->leaf->dval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_dval(R_CONST, expr->kids[2]->leaf->dval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == STRING_TYPE && expr->kids[0]->type->basetype == STRING_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_name(R_STR, expr->kids[0]->leaf->sval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == CHAR_TYPE && expr->kids[0]->type->basetype == CHAR_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_name(R_STR, expr->kids[0]->leaf->sval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == BOOL_TYPE && expr->kids[0]->type->basetype == BOOL_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_ival(R_CONST, expr->kids[0]->leaf->ival), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  }
               }*/
            //}

            /*if (expr->kids[0]->address != NULL) {
               expr->icode = concat(gen(O_ASN, *expr->kids[0]->address, *create_addr_ival(R_CONST, expr->kids[0]->address->val_i), *create_addr_name(R_NONE, "")), gen(O_ASN, *expr->kids[2]->address, *create_addr_ival(R_CONST, expr->kids[2]->address->val_i), *create_addr_name(R_NONE, "")));
            }*/
         break;
         case DOTDOT:
            if (expr->kids[0] != NULL) {
               free(expr->kids[0]->address);
            }
            if (expr->kids[2] != NULL) {
               free(expr->kids[2]->address);
            }
         break;
         case '(':
         /*tree->icode = concat(tree->icode, tree->kids[3]->icode);
         //tacprint(tree->kids[3]->icode);
         //printf("\n");
         if (strcmp(tree->kids[0]->leaf->text, "println") == 0) {
            //printf("skjnsidbsiugbwiew\n");
            tree->icode = concat(tree->icode, gencode_println(tree, tree->kids[0]->address));
         }*/


            SymbolTableEntry func = lookup(expr->kids[0]->leaf->text, expr->symtab);
            //tacprint(func->icode);
            //expr->address = expr->kids[0]->address;
            expr->first = expr->kids[2]->first;
            expr->icode = func_gencode(expr, func->s, expr->address);
            //expr->icode = concat(expr->icode, func->icode);
            
         break;
         case '[':
            //SymbolTableEntry func = lookup(expr->kids[0]->leaf->text, expr->symtab);
            //tacprint(func->icode);
            //expr->address = expr->kids[0]->address;
            
            free(expr->kids[2]->address);
            
         break;
         case '.':
         

         break;
      }
   }

   if (expr->kids[0] != NULL && expr->kids[0]->leaf != NULL) {
      switch (expr->kids[0]->leaf->category) {
         case '!':
            expr->icode = concat(expr->icode, expr->kids[1]->icode);

            if ((expr->kids[1] != NULL && expr->kids[1]->leaf != NULL) || 
               (expr->kids[1] != NULL && expr->kids[1]->kids[1] != NULL && 
               ((expr->kids[1]->kids[1]->leaf != NULL && expr->kids[1]->kids[1]->leaf->category == '[') ||
               (expr->kids[1]->kids[0]->leaf != NULL && expr->kids[1]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[1]->address;
            } else {
               a1 = copyaddr(expr->kids[1]->address);
            }

            g = gen(O_BNE, expr->address, a1, create_addr_name(R_NONE, ""));
            expr->icode = concat(expr->icode, g);
         break;
         case '(':
            expr->icode = concat(expr->icode, expr->kids[1]->icode);
            //expr->icode = expr->kids[1]->icode;
         break;
         case '[':
            gencode_array(expr->kids[1]);
            expr->icode = concat(expr->icode, expr->kids[1]->icode);
         break;
         case RETURN:
            expr->icode = gen(O_RET, expr->kids[1]->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, ""));
         break;
         case '=':
            expr->icode = concat(expr->icode, expr->kids[1]->icode);
         break;
         case '-':
            expr->icode = concat(expr->icode, expr->kids[1]->icode);
            
            if ((expr->kids[1] != NULL && expr->kids[1]->leaf != NULL) || 
               (expr->kids[1] != NULL && expr->kids[1]->kids[1] != NULL && 
               ((expr->kids[1]->kids[1]->leaf != NULL && expr->kids[1]->kids[1]->leaf->category == '[') ||
               (expr->kids[1]->kids[0]->leaf != NULL && expr->kids[1]->kids[0]->leaf->category == '(')))) {
               
               a1 = expr->kids[1]->address;
            } else {
               a1 = copyaddr(expr->kids[1]->address);
            }

            g = gen(O_NEG, expr->address, a1, create_addr_name(R_NONE, ""));
            expr->icode = concat(expr->icode, g);
         break;
      }
   }


   return 0;
}

void gencode_array(tree_t *expr) {
   int i;
   if (expr->nkids > 0) {
      for(i=0; i<20; i++) {
         if (expr->kids[i] != NULL) {
            gencode_array(expr->kids[i]);
         }
      }
   }
   if (expr->kids[1] != NULL &&  expr->kids[1]->leaf != NULL) {
      switch (expr->kids[1]->leaf->category) {
         case ',':
            if (expr->kids[0]->prodrule == YYSYMBOL_exprs) {
                  if (expr->kids[2]->type->basetype == INT_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == FLOAT_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_dval(R_CONST, expr->kids[2]->leaf->dval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == STRING_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == CHAR_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == BOOL_TYPE) {
                     expr->icode = concat(expr->kids[0]->icode, gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  }
               } else if (expr->kids[0]->nkids < 2) {
                  if (expr->kids[2]->type->basetype == INT_TYPE && expr->kids[0]->type->basetype == INT_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_ival(R_CONST, expr->kids[0]->leaf->ival), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == FLOAT_TYPE && expr->kids[0]->type->basetype == FLOAT_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_dval(R_CONST, expr->kids[0]->leaf->dval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_dval(R_CONST, expr->kids[2]->leaf->dval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == STRING_TYPE && expr->kids[0]->type->basetype == STRING_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_name(R_STR, expr->kids[0]->leaf->sval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == CHAR_TYPE && expr->kids[0]->type->basetype == CHAR_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_name(R_STR, expr->kids[0]->leaf->sval), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_name(R_STR, expr->kids[2]->leaf->sval), create_addr_name(R_NONE, "")));
                  } else if (expr->kids[2]->type->basetype == BOOL_TYPE && expr->kids[0]->type->basetype == BOOL_TYPE) {
                     expr->icode = concat(gen(O_ASN, expr->kids[0]->address, create_addr_ival(R_CONST, expr->kids[0]->leaf->ival), create_addr_name(R_NONE, "")), gen(O_ASN, expr->kids[2]->address, create_addr_ival(R_CONST, expr->kids[2]->leaf->ival), create_addr_name(R_NONE, "")));
                  }
               }
         break;
      }
   }
}

struct instr *gencode_println(tree_t *t, struct addr *address, char *name) {
   struct instr *list = NULL;
   struct instr *conc = NULL;
   //conc = concat(gen(O_PARM, *create_addr_name(R_STR, ""), *create_addr_name(R_NONE, ""), *create_addr_name(R_NONE, "")), conc);

   list = println_params_tostr(t->kids[3], t->first, list, conc);

   //list =concat(list, conc);
   list = concat(list, gen(O_CALL, address, create_addr_name(R_NAME, name), create_addr_ival(R_CONST, linked_len(list))));

   t->kids[0]->icode = list;
   //tacprint(list);
   return list;
}

struct instr *println_params_tostr(tree_t *tree, struct addr *first, struct instr *list, struct instr *conc) {
   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL && (tree->kids[i]->prodrule == YYSYMBOL_token_trees || tree->kids[i]->address != NULL)) {
            list = println_params_tostr(tree->kids[i], first, list, conc);
         }
      }
   }
   
   if (tree->address != NULL /*&& tree->address->u.offset != first->u.offset*/) {
      //struct addr *newaddr = create_addr_offset(R_LOCAL);

      //list = concat(gen(O_CALL, *newaddr, *create_addr_name(R_NAME, "tostr"), *create_addr_ival(R_CONST, 1)), list);
      if (tree->leaf != NULL && tree->leaf->category == IDENT) {
         list = concat(list, gen(O_PARM, tree->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
      } else {
         list = concat(list, gen(O_PARM, create_addr_spec(tree->address->region, tree->address->u.offset), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
      }
      //create_addr_spec(tree->address->region, tree->address->u.offset)
      //conc = concat(gen(O_PARM, *newaddr, *create_addr_name(R_NONE, ""), *create_addr_name(R_NONE, "")), conc);
      //conc = concat(gen(O_PARM, *create_addr_name(R_STR, ""), *create_addr_name(R_NONE, ""), *create_addr_name(R_NONE, "")), conc);
   }
   //tacprint(list);
   //printf("call %s", tree->symbolname);
   //printf("\n");

   return list;
}

struct instr *func_gencode(tree_t *t, char *name, struct addr *address) {
   struct instr *list = NULL;
   struct instr *conc = NULL;
   list = params_gencode(t->kids[2], t->first, list, conc);

   //list =concat(list, conc);
   list = concat(list, gen(O_CALL, address, create_addr_name(R_NAME, name), create_addr_ival(R_CONST, linked_len(list))));
   //tacprint(list);
   return list;
}

struct instr *params_gencode(tree_t *tree, struct addr *first, struct instr *list, struct instr *conc) {
   if (tree->prodrule == YYSYMBOL_exprs) {
      list = concat(params_gencode(tree->kids[0], first, list, conc), params_gencode(tree->kids[2], first, list, conc));
   }

   if (tree->address != NULL) {
      //list = concat(list, gen(O_PARM, tree->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
      struct addr *a1 = NULL;
      if ((tree != NULL && tree->leaf != NULL) || 
         (tree != NULL && tree->kids[1] != NULL && (tree->kids[1]->leaf->category == '['))) {
         a1 = tree->address;
      } else {
         //printf("sidubsirugbie %s\n", expr->kids[2]->symbolname);
         a1 = copyaddr(tree->address);
      }

      list = concat(gen(O_PARM, a1, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")), list);
   }
   //printf("%s\n", tree->symbolname);
   //tacprint(list);
   //printf("call %s", tree->symbolname);
   //printf("\n");

   return list;
}

struct instr *params_gencode_decl(tree_t *tree, struct instr *list_in) 
{
   if (list_in == NULL) {
      return NULL;
   }
   struct instr *list = list_in;
   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL && tree->kids[i] != NULL) {
            list = concat(list, params_gencode_decl(tree->kids[i], list));
         }
      }
   }

   if (tree->address != NULL) {
      //list = concat(list, gen(O_PARM, tree->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
      if (tree->leaf != NULL && tree->leaf->category == IDENT && tree->address != NULL) {
         list = concat(list, gen(O_ADDR, tree->address, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, "")));
      }
   }
   //printf("%s\n", tree->symbolname);
   //tacprint(list);
   //printf("call %s", tree->symbolname);
   //printf("\n");

   return list;
}

void treeprint_addresses_first(tree_t *t, int depth)
{
   if (t == NULL) {
      return;
   }
   int i;
    
   if(t->symbolname != NULL && t->nkids > 0) {
      printf("%*s BRANCH-%s: %d", depth*2, " ", t->symbolname, t->nkids);
      if (t->address != NULL) {
         if (t->address->region == R_STR) {
            printf("                       address: %d", t->address->u.offset);
         } else if (t->address->region == R_CONST) {
            printf("                       address: %d", t->address->val_i);
         } else if (t->address->region == R_FLOAT) {
            printf("                       address: %f", t->address->val_d);
         } else {
            printf("                       address: %d", t->address->u.offset);
            printf(" value: %d", t->address->val_i);
         }
      }
      if (t->first != NULL) {
         //printf("thinggggggg, %d\n", t->first->region);
         if (t->first->region == R_STR) {
            printf("                       first: %d", t->first->u.offset);
         } else if (t->first->region == R_CONST) {
            printf("                       first: %d", t->first->val_i);
         } else if (t->first->region == R_FLOAT) {
            printf("                       first: %f", t->first->val_d);
         } else {
            printf("                       first: %d", t->first->u.offset);
            printf(" value: %d", t->first->val_i);
         }
      }
      if (t->follow != NULL) {
         if (t->follow->region == R_STR) {
            printf("                       follow: %d", t->follow->u.offset);
         } else if (t->follow->region == R_CONST) {
            printf("                       follow: %d", t->follow->val_i);
         } else if (t->follow->region == R_FLOAT) {
            printf("                       follow: %f", t->follow->val_d);
         } else {
            printf("                       follow: %d", t->follow->u.offset);
            printf(" value: %d", t->follow->val_i);
         }
      }
      printf("\n");

   }

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
               treeprint_addresses_first(t->kids[i], depth+1);
            }
        }
    } else {
         if (t->leaf != NULL && t->symbolname != NULL) {
            printf("%*s LEAF-%s: %s-%d", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
            if (t->address != NULL) {
               if (t->address->region == R_STR) {
                  printf("                       address: %d", t->address->u.offset);
               } else if (t->address->region == R_CONST) {
                  printf("                       address: %d", t->address->val_i);
               } else if (t->address->region == R_FLOAT) {
                  printf("                       address: %f", t->address->val_d);
               } else {
                  printf("                       address: %d", t->address->u.offset);
                  printf(" value: %d", t->address->val_i);
               }
            }
            if (t->first != NULL) {
               if (t->first->region == R_STR) {
                  printf("                       first: %d", t->first->u.offset);
               } else if (t->first->region == R_CONST) {
                  printf("                       first: %d", t->first->val_i);
               } else if (t->first->region == R_FLOAT) {
                  printf("                       first: %f", t->first->val_d);
               } else {
                  printf("                       first: %d", t->first->u.offset);
               }
            }
            if (t->follow != NULL) {
               if (t->follow->region == R_STR) {
                  printf("                       follow: %d", t->follow->u.offset);
               } else if (t->follow->region == R_CONST) {
                  printf("                       follow: %d", t->follow->val_i);
               } else if (t->follow->region == R_FLOAT) {
                  printf("                       follow: %f", t->follow->val_d);
               } else {
                  printf("                       follow: %d", t->follow->u.offset);
               }
            }
            
            printf("\n");
         }
    }
   
}

void treeprint_icode(tree_t *t, int depth)
{
   int i;
    
   if(t->symbolname != NULL && t->nkids > 0) {
      printf("%*s BRANCH-%s: %d", depth*2, " ", t->symbolname, t->nkids);
      if (t->icode != NULL) {
         printf("                       has icode"); 
      }
      printf("\n");

   }

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
               treeprint_icode(t->kids[i], depth+1);
            }
        }
    } else {
         if (t->leaf != NULL && t->symbolname != NULL) {
            printf("%*s LEAF-%s: %s-%d", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
            if (t->icode != NULL) {
               printf("                       has icode"); 
            }
            
            printf("\n");
         }
    }
   
}

void treeprint_isog(tree_t *t, int depth)
{
   int i;
    
   if(t->symbolname != NULL && t->nkids > 0) {
      printf("%*s BRANCH-%s: %d", depth*2, " ", t->symbolname, t->nkids);
      if (t->address != NULL) {
         printf("                       has icode"); 
      }
      printf("\n");

   }

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
               treeprint_icode(t->kids[i], depth+1);
            }
        }
    } else {
         if (t->leaf != NULL && t->symbolname != NULL) {
            printf("%*s LEAF-%s: %s-%d", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
            if (t->address != NULL) {
               printf("                       has icode"); 
            }
            
            printf("\n");
         }
    }
   
}

void treeprint_truefalse(tree_t *t, int depth)
{
   int i;
    
   if(t->symbolname != NULL && t->nkids > 0) {
      printf("%*s BRANCH-%s: %d", depth*2, " ", t->symbolname, t->nkids);
      if (t->onTrue != NULL) {
         if (t->onTrue->region == R_STR) {
            printf("                       onTrue: %s", t->onTrue->u.name);
         } else if (t->onTrue->region == R_CONST) {
            printf("                       onTrue: %d", t->onTrue->val_i);
         } else if (t->onTrue->region == R_FLOAT) {
            printf("                       onTrue: %f", t->onTrue->val_d);
         } else {
            printf("                       onTrue: %d", t->onTrue->u.offset);
            printf(" value: %d", t->onTrue->val_i);
         }
      }
      if (t->onFalse != NULL) {
         if (t->onFalse->region == R_STR) {
            printf("                       onFalse: %s", t->onFalse->u.name);
         } else if (t->onFalse->region == R_CONST) {
            printf("                       onFalse: %d", t->onFalse->val_i);
         } else if (t->onFalse->region == R_FLOAT) {
            printf("                       onFalse: %f", t->onFalse->val_d);
         } else {
            printf("                       onFalse: %d", t->onFalse->u.offset);
            printf(" value: %d", t->onFalse->val_i);
         }
      }
      printf("\n");

   }

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
               treeprint_truefalse(t->kids[i], depth+1);
            }
        }
    } else {
         if (t->leaf != NULL && t->symbolname != NULL) {
            printf("%*s LEAF-%s: %s-%d", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
            if (t->onTrue != NULL) {
               if (t->onTrue->region == R_STR) {
                  printf("                       onTrue: %s", t->onTrue->u.name);
               } else if (t->onTrue->region == R_CONST) {
                  printf("                       onTrue: %d", t->onTrue->val_i);
               } else if (t->onTrue->region == R_FLOAT) {
                  printf("                       onTrue: %f", t->onTrue->val_d);
               } else {
                  printf("                       onTrue: %d", t->onTrue->u.offset);
                  printf(" value: %d", t->onTrue->val_i);
               }
            }
            if (t->onFalse != NULL) {
               if (t->onFalse->region == R_STR) {
                  printf("                       onFalse: %s", t->onFalse->u.name);
               } else if (t->onFalse->region == R_CONST) {
                  printf("                       onFalse: %d", t->onFalse->val_i);
               } else if (t->onFalse->region == R_FLOAT) {
                  printf("                       onFalse: %f", t->onFalse->val_d);
               } else {
                  printf("                       onFalse: %d", t->onFalse->u.offset);
                  printf(" value: %d", t->onFalse->val_i);
               }
            }
            
            printf("\n");
         }
    }
   
}

void tacprint (struct instr *head) 
{
    struct instr *ptr = head;
    

   while(ptr != NULL) {
      printf("%s\t", opcodename(ptr->opcode));

      if (ptr->dest != NULL && ptr->dest->region == R_NAME) {
         printf("%s", ptr->dest->u.name);
      } else if (ptr->dest != NULL && ptr->dest->region == R_NONE) {

      } else if (ptr->dest == NULL) {
         printf(",NULL");
      } else {
         if (ptr->dest != NULL && ptr->dest->region == R_STR) {
            printf("%s:%d", regionname(ptr->dest->region), ptr->dest->u.offset);
         } else if (ptr->dest != NULL && ptr->dest->region == R_CONST) {
            printf("%s:%d", regionname(ptr->dest->region), ptr->dest->val_i);
         } else {
            printf("%s:%d", regionname(ptr->dest->region), ptr->dest->u.offset);
         }
      }

      if (ptr->src1 != NULL && ptr->src1->region == R_NAME) {
         printf(",%s", ptr->src1->u.name);
      } else if (ptr->src1 != NULL && ptr->src1->region == R_NONE) {

      } else if (ptr->src1 == NULL) {
         printf(",NULL");
      } else {
         if (ptr->src1 != NULL && ptr->src1->region == R_STR) {
            printf(",%s:%d", regionname(ptr->src1->region), ptr->src1->u.offset);
         } else if (ptr->src1 != NULL && ptr->src1->region == R_CONST) {
            printf(",%s:%d", regionname(ptr->src1->region), ptr->src1->val_i);
         } else {
            printf(",%s:%d", regionname(ptr->src1->region), ptr->src1->u.offset);
         }
      }

      if (ptr->src2 != NULL && ptr->src2->region == R_NAME) {
         printf(",%s", ptr->src2->u.name);
      } else if (ptr->src2->region == R_NONE) {

      }  else if (ptr->src2 == NULL) {
         printf(",NULL");
      } else {
         if (ptr->src2 != NULL && ptr->src2->region == R_STR) {
            printf(",%s:%d", regionname(ptr->src2->region), ptr->src2->u.offset);
         } else if (ptr->src2 != NULL && ptr->src2->region == R_CONST) {
            printf(",%s:%d", regionname(ptr->src2->region), ptr->src2->val_i);
         } else {
            printf(",%s:%d", regionname(ptr->src2->region), ptr->src2->u.offset);
         }
      }
      printf("\n");

      ptr = ptr->next;
   }
}

int linked_len(struct instr *list) {
   struct instr *ptr = list;
   int num = 0;

   while (ptr != NULL) {
      ptr = ptr->next;
      num++;
   }

   return num;
}

void take_a_break (tree_t *tree, struct addr *follow_a) {
   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            take_a_break(tree->kids[i], follow_a);
         }
      }
   }

   if (tree->leaf != NULL && tree->leaf->category == BREAK) {
      //printf("break");
      if (follow_a != NULL) {
         tree->icode = gen(O_GOTO, follow_a, create_addr_name(R_NONE, ""), create_addr_name(R_NONE, ""));
      } else {
         tree->icode = gen(O_RET, create_addr_ival(R_CONST, 0), create_addr_name(R_NONE, ""), create_addr_name(R_NONE, ""));
      }
      free(tree->address);
   }
}

int len_eles(tree_t *tree) {
   int num = 0;
   
   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL && tree->prodrule == YYSYMBOL_exprs) {
            num += len_eles(tree->kids[i]);
         }
      }
   }

   if (tree->kids[0] != NULL && tree->kids[0]->leaf != NULL) {
      num++;
   }

   if (tree->kids[2] != NULL && tree->kids[2]->leaf != NULL) {
      num++;
   }
   return num;

}

void write_icode_func(FILE *filename, SymbolTable symtab)
{
   FILE *fptr = filename;
   //printf("%s\n", filename);

   SymbolTable curr = symtab;

   fprintf(fptr, ".code\n");

    while (curr != NULL) {
         for (int i = 0; i < 16; i++) {
            SymbolTableEntry entry = curr->tbl[i];

            while (entry != NULL) {
               if(entry->typ != NULL && entry->typ->basetype == FUNC_TYPE && entry->is_builtin == 0) {
                  //printf("kdnsdvbirbdud\n");
                  fprintf(fptr, "%s:\n", entry->s);

                  struct instr *ptr = entry->icode;

                  while(ptr != NULL) {
                     fprintf(fptr, "\t%s\t\t\t", opcodename(ptr->opcode));

                     if (ptr->dest->region == R_NAME) {
                        fprintf(fptr, "%s", ptr->dest->u.name);
                     } else if (ptr->dest->region == R_NONE) {

                     } else {
                        if (ptr->dest->region == R_STR) {
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->u.offset);
                        } else if (ptr->dest->region == R_CONST) {
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->val_i);
                        } else {
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->u.offset);
                        }
                     }

                     if (ptr->src1->region == R_NAME) {
                        fprintf(fptr, " %s", ptr->src1->u.name);
                     } else if (ptr->src1->region == R_NONE) {

                     } else {
                        if (ptr->src1->region == R_STR) {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        } else if (ptr->src1->region == R_CONST) {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->val_i);
                        } else {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        }
                     }

                     if (ptr->src2->region == R_NAME) {
                        fprintf(fptr, " %s", ptr->src2->u.name);
                     } else if (ptr->src2->region == R_NONE) {

                     } else {
                        if (ptr->src2->region == R_STR) {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        } else if (ptr->src2->region == R_CONST) {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->val_i);
                        } else {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        }
                     }
                     fprintf(fptr, "\n");

                     ptr = ptr->next;
                  }

                  /*struct instr *params = entry->paramcode;

                  while(params != NULL) {
                     fprintf(fptr, "\t%s\t\t\t", opcodename(params->opcode));

                     if (params->dest->region == R_NAME) {
                        fprintf(fptr, "%s", params->dest->u.name);
                     } else if (params->dest->region == R_NONE) {

                     } else {
                        if (params->dest->region == R_STR) {
                           fprintf(fptr, "%s:%d", regionname(params->dest->region), params->dest->u.offset);
                        } else if (params->dest->region == R_CONST) {
                           fprintf(fptr, "%s:%d", regionname(params->dest->region), params->dest->val_i);
                        } else {
                           fprintf(fptr, "%s:%d", regionname(params->dest->region), params->dest->u.offset);
                        }
                     }

                     if (ptr->src1->region == R_NAME) {
                        fprintf(fptr, ",%s", ptr->src1->u.name);
                     } else if (ptr->src1->region == R_NONE) {

                     } else {
                        if (ptr->src1->region == R_STR) {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        } else if (ptr->src1->region == R_CONST) {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src1->region), ptr->src1->val_i);
                        } else {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        }
                     }

                     if (ptr->src2->region == R_NAME) {
                        fprintf(fptr, ",%s", ptr->src2->u.name);
                     } else if (ptr->src2->region == R_NONE) {

                     } else {
                        if (ptr->src2->region == R_STR) {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        } else if (ptr->src2->region == R_CONST) {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src2->region), ptr->src2->val_i);
                        } else {
                           fprintf(fptr, ",%s:%d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        }
                     }
                     fprintf(fptr, "\n");

                     ptr = ptr->next;
                  }*/

                  free_istruct(entry->icode);




               }
               entry = entry->next;
            }
         }
         //print_symtable(curr);
      curr = NULL;
   }
   return;
}

void fprint_string_num(FILE *filename) {
   fprintf(filename, " %d\n", num_strings);
   
   return;
}

void write_icode_string(FILE *filename,  tree_t *tree)
{
   FILE *fptr = filename;

   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            write_icode_string(filename, tree->kids[i]);
         }
      }
   }
   
   if (tree->leaf != NULL && (tree->leaf->category == LIT_STR || tree->leaf->category == LIT_CHAR)) {
      char *string  = tree->leaf->text;
      fprintf(fptr, "\t");

      for (int j = 1; j < strlen(string) - 1; j++) {
         if (string[j] == '\\') {
            /*fprintf(fptr, "\\%o", string[j]);
            j++;
            fprintf(fptr, "\\%o", string[j]);*/
            fprintf(fptr, "%c", string[j]);
         } else if (string[j] == '{') {
            fprintf(fptr, "%%s");
            j++;
         } else {
            fprintf(fptr, "%c", string[j]);
         }
      }
      fprintf(fptr, "\n");
   }

   return;
}

void write_icode_globals(FILE *filename,  tree_t *tree)
{
   FILE *fptr = filename;

   if (tree->nkids > 0) {
      for(int i=0; i<20; i++) {
         if (tree->kids[i] != NULL) {
            write_icode_globals(filename, tree->kids[i]);
         }
      }
   }
   
   if (tree->prodrule == YYSYMBOL_item_static) {
      
      fprintf(fptr, "\t");

      struct instr *ptr = tree->icode;
      struct instr *old_ptr = tree->icode;

                  while(ptr != NULL) {
                     fprintf(fptr, "\t%s\t\t\t", opcodename(ptr->opcode));

                     if (ptr->dest->region == R_NAME) {
                        fprintf(fptr, "%s", ptr->dest->u.name);
                     } else if (ptr->dest->region == R_NONE) {

                     } else {
                        if (ptr->dest->region == R_STR) {
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->u.offset);
                        } else if (ptr->dest->region == R_CONST) {
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->val_i);
                        } else {
                           ptr->dest->region = R_GLOBAL;
                           fprintf(fptr, "%s %d", regionname(ptr->dest->region), ptr->dest->u.offset);
                        }
                     }
                     
                     

                     if (ptr->src1->region == R_NAME) {
                        fprintf(fptr, " %s", ptr->src1->u.name);
                     } else if (ptr->src1->region == R_NONE) {

                     } else {
                        if (ptr->src1->region == R_STR) {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        } else if (ptr->src1->region == R_CONST) {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->val_i);
                        } else {
                           fprintf(fptr, " %s %d", regionname(ptr->src1->region), ptr->src1->u.offset);
                        }
                     }
                     

                     if (ptr->src2->region == R_NAME) {
                        fprintf(fptr, " %s", ptr->src2->u.name);
                     } else if (ptr->src2->region == R_NONE) {

                     } else {
                        if (ptr->src2->region == R_STR) {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        } else if (ptr->src2->region == R_CONST) {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->val_i);
                        } else {
                           fprintf(fptr, " %s %d", regionname(ptr->src2->region), ptr->src2->u.offset);
                        }
                     }
                     fprintf(fptr, "\n");

                     free(ptr->dest);
                     free(ptr->src1);
                     free(ptr->src2);
                     
                     old_ptr = ptr;
                     ptr = ptr->next;
                     free(old_ptr);
                  }
   }

   return;
}

void free_istruct(struct instr *ic) 
{
   struct instr *list = ic;

   int i = 0;

   while (list != NULL) {
      //free(&list->dest);

      //free(&list->src1);
      //free(&list->src2);
      //tacprint(list);
      //printf("\n");
      i++;
      
      if (list->dest != NULL) {
         //printf("%d ", list->dest->region);
         free(list->dest);
      }
      if (list->src1 != NULL) {
         //printf("%d ", list->src1->region);
         free(list->src1);
      }
      if (list->src2 != NULL) {
         //printf("%d ", list->src2->region);
         free(list->src2);
      }
      //printf("%d\n", i);

      struct instr *to_free = list;

      list = list->next;
      free(to_free);
   }
}