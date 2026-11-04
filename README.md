
# ParaCL Language Interpreter

A modern, light-weight, AST-based interpreter for the **ParaCL** programming language, implemented in **C++23**.

The project features a full compiler pipeline: from lexical analysis (Lexer) and recursive descent parsing with operator precedence climbing (Parser) to Abstract Syntax Tree (AST) evaluation and automated Graphviz tree visualization.

---

## 🛠 Architecture & Features

### 1. Lexical Analysis (`Lexer`)

- **Tokenization:** Converts raw source code into structured tokens.
- **Language Keywords:** Supports control flow (`while`, `if`, `else`), input/output (`print`, `?`), and identifiers.
- **Robust Error Handling:** Detects invalid character inputs (e.g., unexpected symbols or broken compound operators).

### 2. Syntax Analysis & Symbol Table (`Parser`)

- **Recursive Descent Parser:** Evaluates grammar rules cleanly and predictably.
- **Operator Precedence Climbing:** Correctly builds AST subtrees according to standard mathematical and logical operator precedence (`*`, `/`, `%` > `+`, `-` > `<`, `>`, `<=`, `>=` > `==`, `!=`).
- **$O(1)$ Symbol Table:** Identifiers are mapped to zero-based numeric IDs at parse time, enabling fast array-indexed variable access during execution without string hashing.

### 3. AST & Execution Engine (`Nodes`)

- **Polymorphic AST Design:** Every node implements `eval(ExecutionContext&)` and `dump_dot(std::ostream&, int&)`.
- **Control Flow:** Full support for conditional execution (`if-else`), loops (`while`), and block scoping (`{ ... }`).
- **Automated Visualization:** Automatically generates an `ast.dot` graph description and invokes Graphviz (`dot -Tpng ast.dot -o ast.png`) upon execution to create an `ast.png` visual diagram of your program's syntax tree.

---

## ⚙️ Prerequisites & Dependencies

- **C++ Compiler:** Supporting **C++23** standard:
  - GCC $\ge$ 13 (recommended: GCC 13+)
  - Clang $\ge$ 16 (recommended: Clang 16+)
  - Apple Clang (Xcode $\ge$ 15, included with macOS Sonoma/Sequoia)
- **Build System:** CMake $\ge$ 3.20 (version 3.20+ recommended for complete C++23 feature support).
- **Unit Testing:** Google Test (fetched automatically via CMake `FetchContent`).
- **Visualization (Optional):** [Graphviz](https://graphviz.org/) to automatically generate `ast.png` diagrams.
