#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct PARAnode {
    char word[50];
    struct PARAnode* left;
    struct PARAnode* right;
} PARAnode;

PARAnode *CREATING_NW(char *word);
PARAnode *INSERT_NW(PARAnode *R, char *word);
bool SEARCH_NW(PARAnode *R, char *word);
void DESTROY_NW(PARAnode *R);
void INORDER_TRAVERSAL(PARAnode* R);
void to_lower_str(char *word);
void remove_punct(char *word);
PARAnode* read_para(char *para);
int read_file(char *filename, char paragraphs[100][1000]);
PARAnode* cpy_para(PARAnode *A, PARAnode *B);
PARAnode* UNION(PARAnode *A,PARAnode *B);

#endif