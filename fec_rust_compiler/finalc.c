#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "symtab.h"
#include "type.h"
#include "tac.h"
#include "finalc.h"

int typearray[1000];
char loc[1000];
char * paramstack = "";

void final_code_gen(FILE *icode, FILE *output, SymbolTable symtab) 
{
    int string_label = 0;
    int tempstring_counter = 0;
    for (int i = 0; i < 1000; i++) {
        typearray[i] = 0;
    }

    char *code = malloc(255);
    char *num_strings = malloc(255);
    char str[1000] = "";
    char temp_str[1000];
    char param_f[1000] = "";
    char params_f[1000] = "";

    char *operation = malloc(255);
    //operation = NULL;

    char *dest_r = malloc(255);
    //dest_r = NULL;
    char *dest_a = malloc(255);
    //dest_a = NULL;
    char *addr1_r = malloc(255);
    //addr1_r = NULL;
    char *addr1_a = malloc(255);
    //addr1_a = NULL;
    char *addr2_r = malloc(255);
    //addr2_r = NULL;
    char *addr2_a = malloc(255);
    //addr2_a = NULL;

    fprintf(output, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n");
    
    while (fscanf(icode, "%255s", code) == 1) {
        //printf("%s\n", code);

        if (strcmp(code, ".global") == 0) {
            //printf("do global stuff\n");
        } else if (strcmp(code, ".string") == 0) {
            //printf("do string stuff\n");
            fscanf(icode, "%255s", num_strings);
            int num = atoi(num_strings);
            
            //printf("%d\n", num);

            fprintf(output, "char str[1000];\n");

            for (int i = 0; i < num; i++) {
                fscanf(icode, "%255s", code);
                //fprintf(output, "strcpy(str + %d, \"%s\");\n", string_label, code);

                sprintf(temp_str, "strcpy(str + %d, \"%s\");\n", string_label, code);
                strcat(str, temp_str);
                //printf("%s\n", code);
                string_label += strlen(code) - (strlen(code) % 8) + 8;
            }

        } else if (strcmp(code, ".code") == 0) {
            //printf("do code stuff\n");
            while (fscanf(icode, "%255s", code) == 1) {
                int length = strlen(code);
                if (code[length - 1] == ':') {
                    //printf("\tfunc:  %s\n", code);

                    code[length - 1] = '\0';
                    SymbolTableEntry func = lookup(code, symtab);
                    //printf("%d\n", func->typ->u.f.returntype->basetype);
                    if (strcmp(code, "main") == 0) {
                        fprintf(output, "int %s () {\n", code);
                        fprintf(output, "%s", str);
                    } else {
                        switch (func->typ->u.f.returntype->basetype) 
                        {
                            case NULL_TYPE:
                                fprintf(output, "void %s () {\n", code);
                            break;
                            case INT_TYPE:
                                fprintf(output, "int %s () {\n", code);
                            break;
                            case FLOAT_TYPE:
                                fprintf(output, "float %s () {\n", code);
                            break;
                            case BOOL_TYPE:
                                fprintf(output, "int %s () {\n", code);
                            break;
                            case STRING_TYPE:
                                fprintf(output, "char * %s () {\n", code);
                            break;
                            case CHAR_TYPE:
                                fprintf(output, "char * %s () {\n", code);
                            break;
                        }
                    }
                    fprintf(output, "char loc[1000];\n");

                } else {
                    //printf("%s\n", code);
                    strcpy(operation, code);

                    //printf("\top:  %s\n", operation);

                    if (strcmp(operation, "ADD") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) + *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) + %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d + *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d + %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) + *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) + %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f + *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f + %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "SUB") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) - *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) - %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d - *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d - %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) - *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) - %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f - *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f - %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "MUL") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) * *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) * %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d * *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d * %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) * *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) * %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f * *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f * %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "DIV") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) / *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) / %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d / *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d / %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) / *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) / %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f / *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f / %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "NEG") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = -*(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a));
                            } else {
                                fprintf(output, "*(int *)(loc+%d) = -%d;\n", atoi(dest_a), atoi(addr1_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = -*(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a));
                            } else {
                                fprintf(output, "*(float *)(loc+%d) = -%f;\n", atoi(dest_a), atof(addr1_a));
                            }
                        }

                    } else if (strcmp(operation, "ASN") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);
                        //printf("%s\n", dest_a);
                        //printf("%d\n", typearray[atoi(dest_a)]);
                    

                        
                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a));
                            } else {
                                fprintf(output, "*(int *)(loc+%d) = %d;\n", atoi(dest_a), atoi(addr1_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a));
                            } else {
                                fprintf(output, "*(float *)(loc+%d) = %f;\n", atoi(dest_a), atof(addr1_a));
                            }
                        }

                    } else if (strcmp(operation, "ADDR") == 0) {

                    } else if (strcmp(operation, "LCONT") == 0) {
                        //printf("ernigige\n");
                        fscanf(icode, "%255s %255s", dest_r, dest_a);
                        fprintf(output, "lab%d:\n", atoi(dest_a));
                    } else if (strcmp(operation, "SCONT") == 0) {

                    } else if (strcmp(operation, "GOTO") == 0) {

                        fscanf(icode, "%255s %255s", dest_r, dest_a);
                        fprintf(output, "goto lab%d;\n", atoi(dest_a));

                    } else if (strcmp(operation, "BLT") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) < *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) < %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d < *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d < %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) < *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) < %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f < *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f < %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BLE") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) <= *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) <= %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d <= *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d <= %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) <= *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) <= %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f <= *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f <= %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BGT") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) > *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) > %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d > *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d > %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) > *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) > %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f > *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f > %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BGE") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) >= *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) >= %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d >= *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d >= %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) >= *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) >= %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f >= *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f >= %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BEQ") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) == *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) == %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d == *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d == %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) == *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) == %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f == *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f == %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BNE") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);
                        //printf("%s\n", dest_a);
                        //printf("%d\n", typearray[atoi(dest_a)]);
                    
                        
                        if (strcmp(addr1_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = !*(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a));
                        } else {
                            fprintf(output, "*(int *)(loc+%d) = %d;\n", atoi(dest_a), atoi(addr1_a));
                        }

                    } else if (strcmp(operation, "BNEQ") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (typearray[atoi(dest_a)] == INT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) != *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) != %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d != *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                                fprintf(output, "*(int *)(loc+%d) = %d != %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            }
                        } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                            if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) != *(float *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = *(float *)(loc+%d) != %f;\n", atoi(dest_a), atoi(addr1_a), atof(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "loc") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f != *(float *)(loc+%d);\n", atoi(dest_a), atof(addr1_a), atoi(addr2_a));
                            } else if (strcmp(addr1_r, "float") == 0 && strcmp(addr2_r, "float") == 0) {
                                fprintf(output, "*(float *)(loc+%d) = %f != %f;\n", atoi(dest_a), atof(addr1_a), atof(addr2_a));
                            }
                        }

                    } else if (strcmp(operation, "BIF") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);
                        
                        if (strcmp(dest_r, "const") == 0) {
                            fprintf(output, "if (%d) goto lab%d;\n", atoi(dest_a), atoi(addr1_a));
                        } else {
                            fprintf(output, "if (*(int *)(loc+%d)) goto lab%d;\n", atoi(dest_a), atoi(addr1_a));
                        }

                    } else if (strcmp(operation, "BNIF") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (strcmp(dest_r, "const") == 0) {
                            fprintf(output, "if (!%d) goto lab%d;\n", atoi(dest_a), atoi(addr1_a));
                        } else {
                            fprintf(output, "if (!*(int *)(loc+%d)) goto lab%d;\n", atoi(dest_a), atoi(addr1_a));
                        }

                    } else if (strcmp(operation, "PARM") == 0) {
                        fscanf(icode, "%255s %255s", dest_r, dest_a);
                        if (strcmp(dest_r, "loc") == 0) {
                            if (typearray[atoi(dest_a)] == INT_TYPE) {
                                
                                
                                fprintf(output, "char sa%d[100];\n", tempstring_counter);
                                fprintf(output, "sprintf(sa%d, \"%%d\", *(int *)(loc+%d));\n", tempstring_counter, atoi(dest_a));

                                sprintf(param_f, "sa%d,", tempstring_counter);
                                strcat(params_f, param_f);
                                tempstring_counter++;
                            } else if (typearray[atoi(dest_a)] == STRING_TYPE) {
                                sprintf(param_f, "(loc+%d),", atoi(dest_a));
                                strcat(params_f, param_f);
                            } else if (typearray[atoi(dest_a)] == FLOAT_TYPE) {
                                fprintf(output, "char sa%d[100];\n", tempstring_counter);
                                fprintf(output, "sprintf(sa%d, \"%%f\", *(float *)(loc+%d));\n", tempstring_counter, atoi(dest_a));

                                sprintf(param_f, "sa%d,", tempstring_counter);
                                strcat(params_f, param_f);
                                tempstring_counter++;
                            }
                        } else if (strcmp(dest_r, "const") == 0) {
                            sprintf(param_f, "\"itoa(%d)\",", atoi(dest_a));
                            strcat(params_f, param_f);
                        } else if (strcmp(dest_r, "float") == 0) {
                            sprintf(param_f, "ftoa(%f),", atof(dest_a));
                            strcat(params_f, param_f);
                        } else if (strcmp(dest_r, "str") == 0) {
                            sprintf(param_f, "str + %d,", atoi(dest_a));
                            strcat(params_f, param_f);
                        }

                        /*struct param_tf *new_param = malloc(sizeof(struct param_tf));
                        new_param->a = dest_a;
                        new_param->r = dest_r;
                        new_param->next = NULL;

                        if (paramstack == NULL) {
                            paramstack = new_param;
                            paramstack_next = paramstack->next;
                        } else {
                            paramstack_next = new_param;
                            paramstack_next = paramstack_next->next;
                        }*/

                    } else if (strcmp(operation, "CALL") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr2_r, addr2_a);
                        params_f[strlen(params_f)-1] = '\0';
                        if (strcmp(addr1_r, "println") == 0) {
                            //fprintf(output, "%s", params_f);

                            fprintf(output, "printf(");
                            fprintf(output, "%s", params_f);
                            fprintf(output, ");\n");
                            fprintf(output, "printf(\"\\n\");\n");
                            params_f[0] = '\0';
                        }

                    } else if (strcmp(operation, "RETURN") == 0) {
                        fscanf(icode, "%255s %255s", dest_r, dest_a);

                        if (strcmp(dest_r, "loc") == 0) {
                            fprintf(output, "return *(int *)(loc+%d);\n", atoi(dest_a));
                        } else if (strcmp(dest_r, "const") == 0) {
                            fprintf(output, "return %d;\n", atoi(dest_a));
                        } else if (strcmp(dest_r, "float") == 0) {
                            fprintf(output, "return %f;\n", atof(dest_a));
                        } else if (strcmp(dest_r, "str") == 0) {
                            fprintf(output, "return *(char *)(loc+%d);\n", atoi(dest_a));
                        }

                    } else if (strcmp(operation, "OR") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) || *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) || %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d || *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d || %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        }

                    } else if (strcmp(operation, "AND") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) && *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) && %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d && *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d && %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        }

                    } else if (strcmp(operation, "OFFASN") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);
                        fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+(*(int *)(loc+%d)));\n", atoi(dest_a), atoi(addr1_a));
                    } else if (strcmp(operation, "MOD") == 0) {
                        fscanf(icode, "%255s %255s %255s %255s %255s %255s", dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
                        assign_type(dest_r, dest_a, addr1_r, addr1_a);

                        if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) %% *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "loc") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = *(int *)(loc+%d) %% %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "loc") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d %% *(int *)(loc+%d);\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        } else if (strcmp(addr1_r, "const") == 0 && strcmp(addr2_r, "const") == 0) {
                            fprintf(output, "*(int *)(loc+%d) = %d %% %d;\n", atoi(dest_a), atoi(addr1_a), atoi(addr2_a));
                        }
                    }
                    /*fscanf(icode, "%255s[^:]:%d", dest_r, dest_a);
                    printf("\t\tregs:  %s\n", dest_r);
                    printf("\t\tregs:  %s\n", dest_a);*/
                    
                }
            }
        }
    }

    fprintf(output, "}");



    /*for (int i = 0; i < 25; i++
    ) 
    {
        fscanf(icode, "%255s\t\t\t%255s:%255s,%255s:%255s,%255s:%255s", 
        operation, dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);
        printf("%d\n", i);
        printf("%s\n", operation);
    }*/
    //fscanf(icode, "\t\t%255s\t\t\t%255s:%255s,%255s:%255s,%255s:%255s", 
    //operation, dest_r, dest_a, addr1_r, addr1_a, addr2_r, addr2_a);

    //printf("%s", operation);

    free(code);
    free(num_strings);
    free(operation);
    free(dest_r);
    free(dest_a);
    free(addr1_r);
    free(addr1_a);
    free(addr2_r);
    free(addr2_a);
}

void assign_type(char *dest_r, char *dest_a, char *addr1_r, char *addr1_a) {
    if (strcmp(addr1_r, "loc") == 0) {
        typearray[atoi(dest_a)] = typearray[atoi(addr1_a)];

    } else if (strcmp(addr1_r, "const") == 0) {
        typearray[atoi(dest_a)] = INT_TYPE;

    } else if (strcmp(addr1_r, "float") == 0) {
        typearray[atoi(dest_a)] = FLOAT_TYPE;

    } else if (strcmp(addr1_r, "str") == 0) {
        typearray[atoi(dest_a)] = STRING_TYPE;

    }
    //printf("%s\n", dest_a);
    //printf("%s\n", addr1_r);
}