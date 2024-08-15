#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ytab.h"
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

/*-------------------Symbol functions----------------------*/

int hash(SymbolTable st, char *s)
{
   register int h = 0;
   register char c;
   while ((c = *s++)) {
      h += c & 0377;
      h *= 37;
      }
   if (h < 0) h = -h;
   return h % st->nBuckets;
}

SymbolTable init_symtable (int nbuckets, char *scope, char *symname) {
    SymbolTable new_symtable = malloc(sizeof(struct sym_table));

    new_symtable->nBuckets = nbuckets;
    new_symtable->nEntries = 0;
    new_symtable->level = 0;

    new_symtable->parent = NULL;

    new_symtable->scope = malloc((strlen(scope) + 1)*sizeof(char));
    strcpy(new_symtable->scope, scope);
    if (symname != NULL) {
        new_symtable->scope_sym = malloc((strlen(symname) + 1)*sizeof(char));
        strcpy(new_symtable->scope_sym, symname);
    } else {
        new_symtable->scope_sym = NULL;
    }

    new_symtable->tbl = malloc(nbuckets * sizeof(struct sym_entry *));

    for (int i = 0; i < nbuckets; i++) {
        new_symtable->tbl[i] = NULL;
    }

    return new_symtable;
}

SymbolTable enter_newscope(int nbuckets, SymbolTable parent, char *scope, char *symname) {
    SymbolTable newst = init_symtable(nbuckets, scope, symname);
    newst->level = parent->level + 1;
    newst->parent = parent;

    return newst;
}



SymbolTableEntry insertsymbol(SymbolTable symtab, tree_t *tree, SymbolTable nest) {
    if(symtab == NULL || tree == NULL)
        return NULL;
        
    char *name = tree->leaf->text;
    int index = hash(symtab, name);
    SymbolTableEntry prev = NULL;
    SymbolTableEntry e = symtab->tbl[index];

    // Search for symbol if it already exists
    while (e != NULL) {
        prev = e;
        if (strcmp(e->s, name) == 0) {
            error_redeclared(tree->leaf);
        }
        e = e->next;
    }

    SymbolTableEntry entry = malloc(sizeof(struct sym_entry));
    entry->s = strdup(name);
    entry->filename = strdup(tree->leaf->filename);
    entry->lineno = tree->leaf->lineno;
    entry->table = symtab;
    entry->nested = nest;
    entry->next = NULL;

    entry->is_const = 0;
    entry->is_static = 0;
    entry->is_mut = 0;
    entry->is_builtin = 0;

    entry->typ = NULL;
    entry->address = NULL;
    entry->icode = NULL;
    
    // Attach to symbol table
    if(prev != NULL)
        prev->next = entry;
    else
        symtab->tbl[index] = entry;

    symtab->nEntries++;
    //printf("  %s\n", entry->s);

    return entry;
}

SymbolTableEntry insertname(SymbolTable symtab, char *text) {
    if(symtab == NULL)
        return NULL;
        
    char *name = text;
    int index = hash(symtab, name);
    SymbolTableEntry prev = NULL;
    SymbolTableEntry e = symtab->tbl[index];

    // Search for symbol if it already exists and return it
    while (e != NULL) {
        prev = e;

        e = e->next;
    }

    SymbolTableEntry entry = malloc(sizeof(struct sym_entry));
    entry->s = strdup(name);
    entry->filename = NULL;
    entry->lineno = 0;
    entry->table = symtab;
    entry->nested = NULL;
    entry->next = NULL;

    entry->typ = NULL;

    entry->is_const = 0;
    entry->is_static = 0;
    entry->is_mut = 0;
    entry->is_builtin = 1;
    entry->icode = NULL;
    entry->paramcode = NULL;
    entry->address = NULL;
    
    // Attach to symbol table
    if(prev != NULL)
        prev->next = entry;
    else
        symtab->tbl[index] = entry;

    symtab->nEntries++;
    //printf("  %s\n", entry->s);
    return entry;
}



