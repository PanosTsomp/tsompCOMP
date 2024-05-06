bash
Copy code
#!/bin/bash

# Exit on any error
set -e

# Generate the lexer and parser source files
flex -o build/ts_lexer.c src/ts_lexer.l
bison -d -o build/ts_parser.c src/ts_parser.y

# Compile the lexer, parser, and other source files
gcc -Wall -Isrc -c build/ts_parser.c -o build/ts_parser.o
gcc -Wall -Isrc -c build/ts_lexer.c -o build/ts_lexer.o
gcc -Wall -Isrc -c src/semantic_analysis.c -o build/semantic_analysis.o
gcc -Wall -Isrc -c src/code_generation.c -o build/code_generation.o
gcc -Wall -Isrc -c src/main.c -o build/main.o

# Link all object files into the executable
gcc build/ts_parser.o build/ts_lexer.o build/semantic_analysis.o build/code_generation.o build/main.o -o build/tsompCC -lfl


echo "Build completed successfully."