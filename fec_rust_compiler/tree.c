#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ytab.h"
#include "tree.h"
#include "rustgram.tab.h"
#include "symtab.h"
#include "type.h"
#include "tac.h"


extern YYSTYPE yylval;
extern char *yytext;

extern char yyfilename[256];

extern int yylineno;

int tree_count = 0, token_count = 0;

int serial = 0;

/* Def: Initializes memory for tree
 * sym_name: The symbolname of the branch or leaf
 * return: The newly allocated tree
 */
tree_t* init_tree(char *sym_name)
{
    tree_t *newtree = malloc(sizeof(tree_t));
    newtree->symbolname = malloc((strlen(sym_name) + 1)*sizeof(char));
    newtree->nkids = 0;
    newtree->leaf = NULL;
    newtree->symtab = NULL;
    newtree->type = NULL;
    newtree->first = NULL;
    newtree->follow = NULL;
    newtree->onFalse = NULL;
    newtree->onTrue = NULL;
    newtree->icode = NULL;
    newtree->address = NULL;
    
    strcpy(newtree->symbolname, sym_name);
    for (int i = 0; i < 20; i++) {
        newtree->kids[i] = NULL;
    }

    return newtree;
}

/* Def: Returns the string of a given token
 * category: The integer value representing the token
 * return: String of the given token
 */
char *get_symname(int category){

    switch(category)
    {
        case BREAK:return  "BREAK";
        case CONST:return  "CONST";
        case ELSE:return  "ELSE";	
        case FALSE:return  "FALSE";
        case FN:return  "FN";
        case FOR:return  "FOR";
        case IF:return  "IF";
        case IN:return  "IN";
        case LET:return  "LET";
        case MUT:return  "MUT";
        case REF:return  "REF";
        case RETURN:return  "RETURN";
        case STATIC:return  "STATIC";
        case TRUE:return  "TRUE";
        case WHILE:return  "WHILE";

        case '=':return  "=";
        case PLUSEQ:return  "PLUSEQ";
        case MINUSEQ:return  "MINUSEQ";
        case '+':return  "+";
        case '-':return  "-";
        case '*':return  "*";
        case '/':return  "/";
        case '%':return  "%";
        case EQEQ:return  "EQEQ";
        case NE:return  "NEQ";
        case '>':return  "'>'";
        case '<':return  "'<'";
        case LE:return  "LEQ";
        case GE:return  "GEQ";
        case ANDAND:return  "ANDAND";
        case OROR:return  "OROR";
        case '!':return  "!";
        case '.':return  ".";
        case DOTDOT:return  "DOTDOT";
        case '[':return  "[";
        case ']':return  "]";

        case '(':return  "(";
        case ')':return  ")";
        case ',':return  ",";
        case ';':return  ";";
        case '{':return  "{";
        case '}':return  "}";
        case ':':return  ":";
        case RARROW:return  "RARROW";
        case LIT_STR:return  "LIT_STR";
        case LIT_CHAR:return "LIT_CHAR";
        case LIT_INTEGER:return  "LIT_INTEGER";
        case LIT_FLOAT:return  "LIT_FLOAT";
        case IDENT:return  "IDENT";
    }
    return "Error: Unknown Category from get_symname()";  
}

/* Def: Allocates tokens as leafs in the syntax tree, assigning it to yylval to be used
 * category: The integer value representing the token
 * return: The category of the token
 */
