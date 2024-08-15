typedef struct sym_table {
   int nBuckets;			/* # of buckets */
   int nEntries;			/* # of symbols in the table */
   char *scope;         /* Scope name */
   int level;           /* Nesting level */
   char *scope_sym;         /* Scope symbol */
   struct sym_table *parent;		 /*enclosing scope, superclass etc. */
   struct sym_entry **tbl;
   /* more per-scope/per-symbol-table attributes go here */
   } *SymbolTable;

typedef struct sym_entry {
   SymbolTable table;			 /*what symbol table do we belong to*/
   char *s;				/* string */
   /* more symbol attributes go here for code generation */
   char *filename;
   int lineno;
   int is_const;
   int is_static;
   int is_mut;
   int is_builtin;

   struct instr *icode;
   struct instr *paramcode;

   struct addr *address;
   
   struct typeinfo *typ;

   SymbolTable nested; 

   struct sym_entry *next;
   } *SymbolTableEntry;

/*-------------------Symbol functions----------------------*/

int hash(SymbolTable st, char *s);
SymbolTable init_symtable (int nbuckets, char *scope, char *symname);
void fill_symtable(tree_t *tree, SymbolTable symtab);
SymbolTable enter_newscope(int nbuckets, SymbolTable parent, char *scope, char *symname);
SymbolTableEntry insertsymbol(SymbolTable symtab, tree_t *tree, SymbolTable nest);
void print_symtable(SymbolTable symtab);
SymbolTableEntry lookup(char *name, SymbolTable st);

SymbolTableEntry insertname(SymbolTable symtab, char *text);
void insert_inbuilt(SymbolTable symtab);
void free_symtab (SymbolTable symtab);
SymbolTableEntry fill_pat(tree_t *tree, SymbolTable symtab);

/*-------------------Error functions----------------------*/

void error_semantic(const char *error, char *filename, int lineno, char *name);
void error_undeclared(token_t *tok, SymbolTable symtab);
void error_redeclared(token_t *tok);