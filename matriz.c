#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <limits.h>
#include <math.h>
#include <time.h> // time() fornece a aleatoriedade para srand() (esse faz parte de stdlib.h)


#define MATRIX_LIMIT 1000
#define EPSILON 1e-12

typedef struct {
    double** data;   // Ponteiro para os ponteiros das linhas
    double* block;   // Ponteiro para o bloco de dados contíguo
    size_t linhas, colunas;        // Número de linhas e colunas da matriz
} Matrix;

// Funcoes de alocacao/liberacao
Matrix* create_matrix(size_t linhas, size_t colunas); // Criar Matriz
void free_matrix(Matrix* m); // Liberar Matriz

// Funcoes de insercao de elementos
int ins_elem_matrix(Matrix* m, size_t i, size_t j, double num); // inserção dos elementos da matriz

// Funcoes de manipulacao da matriz
void swap_rows(Matrix* m, size_t r1, size_t r2);    // Trocar linhas
void scale_row(Matrix* m, size_t r, double scalar); // Multiplicar por escalar
void add_rows(Matrix* m, size_t target_row, size_t source_row, double scalar); // Somar linhas no sentido de: Li -> Li + 2Lj
size_t find_pivot_row(Matrix *m, size_t col, size_t start_row); // Adicionado para melhorar a implemetação da função to_rref
void to_rref(Matrix* m); // Escalonamento completo de Matrizes

// Etc
int ler_double(const char *prompt, double *out); // Funcao para entrada de elementos robusta
void print_matrix(Matrix* m); // Adicionei para ajudar a visualizar
void clear_input_buffer(); //Adicionada para evitar problemas com o buffer cheio

int valor_aleatorio(int min, int max);
void gerador_matriz_aleatorio();


// Funcoes de leitura de arquivo
Matrix* ler_matriz_arquivo(FILE *arq);
size_t ler_todas_as_matrizes(const char *nome_arquivo, Matrix ***vetor_saida);


