/*
    Estevan Villegas
    CSCI 4355.01
    ID: 225245
    
    Term Project: Hawk     
    A scanner and recursive descent parser 
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Tokens */
// misc
#define UNKNOWN 99
#define MAX_SYMBOLS 98
#define LETTER 1
#define DIGIT 2 
#define IDENTIFIER 3
#define NUMBER 4

// reserved words
#define PROGRAM_CODE 10
#define BEGIN 11
#define END 12
#define IF 13
#define THEN 14
#define ELSE 15
#define INPUT_CODE 16
#define OUTPUT_CODE 17
#define INT 18
#define FLOAT 19
#define DOUBLE 20
#define WHILE 21
#define LOOP 22 

// operators
#define ASSIGNMENT 30   // :=
#define LESS_THAN 31    // <
#define GREATER_THAN 32 // >
#define EQUALS 33       // =
#define NOT_EQUALS 34   // <>
#define ADD 35          // +
#define SUB 36          // -
#define MUL 37          // *
#define DIV 38          // /
#define PAREN_L 39      // (
#define PAREN_R 40      // )

// punctuation 
#define SEMICOLON 41    // ;
#define COMMA 42        // ,
#define COLON 43        // :

/* global variables */
char *script = "input.txt"; // can change to desired file
char *symbolTable[MAX_SYMBOLS], lexeme[100], nextChar;
int charClass, lexLen, token, nextToken, prevTokenLine, nextTokenLine; 
int lineNumber = 1, lexemeLine = 1;
int symbolCount = 0, declarations = 0;
FILE *in_fp;

/* functions */
void getNonBlank();
void getChar(); 
void addChar();
void symbolTableAdd(const char *id);
void error(const char *message, int line);
const char* MIA(int expectedToken);
int ReservedWords();
int lex();
int lookup(char ch);
int match(int expectedToken);
int declared(const char *id);

/* Parsing Rules 1 - 18 */
void PROGRAM();         // 1
void DECL_SEC();        // 2
void DECL();            // 3
void ID_LIST();         // 4
void ID();              // 5
void STMT_SEC();        // 6
void STMT();            // 7
void ASSIGN();          // 8
void IFSTMT();          // 9
void WHILESTMT();       // 10
void INPUT();           // 11
void OUTPUT();          // 12
void EXPR();            // 13
void FACTOR();          // 14
void OPERAND();         // 15
void NUM();             // 16
void COMP();            // 17
void TYPE();            // 18

int main() {
    if ( (in_fp = fopen(script, "r")) == NULL ) { printf("ERROR - cannot open file '%s'\n", script); }
    else {
        getChar();
        lex();
        PROGRAM();
        fclose(in_fp); }
}

void addChar() {
    if (lexLen == 0) { 
        lexemeLine = lineNumber;
        /* debug code below */
        //printf("DEBUG: Starting lexeme '%c' on line %d\n", nextChar, lexemeLine);
    }
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0; }
    else { printf("ERROR - lexeme is too long\n"); }
}

void getChar() {
    if ( (nextChar = getc(in_fp)) != EOF ) {
        if (nextChar == '\n') { lineNumber++; }
        if ( isalpha(nextChar) ) { charClass = LETTER; }
        else if ( isdigit(nextChar) ) { charClass = DIGIT; }
        //else if (nextChar == '.') { charClass = DIGIT; }
        //else if (nextChar == ';') { charClass = UNKNOWN; }
        //else if (isspace(nextChar)) { charClass = UNKNOWN; }
        else { charClass = UNKNOWN; }
    }
    else { charClass = EOF; }
}

void getNonBlank() { while ( isspace(nextChar) ) { getChar(); } }

int ReservedWords() {
    if (strcmp(lexeme, "program") == 0) { return PROGRAM_CODE; }
    else if (strcmp(lexeme, "begin") == 0) { return BEGIN; }
    else if (strcmp(lexeme, "end") == 0) { return END; }
    else if (strcmp(lexeme, "if") == 0) { return IF; }
    else if (strcmp(lexeme, "then") == 0) { return THEN; }
    else if (strcmp(lexeme, "else") == 0) { return ELSE; }
    else if (strcmp(lexeme, "input") == 0) { return INPUT_CODE; }
    else if (strcmp(lexeme, "output") == 0) { return OUTPUT_CODE; }
    else if (strcmp(lexeme, "int") == 0) { return INT; }
    else if (strcmp(lexeme, "while") == 0) { return WHILE; }
    else if (strcmp(lexeme, "loop") == 0) { return LOOP; }
    else if (strcmp(lexeme, "double") == 0 ) { return DOUBLE; }
    else if (strcmp(lexeme, "float") == 0) { return FLOAT; }
    else { return IDENTIFIER; }
}

void symbolTableAdd(const char *id) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i], id) == 0) { error("Redeclaration of variable", nextTokenLine); }
    }
    if (symbolCount < MAX_SYMBOLS) { symbolTable[symbolCount++] = strdup(id); }
    else { error("Too many symbols in table", nextTokenLine); }
}

int declared(const char *id) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i], id) == 0) { return 1; }
    }
    return 0;
}

