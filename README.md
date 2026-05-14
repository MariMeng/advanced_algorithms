# Algoritmos Avançados I

Repositório de implementações práticas da disciplina de **Algoritmos Avançados I** da **Universidade Federal de Rondônia (UNIR)**.

Cada diretório contém a implementação de um problema clássico de algoritmos, com foco na compreensão das técnicas, análise de complexidade e, onde aplicável, uma demonstração visual interativa construída com a biblioteca [Raylib](https://www.raylib.com/).

---

## Projetos

### Backtracking — Word Search (Busca de Palavras)
**Técnica:** Backtracking com poda  
**Complexidade:** Tempo $O(M \cdot N \cdot 3^L)$ · Espaço $O(L)$

Dado uma grade de caracteres e uma palavra-alvo, determina se a palavra pode ser formada percorrendo células adjacentes (horizontal ou verticalmente) sem reutilizar a mesma célula. Inclui um visualizador animado com Raylib que exibe cada etapa recursiva em tempo real — verde para o caminho ativo, branco para células com backtrack.

```
backtracking/
├── main.cpp         # lógica pura
└── word_search.cpp  # visualizador interativo (Raylib)
```

---

### Closest Pair of Points (Par de Pontos Mais Próximo)
**Técnica:** Divisão e Conquista  
**Complexidade:** Tempo $O(n \log n)$ · Espaço $O(n)$

Dado um conjunto de $n$ pontos em um plano bidimensional, encontra o par com menor distância euclidiana. Contrasta a força bruta $O(n^2)$ com a solução por divisão e conquista, que divide o plano recursivamente e restringe as comparações a uma faixa central geométrica.

```
closest_Pair_of_Points_problem/
└── main.cpp
```

---

### Minimizing Maximum Lateness (Minimização do Atraso Máximo)
**Técnica:** Algoritmo Guloso — EDF (Earliest Deadline First)  
**Complexidade:** Tempo $O(n \log n)$ · Espaço $O(n)$

Dado $n$ tarefas, cada uma com tempo de processamento $t_i$ e prazo $d_i$, escalonadas em uma única máquina, minimiza o atraso máximo $L^* = \max(0, f_i - d_i)$. A estratégia gulosa — ordenar pelo menor deadline primeiro — é provadamente ótima pelo argumento de troca.

Inclui um visualizador passo a passo com Raylib: instâncias geradas aleatoriamente (10–15 tarefas), ordenação EDF animada sobre um diagrama de Gantt, com marcadores de deadline e colchetes de atraso desenhados em tempo real.

```
minimizing_maximum_lateness/
├── main.cpp   # algoritmo + visualizador Raylib
├── Makefile   # detecta raylib e fontes automaticamente, verifica dependências
└── README.md
```

**Para rodar:**
```bash
cd minimizing_maximum_lateness
make run
```

---

## Stack

| | |
|---|---|
| Linguagem | C++17 |
| Visualização | Raylib |
| Build | GNU Make |
| Disciplina | Algoritmos Avançados I — UNIR 2026 |
| Autores | José Estevão · Mariana Feitoza · Pedro Augusto |
