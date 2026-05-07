// ============================================================
//  ADDS Lab - Text Set Operations
//  by: GHEDBANE INES RYM & AGGOUN HOUCINE
//
//  Menu structure:
//
//  Main Menu
//  |-- 1. Files
//  |     |-- 1. Load text file(s)                  [option 1]
//  |     +-- 2. View paragraphs of a file
//  |-- 2. Operations
//  |     |-- 1. Select operation (U/I/D)            [option 2]
//  |     +-- 2. Select file & paragraph to apply    [option 3]
//  |-- 3. Result
//  |     |-- 1. View current result                 [option 4]
//  |     +-- 2. Repeat from step 2 or exit          [option 5]
//  +-- 0. Exit
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define MAX_FILES 16

// --- Global state --------------------------------------------

ParaList* files[MAX_FILES];           // array of loaded paragraph lists
char      file_names[MAX_FILES][256]; // corresponding filenames
int       file_count = 0;             // number of files currently loaded

WordNode* result     = NULL;  // the current result buffer (BST of words)
char      current_op = '\0';  // currently selected operation: 'U', 'I', or 'D'

// --- Utility functions ---------------------------------------

// Discard leftover characters in stdin (after a bad scanf)
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Read an integer from stdin with a prompt, returns 1 on success
int read_int(const char* prompt, int* out) {
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        flush_input();
        printf("Invalid input. Please enter a number.\n");
        return 0;
    }
    return 1;
}

// Display persistent status bar
void print_status_bar() {
    printf("+-- STATUS [Files: %d loaded] [Operation: %s] [Result: %s] ---+\n",
           file_count,
           current_op == '\0' ? "none"
           : current_op == 'U' ? "UNION"
           : current_op == 'I' ? "INTERSECTION" : "DIFFERENCE",
           result == NULL ? "empty" : "ready");
}

// Print the list of currently loaded files with their paragraph counts
void print_loaded_files() {
    if (file_count == 0) {
        printf("No files loaded yet.\n");
        return;
    }
    printf("Loaded files:\n");
    for (int i = 0; i < file_count; i++) {
        int pc = count_paragraphs(files[i]);
        printf("  [%d] %s  (%d paragraph%s)\n",
               i, file_names[i], pc, pc == 1 ? "" : "s");
    }
}

// --- Files sub-menu actions ----------------------------------

// Option 1 - Load one or more text files into memory.
// Can be called multiple times to add more files at any point.
void action_load_files() {
    int n;
    if (!read_int("How many files do you want to load? ", &n)) return;
    if (n <= 0) { printf("Nothing to load.\n"); return; }

    for (int i = 0; i < n; i++) {
        if (file_count >= MAX_FILES) {
            printf("Maximum number of files (%d) already loaded.\n", MAX_FILES);
            break;
        }

        char fname[256];
        printf("Enter filename %d: ", i + 1);
        scanf("%255s", fname);

        // Prevent loading the same file twice
        int already = 0;
        for (int j = 0; j < file_count; j++) {
            if (strcmp(file_names[j], fname) == 0) { already = 1; break; }
        }
        if (already) {
            printf("'%s' is already loaded. Skipping.\n", fname);
            continue;
        }

        // Parse the file into a paragraph list (each paragraph -> word BST)
        ParaList* loaded = para_list_load(fname);
        if (loaded == NULL) {
            printf("Could not open '%s'. Check the filename and try again.\n", fname);
            continue;
        }

        files[file_count] = loaded;
        strncpy(file_names[file_count], fname, 255);
        file_names[file_count][255] = '\0';
        printf("File [%d] '%s' loaded - %d paragraph(s).\n",
               file_count, fname, count_paragraphs(loaded));
        file_count++;
    }
}

// Show all paragraphs of a chosen file, with their original text and word sets
void action_view_paragraphs() {
    if (file_count == 0) { printf("No files loaded.\n"); return; }

    print_loaded_files();

    int fi;
    if (!read_int("Select file index: ", &fi)) return;
    if (fi < 0 || fi >= file_count) { printf("Invalid file index.\n"); return; }

    printf("\nParagraphs of '%s':\n", file_names[fi]);

    ParaNode* cur = files[fi]->head;
    if (cur == NULL) { printf("(no paragraphs)\n"); return; }

    while (cur != NULL) {
        printf("\n  Paragraph number : %d\n", para_num(cur));

        // Print the raw original lines of this paragraph
        printf("  Content:\n");
        for (int i = 0; i < cur->line_count; i++)
            printf("    %s\n", cur->lines[i]);

        // Print the normalised, sorted unique word set for this paragraph
        printf("  Word set (%d unique words): ", count_words(word_set(cur)));
        inorder_traversal(word_set(cur));
        printf("\n");

        cur = next_para(cur);
    }
}

