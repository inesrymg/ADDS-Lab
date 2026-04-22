#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct WordNode {
    char word[50];
    struct WordNode* left;
    struct WordNode* right;
} WordNode;

typedef struct ParaNode {
    int para_id;
    WordNode* word_set;
    struct ParaNode* next; 
} ParaNode;


WordNode *create_NW(char *word);
WordNode *insert_NW(WordNode *R, char *word);
bool search_NW(WordNode *R, char *word);
WordNode *copy_bst(WordNode *R);
void free_bst(WordNode *R);
void to_lower_str(char *str);
void remove_punct(char *word);



#endif