/*
    Sistema de Varredura para C-
    
    Um scanner simples para uma linguagem C- simplificada.
    Implementa análise léxica usando uma máquina de estados finitos.
    
    Autor: Gabriel Danieli
    RA: 191752
    Data: 04/11/2025
*/

// --- 0. Inclusões de Bibliotecas ---
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- 1. Definições de Tokens ---
#define MAX_LEXEME_SIZE 100 // Tamanho máximo do lexema

typedef enum {
    TOKEN_EOF, TOKEN_ERROR, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_INT,
    TOKEN_VOID, TOKEN_RETURN, TOKEN_ID, TOKEN_NUM, TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_TIMES, TOKEN_DIVIDE, TOKEN_LT, TOKEN_LTE, TOKEN_GT, TOKEN_GTE,
    TOKEN_EQ_EQ, TOKEN_NEQ, TOKEN_ASSIGN, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_SEMI,
    TOKEN_COMMA
} TokenType; // Tipos de tokens

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_SIZE];
    int lineno;
} Token; // Estrutura do token

// --- 2. Definições da Máquina de Estados ---
typedef enum { 
    STATE_START, STATE_INID, STATE_INNUM, STATE_INLT, STATE_INGT, STATE_INEQ,
    STATE_INNEQ, STATE_INSLASH, STATE_INCOMMENT, STATE_INCOMMENT_STAR, STATE_DONE
} StateType; // Estados da máquina

// --- 3. Globais do Scanner ---
FILE *sourceFile;
int lineno = 1;

// --- 4. Funções Auxiliares de Leitura ---
static int ungetBuffer = 0;  // Buffer para caractere "deslido"

int getChar() { 
    // Função getChar com suporte a ungetChar e ajuste de linha, lendo um caractere do arquivo fonte
    int c; 

    if (ungetBuffer != 0) {
        c = ungetBuffer;
        ungetBuffer = 0; 
    } else {
        c = fgetc(sourceFile);
    }
    
    if (c == '\n') { 
        // Incrementa o número da linha ao ler uma nova linha
        lineno++;
    }
    
    return c;
}

void ungetChar(int c) { 
    // Função ungetChar com ajuste de linha, servindo para voltar um caractere lido
    if (ungetBuffer != 0) {
        fprintf(stderr, "ERRO DO SCANNER: Buffer de unget cheio!\n");
        exit(1);
    }
    
    ungetBuffer = c;
    
    if (c == '\n') { 
        // Decrementa o número da linha ao "desler" uma nova linha
        lineno--;
    }
}

// --- Função lookupKeyword ---
TokenType lookupKeyword(char *lexeme) { 
    // Verifica se o lexema é uma palavra-chave reservada, retornando o token apropriado
    if (strcmp(lexeme, "if") == 0) return TOKEN_IF;
    if (strcmp(lexeme, "else") == 0) return TOKEN_ELSE;
    if (strcmp(lexeme, "while") == 0) return TOKEN_WHILE;
    if (strcmp(lexeme, "return") == 0) return TOKEN_RETURN;
    if (strcmp(lexeme, "int") == 0) return TOKEN_INT;
    if (strcmp(lexeme, "void") == 0) return TOKEN_VOID;
    return TOKEN_ID;
}


