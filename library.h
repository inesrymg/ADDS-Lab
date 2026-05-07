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
    int para_num;
    WordNode* word_set;
    char** lines;
    int line_count;
    struct ParaNode* next;
} ParaNode;

typedef struct {
    ParaNode* head;
} ParaList;

// BST operations
char* node_value(WordNode* node);
WordNode* LC(WordNode* node);
WordNode* RC(WordNode* node);
void ass_val(WordNode* node, char* word);
void ass_LC(WordNode* node, WordNode* left_child);
void ass_RC(WordNode* node, WordNode* right_child);
WordNode* create_bst_NW(char* word);
void free_bst(WordNode* R);
WordNode* insert_bst_NW(WordNode* R, char* word);
bool search_bst(WordNode* R, char* word);
WordNode* copy_bst(WordNode* R);
void inorder_traversal(WordNode* R);
int count_words(WordNode* R);

// paragraph list operations
int para_num(ParaNode* node);
WordNode* word_set(ParaNode* node);
ParaNode* next_para(ParaNode* node);
void ass_para_num(ParaNode* node, int num);
void ass_word_set(ParaNode* node, WordNode* ws);
void ass_next(ParaNode* node, ParaNode* next);
ParaList* create_para_list();
void insert_para(ParaList* list, WordNode* para, int num, char** lines, int line_count);
ParaNode* get_para(ParaList* list, int para_num);
void free_para_list(ParaList* list);
ParaList* copy_para_list(ParaList* list);
void print_para_list(ParaList* list);
int count_paragraphs(ParaList* list);

ParaList* para_list_load(const char* filename);

// useful functions
void to_lower_str(char* word);
void remove_punct(char* word);
int normalise_word(char* word);

// operations
WordNode* UNION(WordNode* A, WordNode* B);
WordNode* INTERSECTION(WordNode* A, WordNode* B);
WordNode* DIFFERENCE(WordNode* A, WordNode* B);








#endif