void fill_symtable(tree_t *tree, SymbolTable symtab) {

    int i = 0;
    if (tree == NULL || symtab == NULL) {
        return;
    }

    SymbolTableEntry sym_touse = NULL;
    tree->symtab = symtab;

    //printf("%s\n", tree->symbolname);

    switch(tree->prodrule) {
        case YYSYMBOL_meta_item:
            //printf("meta_item");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[0], NULL);
            }

        break;
        case YYSYMBOL_item_static:
            //printf("item_static");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
                if (tree->kids[3] != NULL && tree->kids[3]->leaf != NULL) {
                    sym_touse->typ = cmptype(tree->kids[3]->leaf->text);
                }
            }
            if (tree->kids[2]->leaf != NULL && tree->kids[2]->symbolname != NULL && tree->kids[2]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[2], NULL);
                if (tree->kids[4] != NULL && tree->kids[4]->leaf != NULL) {
                    sym_touse->typ = cmptype(tree->kids[4]->leaf->text);
                }
            }
            
        break;
        case YYSYMBOL_item_const:
            //printf("item_const");
            sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
        break;
        case YYSYMBOL_item_fn:
            //printf("item_fn");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                SymbolTable new_scope = enter_newscope(16, symtab, tree->kids[1]->leaf->text, tree->symbolname);
                sym_touse = insertsymbol(symtab, tree->kids[1], new_scope);
                
                sym_touse->typ = alcfunctype(tree->kids[3]->kids[1], tree->kids[3]->kids[0], symtab);

                if (tree->nkids > 0) {
                    for(i=0; i<20; i++) {
                        if (tree->kids[i] != NULL) {
                            fill_symtable(tree->kids[i], new_scope);
                        }
                    }
                }

                //print_symtable(new_scope);
                
            } else if (tree->kids[2]->leaf != NULL && tree->kids[2]->symbolname != NULL && tree->kids[2]->leaf->category == IDENT) {
                SymbolTable new_scope = enter_newscope(16, symtab, tree->kids[2]->leaf->text, tree->symbolname);
                sym_touse = insertsymbol(symtab, tree->kids[2], new_scope);

                sym_touse->typ = alcfunctype(tree->kids[4]->kids[1], tree->kids[4]->kids[0], symtab);
                
                if (tree->nkids > 0) {
                    for(i=0; i<20; i++) {
                        if (tree->kids[i] != NULL) {
                            
                            fill_symtable(tree->kids[i], new_scope);
                        }
                    }
                }
                //print_symtable(new_scope);
                

            }
            return;
        break;
        case YYSYMBOL_named_arg:
            //printf("named_arg");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[0], NULL);
            }
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
            }

            if (tree->kids[2] != NULL) {
                sym_touse->typ = cmptype(tree->kids[2]->kids[1]->leaf->text);
            }
        break;
        /*case YYSYMBOL_path_generic_args_without_colons:
            printf("path_generic_args_without_colons");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
            if (tree->kids[3]->leaf != NULL && tree->kids[3]->symbolname != NULL && tree->kids[3]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[3]);
            }
        break;*/
        /*case YYSYMBOL_pat:
            //printf("pat");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[0], NULL);
            }
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
                if (tree->kids[0]->prodrule == MUT) {
                    sym_touse->is_mut = 1;
                }
            }
            if (tree->nkids >= 6 && tree->kids[5]->leaf != NULL && tree->kids[5]->symbolname != NULL && tree->kids[5]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[5], NULL);
            }
            if (tree->nkids >= 10 && tree->kids[9]->leaf != NULL && tree->kids[9]->symbolname != NULL && tree->kids[9]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[9], NULL);
            }
        break;*/
        /*case YYSYMBOL_pat_field:
            printf("pat_field");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[1]);
            }
        break;
        case YYSYMBOL_path_generic_args_with_colons:
            printf("path_generic_args_with_colons");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
            if (tree->kids[2]->leaf != NULL && tree->kids[2]->symbolname != NULL && tree->kids[2]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[2]);
            }
        break;
        case YYSYMBOL_expr:
            printf("expr");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[1]);
            }
        break;
        case YYSYMBOL_expr_nostruct:
        printf("expr_nostruct");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[1]);
            }
        break;
        case YYSYMBOL_field_init:
            printf("field_init");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
        break;
        case YYSYMBOL_binding:
            printf("binding");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
        break;
        case YYSYMBOL_ty_param:
            printf("ty_param");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[0]);
            }
        break;*/
        case YYSYMBOL_param:
            //printf("param");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[0], NULL);
                sym_touse->typ = cmptype(tree->kids[2]->leaf->text);
            }
        break;
        case YYSYMBOL_let:
            //printf("let");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
                //printf("symbol: %s\n", tree->kids[2]->symbolname);
            } else {
                sym_touse = fill_pat(tree->kids[1], symtab);
            }

            if (tree->kids[2] != NULL && tree->kids[2]->kids[1]->prodrule == YYSYMBOL_ty_prim) {
                alcarraytype(sym_touse, tree->kids[2]->kids[1], tree->kids[3]);
            } else if (tree->kids[2] != NULL) {
                sym_touse->typ = cmptype(tree->kids[2]->kids[1]->leaf->text);
            }

        break;
        case YYSYMBOL_expr_for:
            //printf("for");
            if (tree->kids[2]->leaf != NULL && tree->kids[2]->symbolname != NULL && tree->kids[2]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[2], NULL);
            }
        break;
        /*case YYSYMBOL_expr_if:
            printf("expr_if");
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                insertsymbol(symtab, tree->kids[1]);
            }
        break;*/
        case YYSYMBOL_expr_if_let:
            //printf("expr_if_let");
            if (tree->kids[2]->leaf != NULL && tree->kids[2]->symbolname != NULL && tree->kids[2]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[2], NULL);
            }
        break;
        case YYSYMBOL_expr_while_let:
            //printf("expr_while_let");
            if (tree->kids[3]->leaf != NULL && tree->kids[3]->symbolname != NULL && tree->kids[3]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[3], NULL);
            }
        break;
    }
    if (tree->prodrule == IDENT) {
        //printf("%s\n", tree->leaf->text);
        
        error_undeclared(tree->leaf, symtab);
    }

    if (tree->nkids > 0) {
        for(i=0; i<20; i++) {
            if (tree->kids[i] != NULL) {
                fill_symtable(tree->kids[i], symtab);
            }
        }
    }

    return;
}

