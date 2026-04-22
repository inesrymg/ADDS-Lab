# ADDS-Lab  
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
PARAnode *CREATING_NW(char *word){
    PARAnode *p = (PARAnode *)malloc(sizeof(PARAnode));
    strncpy(p->word, word, 49);
    p->word[49] = '\0';
    p->left = NULL;
    p->right = NULL;
    return p;
}
PARAnode *INSERT_NW(PARAnode *R, char *word){
    if(R == NULL) return CREATING_NW(word);
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return R;
    else if(cmp < 0)
        R->left = INSERT_NW(R->left, word);
    else
        R->right = INSERT_NW(R->right, word);
    return R;
}
bool SEARCH_NW(PARAnode *R, char *word) {
    if (R == NULL) return false;
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return true;
    else if (cmp < 0)
        return SEARCH_NW(R->left, word);
    else
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
    printf("%s , ", R->word);
    INORDER_TRAVERSAL(R->right);
}
void to_lower_str(char *word){
    for (int i = 0; word[i]; i++)
        word[i] = tolower((unsigned char)word[i]);
}
void remove_punct(char *word){
    int i, j = 0;
    for(i = 0; word[i] != '\0'; i++){
        if(!ispunct((unsigned char)word[i]) || word[i] == '\''){
            word[j++] = word[i];
        }
    }
    word[j] = '\0';
}
PARAnode* PARA_SET(char *para) {
    PARAnode *R = NULL;
    char *T = strtok(para, " \t\n");
    int is_first = 1;
    while (T != NULL) {
        remove_punct(T);
        to_lower_str(T);
        if (strlen(T) > 0) {
            if (is_first) {
                R = CREATING_NW(T);
                is_first = 0;
            } else {
                R = INSERT_NW(R, T);
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
PARAnode* cpy_para(PARAnode *A, PARAnode *B) {
    if (A == NULL) return B;
    B = INSERT_NW(B, A->word);
    B = cpy_para(A->left,  B);
    B = cpy_para(A->right, B);
    return B;
}
PARAnode* UNION(PARAnode *A,PARAnode *B){
PARAnode *UNIONpara = NULL;
UNIONpara = cpy_para(A,UNIONpara);
UNIONpara = cpy_para(B,UNIONpara);
return UNIONpara;
}
int main(){
    int i= 0;
    char filename[100];
 char paragraphs[100][1000];
printf("write the name of first file :");
 fgets(filename, sizeof(filename), stdin);
     filename[strcspn(filename, "\n")] = '\0';
    int count = read_file(filename, paragraphs);
    printf("Total paragraphs: %d\n\n", count);
    for (int i = 0; i < count; i++) {
        printf("Paragraph %d:\n%s\n", i + 1, paragraphs[i]);
    }
