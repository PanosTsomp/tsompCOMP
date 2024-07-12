#!/bin/bash

# Clean previous build
rm -f tsompcc
rm -f build/*.o

# Flex and Bison steps
echo "Running Flex..."
flex -o build/lexer.cpp src/lexer.l
if [[ $? -ne 0 ]]; then
    echo "Flex failed."
    exit 1
fi

echo "Running Bison..."
bison -d -o build/parser.cpp src/parser.y
if [[ $? -ne 0 ]]; then
    echo "Bison failed."
    exit 1
fi

# Compile each component
echo "Compiling lexer.cpp..."
g++ -Iinclude -c -o build/lexer.o build/lexer.cpp
if [[ $? -ne 0 ]]; then
    echo "Compiling lexer.cpp failed."
    exit 1
fi

echo "Compiling parser.cpp..."
g++ -Iinclude -c -o build/parser.o build/parser.cpp
if [[ $? -ne 0 ]]; then
    echo "Compiling parser.cpp failed."
    exit 1
fi

echo "Compiling AST.cpp..."
g++ -Iinclude -c -o build/AST.o src/AST.cpp
if [[ $? -ne 0 ]]; then
    echo "Compiling AST.cpp failed."
    exit 1
fi

echo "Compiling codegen.cpp..."
g++ -Iinclude -c -o build/codegen.o src/codegen.cpp
if [[ $? -ne 0 ]]; then
    echo "Compiling codegen.cpp failed."
    exit 1
fi

echo "Compiling main.cpp..."
g++ -Iinclude -c -o build/main.o src/main.cpp
if [[ $? -ne 0 ]]; then
    echo "Compiling main.cpp failed."
    exit 1
fi

# Link all objects into the executable
echo "Linking..."
g++ -o tsompcc build/lexer.o build/parser.o build/AST.o build/codegen.o build/main.o $(llvm-config --libs --cxxflags)
if [[ $? -ne 0 ]]; then
    echo "Linking failed."
    exit 1
fi

# Check if executable was created
if [[ -f tsompcc ]]; then
    echo "Running test program..."
    ./tsompcc examples/example2.tcc
else
    echo "Compilation failed or no executable generated."
fi
