# Closest Pair of Points (Par de Pontos Mais Próximo) 

Este repositório contém uma implementação em C++ do clássico problema do **Par de Pontos Mais Próximo** (Closest Pair of Points), servindo como um caso de estudo prático sobre algoritmos avançados e geometria computacional.

O objetivo principal deste projeto é demonstrar como a técnica de **Divisão e Conquista** pode reduzir drasticamente o tempo de execução de um problema que, na sua forma mais ingênua, seria impraticável para grandes volumes de dados.

## O Problema e a Matemática
Dado um conjunto de $n$ pontos em um plano bidimensional, o algoritmo localiza os dois pontos $P_i$ e $P_j$ que possuem a menor distância euclidiana entre si:

$$d(P_i, P_j) = \sqrt{(x_i - x_j)^2 + (y_i - y_j)^2}$$

### A Batalha das Complexidades
* **Força Bruta ($O(n^2)$):** A abordagem padrão compara cada ponto com todos os outros do plano. É fácil de implementar, mas inviável para aplicações em tempo real com muitos pontos.
* **Divisão e Conquista ($O(n \log n)$):** A abordagem otimizada ordena os pontos, divide o plano recursivamente em metades e processa de forma inteligente apenas os pontos que caem dentro de uma "faixa" central geométrica restrita.

## Como Executar

Para compilar e rodar este código na sua máquina, você precisará de um compilador C++ (como o GCC/g++).

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git
    cd NOME_DO_REPOSITORIO
    ```

2.  **Compile o código:**
    ```bash
    g++ main.cpp -o closest_pair
    ```

3.  **Execute o programa:**
    ```bash
    ./closest_pair
    ```

## Tecnologias Utilizadas
* **Linguagem:** C++
* **Conceitos:** Divisão e Conquista, Recursão, Geometria Computacional, Ordenação (Sorting), Análise de Complexidade.

---
