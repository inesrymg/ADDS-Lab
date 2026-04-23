
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "library.h"

// Abstract Machine - Binary Search Tree (Word Set)

WordNode *create_bst_NW(char *word){
    WordNode *p = (WordNode *)malloc(sizeof(WordNode));
    strncpy(p->word, word, 49);
    p->word[49] = '\0';
    p->left = NULL;
    p->right = NULL;
    return p;
} // Create and return a new isolated node containing word 

void free_bst(WordNode *R){
    if(R == NULL) return;
    free_bst(R->left);
    free_bst(R->right);
    free(R);
} // Free the entire BST and all its nodes : postorder 

WordNode *insert_bst_NW(WordNode *R, char *word){
    if(R == NULL) return create_NW(word);
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return R; // Word already exists, the tree is unchanged
    else if(cmp < 0)
        R->left = insert_NW(R->left, word);
    else
        R->right = insert_NW(R->right, word);
    return R;
} // Insert word into the BST rooted at R

bool search_bst(WordNode *R, char *word) {
    if (R == NULL) return false;
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return true;
    else if (cmp < 0)
        return search_bst(R->left, word);
    else
        return search_bst(R->right, word);
} // Search for word in the BST rooted at R, returns true if found and false otherwise

WordNode *copy_bst(WordNode *R) {
    if (R == NULL) return NULL;
    WordNode *new_node = create_NW(R->word);
    new_node->left = copy_bst(R->left);
    new_node->right = copy_bst(R->right);
    return new_node;
} // Create and return a deep copy of the BST rooted at R : preorder


void inorder_traversal(WordNode* R){
    if(R == NULL) return;
    inorder_traversal(R->left);
    printf("%s , ", R->word);
    inorder_traversal(R->right);
} // Perform an in-order traversal of the BST rooted at R, printing each word followed by a comma and space


// Abstract Machine - Paragraph List (Linked List of Word Sets)

ParaList* create_para_list() {
    ParaList* list = (ParaList*)malloc(sizeof(ParaList));
    list->head = NULL;
    return list;
} // Create and return an empty paragraph list

void insert_para_list(ParaList* list, WordNode* para) {
    ParaNode* new_node = (ParaNode*)malloc(sizeof(ParaNode));
    new_node->para = para;
    new_node->next = list->head;
    list->head = new_node;
} // Insert a new paragraph (WordNode) at the beginning of the paragraph list

ParaNode* get_para(ParaList* list, int para_id) {
    ParaNode* current = list->head;
    while (current != NULL) {
        if (current->para_id == para_id) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Return NULL if no paragraph with the given ID is found
} // Retrieve the paragraph node with the specified para_id from the paragraph list, returns NULL if not found

void print_para_list(ParaList* list) {
    ParaNode* current = list->head;
    while (current != NULL) {
        printf("Paragraph ID: %d\n", current->para_id);
        printf("Words: ");
        inorder_traversal(current->word_set);
        printf("\n");
        current = current->next;
    }
} // Print the contents of the paragraph list, including each paragraph's ID and its associated words

void free_para_list(ParaList* list) {
    ParaNode* current = list->head;
    while (current != NULL) {
        ParaNode* temp = current;
        current = current->next;
        free_bst(temp->word_set); // Free the BST associated with the paragraph
        free(temp); // Free the paragraph node
    }
    free(list); // Free the paragraph list itself
} // Free all memory associated with the paragraph list, including all paragraphs and their word sets

ParaList* copy_para_list(ParaList* list) {
    ParaList* new_list = create_para_list();
    ParaNode* current = list->head;
    while (current != NULL) {
        WordNode* copied_para = copy_bst(current->word_set); // Deep copy the BST for the paragraph
        insert_para_list(new_list, copied_para); // Insert the copied paragraph into the new list
        current = current->next;
    }
    return new_list; // Return the new paragraph list which is a deep copy of the original
} // Create and return a deep copy of the given paragraph list, including deep copies of all paragraphs and their word sets



// --------------useful-------------------
void to_lower_str(char *word) {
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
} // Convert all characters in word to lowercase

void remove_punct(char *word) {
    int j = 0;
    for (int i = 0; word[i]; i++) {
        if (!ispunct(word[i])) {
            word[j++] = word[i];
        }
    }
    word[j] = '\0';
} // Remove all punctuation characters from word

int normalise_word(char *word) {
    remove_punct(word);
    to_lower_str(word);
    return strlen(word);
} // Normalize word by removing punctuation and converting to lowercase, returns the length of the normalized word


