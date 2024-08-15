#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/*-------------------Linked list functions----------------------*/

/*int *insert_tail(node_t **head, node_t *new_node) {
    node_t *last = *head;
    new_node->next = NULL;
    //if only the head exists, then insert at head
    if (*head == NULL) {
        *head = new_node;
        return 0;
    }

    //loop to end of list
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
    return 0;
}

node_t *insert_head(node_t **head, node_t *new_node) {
    //insert new node at head
    new_node -> next = *head;
    *head = new_node;
    return new_node;
}

node_t *new_token(token_t *token){
    //allocate new node in linked list
    node_t *new = malloc(sizeof(node_t));
    new -> t = token;
    new -> next = NULL;
    return new;
}


void freeLinkedListGraphing(node_t *head) {
    //loop through linked list
    while (head != NULL) {
        node_t *temp = head;
        head = head->next;
        // free strings, tokens, and nodes
        free(temp->t->sval);
        free(temp->t->text);
        free(temp->t);
        free(temp);
    }
}

void print_list(node_t *head) {
    node_t *temp = head;
    
    printf("Category\t\tText\t\tLineno\t\tFilename\t\tIval\t\tDval\t\tSval\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    //loops thrugh linked list
    while (temp != NULL) {
        printf("%d\t\t\t%s\t\t%d\t\t%s\t\t%d\t\t%f\t\t%s\n\n", temp->t->category, temp->t->text, 
        temp->t->lineno, temp->t->filename, temp->t->ival, temp->t->dval, temp->t->sval);
        temp = temp -> next;
    }
    printf("\n");
}*/