int main() {
    unsigned int opcao = 0;
    unsigned int opcao_leitura = 0;
    unsigned int opcao_aleatoriedade = 0;

    printf("Como deseja usar o programa?\n");
    printf("1 - Matriz individual.\n");
    printf("2 - Ler matriz(es) de um arquivo.\n");
    printf("0 - Terminar programa.\n");
    printf("-> ");
    scanf("%u", &opcao_leitura);

    Matrix *minha_matriz = NULL, *matriz_atual = NULL;
    size_t num_cols, num_rows;

    Matrix** array_matrizes = NULL; size_t num_matrizes = 0, matriz_indice = 0;

    switch (opcao_leitura){
        case 1:
            
            printf("Digite o numero de linhas da matriz: ");
            scanf("%zu", &num_rows);
            clear_input_buffer();
            printf("Digite o numero de colunas da matriz: ");
            scanf("%zu", &num_cols);
            clear_input_buffer();

            if (num_rows <= 0 || num_cols <= 0) {
                printf("Apenas numeros positivos sao validos.\n");
                return 1;
            }

            minha_matriz = create_matrix(num_rows, num_cols);

            if (minha_matriz == NULL) {
                fprintf(stderr, "Falha ao criar a matriz. A sair.\n");
                return 1;
            }

            printf("\n--- Insercao de Elementos ---\n");
            printf("Digite os %zu elementos da matriz.\n", num_rows * num_cols);

            /* loop para pedir cada elemento ao usuário */
            char prompt[256];
            for (size_t i = 0; i < num_rows; i++) {
                for (size_t j = 0; j < num_cols; j++) {
                    snprintf(prompt, sizeof(prompt), "Digite o valor para a posicao [%zu][%zu]: ", i + 1, j + 1);
                    ler_double(prompt, &(minha_matriz->data[i][j]));
                    clear_input_buffer();
                }
            }

            matriz_atual = minha_matriz;
            break;
        
        case 2:
                printf("Como serao as matrizes?\n");
                printf("1 - Matrizes com elementos predeterminados (\"matrizes.txt\")\n");
                printf("2 - Matrizes com elementos aleatorios (\"matrizes_rand.txt\")\n");
                printf("-> ");
                scanf("%u", &opcao_aleatoriedade);
                if (opcao_aleatoriedade < 1 || opcao_aleatoriedade > 2){
                    do {
                        printf("Entrada invalida!\n");
                        printf("-> ");
                        scanf("%u", &opcao_aleatoriedade);
                    }while (opcao_aleatoriedade < 1 || opcao_aleatoriedade > 2);
                }
            

            if(opcao_aleatoriedade == 1){
               num_matrizes = ler_todas_as_matrizes("matrizes.txt", &array_matrizes); 
            }
            else if (opcao_aleatoriedade == 2){
                srand(time(NULL));
                gerador_matriz_aleatorio();
                num_matrizes = ler_todas_as_matrizes("matrizes_rand.txt", &array_matrizes);
            }

            if (num_matrizes == 0){
                printf("O arquivo esta vazio! Saindo do programa...\n");
                return 1;
            }

            matriz_atual = array_matrizes[0];
            num_rows = matriz_atual->linhas;
            num_cols = matriz_atual->colunas;
            break;

        case 0:
            printf("\nSaindo...\n");
            return 0;

        default:
            printf("Opcao invalida!");
            return 0;
    }

    if (!matriz_atual){
        fprintf(stderr, "ERRO! Nenhuma matriz carregada.\n");
        return 1;
    }

    do {
        /* mostra a matriz final */
        printf("\n--- Matriz Preenchida ---\n");
        print_matrix(matriz_atual);

        printf("\n--- Menu de Operacoes ---\n");
        printf("1: Trocar linhas da matriz\n");
        printf("2: Multiplicar por escalar\n");
        printf("3: Escalonar a matriz (RREF)\n");
        printf("4: Proxima matriz (apenas para a opcao de leitura de arquivo).\n");
        printf("5: Sair e liberar a memoria\n");
        printf("Escolha uma opcao: ");
        scanf("%u", &opcao);
        clear_input_buffer();
        
        switch(opcao) {
            case 1: {

                size_t linha1, linha2;
                printf("Digite a primeira linha que deseja trocar (de 1 a %zu): ", num_rows);
                scanf("%zu", &linha1);
                clear_input_buffer();
                printf("Digite a segunda linha que deseja trocar (de 1 a %zu): ", num_rows);
                scanf("%zu", &linha2);
                clear_input_buffer();

                swap_rows(matriz_atual, (linha1 - 1), (linha2 - 1));
                break;
            }

            case 2: {
                size_t linha;
                double escalar; 
                printf("Digite a linha que deseja multiplicar (de 1 a %zu): ", num_rows);
                scanf("%zu", &linha);
                clear_input_buffer();
                if (linha > num_rows || linha < 1){
                    fprintf(stderr, "ERRO: Linha invalida. Deve estar entre 1 e %zu.\n", num_rows);
                } else {
                    printf("Digite o escalar: ");
                    scanf("%lf", &escalar); 
                    clear_input_buffer();
                
                    scale_row(matriz_atual, (linha - 1), escalar);
                    printf("\nLinha multiplicada!\n");
                }
                break;
            }
            case 3:{
                to_rref(matriz_atual);
                printf("\nMatriz escalonada com sucesso!\n");
                break;
            }
            case 4: {
                if (opcao_leitura == 2 && matriz_indice + 1 < num_matrizes){
                    matriz_indice++;
                    matriz_atual = array_matrizes[matriz_indice];
                    num_rows = matriz_atual->linhas;
                    num_cols = matriz_atual->colunas;
                }
                else{
                    printf("\nNao ha mais matrizes.\n");
                    printf("Exibindo a matriz atual:\n");
                }
                break;
            }

            case 5:{
                // Sai do loop e passa para o final do programa para liberar memoria.
                printf("\nLiberando memoria e saindo do programa...\n");
                break;
            }
            default:
                printf("\nOpcao invalida! Por favor, tente novamente.\n");
                break;
        }
    } while (opcao != 5);


    if (opcao_leitura == 1){
        free_matrix(matriz_atual);
    }
    else if (opcao_leitura == 2){
        for (size_t i = 0; i < num_matrizes; i++){
            free_matrix(array_matrizes[i]);
        }
        free(array_matrizes);
    }

    return 0;
}