int alctoken(int category){
    token_count++;

	/*initialize tree fields*/
	yylval.treeptr = malloc(sizeof (tree_t));
    yylval.treeptr->id = serial;
    serial++;
	yylval.treeptr->prodrule = category;
	char *sym_name = get_symname(category);
	yylval.treeptr->symbolname = malloc((strlen(sym_name) + 1) * sizeof(char));
	strcpy(yylval.treeptr->symbolname, sym_name);
	yylval.treeptr->nkids = 0;
	for (int i = 0; i < 20; i++) {
		yylval.treeptr->kids[i] = NULL;
	}
    yylval.treeptr->first = NULL;
    yylval.treeptr->follow = NULL;
    yylval.treeptr->onFalse = NULL;
    yylval.treeptr->onTrue = NULL;
    yylval.treeptr->icode = NULL;
    yylval.treeptr->address = NULL;

	/*initialize token fields*/
	yylval.treeptr->leaf = malloc(sizeof (token_t));
	yylval.treeptr->leaf->category = category;
	yylval.treeptr->leaf->text = malloc((strlen(yytext)+1) * sizeof(char));
	strcpy(yylval.treeptr->leaf->text, yytext);
	yylval.treeptr->leaf->lineno = yylineno;
	yylval.treeptr->leaf->filename = malloc((strlen(yyfilename)+1) * sizeof(char));
    strcpy(yylval.treeptr->leaf->filename, yyfilename);
	if (category == LIT_INTEGER)
		yylval.treeptr->leaf->ival = atoi(yytext);
	if (category == LIT_FLOAT)
		yylval.treeptr->leaf->dval = atof(yytext);
	if (category == LIT_STR || category == LIT_CHAR)
        yylval.treeptr->leaf->sval = string_fix(yytext);
    else {
        yylval.treeptr->leaf->sval = NULL;
    }

	return category;
}

/* Def: creates a new tree by allocating the values of to a particular tree node
 * prod_rule: The integer representing the production rule
 * sym_name: The name of the symbol or pruduction rule
 * children: Other tree nodes to be connected to the new tree
 * num_kids: The number of children a tree node has (NOTE: This also includes NULL children for debugging purposes)
 * return: The newly created tree node
 */
tree_t* create_tree(int prod_rule, char* sym_name, tree_t* children[], int num_kids)
{
    int nchild = 0;
    tree_t *new_tree = init_tree(sym_name);

    for (int i = 0; i < 20; i++) {
        if (children[i] != NULL) {
            new_tree->kids[i] = children[i];
            new_tree->kids[i]->type = NULL;
            nchild++;
        }
        //printf("\t works: %s\n", new_tree->kids[i]->symbolname);
    }

    new_tree->id = serial++;
    new_tree->prodrule = prod_rule;
    new_tree->nkids = num_kids;

    return new_tree;
}

/* Def: Allocates a new tree node through syntax analyzer
 * prod_rule: The integer representing the production rule
 * sym_name: The name of the symbol or pruduction rule
 * argc: The children of the tree
 * return: The newly created tree node
 */
tree_t* alctree(int prod_rule, char* sym_name, int argc, ...)
{
    tree_count++;
    va_list argv;
    //printf("%s \t%d\n", sym_name, prod_rule);
    va_start(argv, argc);
    tree_t *kids[20];

    for (int i = 0; i < 20; i++) {
        kids[i] = NULL;
    }

    for (int j = 0; j < argc; j++) {
        kids[j] = va_arg(argv, tree_t *);
        //printf("\t works: %s\n", kids[j]->symbolname);
        //print_node(kids[j]);
    }

    va_end(argv);
    //printf("trees: %d\n", tree_count);

    return create_tree(prod_rule, sym_name, kids, argc);   
}


/* Def: Prints a given syntax tree
 * t: The root of the tree to be printed
 * depth: The layer of the tree
 */
void treeprint(tree_t *t, int depth)
{
    int i;
    
    if(t->symbolname != NULL && t->nkids > 0)
        printf("%*s BRANCH-%s: %d\n", depth*2, " ", t->symbolname, t->nkids);

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
                treeprint(t->kids[i], depth+1);
            }
        }
    } else {
        if (t->leaf != NULL && t->symbolname != NULL)
            printf("%*s LEAF-%s: %s-%d\n", depth*2, " ", t->leaf->text, t->symbolname, t->leaf->category);
    }
}

/* Def: Frees a given syntax tree
 * root: The root of the tree to be freed
 */