// --- 5. Função getToken() ---
Token getToken() {
    // Função principal do scanner que implementa a máquina de estados finitos para análise léxica
    Token currentToken; // Token a ser retornado
    StateType state = STATE_START; // Estado inicial da máquina
    char lexemeBuffer[MAX_LEXEME_SIZE]; // Buffer para o lexema
    int bufferIndex = 0; // Índice do buffer
    
    memset(lexemeBuffer, 0, MAX_LEXEME_SIZE); 

    while (state != STATE_DONE) {
        int c = getChar(); 
        int save = 1; 

        switch (state) {
            case STATE_START:
                if (isspace((unsigned char)c)) {
                    save = 0; 
                } else if (isalpha((unsigned char)c)) {
                    state = STATE_INID;
                } else if (isdigit((unsigned char)c)) {
                    state = STATE_INNUM;
                } else if (c == '<') {
                    state = STATE_INLT;
                } else if (c == '>') {
                    state = STATE_INGT;
                } else if (c == '=') {
                    state = STATE_INEQ;
                } else if (c == '!') {
                    state = STATE_INNEQ;
                } else if (c == '/') {
                    state = STATE_INSLASH;
                } else if (c == EOF) {
                    save = 0;
                    state = STATE_DONE;
                    currentToken.type = TOKEN_EOF;
                } else {
                    state = STATE_DONE;
                    switch (c) {
                        case '+': currentToken.type = TOKEN_PLUS; break;
                        case '-': currentToken.type = TOKEN_MINUS; break;
                        case '*': currentToken.type = TOKEN_TIMES; break;
                        case '(': currentToken.type = TOKEN_LPAREN; break;
                        case ')': currentToken.type = TOKEN_RPAREN; break;
                        case '{': currentToken.type = TOKEN_LBRACE; break;
                        case '}': currentToken.type = TOKEN_RBRACE; break;
                        case '[': currentToken.type = TOKEN_LBRACKET; break;
                        case ']': currentToken.type = TOKEN_RBRACKET; break;
                        case ';': currentToken.type = TOKEN_SEMI; break;
                        case ',': currentToken.type = TOKEN_COMMA; break;
                        default: currentToken.type = TOKEN_ERROR; break;
                    }
                }
                break; 

            case STATE_INID:
                if (!isalpha((unsigned char)c) && !isdigit((unsigned char)c)) {
                    ungetChar(c); 
                    save = 0;
                    state = STATE_DONE;
                    currentToken.type = TOKEN_ID;
                }
                break;

            case STATE_INNUM:
                if (!isdigit((unsigned char)c)) {
                    ungetChar(c);
                    save = 0;
                    state = STATE_DONE;
                    currentToken.type = TOKEN_NUM;
                }
                break;

            case STATE_INLT:
                state = STATE_DONE;
                if (c == '=') {
                    currentToken.type = TOKEN_LTE;
                } else {
                    ungetChar(c);
                    save = 0;
                    currentToken.type = TOKEN_LT;
                }
                break;

            case STATE_INGT:
                state = STATE_DONE;
                if (c == '=') {
                    currentToken.type = TOKEN_GTE;
                } else {
                    ungetChar(c);
                    save = 0;
                    currentToken.type = TOKEN_GT;
                }
                break;

            case STATE_INEQ:
                state = STATE_DONE;
                if (c == '=') {
                    currentToken.type = TOKEN_EQ_EQ;
                } else {
                    ungetChar(c);
                    save = 0;
                    currentToken.type = TOKEN_ASSIGN;
                }
                break;

            case STATE_INNEQ:
                state = STATE_DONE;
                if (c == '=') {
                    currentToken.type = TOKEN_NEQ;
                } else {
                    ungetChar(c);
                    save = 0;
                    currentToken.type = TOKEN_ERROR;
                }
                break;
            
            case STATE_INSLASH:
                if (c == '*') {
                    save = 0;
                    bufferIndex = 0;
                    state = STATE_INCOMMENT;
                } else {
                    ungetChar(c);
                    save = 0;
                    state = STATE_DONE;
                    currentToken.type = TOKEN_DIVIDE;
                }
                break;

            case STATE_INCOMMENT:
                save = 0;
                if (c == '*') {
                    state = STATE_INCOMMENT_STAR;
                } else if (c == EOF) {
                    state = STATE_DONE;
                    currentToken.type = TOKEN_ERROR;
                    strcpy(lexemeBuffer, "Comentario nao fechado");
                }
                break;
            
            case STATE_INCOMMENT_STAR:
                save = 0;
                if (c == '/') {
                    state = STATE_START;
                } else if (c == '*') {
                    // Continua
                } else if (c == EOF) {
                    state = STATE_DONE;
                    currentToken.type = TOKEN_ERROR;
                    strcpy(lexemeBuffer, "Comentario nao fechado");
                } else {
                    state = STATE_INCOMMENT;
                }
                break;

            case STATE_DONE:
                break;
        }

        if (save && bufferIndex < (MAX_LEXEME_SIZE - 1)) {
            lexemeBuffer[bufferIndex++] = (char) c;
        }
    }
    
    currentToken.lineno = lineno;

    strcpy(currentToken.lexeme, lexemeBuffer);
    
    if (currentToken.type == TOKEN_ID) {
        currentToken.type = lookupKeyword(currentToken.lexeme);
    }

    return currentToken;
}

