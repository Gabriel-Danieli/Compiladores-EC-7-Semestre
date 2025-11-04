# Repositório Atividades Compiladores 

Este repositório foi criado para o envio das atividades da matéria de Compiladores, do 7º semestre de Engenharia de Computação

## Sistema de Varredura para C-

Contém o código-fonte de um analisador léxico (ou scanner) para a linguagem C- (uma versão simplificada da linguagem C).

Este projeto foi desenvolvido como atividade para a disciplina de Compiladores. Seu objetivo é implementar a primeira fase de um compilador: a análise que transforma um arquivo de código-fonte em uma sequência de unidades lógicas chamadas tokens.

### Funcionalidades

O *scanner* (`scannerC-.c`) é construído a partir de um Autômato Finito Determinístico (DFA) e é capaz de:

  * Ler um arquivo de código-fonte `.c-`.
  * Reconhecer e classificar palavras-chave (como `if`, `while`, `int`, `return`).
  * Identificar IDs (nomes de variáveis e funções).
  * Reconhecer números (apenas inteiros).
  * Identificar operadores (`+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>=`, etc.).
  * Identificar delimitadores (`,`, `;`, `(`, `)`, `{`, `}`, `[`, `]`).
  * Ignorar corretamente comentários de bloco (`/* ... */`).
  * Detectar e reportar erros léxicos (caracteres inválidos) e parar a execução.

### Pré-requisitos

Para compilar e executar este projeto, você precisará de:

  * Um compilador C (como o **gcc**).
  * Um terminal (Linux, macOS ou Git Bash/WSL no Windows).

### Como Utilizar

Siga os passos abaixo para compilar e executar o analisador léxico.

#### 1\. Compilação

Abra seu terminal na pasta do projeto (onde o `scannerC-.c` está localizado) e execute o seguinte comando para compilar o programa:

```bash
gcc scannerC-.c -o scannerC-
```

Este comando irá ler o `scannerC-.c`, compilá-lo e criar um arquivo executável chamado `scannerC-` (ou `scannerC-.exe` no Windows).

#### 2\. Execução

Para analisar um arquivo de código C-, execute o programa compilado e passe o nome do arquivo-fonte como argumento. O programa espera exatamente um argumento.

**Uso:**

```bash
./scannerC- <nome_do_arquivo.c->
```

**Exemplo com um arquivo válido (ex: `testeBom.c-`):**

```bash
./scannerC- testeBom.c-
```

**Exemplo com um arquivo inválido (ex: `testeRuim.c-`):**

```bash
./scannerC- testeRuim.c-
```

### Exemplo de Saída

Ao executar o scannerC- em um arquivo C- válido, você verá uma tabela formatada token por token, mostrando a linha, o tipo de token, seu nome (lexema) e seu valor (se aplicável).

**Saída para `testeBom.c-` (parcial):**

```
--- Iniciando Analise Lexica de testeBom.c- ---
----------------------------------------------------------------
Linha     | Tipo             | Nome            | Valor
----------------------------------------------------------------
Linha 5    | Palavra-chave    | int             | -
Linha 5    | ID               | max             | -
Linha 5    | Delimitador      | (               | (
Linha 5    | Palavra-chave    | int             | -
Linha 5    | ID               | n1              | -
Linha 5    | Delimitador      | ,               | ,
Linha 5    | Palavra-chave    | int             | -
Linha 5    | ID               | n2              | -
Linha 5    | Delimitador      | )               | )
Linha 5    | Delimitador      | {               | {
Linha 6    | Palavra-chave    | int             | -
Linha 6    | ID               | resultado       | -
Linha 6    | Delimitador      | ;               | ;
Linha 8    | Palavra-chave    | if              | -
...
Linha 42   | Delimitador      | }               | }
Linha 43   | Fim de Arquivo   | EOF             | -
--- Analise Lexica Concluida ---
```

**Saída para `testeRuim.c-` (detecção de erro):**

```
--- Iniciando Analise Lexica de test_ruim.c- ---
----------------------------------------------------------------
Linha     | Tipo             | Nome            | Valor
----------------------------------------------------------------
Linha 7    | Operador         | /               | /
Linha 7    | Operador         | /               | /
Linha 7    | ID               | Teste           | -
Linha 7    | ID               | de              | -
...
Linha 15   | Erro Léxico      | @               | !
--- Analise Lexica Concluida ---
```