Matrix* create_matrix(size_t linhas, size_t colunas) {
    
    if (linhas == 0 || colunas == 0) {
        fprintf(stderr, "ERRO: Linhas e colunas da matriz nao podem ser zero.\n");
        return NULL;
    }

    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    if (!m) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para a estrutura Matrix.\n");
        return NULL;
    }
    m->linhas = linhas;
    m->colunas = colunas;
    m->data = NULL;
    m->block = NULL; 

    if (linhas > MATRIX_LIMIT / sizeof(double*)) {
        fprintf(stderr, "ERRO: Tamanho da matriz muito grande para alocar ponteiros de linha.\n");
        free(m);
        return NULL;
    }

    m->data = (double**)malloc(linhas * sizeof(double*));
    if (!m->data) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para as linhas.\n");
        free(m);
        return NULL;
    }

    if (linhas * colunas > MATRIX_LIMIT) {
         fprintf(stderr, "ERRO: Overflow de tamanho. Ordem da matriz muito grande.\n");
         free(m->data);
         free(m);
         return NULL;
    }
    size_t num_elements = linhas * colunas;

    m->block = (double*)calloc(num_elements, sizeof(double));
    if (!m->block) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para os elementos da matriz.\n");
        free(m->data); 
        free(m);
        return NULL;
    }

    for (size_t i = 0; i < linhas; i++) {
        m->data[i] = m->block + (i * colunas);
    }

    return m;
}

void free_matrix(Matrix* m) {
    
    if (m == NULL) {
        return;
    }

    free(m->block);
    free(m->data);
    free(m);
}


int ins_elem_matrix(Matrix* m, size_t i, size_t j, double num) {
    
    if (m == NULL || m->data == NULL) {
        fprintf(stderr, "ERRO! Matriz nula ou nao inicializada!\n");
        return 0;
    }

    if (i >= m->linhas || j >= m->colunas) {
        fprintf(stderr, "ERRO: Indices [%zu][%zu] estao fora dos limites da matriz.\n", i, j);
        return 0;
    }

    m->data[i][j] = num;

    return 1;
}

void print_matrix(Matrix* m){
    for (size_t i = 0; i < m->linhas; i++) {
        printf("\n[");
        for (size_t j = 0; j < m->colunas; j++) {
            printf(" %6.2f", m->data[i][j]);
        }
        printf(" ]");
    }
    printf("\n");
}

void swap_rows(Matrix* m, size_t r1, size_t r2) {

    if (m == NULL || m->data == NULL) {
        fprintf(stderr, "ERRO: Matriz nula ou nao inicializada!\n");
        return;
    }

    if (r1 >= m->linhas || r2 >= m->linhas) {
        fprintf(stderr, "ERRO: Linha(s) [%zu, %zu] invalidas para troca.\n", r1 + 1, r2 + 1);
        return;
    }

    if (r1 == r2) {
        return;
    }

    double* temp = m->data[r1];
    m->data[r1] = m->data[r2];
    m->data[r2] = temp;
}

void scale_row(Matrix* m, size_t r, double scalar) {
    if (m == NULL || m->data == NULL) {
        fprintf(stderr, "ERRO: Matriz nula ou nao inicializada!\n");
        return;
    }
    if (r >= m->linhas) {
        fprintf(stderr, "ERRO: Linha [%zu] invalida para escalar.\n", r);
        return;
    }

    if (fabs(scalar - 1.0) < EPSILON) {
        return;
    }

    for (size_t j = 0; j < m->colunas; j++) {
        m->data[r][j] *= scalar;
        // limpeza do resto da conta
        if (fabs(m->data[r][j]) < EPSILON)
            m->data[r][j] = 0.0;
    }
}