// Sub-menu for everything related to files
void menu_files() {
    int running = 1;
    while (running) {
        print_status_bar();
        printf("  FILES MANAGEMENT\n");
        printf("  1. Load text file(s)\n");
        if (file_count > 0) {
            printf("  2. View paragraphs of a file  [OK]\n");
        } else {
            printf("  2. View paragraphs of a file  (load files first)\n");
        }
        printf("  0. Back to Main Menu\n");
        printf("-------------------------------------\n");

        int choice;
        if (!read_int("Your choice: ", &choice)) continue;

        switch (choice) {
            case 1: action_load_files();      break;
            case 2:
                if (file_count == 0)
                    printf(">> Load files first.\n");
                else
                    action_view_paragraphs();
                break;
            case 0: running = 0;              break;
            default: printf(">> Unknown option.\n"); break;
        }
    }
}

// --- Operations sub-menu actions -----------------------------

// Option 2 - Let the user pick which set operation to use next.
// The choice is stored in current_op and reused until changed.
void action_select_operation() {
    printf("Select operation:\n");
    printf("  U. UNION         (all words in A or B)\n");
    printf("  I. INTERSECTION  (only words in both A and B)\n");
    printf("  D. DIFFERENCE    (words in A that are not in B)\n");
    printf("Your choice (U/I/D): ");

    char buf[8];
    scanf("%7s", buf);
    char c = buf[0];
    if (c >= 'a' && c <= 'z') c = (char)(c - 32); // accept lowercase too

    if (c != 'U' && c != 'I' && c != 'D') {
        printf("Invalid operation. Please enter U, I, or D.\n");
        return;
    }

    current_op = c;
    const char* opname = (c == 'U') ? "UNION"
                       : (c == 'I') ? "INTERSECTION" : "DIFFERENCE";
    printf("Operation %s selected. Use option 2 to apply it to a paragraph.\n", opname);
}

// Option 3 - Apply the selected operation between the current result
// and a paragraph chosen by the user (file index + paragraph ID).
// If the result buffer is empty, the chosen paragraph becomes the seed.
// Can be called repeatedly to chain multiple operations.
void action_apply() {
    if (file_count == 0) { printf("No files loaded.\n"); return; }
    if (current_op == '\0') {
        printf("No operation selected yet. Use option 1 first.\n");
        return;
    }

    print_loaded_files();

    int fi;
    if (!read_int("Select file index: ", &fi)) return;
    if (fi < 0 || fi >= file_count) { printf("Invalid file index.\n"); return; }

    int pid;
    if (!read_int("Enter paragraph number: ", &pid)) return;

    ParaNode* p = get_para(files[fi], pid);
    if (p == NULL) {
        printf("No paragraph with number %d in '%s'.\n", pid, file_names[fi]);
        return;
    }

    WordNode* B          = word_set(p);
    WordNode* new_result = NULL;
    const char* opname   = (current_op == 'U') ? "UNION"
                         : (current_op == 'I') ? "INTERSECTION" : "DIFFERENCE";

    if (result == NULL) {
        // Result buffer is empty: seed it with the chosen paragraph
        new_result = copy_bst(B);
        printf("Result seeded with paragraph %d from '%s' (%d words).\n",
               pid, file_names[fi], count_words(new_result));
    } else {
        // Apply the selected operation: result = result OP B
        int before = count_words(result);
        if (current_op == 'U') new_result = UNION(result, B);
        if (current_op == 'I') new_result = INTERSECTION(result, B);
        if (current_op == 'D') new_result = DIFFERENCE(result, B);
        free_bst(result); // free the old result before replacing it
        printf("%s applied. Word count: %d -> %d\n",
               opname, before, count_words(new_result));
    }

    result = new_result;
}