int free_tree(tree_t* root) 
{
    int i;

    //printf("%*s %s: %d\n", depth*2, " ", t->symbolname, t->nkids);
  
    if (root->nkids > 0) {
        for(i=0; i<20; i++) {
            if (root->kids[i] != NULL) {
                free_tree(root->kids[i]);
            }
        }
    } 

    if (root->address != NULL ) {
        
        //free(root->address);
    }


    if (root->leaf != NULL) {
        free(root->leaf->sval);
        free(root->leaf->text);
        free(root->leaf->filename);
        free(root->leaf);
    }

    //printf("%s\n", root->symbolname);
    //free_istruct(root->icode);

    free(root->symbolname);
    //struct addr *addr_ptr = root->address;
    //root->address = NULL;

    free(root);

    return 0;
}

/* For debugging */
void print_node(tree_t *tree) {
    //_t *temp = head;
    
    printf("Category\t\tText\t\tLineno\t\tFilename\t\tIval\t\tDval\t\tSval\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    //loops thrugh linked list
    if (tree != NULL) {
        printf("%d\t\t\t%s\t\t%d\t\t%s\t\t%d\t\t%f\t\t%s\n\n", 
        tree->leaf->category, 
        tree->leaf->text, 
        tree->leaf->lineno, 
        tree->leaf->filename, 
        tree->leaf->ival, 
        tree->leaf->dval, 
        tree->leaf->sval
        );
        //temp = temp -> next;
    }
    printf("\n");
}

/*-------------------Graphing functions----------------------*/

/* add a \ before leading and trailing double quotes */
char *escape(char *s) {
   char *s2 = malloc(strlen(s)+4);
   if (s[0] == '\"') {
      if (s[strlen(s)-1] != '\"') {
	 fprintf(stderr, "What is it?!\n");
	 }
      sprintf(s2, "\\%s", s);
      s2[strlen(s)] = '\\';
      strcat(s2, "\"");

      return s2;
     }
   else return s;
}

char *pretty_print_name(struct tree *t) {
   char *s2 = malloc(40);
   if (t->leaf == NULL) {
      sprintf(s2, "%s#%d", t->symbolname, t->prodrule%10);
      return s2;
      }
   else {
      sprintf(s2,"%s:%d", escape(t->leaf->text), t->leaf->category);
      return s2;
      }
}

void print_branch(struct tree *t, FILE *f) {
   fprintf(f, "N%d [shape=box label=\"%s\"];\n", t->id, pretty_print_name(t));
}

char * yyname(int sym);

void print_leaf(struct tree *t, FILE *f) {
   //printf("category: %d", t->leaf->category);
   char * s = get_symname(t->leaf->category);
   //print_branch(t, f);
   fprintf(f, "N%d [shape=box style=dotted label=\" %s \\n ", t->id, s);
   fprintf(f, "text = %s \\l lineno = %d \\l\"];\n", escape(t->leaf->text),
   t->leaf->lineno);
   //free(s);
}

void print_graph2(struct tree *t, FILE *f) {
   int i;
   
   if (t->leaf != NULL) {
      print_leaf(t, f);
      return;
      }
   /* not a leaf ==> internal node */
   print_branch(t, f);
   for(i=0; i < t->nkids; i++) {
      if (t->kids[i] != NULL) {
        //printf("N%d -> N%d;\n", t->id, t->kids[i]->id);
         fprintf(f, "N%d -> N%d;\n", t->id, t->kids[i]->id);
        print_graph2(t->kids[i], f);
        }
      else { /* NULL kid, epsilon production or something */
         fprintf(f, "N%d -> N%d%d;\n", t->id, t->id, serial);
	 fprintf(f, "N%d%d [label=\"%s\"];\n", t->id, serial, "Empty rule");
	 serial++;
	 }
      }
}

void print_graph(struct tree *t, char *filename){
      FILE *f = fopen(filename, "w"); /* should check for NULL */
      fprintf(f, "digraph {\n");
      print_graph2(t, f);
      fprintf(f,"}\n");
      fclose(f);
}



void printsymbol(char *s)
{
   printf("%s\n", s); fflush(stdout);
}


void printsyms(tree_t *t)
{
    int i;
    
    if(t->leaf != NULL && t->symbolname != NULL && t->leaf->category == IDENT)
        printsymbol(t->leaf->text);

    if (t->nkids > 0) {
        for(i=0; i<20; i++) {
            if (t->kids[i] != NULL) {
                printsyms(t->kids[i]);
            }
        }
    }

}