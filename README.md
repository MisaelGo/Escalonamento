<div align="center">
  <h1>📘 Escalonamento — Motor de Matrizes em C</h1>
  <p><strong>Implementação de operações elementares de matriz, escalonamento RREF e leitura de matrizes em C.</strong></p>
  <p><strong>Arquivo principal:</strong> <code>main.c</code></p>
</div>

<hr>

<div>
  <h2>🧑‍💻 Contribuição Popular (Equipe)</h2>
  <ul>
    <li><strong>Misael Gomes</strong> — Coordenador da equipe</li>
    <li><strong>Vitor Santos &amp; Henrique Soares</strong> — Programadores do <code>main.c</code></li>
    <li><strong>Alyyson</strong> — Suporte Matemático e testes</li>
  </ul>
</div>

<hr>

<div>
  <h2>📌 Visão Geral</h2>
  <p>
    Este projeto implementa um motor completo de operações matriciais, com foco na 
    <strong>redução por linhas (RREF – Reduced Row Echelon Form)</strong> utilizando operações elementares clássicas da Álgebra Linear.
  </p>
  <p>
    O arquivo <code>matriz_definitiva4.c</code> contém todo o núcleo lógico do sistema:
  </p>
  <ul>
    <li>✔ Estrutura de dados otimizada</li>
    <li>✔ Criação/gerenciamento de matrizes</li>
    <li>✔ Operações elementares</li>
    <li>✔ Escalonamento (Gauss-Jordan) estável</li>
    <li>✔ Leitura de múltiplas matrizes via arquivo</li>
    <li>✔ Geração de matrizes aleatórias</li>
    <li>✔ Interface textual interativa</li>
  </ul>
  <p>
    Esse engine foi pensado como base para o <strong>PixeLab</strong>, um software maior que conecta Álgebra Linear ao processamento de imagens.
  </p>
  <p>
    Segue o link do PixeLab: 
    <a href="https://github.com/MisaelGo/PixeLab.git" target="_blank" rel="noopener noreferrer">
      https://github.com/MisaelGo/PixeLab.git
    </a>
  </p>
</div>

<hr>

<div>
  <h2>🧩 Estrutura Geral do Arquivo</h2>

  <h3>1. Includes e defines</h3>
  <p>
    Contém bibliotecas essenciais, constantes de limite (<code>MATRIX_LIMIT</code>) e 
    <code>EPSILON</code>, usado para limpeza de números muito pequenos.
  </p>

  <h3>2. Estrutura <code>Matrix</code></h3>
  <pre><code>typedef struct {
    double** data;   // Ponteiros para cada linha
    double* block;   // Bloco contíguo de memória
    size_t linhas, colunas;
} Matrix;
</code></pre>

  <p>Essa arquitetura traz BENEFÍCIOS:</p>
  <ul>
    <li>Acesso rápido (cache-friendly)</li>
    <li>Fácil realocação e liberação de memória</li>
    <li>Estrutura estável para operações intensivas</li>
    <li>Facilita integração com kernels de convolução e imagens (no PixeLab)</li>
  </ul>
</div>

<hr>

<div>
  <h2>🏗 Funcionalidades Principais</h2>

  <h3>🔹 1. Criação e Destruição de Matrizes</h3>
  <p>
    Funções: <code>create_matrix()</code> e <code>free_matrix()</code>.<br>
    Implementam uma alocação dupla: 
  </p>
  <ul>
    <li><strong>block contíguo</strong> para todos os valores</li>
    <li><strong>vetor de ponteiros</strong> apontando para cada linha</li>
  </ul>
  <p>Isso combina performance e simplicidade.</p>

  <h3>🔹 2. Inserção e Impressão</h3>
  <p>
    <code>ins_elem_matrix()</code> e <code>print_matrix()</code><br>
    Permitem adicionar elementos e visualizar a matriz formatada.
  </p>

  <h3>🔹 3. Operações Elementares</h3>
  <ul>
    <li><strong>a)</strong> Trocar Linhas: <code>swap_rows()</code></li>
    <li>
      <strong>b)</strong> Multiplicar Linha por Escalar: <code>scale_row()</code><br>
      Possui lógica de limpeza automática:
      <pre><code>if (fabs(x) &lt; EPSILON) x = 0.0;</code></pre>
    </li>
    <li>
      <strong>c)</strong> Soma de Linhas: <code>add_rows()</code><br>
      Implementa a operação:
      <pre><code>Li → Li + k * Lj</code></pre>
    </li>
  </ul>
  <p>Essas três funções formam a base do método de Gauss-Jordan.</p>

  <h3>🔹 4. Escalonamento Completo – RREF</h3>
  <p>
    A função <code>to_rref(Matrix *m)</code> faz:
  </p>
  <ul>
    <li>Busca do melhor pivô (<code>find_pivot_row</code>)</li>
    <li>Troca de linhas se necessário</li>
    <li>Normalização do pivô</li>
    <li>Zera acima e abaixo do pivô</li>
    <li>Itera até completar todas as colunas</li>
  </ul>
  <p>
    É uma implementação estável, com busca de pivô por módulo (evita erros numéricos).
  </p>

  <h3>🔹 5. Entrada/Interação com Usuário</h3>
  <p>O menu permite:</p>
  <ul>
    <li>Trocar linhas</li>
    <li>Multiplicar linha por escalar</li>
    <li>Escalonar (Gauss-Jordan)</li>
    <li>Ler próxima matriz de arquivo</li>
    <li>Finalizar e liberar memória</li>
  </ul>
  <p>
    Isso transforma o programa em uma ferramenta manual de estudo e testes.
  </p>

  <h3>🔹 6. Leitura de Matrizes de Arquivo</h3>
  <p>
    <code>ler_todas_as_matrizes()</code>: lê quantas matrizes existirem em 
    <code>matrizes.txt</code> ou <code>matrizes_rand.txt</code>.<br>
    Cria um vetor dinâmico contendo todas elas.
  </p>

  <h3>🔹 7. Geração Aleatória de Matrizes</h3>
  <p>
    Função <code>gerador_matriz_aleatorio()</code> gera:
  </p>
  <ul>
    <li>Número variável de matrizes</li>
    <li>Dimensões aleatórias</li>
    <li>Elementos aleatórios dentro de um intervalo</li>
    <li>Escreve tudo em <code>matrizes_rand.txt</code></li>
  </ul>
  <p>É útil para testes automatizados.</p>