// Sub-menu for set operations
void menu_operations() {
    int running = 1;
    while (running) {
        print_status_bar();
        printf("  SET OPERATIONS\n");
        printf("  1. Select operation (U/I/D)\n");

        if (current_op != '\0') {
            const char* opname = (current_op == 'U') ? "UNION"
                               : (current_op == 'I') ? "INTERSECTION" : "DIFFERENCE";
            printf("  2. Apply %s to a paragraph  [OK]\n", opname);
        } else {
            printf("  2. Apply operation  (select one first)\n");
        }

        if (file_count == 0) {
            printf("  (load files first)\n");
        }
        printf("  0. Back to Main Menu\n");
        printf("------------------------------------\n");

        int choice;
        if (!read_int("Your choice: ", &choice)) continue;

        switch (choice) {
            case 1: action_select_operation(); break;
            case 2:
                if (file_count == 0)
                    printf(">> Load files first.\n");
                else if (current_op == '\0')
                    printf(">> Select an operation first (option 1).\n");
                else
                    action_apply();
                break;
            case 0: running = 0; break;
            default: printf(">> Unknown option.\n"); break;
        }
    }
}

// --- Result sub-menu actions ---------------------------------

// Option 4 - Print the current result buffer (sorted word list)
void action_view_result() {
    if (result == NULL) {
        printf("Result is empty. Load files and apply operations first.\n");
        return;
    }
    int wc = count_words(result);
    printf("\nCurrent result (%d word%s):\n", wc, wc == 1 ? "" : "s");
    inorder_traversal(result);
    printf("\n");
}

// Option 5 - After viewing the result, let the user decide:
//   go back to Operations to chain another operation, or exit the program.
// Returns 1 if the user chose to exit, 0 to stay.
int action_repeat_or_exit() {
    printf("\nWhat would you like to do next?\n");
    printf("  1. Apply another operation\n");
    printf("  0. Exit program\n");

    int choice;
    if (!read_int("Your choice: ", &choice)) return 0;

    if (choice == 1) {
        menu_operations();
        return 0;
    } else if (choice == 0) {
        return 1;
    }
    return 0;
}

// Sub-menu for viewing and acting on the result.
// Returns 1 if the user chose to exit from inside this menu.
int menu_result() {
    int running = 1;
    while (running) {
        print_status_bar();
        printf("  VIEW RESULT\n");

        if (result == NULL) {
            printf("  1. View current result  (no result yet)\n");
            printf("  0. Back to Main Menu\n");
        } else {
            printf("  1. View current result  [OK] (%d words)\n", count_words(result));
            printf("  2. Continue with operations\n");
            printf("  0. Back to Main Menu\n");
        }
        printf("---------------------------\n");

        int choice;
        if (!read_int("Your choice: ", &choice)) continue;

        switch (choice) {
            case 1:
                if (result == NULL)
                    printf(">> Load files and apply operations first.\n");
                else
                    action_view_result();
                break;
            case 2:
                if (result == NULL)
                    printf(">> Generate a result first.\n");
                else if (action_repeat_or_exit())
                    return 1;
                break;
            case 0:
                running = 0;
                break;
            default:
                printf(">> Unknown option.\n");
                break;
        }
    }
    return 0;
}

// --- Main menu -----------------------------------------------

int main(void) {
    printf("\n");
    printf("===============================================\n");
    printf("  WELCOME TO TEXT SET OPERATIONS TOOL\n");
    printf("  Load files -> Select operations -> Chain\n");
    printf("===============================================\n\n");

    int running = 1;
    while (running) {
        print_status_bar();
        printf("  MAIN MENU\n");
        printf("  1. Files     ");
        if (file_count > 0) printf("(%d loaded) [OK]\n", file_count);
        else printf("(0 loaded)\n");

        printf("  2. Operations");
        if (current_op != '\0') {
            const char* op = (current_op == 'U') ? "UNION"
                           : (current_op == 'I') ? "INTERSECTION" : "DIFFERENCE";
            printf(" (%s) [OK]\n", op);
        } else {
            printf("\n");
        }

        printf("  3. Result    ");
        if (result != NULL) printf("(%d words) [OK]\n", count_words(result));
        else printf("\n");

        printf("  0. Exit Program\n");
        printf("-------------------------------\n");

        int choice;
        if (!read_int("Your choice: ", &choice)) continue;

        switch (choice) {
            case 1:
                menu_files();
                break;
            case 2:
                menu_operations();
                break;
            case 3:
                if (menu_result()) running = 0;
                break;
            case 0:
                running = 0;
                printf("\nGoodbye!\n");
                break;
            default:
                printf(">> Unknown option.\n");
                break;
        }
    }

    // Free all dynamically allocated memory before exiting
    for (int i = 0; i < file_count; i++) free_para_list(files[i]);
    if (result) free_bst(result);
    return 0;
}