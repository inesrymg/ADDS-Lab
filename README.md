# ADDS-Lab  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct PARAnode* {
char word[50];
struct PARAnode* left;
struct PARAnode* right;
} PARAnode;

PARAnode *CREATING_NW(char *word){
    PARAnode *p = (PARAnode *)malloc(sizeof(PARAnode));
    strncpy(n->word, word, 49);
    n->word[49]= '\0';
    n->left = NULL;
    n->right = NULL;
    return n;
}

PARAnode *INSERT_NW(PARAnode *R, char *word){
    if(R == NULL) return CREATING_NW(word);
    int cmp = strcmp(word,R->word);
    if (cmp == 0) return R;
    if(cmp < 0) R->left = INSERT_NW(R->left,word);
    return R;
}

bool SEARCH_NW(PARAnode *R, char *word) {
    if (R == NULL) return 0;
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return 1;
    if (cmp < 0)  return SEARCH_NW(R->left,  word);
    return SEARCH_NW(R->right, word);
}
void DESTROY_NW(PARAnode *R){
    if(R == NULL) return;
    DESTROY_NW(R->left);
    DESTROY_NW(R->right);
    free(R);
}
void INORDER_TRAVERSAL(PARAnode* R){
    if(R == NULL) return;
    INORDER_TRAVERSAL(R->left);
    printf("%s ,", R->word);
    INORDER_TRAVERSAL(R->right);
}