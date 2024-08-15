#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Replaces escape sequences in a string with their actual counterparts */
void escape_handler(char *str) {
    int i, j;
    int len = strlen(str);

    for (i = 0; i < len - 1; i++) {
        if (str[i] == '\\') {
            
            switch (str[i + 1]) {
                case 't':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\t';
                    len -= 1;
                    break;
                case 'n':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\n';
                    len -= 1;
                    break;
                case 'r':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\r';
                    len -= 1;
                    break;
                case '\'':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\'';
                    len -= 1;
                    break;
                case '\"':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\"';
                    len -= 1;
                    break;
                case '\\':
                    for (j = i+1; j < len - 1; j++) {
                        str[j] = str[j + 1];
                    }
                    str[i] = '\\';
                    len -= 1;
                    break;
                case 'x':
                    printf("The \\x escape sequence is not supported by irony");
                    break;
                case 'u':
                    printf("The \\u escape sequence is not supported by irony");
                    break;
            }
            
        }
    }

    str[len] = 0;
}

/* Removes quotations flanking string and calls escape_handler() */
char *string_fix(char* input) {
    

    //escape_handler(input);

    // Check if the input string is empty or has only one character
    size_t length = strlen(input);
    if (length <= 2) {
        // Return an empty string
        char* result = malloc(1);
        result[0] = '\0';
        return result;
    }


    // Allocate memory for result string
    char* result = (char*)malloc(strlen(input));

    // Copy the substring (excluding the first and last characters) to the result string
    strncpy(result, input + 1, length - 2);

    // Null-terminate the result string
    result[length - 2] = '\0';

    escape_handler(result);

    return result;
}