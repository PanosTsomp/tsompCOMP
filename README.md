##tsompCC Compiler


tsompCC is a compiler designed to parse and generate LLVM Intermediate Representation (IR) for a simple programming language. This project uses Flex for lexical analysis, Bison for parsing, and LLVM for code generation.

#Features
Lexical Analysis: Tokenizes input source code.
Parsing: Parses tokens into an Abstract Syntax Tree (AST).
Code Generation: Generates LLVM IR from the AST.
Supported Constructs:
Variable declarations
Function declarations and calls
Conditional statements (if-else)
Loops (while, for)
Print statements
Basic arithmetic and logical expressions


#Installation

Prerequisites
LLVM: Install LLVM development tools.



sudo apt-get install llvm llvm-dev
Flex: Install Flex for lexical analysis.


sudo apt-get install flex
Bison: Install Bison for parsing.


sudo apt-get install bison

##Build Instructions
#Clone the Repository:

git clone https://github.com/yourusername/tsompcc.git
cd tsompcc

#Build the Compiler:
Run the provided build script to compile the project.

./build.sh

#Usage
Compiling a Source File
To compile a source file written in the tsompCC language, run:


./tsompcc examples/test_program.tcc

