// ============================================================
//  ADDS Lab - Text Set Operations
//  Supervised by : Dr. KERMI ADEL
//  Authors       : GHEDBANE INES RYM  &  AGGOUN HOUCINE
//  Module        : Algorithmics & Dynamic Data Structures
//  Academic Year : 2025 / 2026
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#ifdef _WIN32
  #include <windows.h>
  #define sleep_ms(ms) Sleep(ms)
  #ifdef DIFFERENCE
    #undef DIFFERENCE
  #endif
#else
  #include <unistd.h>
    #include <sys/ioctl.h>
  #define sleep_ms(ms) usleep((ms) * 1000)
#endif

#define MAX_FILES 16

// === Colour macros ===

#define RST   "\033[0m"
#define BOLD  "\033[1m"

// Primary palette
#define CYN   "\033[0;36m"    // cyan   
#define BCYN  "\033[1;36m"    // bold cyan
#define GLD   "\033[0;33m"    // gold   
#define BGLD  "\033[1;33m"    // bold gold
#define MGT   "\033[0;35m"    // magenta 
#define BMGT  "\033[1;35m"    // bold magenta

// Supporting
#define GRN   "\033[0;92m"    // bright green
#define SGRN  "\033[0;32m"    // soft green   
#define RED   "\033[0;31m"    // red          
#define WHT   "\033[0;97m"    // white        
#define GRY   "\033[0;90m"    // dark grey    
#define IGRY  "\033[2;37m"    // dim grey     

// === Box-drawing ===
#define TL  "╔"
#define TR  "╗"
#define BL  "╚"
#define BR  "╝"
#define HL  "═"
#define VL  "║"
#define LT  "╠"
#define RT  "╣"

#define STL "┌"
#define STR "┐"
#define SBL "└"
#define SBR "┘"
#define SHL "─"
#define SVL "│"
#define SLT "├"
#define SRT "┤"

// Width of all menus 
#define W 62

// Helpers to print padded box rows without ANSI-width bugs

// Compute horizontal offset so framed blocks are centered in the terminal
static int get_terminal_width(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    }
    return 80;
#else
    struct winsize ws;
    if (ioctl(1, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        return (int)ws.ws_col;
    }
    return 80;
#endif
}

static int frame_margin(void) {
    int total_w = W + 2; 
    int term_w = get_terminal_width();
    int pad = (term_w - total_w) / 2;
    return pad > 0 ? pad : 0;
}

static void print_indent(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

static void row(const char* col, const char* text, int content_w) {
    int len = (int)strlen(text);
    int pad = content_w - len;
    if (pad < 0) pad = 0;
    print_indent(frame_margin());
    printf(CYN VL RST " %s%s%s%*s " CYN VL RST "\n", col, text, RST, pad, "");
}

// Same but text is centred
static void rowc(const char* col, const char* text, int content_w) {
    int len   = (int)strlen(text);
    int total = content_w - len;
    int lpad  = total / 2;
    int rpad  = total - lpad;
    print_indent(frame_margin());
    printf(CYN VL RST "%*s%s%s%s%*s" CYN VL RST "\n",
           lpad + 1, "", col, text, RST, rpad + 1, "");
}


static void top_rule(void) {
    print_indent(frame_margin());
    printf(CYN TL);
    for (int i = 0; i < W; i++) printf(HL);
    printf(TR RST "\n");
}
static void mid_rule(void) {
    print_indent(frame_margin());
    printf(CYN LT);
    for (int i = 0; i < W; i++) printf(HL);
    printf(RT RST "\n");
}
static void bot_rule(void) {
    print_indent(frame_margin());
    printf(CYN BL);
    for (int i = 0; i < W; i++) printf(HL);
    printf(BR RST "\n");
}

static void sep_row(void) {
    print_indent(frame_margin());
    printf(CYN VL GRY);
    for (int i = 0; i < W; i++) printf(SHL);
    printf(RST CYN VL RST "\n");
}

static void blank_row(void) {
    print_indent(frame_margin());
    printf(CYN VL "%*s" VL RST "\n", W, "");
}

// ── Global state ─────────────────────────────────────────────
ParaList* files[MAX_FILES];
char      file_names[MAX_FILES][256];
int       file_count = 0;

WordNode* result     = NULL;
char      current_op = '\0';

// ─────────────────────────────────────────────────────────────
//  BASIC UTILITIES
// ─────────────────────────────────────────────────────────────

void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int read_int(const char* prompt, int* out) {
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        flush_input();
        printf(RED "  ✗  Invalid input — please enter a number.\n" RST);
        return 0;
    }
    flush_input();
    return 1;
}