int match(int expectedToken) {
    if (nextToken == expectedToken) {
        int matchedLine = nextTokenLine;
        lex();
        prevTokenLine = matchedLine;
        return 1; }
    else {
        error(MIA(expectedToken), nextTokenLine);
        return 0; }
}

void error(const char *message, int line) {
    fprintf(stderr, "ERROR !! %s in Line %d.\n", message, line);
    exit(1);
}

const char* MIA(int expectedToken) {
    switch (expectedToken) {
        case PROGRAM_CODE: return "'program' missing";
        case BEGIN: return "'begin' missing";
        case END: return "'end' missing";
        case INT: case FLOAT: case DOUBLE: return "Type missing";
        case IDENTIFIER: return "Identifier not declared";
        case SEMICOLON: return "Semicolon missing";
        case COLON: return "Colon missing";
        default: return "Token missing";
    }
}

int numberScan() {
    /* PRINT TROUBLESHOOTING PURPOSES!! */
    //fprintf(stderr, "BEFORE: seeing digit '%c' (count=%d)\n", nextChar, digitCount);
    
    int i = 0, digitCount = 0;
    char numberBuffer[22];
    nextTokenLine = lineNumber;
    
    while (isdigit((unsigned char)nextChar)) {
        numberBuffer[i++] = (char) nextChar;
        digitCount++;
        if (digitCount > 10) { error("Illegal int exceeds max", nextTokenLine); }
        getChar();
    }
    if (nextChar == '.') {
        numberBuffer[i++] = (char) nextChar;
        getChar();
        while (isdigit((unsigned char) nextChar)) { 
            numberBuffer[i++] = (char) nextChar;
            digitCount++;
            if (digitCount > 10) { error("Illegal float exceeds max", nextTokenLine); }
            getChar();
        }
    }
    numberBuffer[i] = '\0';
    strcpy(lexeme, numberBuffer);
    return NUMBER;
}

int lex() {
    lexLen = 0;
    lexemeLine = lineNumber;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            nextTokenLine = lineNumber;
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT || nextChar == '_') {
                addChar();
                getChar(); }
            nextToken = ReservedWords();
            break;
        case DIGIT:
            nextToken = numberScan();
            break;
        case UNKNOWN:
            nextToken = lookup(nextChar);
            break;
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    /* ENABLE BELOW FOR TROUBLESHOOTING PURPOSES!! */
    //printf("Next token is %d Next lexeme is %s\n", nextToken, lexeme);
    return nextToken; 
}

int lookup(char ch) {
    switch (ch) {
        case ':':
            addChar();
            getChar();
            if (nextChar == '=') {          // :=
                addChar();
                nextToken = ASSIGNMENT;
                getChar(); }
            else { nextToken = COLON; }     // :
            break;
	case '<':
	    addChar();
	    getChar();
	    if (nextChar == '>') {          // <>
	        addChar();
	        nextToken = NOT_EQUALS;
	        getChar(); }
	    else {                          // <
	        nextToken = LESS_THAN;
	        getChar(); }
	    break;
	case '>':
	    addChar();
	    getChar();
	    nextToken = GREATER_THAN;
	    break;
	case '=':
	    addChar();
	    getChar();
	    nextToken = EQUALS;
	    break;
	case '+':
	    addChar();
	    getChar();
	    nextToken = ADD;
	    break;
	case '-':
	    addChar();
	    getChar();
	    nextToken = SUB;
	    break;
	case '*':
	    addChar();
	    getChar();
	    nextToken = MUL;
	    break;
	case '/':
	    addChar();
	    getChar();
	    nextToken = DIV;
	    break;
	case '(':
	    addChar();
	    getChar();
	    nextToken = PAREN_L;
	    break;
	case ')':
	    addChar();
	    getChar();
	    nextToken = PAREN_R;
	    break;
	case ';':
	    addChar();
	    getChar();
	    nextToken = SEMICOLON;
	    break;
	case ',':
	    addChar();
	    getChar();
	    nextToken = COMMA;
	    break;
	default:
	    addChar();
	    nextToken = UNKNOWN;
	    break;
    }
    return nextToken;
}

/* parsing rules */

// rule 1
void PROGRAM() {
    printf("PROGRAM\n");
    if (match(PROGRAM_CODE)) {              // program
        if (nextToken == BEGIN) {           // begin STMT_SEC end;
            match(BEGIN);
            STMT_SEC();
            match(END);
            match(SEMICOLON); }
        else if (nextToken == IDENTIFIER) { // DECL_SEC begin STMT_SEC end;
            DECL_SEC();
            match(BEGIN);
            STMT_SEC();
            match(END);
            match(SEMICOLON); }
        else { error("Identifier or 'begin' missing", nextTokenLine); }
    }
    else { error("'program' missing", nextTokenLine); }
}

// rule 2
void DECL_SEC() {
    printf("DECL_SEC\n");
    DECL();
    if (nextToken == IDENTIFIER) { DECL_SEC(); }
}

