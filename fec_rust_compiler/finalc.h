struct operation {
  int type;
  
  char *dest_r;
  char *dest_a;
  char *addr1_r;
  char *addr1_a;
  char *addr2_r;
  char *addr2_a;
  
  struct operation* next;
};

struct param_tf {
  char *r;
  char *a;
  struct param_tf* next;
};

void final_code_gen(FILE *icode, FILE *output, SymbolTable symtab);

void assign_type(char *dest_r, char *dest_a, char *addr1_r, char *addr1_a);