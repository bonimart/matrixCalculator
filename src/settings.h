#pragma once

#define SPARSE_THRESHOLD 0.5
#define DELIM ','
#define END_COMMAND ';'
#define DEC_POINT '.'
#define L_FUNC_PAR '('
#define R_FUNC_PAR ')'
#define L_MAT_PAR '['
#define R_MAT_PAR ']'
#define L_SPARSE_INDEX '{'
#define R_SPARSE_INDEX '}'
#define INDEX_VAL_DELIM ':'
#define VAR_IDENTIFIER '$'
#define FILE_IDENTIFIER '@'
#define HELP "help"
#define PRINT_VARIABLES "vars"
#define SET_VAR "="
#define WRITE_TO_FILE "<"
#define WRITE_PRIORITY 0
#define DIFFERENT_SPARSE_PRINT
#define HELP_MESSAGE "-------------\n\
Matrices must satisfy format: [[a11,a12...],[a21...]...]\n\
Variables can be set with {variableName}={matrix}\n\
Get value of variables with ${variableName}\n\
Import matrices from files with @{fileName} (only [a-zA-Z/.] allowed in fileNames)\n\
Write output to files with {fileName}<{output}\n\
List of commands:\n\
    help - print this message\n\
    vars - print all variabes\n\
    print() - prints matrix to standard output\n\
    tr() - return transposed matrix\n\
    det() - calculate determinant of a matrix\n\
    gem() - apply gaussian elimination to a matrix (returns new matrix)\n\
    rank() - calculate rank of given matrix\n\
    inv() - calculate inverse matrix to a given regular matrix\n\
    sel(,) - returns a part of matrix given as first parameter,\
second parameter is 2x2 matrix with edge points of selection ([[x1, y1],[x2,y2]])\n\
    join(,) - join columns of two matrices\n\
Operators:\n\
    '+' - matrix addition\n\
    '-' - matrix subtraction\n\
    '*' - matrix multiplication\n\
(Commands can take other commands as arguments)\n\
-------------\n"
