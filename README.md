https://www.youtube.com/watch?v=Lvc8qx8ukOI&t=525s
\
tsompCC-compiler/
├── docs/
├── include/
│   ├── tsompcc/
│   │   ├── AST.h
│   │   ├── codegen.h
│   │   ├── lexer.h
│   │   ├── parser.h
│   │   └── utils.h
├── src/
│   ├── AST.cpp
│   ├── codegen.cpp
│   ├── lexer.l
│   ├── main.cpp
│   ├── parser.y
│   └── utils/
│       ├── utils.cpp
├── tests/
├── examples/
├── scripts/
├── build/
├── CMakeLists.txt
└── README.md

tsompCC-compiler/
├── docs/                    # Documentation files
│   ├── language_spec.md     # Language specification
│   ├── compiler_design.md   # Compiler design document
│   └── user_guide.md        # User guide
├── include/                 # Header files
│   ├── AST.h                # AST node definitions
│   ├── lexer.h              # Lexer header
│   ├── parser.h             # Parser header
│   ├── codegen.h            # Code generation header
│   └── ...                  # Other headers
├── src/                     # Source code
│   ├── lexer.l              # Flex definitions
│   ├── parser.y             # Bison grammar rules
│   ├── AST.cpp              # AST node implementations
│   ├── codegen.cpp          # Code generation using LLVM
│   ├── main.cpp             # Main entry point
│   ├── utils/               # Utility functions
│   │   ├── utils.cpp
│   │   └── utils.h
│   └── ...                  # Other source files
├── tests/                   # Test files
│   ├── lexer_tests.cpp
│   ├── parser_tests.cpp
│   ├── codegen_tests.cpp
│   └── ...                  # Other test files
├── examples/                # Example programs written in tsompCC
│   ├── example1.tsompcc
│   ├── example2.tsompcc
│   └── ...
├── scripts/                 # Utility scripts (e.g., build, test)
│   ├── build.sh
│   └── test.sh
├── build/                   # Build output directory
│   ├── ...
├── CMakeLists.txt           # CMake configuration file (or equivalent)
└── README.md  

