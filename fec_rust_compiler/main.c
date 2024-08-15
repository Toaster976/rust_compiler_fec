#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tree.h"
#include "utils.h"
#include "symtab.h"
#include "type.h"
#include "tac.h"
#include "finalc.h"

int yylex();
int yyparse();
int yylex_destroy();
extern char* yytext;
extern char yyfilename;
extern token_t *temp_token;
extern FILE *yyin;
extern int yylineno;
extern tree_t* root;

int main(int argc, char *argv[])
{
    int dot_mode = 0;
    int tree_mode = 0;
    int symtab_mode = 0;
    int compile_mode = 0;
    char* filename = argv[1];
    char* filename_rs = malloc(sizeof(char) * (strlen(filename) + 4));
    int has_extension = 0;

    

    // Determine which arguments are the filename and -dot
    /*if (argc == 2) {
        filename = argv[1];
    } else if (argc == 3) {
        filename = argv[2];
        
        if (strcmp(argv[1], "-dot ") != 0) {
            dot_mode = 1;
        }
    }*/

    // Determine filename and command-line options
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-dot") == 0) {
            dot_mode = 1;
            //printf("dot mode\n");
        }
        if (strcmp(argv[i], "-tree") == 0) {
            tree_mode = 1;
            //printf("tree mode\n");
        }
        if (strcmp(argv[i], "-symtab") == 0) {
            symtab_mode = 1;
            //printf("symtab mode\n");
        }
        if (strcmp(argv[i], "-c") == 0) {
            compile_mode = 1;
            //printf("symtab mode\n");
        }
    }
    filename = argv[argc - 1];


    strcpy(filename_rs,  filename);

    //Detect it an extention exists or not
    for (int i = 0; i < strlen(filename) - 1; i++) {
        if (filename[i] == '.') {
            has_extension = 1;
            break;
        }
    }

    //Add a .rs extension if there is none
    if (has_extension == 1 && strcmp(filename + strlen(filename) - 3, ".rs") != 0) {
        printf("This program can only accept .rs files\n");
        return -1;
    } else if (has_extension == 0)
    {
        rename(filename, strcat(filename_rs, ".rs"));
        strcpy(filename,  filename_rs);
    }

    FILE* file = fopen(filename, "r");

    strcpy(&yyfilename, filename);
    yyin = file;

    fprintf(stdout, "Filename: %s\n", filename);
    
    // Parse syntax tree
    yyparse();
    //printsyms(root);

    // If the command-line option is active, print information
    if (dot_mode == 1) {
        print_graph(root, "foo.rs.dot");
    }

    SymbolTable table = init_symtable (16, "global", NULL);

    insert_inbuilt(table);

    fill_symtable(root, table);

    if (tree_mode == 1) {
        treeprint(root, 1);
    }

    if (symtab_mode == 1) {
        print_symtable(table);
    } else {
        printf("No Errors\n");
    }

    init_types(root);

    typecheck(root);

    assign_first(root);
    assign_follow(root);
    assign_truefalse(root);
    gencode_break(root);
    gencode(root);
    //treeprint_types(root, 0);
    
    //treeprint_addresses_first(root, 0);
    //treeprint_truefalse(root, 0);
    //treeprint_icode(root, 0);

    //SymbolTableEntry main = lookup("main", table);
    //SymbolTableEntry foo = lookup("foo", table);

    //printf("Final thing\n");
    //tacprint (foo->icode);
    //printf("Final thing\n");
    //tacprint (main->icode);

    filename[strlen(filename) - 3] = '\0';
    filename =  strcat(filename, ".ic");
    FILE *fptr = fopen(filename, "w");

    fprintf(fptr, ".global\n");
    write_icode_globals(fptr,  root);
    fprintf(fptr, ".string");
    fprint_string_num(fptr);
    write_icode_string(fptr, root);

    write_icode_func(fptr, table);

    fclose(fptr);

    FILE *fptr_i = fopen(filename, "r");
    filename[strlen(filename) - 3] = '\0';
    filename =  strcat(filename, ".c");
    FILE *fptr_c = fopen(filename, "w");

    final_code_gen(fptr_i, fptr_c, table);


    fclose(fptr_i);
    fclose(fptr_c);


    char command[100];

    if (compile_mode) {
        sprintf(command, "gcc -c %s", filename);
        system(command);
    } else {
        
        sprintf(command, "gcc -c %s", filename);
        //sprintf(command, "gcc -g -Wall -c main.c");
        //printf("%s\n", command);
        system(command);

        /*char * args1[] = {"gcc", "-c", filename, NULL};

        int p1 = fork();

        if (p1 == 0) {
            execvp("gcc", args1);
        } else {
            wait(&childReturns);
        }*/
        //system("sudo ./test.o");
        
        
        //system(command);
        filename[strlen(filename) - 2] = '\0';
        filename =  strcat(filename, ".o");

        char* finalname = malloc(100);
        strcpy(finalname, filename);
        finalname[strlen(finalname) - 2] = '\0';

        //sprintf(command, "gcc -shared -o lib%s.so %s", finalname, filename);
        //sprintf(command, "gcc %s -o %s", filename, finalname);
        //system(command);
        sprintf(command, "gcc %s -o %s", filename, finalname);
        //sprintf(command, "gcc %s", filename);
        //printf("%s\n", command);
        system(command);
        /*char * args2[] = {"gcc", "-lc", filename, "-o", finalname, NULL};

        int p2 = fork();

        if (p2 == 0) {
            execvp("gcc", args2);
        } else {
            wait(&childReturns);
        }*/
    }

    // Free remaining memory
    free(filename_rs);
    free_tree(root);
    free_symtab (table);
    yylex_destroy();
    fclose(file);
    
    
    return 0;
}