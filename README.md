# 🧠 Tema 11 - Resolução Paralela de Sudoku com Multithreading

Trabalho prático da disciplina de **Sistemas Operacionais** da **Universidade Federal de Roraima (UFRR)**, ministrada pelo professor **Hebert Rocha**.

O objetivo deste projeto é implementar uma aplicação em linguagem **C** que utiliza **multithreading com a biblioteca `pthread`** para resolver de forma paralela um tabuleiro de Sudoku, explorando o paralelismo dos processadores modernos.

---

## 👥 Integrantes

- Vinicius Cavalcante Martins - [@vinimartinsufrr](https://github.com/vinimartinsufrr)
- Jasmim Sabini - [@jasmimsabini](https://github.com/jasmimsabini)

---

## 📁 Estrutura do Repositório

### 📦 JasmimVinicius_ws_OS_2025
JasmimVinicius_ws_OS_2025/
├── tema11.c                 // código-fonte em C com uso de pthreads

├── slide-apresentacao.pdf   // slides utilizados para a apresentação

├── diagrama-funcionamento.png // diagrama ilustrando a lógica do programa

└── README.md                // este arquivo

---

## 🧩 Descrição do Projeto

O projeto implementa a **resolução paralela de Sudoku** utilizando até 9 threads, onde cada thread testa uma possibilidade diferente para a primeira célula vazia do tabuleiro. A partir dessa tentativa, o algoritmo aplica **backtracking** para resolver o restante da matriz.

A abordagem visa demonstrar o uso de **threads independentes** para acelerar processos computacionais em problemas complexos e exploráveis em paralelo.

---

## 🧪 Como Compilar e Executar

### Requisitos:
- Sistema operacional Linux (ou WSL no Windows)
- GCC com suporte à pthread

### Compilação:
```bash
gcc tema11.c -o tema11 -lpthread