void add_rows(Matrix* m, size_t target_row, size_t source_row, double scalar) {
     if (m == NULL || m->data == NULL) {
        fprintf(stderr, "ERRO: Matriz nula ou nao inicializada!\n");
        return;
    }
    if (target_row >= m->linhas || source_row >= m->linhas) {
        fprintf(stderr, "ERRO: Linha(s) [%zu, %zu] invalidas para soma.\n", target_row, source_row);
        return;
    }
    
    if (fabs(scalar) < EPSILON) {
        return;
    }

    for (size_t j = 0; j < m->colunas; j++) {
        m->data[target_row][j] += scalar * m->data[source_row][j];
        // limpeza do resto da conta
        if (fabs(m->data[target_row][j]) < EPSILON) {
            m->data[target_row][j] = 0.0;
        }
    }
}

size_t find_pivot_row(Matrix *m, size_t col, size_t start_row){
    if (!m || !m->data || start_row >= m->linhas || col >= m->colunas){
        fprintf(stderr, "\nERRO! Parametros invalidos para achar o pivot!\n");
        return SIZE_MAX;
    }
        
    size_t best_row = start_row;
    double best_value = fabs(m->data[start_row][col]);

    for(size_t i = start_row + 1; i < m->linhas; i++){
        double current_value = fabs(m->data[i][col]);
        if(current_value > best_value){
            best_value = current_value;
            best_row = i;
        }
    }
    
    if (best_value <= EPSILON){
        return SIZE_MAX; // nenhum pivô válido (não-nulo) encontrado
    }
        
    return best_row;
}

void to_rref(Matrix *m){
    if(!m || !m->data) {
        fprintf(stderr, "\nERRO: Matriz nula ou nao inicializada!\n");
        return;
    }
    
    size_t lead_row = 0; 
    size_t pivot_col = 0; 
    size_t num_cols = m->colunas;
    size_t num_rows = m->linhas;

    while(lead_row < num_rows && pivot_col < num_cols) { 
        
        size_t pivot_row_found = find_pivot_row(m, pivot_col, lead_row);
        
        if (pivot_row_found == SIZE_MAX){ 
            pivot_col++; 
            continue;
        }

        if (lead_row != pivot_row_found){
            swap_rows(m, lead_row, pivot_row_found);
        }

        double pivot_value = m->data[lead_row][pivot_col];
        double pivot_inverse = 1.0 / pivot_value;
        scale_row(m, lead_row, pivot_inverse);
        
        for(size_t r = 0; r < num_rows; r++){
            if(r == lead_row)
                continue; 
            
            double factor = m->data[r][pivot_col];
            if (fabs(factor) > EPSILON) {
                add_rows(m, r, lead_row, -factor);
            }
        } 
        
        lead_row++;
        pivot_col++;
    } 
}

void clear_input_buffer() {
    int c;
    while ( (c = getchar()) != '\n' && c != EOF );
}

int ler_double(const char *prompt, double *out){
    printf("%s", prompt);
    while (scanf("%lf", out) != 1){
        fprintf(stderr, "Entrada invalida! Tente novamente: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // flush
    }
    return 0;
}

Matrix* ler_matriz_arquivo(FILE *arq){
    size_t rows, cols;

    // Ler as dimensoes da matriz atual
    if(fscanf(arq, "%zu %zu", &rows, &cols) != 2){ // Verifica se fscanf leu os dois dados
        return NULL;
    }

    Matrix* m = create_matrix(rows, cols);
    if (!m){
        return NULL;
    }

    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (fscanf(arq, "%lf", &m->data[i][j]) != 1){
                fprintf(stderr, "\nERRO! Formato invalido dos elementos da matriz no arquivo!\n");
                free_matrix(m);
                return NULL;
            }
        }
    }

    return m;
}

