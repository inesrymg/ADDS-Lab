#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "library.h"
// Abstract Machine - Binary Search Tree (Word Set)

char* node_value(WordNode* node) {
    if (node == NULL) return NULL;
    return node->word;
}

WordNode* LC(WordNode* node) {
    if (node == NULL) return NULL;
    return node->left;
}

WordNode* RC(WordNode* node) {
    if (node == NULL) return NULL;
    return node->right;
}

void ass_val(WordNode* node, char* word) {
    if (node == NULL) return;
    strncpy(node->word, word, 49);
    node->word[49] = '\0';
}

void ass_LC(WordNode* node, WordNode* left_child) {
    if (node == NULL) return;
    node->left = left_child;
}

void ass_RC(WordNode* node, WordNode* right_child) {
    if (node == NULL) return;
    node->right = right_child;
}


WordNode* create_bst_NW(char* word) {
    WordNode* p = (WordNode*)malloc(sizeof(WordNode));
    ass_val(p, word);
    ass_LC(p, NULL);
    ass_RC(p, NULL);
    return p;
} // Create and return a new isolated node containing word

void free_bst(WordNode* R) {
    if (R == NULL) return;
    free_bst(R->left);
    free_bst(R->right);
    free(R);
} // Frees children before parent so we never lose access to any node

WordNode* insert_bst_NW(WordNode* R, char* word) {
    if (R == NULL) return create_bst_NW(word);

    int cmp = strcmp(word, R->word); // this function compares two strings lexicographically 
    // if cmp = 0 the strings are equal
    // if cmp < 0 the first string is less than the second one
    // if cmp > 0 the first string is greater than the second one

    if (cmp == 0) return R; 
    else if (cmp < 0)
        ass_LC(R, insert_bst_NW(LC(R), word));
    else
        ass_RC(R, insert_bst_NW(RC(R), word));

    return R;
} 

bool search_bst(WordNode* R, char* word) {
    if (R == NULL) return false;

    int cmp = strcmp(word, R->word);

    if (cmp == 0) return true;
    else if (cmp < 0)
        return search_bst(LC(R), word);
    else
        return search_bst(RC(R), word);
} 


WordNode* copy_bst(WordNode* R) {
    if (R == NULL) return NULL;

    WordNode* new_node = create_bst_NW(node_value(R));
    ass_LC(new_node, copy_bst(LC(R)));
    ass_RC(new_node, copy_bst(RC(R)));

    return new_node;
} // Create and return a deep copy of the BST rooted at R : preorder

void inorder_traversal(WordNode* R) {
    if (R == NULL) return;
    inorder_traversal(LC(R));
    printf("%s , ", node_value(R));
    inorder_traversal(RC(R));
} // Perform an in-order traversal of the BST rooted at R, printing each word followed by a comma and space


// Abstract Machine - Paragraph List (Linked List of Word Sets)

int para_id(ParaNode* node) {
    if (node == NULL) return -1;
    return node->para_id;
}

WordNode* word_set(ParaNode* node) {
    if (node == NULL) return NULL;
    return node->word_set;
}

ParaNode* next_para(ParaNode* node) {
    if (node == NULL) return NULL;
    return node->next;
}

void ass_para_id(ParaNode* node, int id) {
    if (node == NULL) return;
    node->para_id = id;
}

void ass_word_set(ParaNode* node, WordNode* ws) {
    if (node == NULL) return;
    node->word_set = ws;
}

void ass_next(ParaNode* node, ParaNode* next) {
    if (node == NULL) return;
    node->next = next;
}

ParaList* create_para_list() {
    ParaList* list = (ParaList*)malloc(sizeof(ParaList));
    list->head = NULL;
    return list;
} 

void insert_para(ParaList* list, WordNode* para, int id, char** lines, int line_count)
// adds a new paragraph at the beginning of the LL
{
    ParaNode* new_node = (ParaNode*)malloc(sizeof(ParaNode));

    ass_para_id(new_node, id);
    ass_word_set(new_node, para);
    new_node->lines = lines;
    new_node->line_count = line_count;
    ass_next(new_node, list->head);

    list->head = new_node;
} 

ParaNode* get_para(ParaList* list, int id)
// searches for a paragraph in the LL
{
    ParaNode* current = list->head;

    while (current != NULL) {
        if (id == para_id(current)) {
            return current;
        }
        current = next_para(current);
    }

    return NULL; // Return NULL if no paragraph with the given ID is found
}  

