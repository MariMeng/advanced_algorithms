# Word Search Algorithm (Backtracking)

Este repositório contém uma implementação eficiente do problema **Word Search** em C++, utilizando a técnica de **Backtracking**. O projeto foi desenvolvido com foco no entendimento dos fundamentos algorítmicos e na otimização de busca em matrizes.

This repository contains an efficient implementation of the **Word Search** problem in C++, using the **Backtracking** technique. The project was developed focusing on understanding algorithmic fundamentals and matrix search optimization.



## O Problema / The Problem

**PT-BR:** O objetivo é verificar se uma palavra específica existe dentro de uma grade de caracteres (matriz). A palavra pode ser construída a partir de letras de células adjacentes sequencialmente (horizontal ou vertical), sendo que a mesma célula de letra não pode ser usada mais de uma vez para a mesma palavra.

**EN-US:** The goal is to determine if a specific word exists within a character grid (matrix). The word can be constructed from letters of sequentially adjacent cells (horizontally or vertically); the same letter cell may not be used more than once for the same word.


## Lógica e Complexidade / Logic & Complexity

**PT-BR (Estudo dos Porquês):**
O algoritmo utiliza **Backtracking** para explorar os caminhos possíveis. Diferente da força bruta pura, ele realiza a "poda" (pruning): se uma letra não coincide com o padrão esperado, o caminho é descartado imediatamente.
* **Complexidade de Tempo:** $O(M \cdot N \cdot 3^L)$, onde $M \times N$ é o tamanho da grade e $L$ o comprimento da palavra.
* **Complexidade de Espaço:** $O(L)$, devido à pilha de recursão.

**EN-US:**
The algorithm uses **Backtracking** to explore possible paths. Unlike pure brute force, it performs "pruning": if a letter does not match the expected pattern, the path is immediately discarded.
* **Time Complexity:** $O(M \cdot N \cdot 3^L)$, where $M \times N$ is the grid size and $L$ is the word length.
* **Space Complexity:** $O(L)$, due to the recursion stack.


## Tecnologias / Technologies
* **Language:** C++17
* **Compiler:** GCC/G++
* **Concepts:** Recursion, Backtracking, Matrix Manipulation.

## Versão Visual (Raylib) / Visual Version (Raylib)

**PT-BR:**
Além da lógica pura, este projeto inclui uma implementação visual utilizando a biblioteca **Raylib**. Esta versão permite observar o "passo a passo" do Backtracking:
* **Verde:** Representa o caminho atual sendo explorado pela recursão.
* **Branco:** Representa células que foram descartadas (Backtracked) após o algoritmo perceber que não levavam à solução.


**EN-US:**
Beyond pure logic, this project includes a visual implementation using the **Raylib** library. This version allows observing the Backtracking "step-by-step":
* **Green:** Represents the current path being explored by recursion.
* **White:** Represents cells that were discarded (Backtracked) after the algorithm realized they did not lead to a solution.



## Compilação e Dependências (Linux Mint) / Compilation & Dependencies

**PT-BR:** Para rodar a versão visual, é necessário ter a Raylib instalada. Recomendamos a compilação via terminal:
**EN-US:** To run the visual version, Raylib must be installed. We recommend compiling via terminal:

```bash
# Compilação / Compilation
g++ visual.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o visual_search

# Execução / Execution
./visual_search

## Como usar / How to use

```cpp
// Exemplo de Matriz / Matrix Example
vector<string> mat = {
    "ABCE",
    "SFCS",
    "ADEE"
};

string target = "ABCCED"; // Palavra a ser buscada / Target word

