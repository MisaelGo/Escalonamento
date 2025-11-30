# Escalonamento
Implementação de operações elementares de matriz, escalonamento RREF e leitura de matrizes em C.

Arquivo principal: main.c

🧑‍💻 Contribuição Popular (Equipe)
Misael Gomes — Coordenador da equipe
Vitor Santos & Henrique Soares — Programadores do main.c
Alyyson — Suporte Matemático e testes

📌 Visão Geral
Este projeto implementa um motor completo de operações matriciais, com foco na redução por linhas (RREF – Reduced Row Echelon Form) utilizando operações elementares clássicas da Álgebra Linear.

O arquivo matriz_definitiva4.c contém todo o núcleo lógico do sistema:
✔ Estrutura de dados otimizada
✔ Criação/gerenciamento de matrizes
✔ Operações elementares
✔ Escalonamento (Gauss-Jordan) estável
✔ Leitura de múltiplas matrizes via arquivo
✔ Geração de matrizes aleatórias
✔ Interface textual interativa

Esse engine foi pensado como base para o PixeLab, um software maior que conecta Álgebra Linear ao processamento de imagens. Segue o Link do PixeLab: https://github.com/MisaelGo/PixeLab.git

🧩 Estrutura Geral do Arquivo
O código está organizado nas seguintes seções:

1. Includes e defines
Contém bibliotecas essenciais, constantes de limite (MATRIX_LIMIT) e EPSILON, usado para limpeza de números muito pequenos.

2. Estrutura Matrix
typedef struct {
    double** data;   // Ponteiros para cada linha
    double* block;   // Bloco contíguo de memória
    size_t linhas, colunas;
} Matrix;

Essa arquitetura traz BENEFÍCIOS: 
- Acesso rápido (cache-friendly)
- Fácil realocação e liberação de memória
- Estrutura estável para operações intensivas
- Facilita integração com kernels de convolução e imagens (no PixeLab).

🏗 Funcionalidades Principais
🔹 1. Criação e Destruição de Matrizes
Funções: create_matrix() e free_matrix().
Implementam uma alocação dupla: block contíguo para todos os valores, vetor de ponteiros apontando para cada linha. Isso combina performance e simplicidade.

🔹 2. Inserção e Impressão
ins_elem_matrix() e print_matrix()
Permitem adicionar elementos e visualizar a matriz formatada.

🔹 3. Operações Elementares
a) Trocar Linhas: swap_rows()
b) Multiplicar Linha por Escalar: scale_row() -> Possui lógica de limpeza automática: if (fabs(x) < EPSILON) x = 0.0;
c) Soma de Linhas: add_rows() -> Implementa a operação: Li → Li + k * Lj

Essas três funções formam a base do método de Gauss-Jordan.

🔹 4. Escalonamento Completo – RREF
A função to_rref(Matrix *m) faz: Busca do melhor pivô (find_pivot_row), troca de linhas se necessário, normalização do pivô, zerando acima e abaixo do pivô, passo até completar todas as colunas. É uma implementação estável, com busca de pivô por módulo (evita erros numéricos).

🔹 5. Entrada/Interação com Usuário
O menu permite: Trocar linhas, Multiplicar linha por escalar, Escalonar (Gauss-Jordan), Ler próxima matriz de arquivo, Finalizar e liberar memória. Isso transforma o programa em uma ferramenta manual de estudo e testes.

🔹 6. Leitura de Matrizes de Arquivo
ler_todas_as_matrizes(): Lê quantas matrizes existirem em matrizes.txt ou matrizes_rand.txt. Cria um vetor dinâmico contendo todas elas.

🔹 7. Geração Aleatória de Matrizes
Função gerador_matriz_aleatorio() gera: Número variável de matrizes, Dimensões aleatórias, Elementos aleatórios dentro de um intervalo, Escreve tudo em matrizes_rand.txt. É útil para testes automatizados.

🧠 Fluxo do Programa
Quando executado,
1) usuário escolhe: 
 - Matriz individual
 - Ler várias matrizes de arquivo
 - Gerar matrizes aleatórias
2) O programa cria as matrizes e mostra um menu interativo
3) Usuário manipula a matriz
4) Pode escalonar para RREF
5) Avança matrizes (se for leitura em lote)
6) No fim, tudo é liberado da memória

🧪 Testes e Estabilidade
Recursos de estabilidade implementados: Normalização com EPSILON, Busca de melhor pivô (Método de pivotamento parcial), Limpeza de resíduos numéricos muito pequenos, Verificações de erro, Controle de limites e overflow. Isso garante que o escalonamento funcione mesmo para matrizes mal condicionadas.

⚠ Limitações Conhecidas
- Limite de elementos baseado em MATRIX_LIMIT
- Apenas matrizes com números de ponto flutuante (double)
- Não trata sistemas simbólicos
- Não imprime frações (somente ponto flutuante)
- Não salva o resultado em arquivo automaticamente

🎯 Casos de Uso
Este arquivo é ideal para:
✔ Estudos de Álgebra Linear
✔ Sistemas lineares
✔ Ensino de operações de linha
✔ Implementações de Gauss-Jordan
✔ Processamento de imagens (kernels 3×3 etc.)
✔ Base para grafos matriciais
✔ Cálculo numérico
✔ Projeto PixeLab (versão de linha de comando)

📄 Como Executar
Compilação:
gcc matriz_definitiva4.c -o matriz -lm

Execução:
./matriz

📚 Arquivos Adicionais Suportados
matrizes.txt
matrizes_rand.txt

Formato do arquivo:
3 3
1 2 3
4 5 6
7 8 9
2 2
1 0
0 1

⭐ Resumo Final

main.c é um módulo completo, robusto e didático para manipulação matricial.
Serve como:
- Ferramenta educacional
- Núcleo matemático do PixeLab
- Base para aplicações científicas
- Algoritmo confiável de Gauss-Jordan

