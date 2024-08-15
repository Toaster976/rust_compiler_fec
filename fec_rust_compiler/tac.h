/*
 * Three Address Code - skeleton for CSE 423
 */
#ifndef TAC_H
#define TAC_H

struct addr {
  int region;
  union {
  int offset;
  char *name;
  } u;
  int val_i;
  double val_d;
  int og;
};

/* Regions: */
#define R_GLOBAL 2001 /* can assemble as relative to the pc */
#define R_LOCAL  2002 /* can assemble as relative to the ebp */
#define R_CLASS  2003 /* can assemble as relative to the 'this' register */
#define R_LABEL  2004 /* pseudo-region for labels in the code region */
#define R_CONST  2005 /* pseudo-region for immediate mode constants */
#define R_NAME   2006 /* pseudo-region for source names */
#define R_NONE   2007 /* pseudo-region for unused addresses */
#define R_STR   2008
#define R_FLOAT   2009

struct instr {
   int opcode;
   struct addr *dest, *src1, *src2;
   struct instr *next;
};
/* Opcodes, per lecture notes */
#define O_ADD   3001
#define O_SUB   3002
#define O_MUL   3003
#define O_DIV   3004
#define O_NEG   3005
#define O_ASN   3006
#define O_ADDR  3007
#define O_LCONT 3008
#define O_SCONT 3009
#define O_GOTO  3010
#define O_BLT   3011
#define O_BLE   3012
#define O_BGT   3013
#define O_BGE   3014
#define O_BEQ   3015
#define O_BNE   3016
#define O_BIF   3017
#define O_BNIF  3018
#define O_PARM  3019
#define O_CALL  3020
#define O_RET   3021
#define O_OR   3022
#define O_AND   3023
#define O_OFFASN  3024
#define O_MOD  3025
#define O_BNEQ   3026
/* declarations/pseudo instructions */
#define D_GLOB  3051
#define D_PROC  3052
#define D_LOCAL 3053
#define D_LABEL 3054
#define D_END   3055
#define D_PROT  3056 /* prototype "declaration" */
#define D_CONST 3057
#define D_NONE 3058

struct instr *gen(int, struct addr*, struct addr*, struct addr*);
struct instr *concat(struct instr *, struct instr *);
struct addr *copyaddr(struct addr *a);
char *regionname(int i);
char *opcodename(int i);
char *pseudoname(int i);
struct addr *genlabel();
struct addr *create_addr_name(int region, char* name);
struct addr *create_addr_offset(int region);
struct addr *create_addr_offset_spec(int region, int counter);
struct addr *create_addr_ival(int region, int ival);
struct addr *create_addr_dval(int region, double dval);
struct addr *create_addr_offset_ival(int region, int ival);
struct addr *create_addr_offset_dval(int region, double dval);
struct addr *create_addr_spec(int region, int counter);
struct addr *create_addr_string(int region, int counter);
struct addr *create_addr_label(int region);

void assign_first(struct tree *t);
void assign_follow(struct tree *t);
void assign_truefalse(struct tree *t);

int gen_expr_first(tree_t *expr);
void gen_array_first(tree_t *t);
int gen_expr_follow(tree_t *expr);
int gen_expr_truefalse(tree_t *expr);

void gencode(tree_t *tree);
void gencode_array(tree_t *expr);
int gencode_expr(tree_t *expr);
void gencode_break(tree_t *tree);

struct instr *gencode_println(tree_t *t, struct addr *address, char *name);
struct instr *println_params_tostr(tree_t *tree, struct addr *first, struct instr *list, struct instr *conc);
struct instr *func_gencode(tree_t *t, char *name, struct addr *address);
struct instr *params_gencode(tree_t *tree, struct addr *first, struct instr *list, struct instr *conc);
struct instr *params_gencode_decl(tree_t *tree, struct instr *list_in);

void treeprint_addresses_first(tree_t *t, int depth);
void treeprint_icode(tree_t *t, int depth);
void treeprint_truefalse(tree_t *t, int depth);
void tacprint (struct instr *head);

int linked_len(struct instr *list);
void take_a_break (tree_t *tree, struct addr *follow_a);
int len_eles(tree_t *tree);

void write_icode_func(FILE *filename, SymbolTable symtab);
void write_icode_string(FILE *filename,  tree_t *tree);
void write_icode_globals(FILE *filename,  tree_t *tree);
void fprint_string_num(FILE *filename);

void free_istruct(struct instr *ic);

#endif
