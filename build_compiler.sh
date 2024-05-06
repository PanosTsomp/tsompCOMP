bash
Copy code
#!/bin/bash

# Exit on any error
set -e

# Generate the lexer and parser source files
flex -o src/ts_lexer.c src/ts_lexer.l
bison -d -o src/ts_parser.c src/ts_parser.y

# Compile the lexer, parser, and other source files
gcc -Wall -Isrc -c src/ts_parser.c -o src/ts_parser.o
gcc -Wall -Isrc -c src/ts_lexer.c -o src/ts_lexer.o
gcc -Wall -Isrc -c src/semantic_analysis.c -o src/semantic_analysis.o
gcc -Wall -Isrc -c src/code_generation.c -o src/code_generation.o
gcc -Wall -Isrc -c src/main.c -o src/main.o

# Link all object files into the executable
gcc src/ts_parser.o src/ts_lexer.o src/semantic_analysis.o src/code_generation.o src/main.o -o bin/tsompCC -lfl


echo "Build completed successfully."