void print_symtable(SymbolTable symtab) {
    if (symtab == NULL) return;

    for(int j = 0; j < symtab->level; j++) {
        printf("   ");
    }

    printf("--- symbol table for: %s %s ---\n", symtab->scope_sym, symtab->scope);

    for (int i = 0; i < symtab->nBuckets; i++) {

        SymbolTableEntry entry = symtab->tbl[i];
        while (entry != NULL) {
            for(int j = 0; j < symtab->level; j++) {
                printf("   ");
            }
            printf("%d : %s ", i, entry->s);
            if(entry->typ != NULL) {
                printf(" type: %d", entry->typ->basetype);
            }
            printf("\n");

            if(entry->nested != NULL) {
                print_symtable(entry->nested);
            }
            
            entry = entry->next;
        }
    }    
}

SymbolTableEntry lookup(char *name, SymbolTable symtab) {   
    SymbolTable curr = symtab;

    while (curr != NULL) {
        int i = hash(curr, name);
        //print_symtable(curr);

        SymbolTableEntry entry = curr->tbl[i];

        while (entry != NULL) {
            if(strcmp(entry->s, name) == 0) {
                return entry;
            }
            entry = entry->next;
        }
        curr = curr->parent;
    }
    
    return NULL;
}


void insert_inbuilt(SymbolTable symtab) {
    SymbolTableEntry read = insertname(symtab, "read");
    SymbolTableEntry println = insertname(symtab, "println");
    SymbolTableEntry format = insertname(symtab, "format");
    insertname(symtab, "i8");
    insertname(symtab, "i16");
    insertname(symtab, "i32");
    insertname(symtab, "i64");
    insertname(symtab, "i128");
    insertname(symtab, "f32");
    insertname(symtab, "f64");
    insertname(symtab, "bool");
    insertname(symtab, "char");
    insertname(symtab, "String");

    read->is_builtin = 1;
    read->typ = alctype(FUNC_TYPE);
    read->typ->u.f.name = "read";
    read->typ->u.f.nparams = 1;
    read->typ->u.f.minparams = -1;
    read->typ->u.f.returntype = alctype(INT_TYPE);
    read->typ->u.f.st = NULL;
    read->typ->u.f.parameters = malloc(sizeof(struct param));
    read->typ->u.f.parameters->name = NULL;
    read->typ->u.f.parameters->next = NULL;
    read->typ->u.f.parameters->type = alctype(STRING_TYPE);

    println->is_builtin = 1;
    println->typ = alctype(FUNC_TYPE);
    println->typ->u.f.name = "println";
    println->typ->u.f.nparams = 1;
    println->typ->u.f.minparams = 1;
    println->typ->u.f.returntype = alctype(NULL_TYPE);
    println->typ->u.f.st = NULL;
    println->typ->u.f.parameters = malloc(sizeof(struct param));
    println->typ->u.f.parameters->name = NULL;
    println->typ->u.f.parameters->next = NULL;
    println->typ->u.f.parameters->type = alctype(STRING_TYPE);

    format->is_builtin = 1;
    format->typ = alctype(FUNC_TYPE);
    format->typ->u.f.name = "format";
    format->typ->u.f.nparams = 1;
    format->typ->u.f.minparams = 1;
    format->typ->u.f.returntype = alctype(NULL_TYPE);
    format->typ->u.f.st = NULL;
    format->typ->u.f.parameters = malloc(sizeof(struct param));
    format->typ->u.f.parameters->name = NULL;
    format->typ->u.f.parameters->next = NULL;
    format->typ->u.f.parameters->type = alctype(STRING_TYPE);

}

