# Minimização do Atraso Máximo (Minimizing Maximum Lateness)

Este diretório contém uma implementação em C++ do problema da **Minimização do Atraso Máximo**, utilizando o algoritmo guloso **EDF (Earliest Deadline First)**. O projeto foi desenvolvido como trabalho prático da disciplina de Algoritmos Avançados I e inclui uma visualização interativa passo a passo construída com a biblioteca **Raylib**.

## O Problema

Dado um conjunto de **n** tarefas que devem ser executadas em um único recurso (máquina, processador ou servidor), cada tarefa **i** possui:

- Um **tempo de processamento** `t`
- Um **prazo final (deadline)** `d`

Se a tarefa começa no instante `s`, ela termina em `f = s + t`. O **atraso** da tarefa é definido como:

```
lateness = max(0, f - d)
```

O objetivo é escalonar todas as tarefas em uma ordem tal que o **atraso máximo** `L* = max(lateness)` seja minimizado.

## Algoritmo Guloso — EDF

A intuição é simples: tarefas com prazo mais apertado devem ser executadas primeiro. Os passos são:

1. Ordenar as tarefas em ordem não-decrescente de deadline
2. Inicializar o tempo corrente e o atraso máximo em zero
3. Para cada tarefa nessa ordem, agendá-la imediatamente após a anterior
4. Calcular o término `f = tempo_corrente + t` e o atraso `lateness = max(0, f - d)`
5. Atualizar o atraso máximo

A otimalidade do EDF é provada por **argumento de troca (exchange argument)**: qualquer inversão na ordem pode ser corrigida sem aumentar o atraso máximo.

## Análise de Complexidade

| | |
|---|---|
| **Tempo** | O(n log n) — dominado pela ordenação inicial |
| **Espaço** | O(n) — para armazenar a permutação ordenada |

## Visualização Interativa

A cada execução, o programa gera aleatoriamente entre 10 e 15 tarefas com durações e deadlines variados, ordena pelo EDF e anima o escalonamento em um diagrama de Gantt:

- As tarefas são colocadas uma a uma na linha do tempo
- Marcadores de deadline são exibidos (verde = no prazo, vermelho = em atraso)
- Colchetes de atraso são desenhados abaixo do Gantt para cada tarefa atrasada
- A tabela lateral atualiza o término e o atraso de cada tarefa conforme o escalonamento avança

**Controles:**

| Tecla | Acao |
|-------|------|
| `SPACE` | Pausar / retomar |
| `< >` | Avançar / recuar um passo |
| `^ v` | Aumentar / diminuir velocidade |
| `R` | Gerar nova instância aleatória |

## Compilação e Execução

```bash
cd minimizing_maximum_lateness
make run
```

O `make` verifica automaticamente as dependências antes de compilar e indica como instalá-las caso estejam faltando.

**Dependências:**
- Compilador `g++` com suporte a C++17
- Biblioteca **Raylib** (`libraylib.a`) — [github.com/raysan5/raylib](https://github.com/raysan5/raylib)
- Fontes **DejaVu** — `sudo apt install fonts-dejavu-core`

## Tecnologias

- **Linguagem:** C++17
- **Visualização:** Raylib
- **Build:** GNU Make
- **Conceitos:** Algoritmo Guloso, EDF, Análise de Complexidade, Prova por Argumento de Troca
