#!/bin/bash

# exit on error
set -e

# Variable paths
LEXER_FILE="ts_lexer.l"
PARSER_FILE="ts_parser.y"
OUTPUT_EXECUTABLE="tsompcc"

# Are flex and bison installed?
if ! command -v flex &> /dev/null
then
    echo "Flex could not be found"
    exit 1
fi

if ! command -v bison &> /dev/null
then
    echo "Bison could not be found"
    exit 1
fi

# Generate C source from Flex and Bison specifications
echo "Generating lexer and parser code..."
flex -o ts_lexer.c $LEXER_FILE
bison -d $PARSER_FILE





# Compile the generated source code into an executable
echo "Compiling the source code..."
gcc ts_lexer.c ts_parser.tab.c -o ts_parser 


# Check if the compiler was created successfully
if [ -f "$OUTPUT_EXECUTABLE" ]; then
    echo "Compilation successful! The executable '$OUTPUT_EXECUTABLE' is ready."
else
    echo "Compilation failed."
    exit 1
fi