</div>

<hr>

<div>
  <h2>🧠 Fluxo do Programa</h2>
  <ol>
    <li>Usuário escolhe:
      <ul>
        <li>Matriz individual</li>
        <li>Ler várias matrizes de arquivo</li>
        <li>Gerar matrizes aleatórias</li>
      </ul>
    </li>
    <li>O programa cria as matrizes e mostra um menu interativo</li>
    <li>Usuário manipula a matriz</li>
    <li>Pode escalonar para RREF</li>
    <li>Avança matrizes (se for leitura em lote)</li>
    <li>No fim, tudo é liberado da memória</li>
  </ol>
</div>

<hr>

<div>
  <h2>🧪 Testes e Estabilidade</h2>
  <p>Recursos de estabilidade implementados:</p>
  <ul>
    <li>Normalização com <code>EPSILON</code></li>
    <li>Busca de melhor pivô (método de pivotamento parcial)</li>
    <li>Limpeza de resíduos numéricos muito pequenos</li>
    <li>Verificações de erro</li>
    <li>Controle de limites e overflow</li>
  </ul>
  <p>
    Isso garante que o escalonamento funcione mesmo para matrizes mal condicionadas.
  </p>
</div>

<hr>

<div>
  <h2>⚠ Limitações Conhecidas</h2>
  <ul>
    <li>Limite de elementos baseado em <code>MATRIX_LIMIT</code></li>
    <li>Apenas matrizes com números de ponto flutuante (<code>double</code>)</li>
    <li>Não trata sistemas simbólicos</li>
    <li>Não imprime frações (somente ponto flutuante)</li>
    <li>Não salva o resultado em arquivo automaticamente</li>
  </ul>
</div>

<hr>

<div>
  <h2>🎯 Casos de Uso</h2>
  <p>Este arquivo é ideal para:</p>
  <ul>
    <li>✔ Estudos de Álgebra Linear</li>
    <li>✔ Sistemas lineares</li>
    <li>✔ Ensino de operações de linha</li>
    <li>✔ Implementações de Gauss-Jordan</li>
    <li>✔ Processamento de imagens (kernels 3×3 etc.)</li>
    <li>✔ Base para grafos matriciais</li>
    <li>✔ Cálculo numérico</li>
    <li>✔ Projeto PixeLab (versão de linha de comando)</li>
  </ul>
</div>

<hr>

<div>
  <h2>📄 Como Executar</h2>

  <h3>Compilação</h3>
  <pre><code>gcc matriz_definitiva4.c -o matriz -lm</code></pre>

  <h3>Execução</h3>
  <pre><code>./matriz</code></pre>
</div>

<hr>

<div>
  <h2>📚 Arquivos Adicionais Suportados</h2>
  <ul>
    <li><code>matrizes.txt</code></li>
    <li><code>matrizes_rand.txt</code></li>
  </ul>

  <h3>Formato do arquivo:</h3>
  <pre><code>3 3
1 2 3
4 5 6
7 8 9
2 2
1 0
0 1
</code></pre>
</div>

<hr>

<div>
  <h2>⭐ Resumo Final</h2>
  <p>
    <code>main.c</code> é um módulo completo, robusto e didático para manipulação matricial.
  </p>
  <p>Serve como:</p>
  <ul>
    <li>Ferramenta educacional</li>
    <li>Núcleo matemático do PixeLab</li>
    <li>Base para aplicações científicas</li>
    <li>Algoritmo confiável de Gauss-Jordan</li>
  </ul>
</div>

