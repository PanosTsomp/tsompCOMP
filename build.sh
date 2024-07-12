#!/bin/bash

# Create the build directory
mkdir -p build

# Run Flex to generate the lexer
flex -o build/lexer.cpp src/lexer.l

# Run Bison to generate the parser
bison -d -Wcounterexamples -o build/parser.cpp src/parser.y

# Compile the generated lexer
g++ -Iinclude -c -o build/lexer.o build/lexer.cpp

# Compile the generated parser
g++ -Iinclude -c -o build/parser.o build/parser.cpp

# Compile the AST
g++ -Iinclude -c -o build/AST.o src/AST.cpp

# Compile the codegen
g++ -Iinclude -c -o build/codegen.o src/codegen.cpp

# Compile the main file
g++ -Iinclude -c -o build/main.o src/main.cpp

# Link all object files together
g++ -o tsompcc build/lexer.o build/parser.o build/AST.o build/codegen.o build/main.o $(llvm-config --libs --cxxflags)

# Print success message
echo "Build completed successfully!"