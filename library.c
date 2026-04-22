
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "library.h"

// Abstract Machine - Binary Search Tree (Word Set)

WordNode *create_NW(char *word){
    WordNode *p = (WordNode *)malloc(sizeof(WordNode));
    strncpy(p->word, word, 49);
    p->word[49] = '\0';
    p->left = NULL;
    p->right = NULL;
    return p;
} // Create and return a new isolated node containing word 

WordNode *insert_NW(WordNode *R, char *word){
    if(R == NULL) return create_NW(word);
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return R; // Word already exists, the tree is unchanged
    else if(cmp < 0)
        R->left = insert_NW(R->left, word);
    else
        R->right = insert_NW(R->right, word);
    return R;
} // Insert word into the BST rooted at R

bool search_NW(WordNode *R, char *word) {
    if (R == NULL) return false;
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return true;
    else if (cmp < 0)
        return search_NW(R->left, word);
    else
        return search_NW(R->right, word);
} // Search for word in the BST rooted at R, returns true if found and false otherwise

WordNode *copy_bst(WordNode *R) {
    if (R == NULL) return NULL;
    WordNode *new_node = create_NW(R->word);
    new_node->left = copy_bst(R->left);
    new_node->right = copy_bst(R->right);
    return new_node;
} // Create and return a deep copy of the BST rooted at R

void free_bst(WordNode *R){
    if(R == NULL) return;
    free_bst(R->left);
    free_bst(R->right);
    free(R);
} // Free the entire BST and all its nodes

void inorder_traversal(WordNode* R){
    if(R == NULL) return;
    inorder_traversal(R->left);
    printf("%s , ", R->word);
    inorder_traversal(R->right);
} // Perform an in-order traversal of the BST rooted at R, printing each word followed by a comma and space

void to_lower_str(char *word){
    for (int i = 0; word[i]; i++)
        word[i] = tolower((unsigned char)word[i]);
} // Convert all characters in word to lowercase

void remove_punct(char *word){
    int i, j = 0;
    for(i = 0; word[i] != '\0'; i++){
        if(!ispunct((unsigned char)word[i]) || word[i] == '\''){
            word[j++] = word[i];
        }
    }
    word[j] = '\0';
}  // Remove all punctuation characters from word, except for apostrophes

// Abstract Machine - Paragraph (Linked List node containing a word set)




// ------------------------------------------------------------
WordNode* PARA_SET(char *para) {
    WordNode *R = NULL;
    char *T = strtok(para, " \t\n");
    int is_first = 1;
    while (T != NULL) {
        remove_punct(T);
        to_lower_str(T);
        if (strlen(T) > 0) {
            if (is_first) {
                R = create_NW(T);
                is_first = 0;
            } else {
                R = insert_NW(R, T);
            }
        }
        T = strtok(NULL, " \t\n");
    }
    return R;
}
int read_file(char *filename, char paragraphs[100][1000]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    char line[1000];
    int p = 0;
    paragraphs[p][0] = '\0';
    while (fgets(line, sizeof(line), fp)) {
        if (strcmp(line, "\n") == 0) {
            if (strlen(paragraphs[p]) > 0) {
                p++;
                paragraphs[p][0] = '\0';
            }
        } else {
            strcat(paragraphs[p], line);
        }
    }
    fclose(fp);
    return p + 1;
}
WordNode* cpy_para(WordNode *A, WordNode *B) {
    if (A == NULL) return B;
    B = INSERT_NW(B, A->word);
    B = cpy_para(A->left,  B);
    B = cpy_para(A->right, B);
    return B;
}