void free_para_list(ParaList* list) {
    ParaNode* current = list->head;

    while (current != NULL) {
        ParaNode* tmp = current;
        current = next_para(current);
        free_bst(word_set(tmp)); // free the bst of words for the paragraph
        for (int i = 0; i < tmp->line_count; i++) free(tmp->lines[i]); // free each line
        free(tmp->lines); 
        free(tmp); // Free the paragraph node
    }

    free(list); // Free the paragraph list itself
} // Free all memory associated with the paragraph list, including all paragraphs and their word sets

ParaList* copy_para_list(ParaList* list) {
    ParaList* new_list = create_para_list();
    ParaNode* current = list->head;

    while (current != NULL) {
        WordNode* copied_para = copy_bst(word_set(current));
        char** copied_lines = (char**)malloc(current->line_count * sizeof(char*));
        for (int i = 0; i < current->line_count; i++)
            copied_lines[i] = strdup(current->lines[i]);
        insert_para(new_list, copied_para, para_id(current), copied_lines, current->line_count); // Insert the copied paragraph into the new list
        current = next_para(current);
    }

    return new_list; // copy 
} 

void print_para_list(ParaList* list) {
    ParaNode* current = list->head;

    while (current != NULL) {
        printf("Paragraph ID: %d\n", para_id(current));
        printf("Words: ");
        inorder_traversal(word_set(current));
        printf("\n");

        current = next_para(current);
    }
}

// -------- useful --------

void to_lower_str(char* word) {
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
} // Convert all characters in word to lowercase

void remove_punct(char* word) {
    int j = 0;

    for (int i = 0; word[i]; i++) {
        if (word[i] == '\'' && i > 0 && isalpha(word[i-1])) {
            word[j++] = word[i];
        } else if (!ispunct(word[i])) {
            word[j++] = word[i];
        }
    }
    word[j] = '\0';
} // Remove all punctuation characters from word

int normalise_word(char* word) {
    remove_punct(word);
    to_lower_str(word);
    return strlen(word);
} // Normalize word by removing punctuation and converting to lowercase, returns the length of the normalized word

ParaList* para_list_load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        return NULL;
    }

    ParaList* list = create_para_list();
    WordNode* current_para = NULL;
    char line[1024];
    int para_id = 1;

    char** current_lines = NULL;
    int current_line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0') {
            if (current_para != NULL) {
                insert_para(list, current_para, para_id++, current_lines, current_line_count);
                current_para = NULL;
                current_lines = NULL;
                current_line_count = 0;
            }
        } else {
            // Save raw line
            current_lines = realloc(current_lines, (current_line_count + 1) * sizeof(char*));
            current_lines[current_line_count++] = strdup(line);

            char line_copy[1024];
            strncpy(line_copy, line, 1023);
            line_copy[1023] = '\0';
            char* token = strtok(line_copy, " \t");
            while (token != NULL) {
                char word[50];
                strncpy(word, token, 49);
                word[49] = '\0';

                if (normalise_word(word) > 0)
                    current_para = insert_bst_NW(current_para, word);
                token = strtok(NULL, " \t");
            }
        }
    }

    if (current_para != NULL)
        insert_para(list, current_para, para_id, current_lines, current_line_count);

    fclose(file);
    return list;
} //loading the files

// -------- SET OPERATIONS --------
WordNode* copy_into(WordNode* A, WordNode* B) {
    if (A == NULL) return B;

    B = insert_bst_NW(B, A->word);
    B = copy_into(A->left, B);
    B = copy_into(A->right, B);

    return B;
} // Copy all words from BST A into BST B

WordNode* UNION(WordNode* A, WordNode* B) {
    WordNode* unionTree = NULL;
    unionTree = copy_into(A, unionTree);
    unionTree = copy_into(B, unionTree);
    return unionTree;
} // Return the union of two BSTs A and B

WordNode* common_words(WordNode* A, WordNode* B, WordNode* result) {
    if (A == NULL) return result;
    if (search_bst(B, A->word))
        result = insert_bst_NW(result, A->word);
    result = common_words(A->left, B, result);
    result = common_words(A->right, B, result);
    return result;
} // Collect common words between BST A and B

WordNode* INTERSECTION(WordNode* A, WordNode* B) {
    WordNode* interTree = NULL;
    interTree = common_words(A, B, interTree);
    return interTree;
} // Return the intersection of two BSTs A and B

WordNode* diff_words(WordNode* A, WordNode* B, WordNode* result) {
    if (A == NULL) return result;

    if (!search_bst(B, A->word))
        result = insert_bst_NW(result, A->word);

    result = diff_words(A->left, B, result);
    result = diff_words(A->right, B, result);

    return result;
} // Collect words in A that are not in B

WordNode* DIFFERENCE(WordNode* A, WordNode* B) {
    WordNode* diffTree = NULL;
    diffTree = diff_words(A, B, diffTree);
    return diffTree;
} // Return the difference of two BSTs A and B