// rule 3
void DECL() {
    printf("DECL\n");
    if (nextToken == IDENTIFIER) {
        declarations = 1;
        ID_LIST();
        declarations = 0;
        if (match(COLON)) {
            TYPE(); 
            if (!match(SEMICOLON)) { error(MIA(SEMICOLON), prevTokenLine); }
        }
        else { error(MIA(COLON), nextTokenLine); }
    }
    else { error(MIA(IDENTIFIER), nextTokenLine); }
}

// rule 4
void ID_LIST() {
    printf("ID_LIST\n");
    if (nextToken == IDENTIFIER) {
        ID();
        while (nextToken == COMMA) {
            match(COMMA);
            ID_LIST(); }
    }
    else { error(MIA(IDENTIFIER), nextTokenLine); }
}

// rule 5
void ID() { 
    if (nextToken == IDENTIFIER) {
        if (declarations) { symbolTableAdd(lexeme); }
        else {
            if (!declared(lexeme)) { error("Identifier not declared", nextTokenLine); }
        }
        match(IDENTIFIER);
    }
    else { error(MIA(IDENTIFIER), nextTokenLine); }
}

// rule 6
void STMT_SEC() {
    printf("STMT_SEC\n");
    STMT();
    while (
        nextToken == IDENTIFIER ||
        nextToken == IF         ||
        nextToken == WHILE      ||
        nextToken == INPUT_CODE ||
        nextToken == OUTPUT_CODE ) 
    { STMT_SEC(); }
}

// rule 7
void STMT() {
    printf("STMT\n");
    if (nextToken == IDENTIFIER) { ASSIGN(); }
    else if (nextToken == IF) { IFSTMT(); }
    else if (nextToken == WHILE) { WHILESTMT(); }
    else if (nextToken == INPUT_CODE) { INPUT(); }
    else if (nextToken == OUTPUT_CODE) { OUTPUT(); }
    else { error("Unexpected token in statement", nextTokenLine); }
}

// rule 8
void ASSIGN() {
    printf("ASSIGN\n");
    ID();
    match(ASSIGNMENT);
    EXPR();
    match(SEMICOLON);
}

// rule 9
void IFSTMT() {
    printf("IF_STMT\n");
    match(IF);
    COMP();
    match(THEN);
    STMT_SEC();
    if (nextToken == ELSE) {
        match(ELSE);
        STMT_SEC(); }
    match(END);
    match(IF);
    match(SEMICOLON);
}

// rule 10
void WHILESTMT() {
    printf("WHILE_STMT\n");
    match(WHILE);
    COMP();
    match(LOOP);
    STMT_SEC();
    match(END);
    match(LOOP);
    match(SEMICOLON);
}

// rule 11
void INPUT() {
    printf("INPUT\n");
    match(INPUT_CODE);
    ID_LIST();
    match(SEMICOLON);
}

// rule 12
void OUTPUT() {
    printf("OUTPUT\n");
    match(OUTPUT_CODE);
    if (nextToken == IDENTIFIER) {
        ID_LIST();
        match(SEMICOLON); }
    else if (nextToken == NUMBER) {
        NUM();
        match(SEMICOLON); }
    else { error("Missing identifier or number", nextTokenLine); }
}

// rule 13
void EXPR() {
    printf("EXPR\n");
    FACTOR();
    if (nextToken == ADD) {
        match(ADD);
        EXPR(); }
    else if (nextToken == SUB) {
        match(SUB);
        EXPR(); }
    else { /* do nothing */ }
}

// rule 14
void FACTOR() { 
    printf("FACTOR\n");
    OPERAND();
    if (nextToken == MUL) {
        match(MUL);
        FACTOR(); }
    else if (nextToken == DIV) {
        match(DIV);
        FACTOR(); }
    else {/* do nothing */ }
}

// rule 15
void OPERAND() {
    printf("OPERAND\n");
    if (nextToken == NUMBER) { NUM(); }
    else if (nextToken == IDENTIFIER) { ID(); }
    else if (nextToken == PAREN_L) {
        match(PAREN_L);
        EXPR();
        match(PAREN_R); }
    else { error("Unexpected token in OPERAND: expected number, identifier, or '('", nextTokenLine); }
}

// rule 16
void NUM() {
    if (nextToken == NUMBER) {
        do { match(NUMBER); }
        while (nextToken == NUMBER);
    }
    else { error(MIA(NUMBER), nextTokenLine); }
}

// rule 17
void COMP() {
    printf("COMP\n");
    match(PAREN_L);
    OPERAND();
    if (nextToken == EQUALS) { match(EQUALS); }
    else if (nextToken == NOT_EQUALS) { match(NOT_EQUALS); }
    else if (nextToken == GREATER_THAN) { match(GREATER_THAN); }
    else if (nextToken == LESS_THAN) { match(LESS_THAN); }
    else { error("Unexpected token in COMP: expected '!=', '<>', '>', or '<'", nextTokenLine); }
    OPERAND();
    match(PAREN_R);
}

// rule 18
void TYPE() {
    if (nextToken == INT || nextToken == FLOAT || nextToken == DOUBLE) { 
        if (match(nextToken)) { prevTokenLine = nextTokenLine; }
        else { error(MIA(nextToken), nextTokenLine); }
    }
    else { error(MIA(nextToken), nextTokenLine); }
}