// --- 6. Função Auxiliar para Imprimir Tokens ---

void printToken(Token token) {
    char tipoStr[20];
    char nomeStr[MAX_LEXEME_SIZE];
    char valorStr[MAX_LEXEME_SIZE];

    strcpy(tipoStr, " ");
    strcpy(nomeStr, " ");
    strcpy(valorStr, " ");

    switch (token.type) {
        // --- CASOS ESPECIAIS ---
        case TOKEN_EOF:
            strcpy(tipoStr, "Fim de Arquivo");
            strcpy(nomeStr, "EOF");
            strcpy(valorStr, "EOF");
            break;
        
        case TOKEN_ERROR:
            strcpy(tipoStr, "Erro Lexico");
            strcpy(nomeStr, token.lexeme); 
            strcpy(valorStr, "!");         
            break;

        // --- PALAVRAS-CHAVE ---
        case TOKEN_IF:
        case TOKEN_ELSE:
        case TOKEN_WHILE:
        case TOKEN_INT:
        case TOKEN_VOID:
        case TOKEN_RETURN:
            strcpy(tipoStr, "Palavra-chave");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, "-");
            break;
        
        // --- IDENTIFICADORES ---
        case TOKEN_ID:
            strcpy(tipoStr, "ID");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, "-");
            break;
        
        // --- NÚMEROS ---
        case TOKEN_NUM:
            strcpy(tipoStr, "Numero");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, token.lexeme); // Valor é o próprio número
            break;
        
        // --- OPERADORES ---
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_TIMES:
        case TOKEN_DIVIDE:
        case TOKEN_LT:
        case TOKEN_LTE:
        case TOKEN_GT:
        case TOKEN_GTE:
        case TOKEN_EQ_EQ:
        case TOKEN_NEQ:
        case TOKEN_ASSIGN:
            strcpy(tipoStr, "Operador");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, token.lexeme);
            break;

        // --- DELIMITADORES ---
        case TOKEN_LPAREN:
        case TOKEN_RPAREN:
        case TOKEN_LBRACE:
        case TOKEN_RBRACE:
        case TOKEN_LBRACKET:
        case TOKEN_RBRACKET:
        case TOKEN_SEMI:
        case TOKEN_COMMA:
            strcpy(tipoStr, "Delimitador");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, token.lexeme);
            break;
        
        default:
            strcpy(tipoStr, "Desconhecido");
            strcpy(nomeStr, token.lexeme);
            strcpy(valorStr, "?");
            break;
    }

    // A impressão é a mesma para TODOS os tokens, formato de tabela "Linha | Tipo | Nome | Valor"
    printf("Linha %-4d | %-16s | %-15s | %s\n", 
           token.lineno, 
           tipoStr, 
           nomeStr, 
           valorStr);
}
// --- 7. Função Principal (Driver) ---

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo.c->\n", argv[0]);
        exit(1);
    }

    sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        exit(1);
    }

    printf("--- Iniciando Analise Lexica de %s ---\n", argv[1]);
    printf("----------------------------------------------------------------\n");
    printf("Linha     | Tipo             | Nome            | Valor\n");
    printf("----------------------------------------------------------------\n");

    Token token;
    do {
        token = getToken();
        printToken(token);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);

    printf("--- Analise Lexica Concluida ---\n");

    fclose(sourceFile);
    return 0;
}