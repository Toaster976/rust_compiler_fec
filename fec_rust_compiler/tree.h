#include <stdio.h>
struct sym_table;
struct typeinfo;

typedef struct tree {
   int id;
   int prodrule;
   char *symbolname;
   int nkids;
   struct tree *kids[20]; /* if nkids >0 */
   struct token *leaf;   /* if nkids == 0; NULL for ε productions */
   struct sym_table *symtab;
   struct typeinfo *type;

   struct addr *address;
   struct addr *first;
   struct addr *follow;
   struct addr *onTrue;
   struct addr *onFalse;
   struct instr *icode;

} tree_t;

typedef struct token {
   int category;   /* the integer code returned by yylex */
   char *text;     /* the actual string (lexeme) matched */
   int lineno;     /* the line number on which the token occurs */
   char *filename; /* the source file in which the token occurs */
   int ival;       /* for integer constants, store binary value here */
   double dval;	   /* for real constants, store binary value here */
   char *sval;     /* for string constants, malloc space, de-escape, store */
                   /*    the string (less quotes and after escapes) here */
} token_t;

/* Used for HW1*/
/*typedef struct tokenlist {
      struct token *t;
      struct tokenlist *next;
} node_t;*/

/*-------------------Tree and token functions----------------------*/
tree_t* init_tree(char *sym_name);
char *get_symname(int category);
int alctoken(int category);
void print_node(tree_t *tree);
tree_t* create_tree(int prod_rule, char* sym_name, tree_t* children[], int num_kids);
tree_t* alctree(int prod_rule, char* sym_name, int argc, ...);
void print_tree(tree_t * tree, int depth);
void treeprint(struct tree *t, int depth);
int free_tree(tree_t* root);

/*-------------------Graphing functions----------------------*/
char *escape(char *s);
char *pretty_print_name(struct tree *t);
void print_branch(struct tree *t, FILE *f);
void print_leaf(struct tree *t, FILE *f);
void print_graph2(struct tree *t, FILE *f);
void print_graph(struct tree *t, char *filename);

void printsymbol(char *s);
void printsyms(tree_t *t);