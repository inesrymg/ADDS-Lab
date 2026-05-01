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
    char** lines;
    int line_count;
    struct ParaNode* next;
} ParaNode;

typedef struct {
    ParaNode* head;
} ParaList;

// BST operations
WordNode* create_bst_NW(char* word);
void free_bst(WordNode* R);
WordNode* insert_bst_NW(WordNode* R, char* word);
bool search_bst(WordNode* R, char* word);
WordNode* copy_bst(WordNode* R);
void inorder_traversal(WordNode* R);

// paragraph list operations
ParaList* create_para_list();
void insert_para_list(ParaList* list, WordNode* para, int id, char** lines, int line_count);
ParaNode* get_para(ParaList* list, int para_id);
void print_para_list(ParaList* list);


void to_lower_str(char* word);
void remove_punct(char* word);
int normalise_word(char* word);






#endif