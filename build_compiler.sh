bash
Copy code
#!/bin/bash

# Exit on any error
set -e

# Define the source and binary directory variables
SRC_DIR="src"
BIN_DIR="build"


# Generate the lexer and parser source files
flex -o src/ts_lexer.c src/ts_lexer.l
bison -d -o src/ts_parser.c src/ts_parser.y

# Compile the lexer, parser, and other source files
gcc src/ts_lexer.c -o src/ts_lexer.o
gcc ${SRC_DIR}/ts_parser.c -o ${SRC_DIR}/ts_parser.o
gcc ${SRC_DIR}/semantic_analysis.c -o ${SRC_DIR}/semantic_analysis.o
gcc ${SRC_DIR}/code_generation.c -o ${SRC_DIR}/code_generation.o
gcc ${SRC_DIR}/main.c -o ${SRC_DIR}/main.o

# Link all object files into the executable
gcc ${SRC_DIR}/lexer.o ${SRC_DIR}/parser.o ${SRC_DIR}/semantic_analysis.o ${SRC_DIR}/code_generation.o ${SRC_DIR}/main.o -o ${BIN_DIR}/tsompCC -lfl

echo "Build completed successfully."