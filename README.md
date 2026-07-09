# Text Set Operations Lab
A C-based interactive CLI application for performing **set operations on paragraph word sets** extracted from text files. This tool lets you load documents, decompose them into paragraph-level word sets stored as Binary Search Trees, and chain Union, Intersection, and Difference operations across any combination of paragraphs.

---

## 🚀 Features

### 📂 File Management
* Load up to 16 text files simultaneously
* Duplicate file detection — same file won't be loaded twice
* Browse loaded files at any time from the Files menu

### 🔍 Paragraph Exploration
* View all paragraphs of any loaded file as formatted cards
* Each card shows the raw text and its extracted word set
* Word counts displayed per paragraph

### ➗ Set Operations
* **Union** — all words in A or B
* **Intersection** — words common to both A and B
* **Difference** — words in A that are not in B
* Chain multiple operations sequentially on the result
* Venn diagram displayed after each operation with word counts

### 📊 Result Inspection
* View the result word set in a formatted grid
* Word count tracked and displayed in the status bar at all times

### 🎨 Terminal UI
* Colored output with ANSI escape codes
* Box-drawing borders and visual separators
* Animated splash screen and exit screen
* Auto-centered layout that adapts to terminal width
* Cross-platform: Windows (PowerShell) & Linux / macOS

---

## 📂 Project Structure
```text
ADDS-Lab/
├── include/
│   └── library.h    # Data structure definitions and function declarations
├── src/
│   ├── main.c       # UI, menus, animations, and program entry point
│   └── library.c    # BST, paragraph list, set operations implementations
├── tests/
│   └── (sample text files)
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

---

## 🛠️ System Requirements

* C compiler (GCC recommended)
* Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `stdbool.h`)
* **Windows:** PowerShell (not CMD) for correct UTF-8 rendering

---

## ⚙️ Compilation & Execution

### 🪟 Windows — PowerShell

> ⚠️ Use **PowerShell**, not Command Prompt (CMD).

```powershell
chcp 65001
make
.\adds-lab.exe
```

> `chcp 65001` switches the Windows console to UTF-8 so box-drawing characters and symbols render correctly.

### 🐧 Linux / 🍎 macOS

```bash
make
./adds-lab
```

---

## 🔄 Workflow

### 📥 Loading Files
* Launch the program and navigate to **Files → Load Text Files**
* Enter the number of files to load, then provide each filename
* Text files must use **blank lines** to separate paragraphs
* The program parses each paragraph into a normalized word set (BST)

### ➗ Applying Operations
* Navigate to **Operations → Choose Set Operation** and select U, I, or D
* Then **Apply Operation** — pick a file and paragraph number
* The first paragraph selected seeds the result; subsequent applications chain onto it
* A Venn diagram is shown after each operation

### 📊 Viewing Results
* Navigate to **Result → View & Inspect Result Set**
* Words are displayed in a 4-column alphabetically sorted grid
* You can chain another operation directly from this menu

---

## 📐 Data Model

### 🌳 Word Set — BST (`WordNode`)
Each paragraph's vocabulary is stored as a Binary Search Tree for efficient insertion and lookup.

Each node contains:
* **Word:** Up to 50 characters, normalized (lowercased, punctuation-stripped)
* **Left / Right pointers:** For BST structure (alphabetical ordering)

### 📄 Paragraph Node (`ParaNode`)
Each paragraph in a loaded file is represented as a node in a linked list.

Each node contains:
* **Paragraph number:** Sequential ID assigned during loading
* **Word set:** Pointer to the paragraph's BST
* **Raw lines:** Array of original text lines for display
* **Line count:** Number of lines in the paragraph
* **Next pointer:** Link to the next paragraph

### 📋 Paragraph List (`ParaList`)
A singly linked list holding all paragraphs of one loaded file, in order of appearance.

---

## 🔧 Word Normalization

Before a word is inserted into a BST, it goes through three steps:

1. **Punctuation removal** — all punctuation stripped, except apostrophes within words (e.g. `don't` is preserved)
2. **Lowercasing** — converted to lowercase for case-insensitive comparison
3. **Empty check** — if the result is an empty string, the word is discarded

---

## 📖 Set Operations

| Operation | Description | Result |
|-----------|-------------|--------|
| `UNION(A, B)` | All words in A or B | New BST, A and B untouched |
| `INTERSECTION(A, B)` | Words present in both A and B | New BST, A and B untouched |
| `DIFFERENCE(A, B)` | Words in A that are not in B | New BST, A and B untouched |

All operations produce a **fresh BST** — the original paragraph word sets are never modified.

---

## 👥 Contributors

By **GHEDBANE Ines Rym** (G04) and **AGGOUN Houcine** (G03)

* This project was developed as part of **TP C — Text Set Operations** in the context of the **Algorithmics & Dynamic Data Structures** course.