size_t ler_todas_as_matrizes(const char *nome_arquivo, Matrix ***vetor_saida){
    FILE *arq = fopen(nome_arquivo, "rt");
    if (!arq){
        fprintf(stderr, "ERRO! Nao foi possivel abrir o arquivo!");
        arq = fopen("matrizes.txt", "wt");
        fclose(arq);
        return 0;
    }

    size_t capacidade = 4;
    size_t contador = 0;
    Matrix **lista = malloc(capacidade * sizeof(Matrix*));

    while(1){
        Matrix *m = ler_matriz_arquivo(arq);
        if(!m){
            break; // Nao existem mais matrizes para serem lidas
        } 

        if (contador == capacidade){
            capacidade *= 2;
            Matrix **tmp = realloc(lista, capacidade * sizeof(Matrix*));
            if (!tmp){
                fprintf(stderr, "ERRO! Falha ao realocar a memoria");
                for (size_t i = 0; i < contador; i++){
                    free_matrix(lista[i]);
                }
                free(lista);
                fclose(arq);
                return 0;
            }
            lista = tmp;
        }

        lista[contador] = m;
        contador += 1;
    }

    fclose(arq);
    *vetor_saida = lista;
    return contador;
}



void gerador_matriz_aleatorio(){
    size_t max_linhas = 0, max_cols = 0;
    unsigned int num_matrizes = 0;
    int limite_inferior = 0, limite_superior = 0; // Para os elementos da(s) matriz(es)

    printf("---Inicializando Gerador---\n\n");
    printf("Insira a quantidade de matrizes a serem geradas\n");
    printf("-> ");
    scanf("%u", &num_matrizes);
    printf("Insira o valor maximo de linhas da(s) matriz(es)\n");
    printf("-> ");
    scanf("%zu", &max_linhas);
    printf("Insira o valor maximo de colunas da(s) matriz(es)\n");
    scanf("%zu", &max_cols);
    
    if (max_linhas == 0 || max_cols == 0){
        printf("Valor(es) invalido(s) para linhas e\\ou colunas!\n");
        do{
            printf("Maximo de linhas -> ");
            scanf("%zu", &max_linhas);
            printf("Maximo de colunas -> ");
            scanf("%zu", &max_cols);
        }while(max_linhas == 0 || max_cols == 0);
    }

    printf("Insira o valor minimo de cada elemento da matriz (pode ser negativo)\n");
    printf("-> ");
    scanf("%d", &limite_inferior);
    printf("Insira o valor maximo de cada elemento da matriz (pode ser negativo)\n");
    printf("-> ");
    scanf("%d", &limite_superior);
    if (limite_superior < limite_inferior){
        printf("Limites invalidos! O limite superior deve ser maior que o inferior.\n");
        do{
            printf("Limite inferior -> ");
            scanf("%d", &limite_inferior);
            printf("Limite superior -> ");
            scanf("%d", &limite_superior);
        }while (limite_superior < limite_inferior);
    }


    Matrix *buff;

    FILE *arq = fopen("matrizes_rand.txt", "wt");

    int elem;
    unsigned int contador = 0;
    for (contador = 1; contador <= num_matrizes; contador++){
        buff = create_matrix((rand() % max_linhas) + 1, (rand() % max_cols) + 1);
        fprintf(arq, "%zu %zu\n", buff->linhas, buff->colunas);
        
        for (size_t i = 0; i < buff->linhas; i++){
            for (size_t j = 0; j < buff->colunas; j++){
                elem = valor_aleatorio(limite_inferior, limite_superior);
                fprintf(arq, "%d ", elem);
            }
            fprintf(arq, "\n");
        }
        free_matrix(buff);
    }

    fclose(arq);

}

int valor_aleatorio(int min, int max){
    int intervalo = max - min + 1;
    return (min + rand() % intervalo);
}