void free_symtab (SymbolTable symtab) {
    if (symtab == NULL) return;

    for (int i = 0; i < symtab->nBuckets; i++) {

        SymbolTableEntry entry = symtab->tbl[i];
        
        while (entry != NULL) {
            SymbolTableEntry next = entry->next;
            
            if(entry->nested != NULL) {
                free_symtab(entry->nested);
            }

            free(entry->s);
            free(entry->filename);
            free_typ(entry->typ);

            //free(entry->address);

            free(entry);
            

            entry = next;
        }
    }    
    if (symtab->tbl != NULL) {
        free(symtab->tbl);
    }
    if (symtab->scope != NULL) {
        free(symtab->scope);
    }
    if (symtab->scope_sym != NULL) {
        free(symtab->scope_sym);
    }
    free(symtab);
}

SymbolTableEntry fill_pat(tree_t *tree, SymbolTable symtab) {
    int i = 0;

    SymbolTableEntry sym_touse = NULL;

    if (tree == NULL || symtab == NULL) {
        return NULL;
    }

    //printf("%s\n", tree->symbolname);

    switch(tree->prodrule) {
        case YYSYMBOL_pat:
            //printf("pat");
            if (tree->kids[0]->leaf != NULL && tree->kids[0]->symbolname != NULL && tree->kids[0]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[0], NULL);
            }
            if (tree->kids[1]->leaf != NULL && tree->kids[1]->symbolname != NULL && tree->kids[1]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[1], NULL);
                if (tree->kids[0]->prodrule == MUT || tree->kids[0]->prodrule == YYSYMBOL_binding_mode ) {
                    sym_touse->is_mut = 1;
                }
            }
            if (tree->nkids >= 6 && tree->kids[5]->leaf != NULL && tree->kids[5]->symbolname != NULL && tree->kids[5]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[5], NULL);
            }
            if (tree->nkids >= 10 && tree->kids[9]->leaf != NULL && tree->kids[9]->symbolname != NULL && tree->kids[9]->leaf->category == IDENT) {
                sym_touse = insertsymbol(symtab, tree->kids[9], NULL);
            }
        break;
    }
    
    if (tree->nkids > 0) {
        for(i=0; i<20; i++) {
            if (tree->kids[i] != NULL) {
                fill_symtable(tree->kids[i], symtab);
            }
        }
    }

    return sym_touse;
}



/*---------------------- Error Functions ----------------------*/

void error_semantic(const char *error, char *filename, int lineno, char *name) {
	fprintf(stderr, "Semantic error: %s\nFilename: %s\nLineno: %d\ntoken: %s\n", error, filename, lineno, name);
	exit(3);
}

void error_undeclared(token_t *tok, SymbolTable symtab) {

    if (lookup(tok->text, symtab) == NULL) {
        error_semantic("Variable is undeclared", tok->filename, tok->lineno, tok->text);
    }
}

void error_redeclared(token_t *tok) {
    error_semantic("Variable is redeclared", tok->filename, tok->lineno, tok->text);
}