void clrscr(void) { printf("\033[2J\033[H"); }

void pause_return(const char* menu_name) {
    printf("\n" IGRY "  Press Enter to return to %s..." RST, menu_name);
    getchar();
}

// ─────────────────────────────────────────────────────────────
//  SPLASH SCREEN
// ─────────────────────────────────────────────────────────────

void printESI(void) {
    clrscr();

    printf("\n\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m     \033[0;104m         \033[0m \n");
    sleep_ms(110);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m   \033[0;104m         \033[0m  \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;104m         \033[0m \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m             \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m                      \033[0;107m       \033[0m       L'ECOLE  NATIONALE\n");
    sleep_ms(110);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(110);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m       SUPERIEURE\n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m                                        \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(110);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m       D'INFORMATIQUE\n");
    sleep_ms(110);
    printf("\t      \033[0;107m      \033[0m          \033[0;107m      \033[0m      \033[0;107m      \033[0m          \033[0;107m      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(110);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(110);
    printf("\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m       \033[0;107m       \033[0m  \n");
    sleep_ms(220);

    printf("\n");
    top_rule();
    rowc(BGLD, "WELCOME TO THE TEXT SET OPERATIONS LAB", W - 2);
    mid_rule();
        sleep_ms(80);
    blank_row();
    rowc(MGT,  "Algorithmics & Dynamic Data Structures", W - 2);
        sleep_ms(70);
    blank_row();
    rowc(WHT,  "This tool lets you load text files, decompose them into", W - 2);
        sleep_ms(60);
    rowc(WHT,  "paragraph word sets (stored as BSTs), then chain set", W - 2);
        sleep_ms(60);
    rowc(WHT,  "operations — Union, Intersection, Difference — across", W - 2);
        sleep_ms(60);
    rowc(WHT,  "any combination of paragraphs from any loaded file.", W - 2);
    blank_row();
    sep_row();
    blank_row();

        print_indent(frame_margin());
    printf(CYN VL RST "  " GLD "Supervised by  " RST WHT ":  " BCYN "Dr. KERMI ADEL" RST
           "%*s" CYN VL RST "\n", W - 34, "");
        print_indent(frame_margin());
    printf(CYN VL RST "  " GLD "Authors        " RST WHT ":  " MGT  "GHEDBANE Ines Rym   &   AGGOUN Houcine" RST
           "%*s" CYN VL RST "\n", W - 57, "");
        print_indent(frame_margin());
    printf(CYN VL RST "  " GLD "Academic Year  " RST WHT ":  " GRN  "2025 / 2026" RST
           "%*s" CYN VL RST "\n", W - 31, "");

    blank_row();
    bot_rule();

    printf("\n" RED "              Please — Press Enter to begin..." RST "  ");
    getchar();
    clrscr();
}

// ─────────────────────────────────────────────────────────────
//  EXIT ANIMATION
// ─────────────────────────────────────────────────────────────

void print_exit_animation(void) {
    clrscr();
    sleep_ms(80);
    printf("\n\n");
    top_rule();
    rowc(BGLD, "SESSION ENDED", W - 2);
    mid_rule();
    blank_row();
    rowc(WHT,  "Thank you for using the Text Set Operations Lab.", W - 2);
    blank_row();
        print_indent(frame_margin());
    printf(CYN VL RST "  " GLD "Supervised by  " RST WHT ":  " BCYN "Dr. KERMI ADEL" RST
           "%*s" CYN VL RST "\n", W - 34, "");
        print_indent(frame_margin());
    printf(CYN VL RST "  " GLD "Authors        " RST WHT ":  " MGT  "GHEDBANE Ines Rym   &   AGGOUN Houcine" RST
           "%*s" CYN VL RST "\n", W - 57, "");
    blank_row();
    rowc(SGRN, "Goodbye!", W - 2);
    blank_row();
    bot_rule();
    printf("\n\n");
}

// ─────────────────────────────────────────────────────────────
//  STATUS BAR
// ─────────────────────────────────────────────────────────────

void print_status_bar(void) {
    int all_ready = (file_count > 0) && (current_op != '\0') && (result != NULL);

    const char* op_str =
        current_op == 'U' ? "UNION" :
        current_op == 'I' ? "INTERSECTION" :
        current_op == 'D' ? "DIFFERENCE"   : "none";

    const char* op_col = (current_op == '\0') ? GRY : BMGT;
    const char* bar_col = all_ready ? SGRN : GLD;

    char files_str[32], op_disp[32], res_str[32];
    if (file_count > 0)
        snprintf(files_str, sizeof(files_str), "%d loaded", file_count);
    else
        snprintf(files_str, sizeof(files_str), "none");

    snprintf(op_disp,  sizeof(op_disp),  "%s", op_str);

    if (result != NULL)
        snprintf(res_str, sizeof(res_str), "%d word%s",
                 count_words(result), count_words(result) == 1 ? "" : "s");
    else
        snprintf(res_str, sizeof(res_str), "—");

    int cw = (W - 6) / 3;  

    top_rule();

    // Row: ║  Files: X  ║  Operation: X  ║  Result: X  ║
    print_indent(frame_margin());
    printf(CYN VL RST);
    printf("  " GLD "Files" RST ": %s%-*s" RST, bar_col, cw - 7, files_str);
    printf(CYN VL RST);
    printf("  " GLD "Operation" RST ": %s%-*s" RST, op_col, cw - 12, op_disp);
    printf(CYN VL RST);
    printf("  " GLD "Result" RST ": %s%-*s" RST, MGT, cw - 9, res_str);
    printf(CYN VL RST "\n");

    bot_rule();
    printf("\n");
}

// ─────────────────────────────────────────────────────────────
//  PARAGRAPH PREVIEW CARDS  (single-border inner style)
// ─────────────────────────────────────────────────────────────

void print_para_card(ParaNode* node, int fi) {
    int wc = count_words(word_set(node));

    print_indent(frame_margin());
    printf(CYN STL SHL " " BGLD "Paragraph #%d" RST CYN " " SHL SHL " " GLD "%s" RST CYN,
           para_num(node), file_names[fi]);
 
    int used = 16 + (int)strlen(file_names[fi]);
    for (int i = used; i < W - 2; i++) printf(SHL);
    printf(STR RST "\n");

    for (int i = 0; i < node->line_count; i++)
        print_indent(frame_margin()),
        printf(CYN SVL RST "   " WHT "%s" RST "\n", node->lines[i]);

    print_indent(frame_margin());
    printf(CYN SLT);
    for (int i = 0; i < W - 2; i++) printf(SHL);
    printf(SRT RST "\n");

    print_indent(frame_margin());
    printf(CYN SVL RST "  " BMGT "Word set " GRY "(%d unique word%s)" RST ":  ",
           wc, wc == 1 ? "" : "s");
    inorder_traversal(word_set(node));
    printf("\n");

    print_indent(frame_margin());
    printf(CYN SBL);
    for (int i = 0; i < W - 2; i++) printf(SHL);
    printf(SBR RST "\n");
}

// ─────────────────────────────────────────────────────────────
//  WORD GRID
// ─────────────────────────────────────────────────────────────

static char** _warr   = NULL;
static int    _warr_n = 0;

void _collect(WordNode* R) {
    if (!R) return;
    _collect(R->left);
    _warr[_warr_n++] = R->word;
    _collect(R->right);
}

void print_word_grid(WordNode* R) {
    int total = count_words(R);
    if (total == 0) { printf(GRY "  (empty)\n" RST); return; }

    _warr   = malloc(total * sizeof(char*));
    _warr_n = 0;
    _collect(R);

    int cols  = 4;
    int cw    = (W - 2) / cols;   

    top_rule();
    char title[32];
    snprintf(title, sizeof(title), "RESULT  —  %d WORD%s", total, total == 1 ? "" : "S");
    rowc(BGLD, title, W - 2);
    mid_rule();

    for (int i = 0; i < total; i += cols) {
        print_indent(frame_margin());
        printf(CYN VL RST);
        for (int c = 0; c < cols; c++) {
            if (i + c < total)
                printf(" " BMGT "%-*s" RST CYN VL RST, cw - 2, _warr[i + c]);
            else
                printf(" %-*s" CYN VL RST, cw - 2, "");
        }
        printf("\n");
    }

    bot_rule();
    free(_warr);
    _warr   = NULL;
    _warr_n = 0;
}

// ─────────────────────────────────────────────────────────────
//  VENN DIAGRAM
// ─────────────────────────────────────────────────────────────

void print_venn(char op, int a_only, int shared, int b_only) {
    const char* op_name =
        op == 'U' ? "UNION" : op == 'I' ? "INTERSECTION" : "DIFFERENCE";
    const char* op_sym  =
        op == 'U' ? "A \xe2\x88\xaa B" : op == 'I' ? "A \xe2\x88\xa9 B" : "A \\ B";

    int result_size =
        op == 'U' ? a_only + shared + b_only :
        op == 'I' ? shared : a_only;

    printf("\n");
    top_rule();

    char title[64];
    snprintf(title, sizeof(title), "SET OPERATION DIAGRAM  \xe2\x80\x94  %s", op_name);
    rowc(BGLD, title, W - 2);
    mid_rule();
    blank_row();

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD  "  \xe2\x95\xad\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x95\xae" RST
           "     " MGT "\xe2\x95\xad\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x95\xae" RST
           "        " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "\xe2\x94\x82  A  only" RST
           "                    " BCYN "\xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97" RST
           "                    " MGT "\xe2\x94\x82  B  only" RST
           "         " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "\xe2\x94\x82" RST
           "                     " BCYN "\xe2\x95\x91" RST " " BCYN "\xe2\x95\x91" RST
           "                     " MGT "\xe2\x94\x82" RST
           "         " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "\xe2\x94\x82     " RST);
    printf(GLD "%5d" RST, a_only);
    printf("               " BCYN "\xe2\x95\x91" RST);
    printf(BCYN " %3d " RST, shared);
    printf(BCYN "\xe2\x95\x91" RST);
    printf("               " MGT "%5d" RST, b_only);
    printf(MGT "     \xe2\x94\x82" RST "         " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "\xe2\x94\x82" RST
           "                     " BCYN "\xe2\x95\x91" RST " " BCYN "\xe2\x95\x91" RST
           "                     " MGT "\xe2\x94\x82" RST
           "         " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "\xe2\x94\x82  A  only" RST
           "                    " BCYN "\xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d" RST
           "                    " MGT "\xe2\x94\x82  B  only" RST
           "         " CYN VL RST "\n");

        print_indent(frame_margin());
    printf(CYN VL RST "    " GLD "  \xe2\x95\xb0\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x95\xaf" RST
           "     " MGT "\xe2\x95\xb0\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x95\xaf" RST
           "        " CYN VL RST "\n");

    blank_row();
    sep_row();

    char stat[80];
    snprintf(stat, sizeof(stat), "Operation: %s     Formula: %s     |result| = %d word%s",
             op_name, op_sym, result_size, result_size == 1 ? "" : "s");
    row(GLD, stat, W - 2);

    bot_rule();
    printf("\n");
}

// ─────────────────────────────────────────────────────────────
//  LOADED FILES LIST
// ─────────────────────────────────────────────────────────────

void print_loaded_files(void) {
    if (file_count == 0) {
        printf(GLD "  No files loaded yet.\n" RST);
        return;
    }
    printf(GLD "  Loaded files:\n" RST);
    for (int i = 0; i < file_count; i++) {
        int pc = count_paragraphs(files[i]);
        printf("  " CYN "[" GRN "%d" CYN "]" RST "  " BCYN "%-28s" RST
               "  " GRY "(%d paragraph%s)\n" RST,
               i, file_names[i], pc, pc == 1 ? "" : "s");
    }
}

// ─────────────────────────────────────────────────────────────
//  EXIT CONFIRMATION
// ─────────────────────────────────────────────────────────────

int confirm_exit(void) {
    printf("\n");
    top_rule();
    rowc(RED, "SAVE SESSION & EXIT", W - 2);
    mid_rule();
    blank_row();
    rowc(WHT, "Are you sure you want to exit?", W - 2);
    rowc(GRY,  "All loaded files and the current result will be cleared.", W - 2);
    blank_row();
    sep_row();
    rowc(GRN,  "Y — Yes, exit          N — No, go back", W - 2);
    blank_row();
    bot_rule();
    printf(IGRY "  Your choice (Y/N): " RST);

    char buf[8];
    scanf("%7s", buf);
    flush_input();
    char c = buf[0];
    if (c >= 'a' && c <= 'z') c = (char)(c - 32);
    return (c == 'Y') ? 1 : 0;
}

// ─────────────────────────────────────────────────────────────
//  ACTION — LOAD FILES
// ─────────────────────────────────────────────────────────────

void action_load_files(void) {
    int n;
    if (!read_int(GLD "  How many files to load? " RST, &n)) return;
    if (n <= 0) { printf(GRY "  Nothing to load.\n" RST); return; }

    for (int i = 0; i < n; i++) {
        if (file_count >= MAX_FILES) {
            printf(RED "  ✗  Maximum files (%d) already loaded.\n" RST, MAX_FILES);
            break;
        }
        char fname[256];
        printf(GLD "  Filename %d: " RST, i + 1);
        scanf("%255s", fname);
        flush_input();

        int already = 0;
        for (int j = 0; j < file_count; j++)
            if (strcmp(file_names[j], fname) == 0) { already = 1; break; }
        if (already) {
            printf(GLD "  ⚠  '%s' is already loaded. Skipping.\n" RST, fname);
            continue;
        }

        ParaList* loaded = para_list_load(fname);
        if (!loaded) {
            printf(RED "  ✗  Could not open '%s'.\n" RST, fname);
            continue;
        }
        files[file_count] = loaded;
        strncpy(file_names[file_count], fname, 255);
        file_names[file_count][255] = '\0';
        int pc = count_paragraphs(loaded);
        printf(SGRN "  ✓  [%d] '%s' loaded — %d paragraph%s.\n" RST,
               file_count, fname, pc, pc == 1 ? "" : "s");
        file_count++;
    }
}

// ─────────────────────────────────────────────────────────────
//  ACTION — VIEW PARAGRAPHS
// ─────────────────────────────────────────────────────────────

void action_view_paragraphs(void) {
    clrscr();
    print_status_bar();
    print_loaded_files();
    printf("\n");

    int fi;
    if (!read_int(GLD "  Select file index: " RST, &fi)) return;
    if (fi < 0 || fi >= file_count) { printf(RED "  ✗  Invalid index.\n" RST); return; }

    printf("\n" BGLD "  Paragraphs of  " BCYN "'%s'" RST "\n\n", file_names[fi]);

    ParaNode* cur = files[fi]->head;
    if (!cur) { printf(GRY "  (no paragraphs)\n" RST); return; }
    while (cur) { print_para_card(cur, fi); printf("\n"); cur = next_para(cur); }
}

// ─────────────────────────────────────────────────────────────
//  FILES SUB-MENU
// ─────────────────────────────────────────────────────────────

void menu_files(void) {
    int running = 1;
    while (running) {
        clrscr();
        print_status_bar();
        top_rule();
        rowc(BGLD, "FILES MANAGEMENT", W - 2);
        mid_rule();

        row(GRN,  "1.  Load Text Files into Memory", W - 2);

        if (file_count > 0) {
            char buf[64];
            snprintf(buf, sizeof(buf), "2.  Explore Paragraphs & Word Sets   [%d file%s ready]",
                     file_count, file_count == 1 ? "" : "s");
            row(GRN, buf, W - 2);
        } else {
            row(GRY, "2.  Explore Paragraphs & Word Sets   (load files first)", W - 2);
        }

        sep_row();
        row(RED,  "0.  Back to Main Menu", W - 2);
        bot_rule();
        printf(IGRY "  Your choice: " RST);

        int ch;
        if (scanf("%d", &ch) != 1) { flush_input(); printf(RED "  ✗  Invalid input.\n" RST); continue; }
        flush_input();

        switch (ch) {
            case 1: action_load_files(); break;
            case 2:
                if (file_count == 0) printf(GLD "  ⚠  Load files first.\n" RST);
                else                 action_view_paragraphs();
                break;
            case 0: running = 0; break;
            default: printf(RED "  ✗  Unknown option.\n" RST); break;
        }
        if (running && ch != 0) pause_return("Files Menu");
    }
}

// ─────────────────────────────────────────────────────────────
//  ACTION — SELECT OPERATION
// ─────────────────────────────────────────────────────────────

void action_select_operation(void) {
    printf("\n");
    top_rule();
    rowc(BGLD, "CHOOSE A SET OPERATION", W - 2);
    mid_rule();
    row(BCYN, "U  —  UNION          (all words in A or B)", W - 2);
    row(BCYN, "I  —  INTERSECTION   (words common to both A and B)", W - 2);
    row(BCYN, "D  —  DIFFERENCE     (words in A that are not in B)", W - 2);
    bot_rule();
    printf(GLD "  Your choice (U / I / D): " RST);

    char buf[8];
    scanf("%7s", buf);
    flush_input();
    char c = buf[0];
    if (c >= 'a' && c <= 'z') c = (char)(c - 32);

    if (c != 'U' && c != 'I' && c != 'D') {
        printf(RED "  ✗  Invalid — please enter U, I, or D.\n" RST); return;
    }
    current_op = c;
    const char* n = c == 'U' ? "UNION" : c == 'I' ? "INTERSECTION" : "DIFFERENCE";
    printf(SGRN "  ✓  Operation " BMGT "%s" SGRN " selected.\n" RST, n);
}

// ─────────────────────────────────────────────────────────────
//  ACTION — APPLY OPERATION
// ─────────────────────────────────────────────────────────────

void action_apply(void) {
    clrscr();
    print_status_bar();
    print_loaded_files();
    printf("\n");

    int fi;
    if (!read_int(GLD "  Select file index: " RST, &fi)) return;
    if (fi < 0 || fi >= file_count) { printf(RED "  ✗  Invalid index.\n" RST); return; }

    int pid;
    if (!read_int(GLD "  Enter paragraph number: " RST, &pid)) return;

    ParaNode* p = get_para(files[fi], pid);
    if (!p) { printf(RED "  ✗  Paragraph #%d not found in '%s'.\n" RST, pid, file_names[fi]); return; }

    WordNode* B = word_set(p);
    WordNode* new_result = NULL;
    const char* opname = current_op == 'U' ? "UNION"
                       : current_op == 'I' ? "INTERSECTION" : "DIFFERENCE";

    if (result == NULL) {
        new_result = copy_bst(B);
        printf(SGRN "\n  ✓  Result seeded with paragraph #%d from '%s'  (%d words).\n" RST,
               pid, file_names[fi], count_words(new_result));
    } else {
        int before = count_words(result);

        WordNode* tmp_ao = DIFFERENCE(result, B);
        WordNode* tmp_sh = INTERSECTION(result, B);
        WordNode* tmp_bo = DIFFERENCE(B, result);
        int a_only = count_words(tmp_ao);
        int shared = count_words(tmp_sh);
        int b_only = count_words(tmp_bo);
        free_bst(tmp_ao); free_bst(tmp_sh); free_bst(tmp_bo);

        if (current_op == 'U') new_result = UNION(result, B);
        if (current_op == 'I') new_result = INTERSECTION(result, B);
        if (current_op == 'D') new_result = DIFFERENCE(result, B);
        free_bst(result);

        int after = count_words(new_result);
        printf(SGRN "\n  ✓  %s applied.  " RST MGT "%d" RST SGRN "  →  " RST BMGT "%d words\n" RST,
               opname, before, after);

        print_venn(current_op, a_only, shared, b_only);
    }
    result = new_result;
}

// ─────────────────────────────────────────────────────────────
//  OPERATIONS SUB-MENU
// ─────────────────────────────────────────────────────────────

void menu_operations(void) {
    int running = 1;
    while (running) {
        clrscr();
        print_status_bar();
        top_rule();
        rowc(BGLD, "SET OPERATIONS", W - 2);
        mid_rule();
        row(GRN, "1.  Choose Set Operation   [ U · I · D ]", W - 2);

        if (current_op != '\0') {
            char buf[64];
            const char* n = current_op == 'U' ? "UNION"
                          : current_op == 'I' ? "INTERSECTION" : "DIFFERENCE";
            snprintf(buf, sizeof(buf), "2.  Apply Operation  ->  Build Result    [%s ready]", n);
            row(GRN, buf, W - 2);
        } else {
            row(GRY, "2.  Apply Operation  ->  Build Result    (select op first)", W - 2);
        }
        if (file_count == 0)
            row(GRY, "    (load files first via the Files menu)", W - 2);

        sep_row();
        row(RED, "0.  Back to Main Menu", W - 2);
        bot_rule();
        printf(IGRY "  Your choice: " RST);

        int ch;
        if (scanf("%d", &ch) != 1) { flush_input(); printf(RED "  ✗  Invalid input.\n" RST); continue; }
        flush_input();

        switch (ch) {
            case 1: action_select_operation(); break;
            case 2:
                if (file_count == 0)       printf(GLD "  ⚠  Load files first.\n" RST);
                else if (current_op == '\0') printf(GLD "  ⚠  Select an operation first (option 1).\n" RST);
                else                        action_apply();
                break;
            case 0: running = 0; break;
            default: printf(RED "  ✗  Unknown option.\n" RST); break;
        }
        if (running && ch != 0) pause_return("Operations Menu");
    }
}

// ─────────────────────────────────────────────────────────────
//  ACTION — VIEW RESULT
// ─────────────────────────────────────────────────────────────

void action_view_result(void) {
    if (!result) { printf(GLD "  ⚠  No result yet.\n" RST); return; }
    printf("\n");
    print_word_grid(result);
    printf("\n");
}

// ─────────────────────────────────────────────────────────────
//  ACTION — REPEAT OR EXIT
// ─────────────────────────────────────────────────────────────

int action_repeat_or_exit(void) {
    printf("\n");
    top_rule();
    rowc(BGLD, "WHAT WOULD YOU LIKE TO DO NEXT?", W - 2);
    mid_rule();
    row(GRN,  "1.  Chain Another Operation on the Result", W - 2);
    sep_row();
    row(RED,  "0.  Save Session & Exit", W - 2);
    bot_rule();
    printf(IGRY "  Your choice: " RST);

    int ch;
    if (scanf("%d", &ch) != 1) { flush_input(); return 0; }
    flush_input();
    if (ch == 1) { menu_operations(); return 0; }
    if (ch == 0) return confirm_exit();
    return 0;
}

// ─────────────────────────────────────────────────────────────
//  RESULT SUB-MENU
// ─────────────────────────────────────────────────────────────

int menu_result(void) {
    int running = 1;
    while (running) {
        clrscr();
        print_status_bar();
        top_rule();
        rowc(BGLD, "VIEW RESULT", W - 2);
        mid_rule();

        if (!result) {
            row(GRY, "1.  View & Inspect Result Set          (no result yet)", W - 2);
        } else {
            char buf[64];
            snprintf(buf, sizeof(buf), "1.  View & Inspect Result Set            [%d word%s]",
                     count_words(result), count_words(result) == 1 ? "" : "s");
            row(GRN, buf, W - 2);
            row(GRN, "2.  Chain Another Operation", W - 2);
        }
        sep_row();
        row(RED, "0.  Back to Main Menu", W - 2);
        bot_rule();
        printf(IGRY "  Your choice: " RST);

        int ch;
        if (scanf("%d", &ch) != 1) { flush_input(); printf(RED "  ✗  Invalid input.\n" RST); continue; }
        flush_input();

        switch (ch) {
            case 1:
                if (!result) printf(GLD "  ⚠  Load files and apply an operation first.\n" RST);
                else         action_view_result();
                break;
            case 2:
                if (!result) printf(GLD "  ⚠  Generate a result first.\n" RST);
                else if (action_repeat_or_exit()) return 1;
                break;
            case 0: running = 0; break;
            default: printf(RED "  ✗  Unknown option.\n" RST); break;
        }
        if (running && ch != 0) pause_return("Result Menu");
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────────────────────

void print_main_menu(void) {
    clrscr();
    print_status_bar();
    top_rule();
    rowc(BGLD, "TEXT SET OPERATIONS  —  MAIN MENU", W - 2);
    mid_rule();

    // Files
    if (file_count > 0) {
        char buf[64];
        snprintf(buf, sizeof(buf), "1.  Files                              [%d file%s loaded]",
                 file_count, file_count == 1 ? "" : "s");
        row(GRN, buf, W - 2);
    } else {
        row(GRN, "1.  Files", W - 2);
    }
    // Operations
    if (current_op != '\0') {
        char buf[64];
        const char* n = current_op == 'U' ? "UNION"
                      : current_op == 'I' ? "INTERS." : "DIFF.";
        snprintf(buf, sizeof(buf), "2.  Operations                         [%s selected]", n);
        row(GRN, buf, W - 2);
    } else {
        row(GRN, "2.  Operations", W - 2);
    }
    // Result
    if (result) {
        char buf[64];
        snprintf(buf, sizeof(buf), "3.  Result                             [%d word%s]",
                 count_words(result), count_words(result) == 1 ? "" : "s");
        row(MGT, buf, W - 2);
    } else {
        row(GRN, "3.  Result", W - 2);
    }

    sep_row();
    row(RED, "0.  Save Session & Exit", W - 2);
    bot_rule();
    printf(IGRY "  Your choice: " RST);
}

// ─────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────

int main(void) {
    printESI();

    int running = 1;
    while (running) {
        print_main_menu();

        int ch;
        if (scanf("%d", &ch) != 1) {
            flush_input();
            printf(RED "  ✗  Invalid input.\n" RST);
            sleep_ms(600);
            continue;
        }
        flush_input();

        switch (ch) {
            case 1: menu_files();                          break;
            case 2: menu_operations();                     break;
            case 3: if (menu_result()) running = 0;        break;
            case 0: if (confirm_exit()) running = 0;       break;
            default:
                printf(RED "  ✗  Unknown option.\n" RST);
                sleep_ms(600);
                break;
        }
    }

    print_exit_animation();

    for (int i = 0; i < file_count; i++) free_para_list(files[i]);
    if (result) free_bst(result);
    return 0;
}