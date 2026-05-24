# *TRABALHO 3 - SGBD 2026.1*

#### Alunos: Américo Vitor Moreira Barbosa - 571045 // Aluizio Pereira Almendra Neto - 565068

---

Implementação do operador de junção **Sort-Merge Join** entre as tabelas `Wines` e `Grapes`, com ordenação externa limitada a **B = 5 frames** de buffer e páginas de **12 tuplas**.

## Rodar o programa

Compilar e executar o `main.cpp` na raiz do projeto:

```bash
g++ -o main main.cpp
./main
```

Os arquivos `wines.csv` e `grapes.csv` devem estar na pasta `data/`.

---

## Estrutura do projeto

```
trabalho03/
├── src
│    ├── main.cpp                  # Ponto de entrada e casos de teste
│    ├── data/
│    │   ├── wines.csv             # Tabela Wines (511 tuplas)
│    │   └── grapes.csv            # Tabela Grapes (39 tuplas)
│    ├── models/
│    │   ├── esquema.hpp           # Mapeamento nome de coluna → índice
│    │   ├── pagina.hpp            # Structs Tupla e Pagina (máx. 12 tuplas)
│    │   ├── tabela.hpp            # Struct Tabela (lista de páginas + esquema)
│    │   └── disco.hpp             # Simulação de disco (vetor de tabelas)
│    ├── utils/
│    │   └── leitura.hpp           # Leitura de CSV
│    └── sort_merge_join/
│        ├── runs.hpp              # geração de runs ordenadas
│        ├── intercalacao.hpp      # intercalação externa
│        ├── ordenacao.hpp         # Orquestra gerarRuns + intercala
│        ├── merge_join.hpp        # Merge Join
│        └── sort_merge_join.hpp   # Sort Merge join
├── main.exe                       # Executavel do programa no windows
├── main                           # Executavel do programa no unix
└── README.md 
```

---

## Como o Sort-Merge Join funciona

### Parte 1 — Geração de runs (`runs.hpp`)
Lê no máximo 5 páginas da tabela por vez para o buffer, ordena em memória pelo atributo de junção e grava como uma *run* ordenada no disco simulado. Repete até cobrir todas as páginas.

### Parte 2 — Intercalação externa (`intercalacao.hpp`)
Usa até 4 frames como buffers de entrada e 1 frame como buffer de saída. Seleciona a menor tupla entre as runs ativas e a insere no frame de saída. Quando o frame de saída está cheio, grava no disco. Se houver mais de 4 runs, novas passagens são realizadas até restar uma única tabela ordenada.

### Parte 3 — Merge Join (`merge_join.hpp`)
Percorre as duas tabelas ordenadas com dois ponteiros. Quando os valores do atributo de junção coincidem, identifica o grupo inteiro em cada tabela e produz o **produto cartesiano** entre eles. Avança o ponteiro da tabela com menor valor quando não há correspondência. As tabelas originais não são modificadas.

---

## Esquema utilizado

```
Grapes                          Wines
─────────────────────           ──────────────────────────
PK chave_primaria               PK vinho
   grape_synonym                FK chave_estrangeira
   grape_country                   origem
                                   tipo
                                   estilo
```

Junção: `Wines.chave_estrangeira = Grapes.chave_primaria`

---

## Casos de teste (`main.cpp`)

| Teste | Descrição | Resultado esperado |
|---|---|---|
| Teste 1 | Ordenação externa com 70 tuplas fora de ordem | Tabela ordenada pelo atributo |
| Teste 2 | Merge Join simples sem duplicatas | 4 tuplas |
| Teste 3 | Merge Join sem nenhuma correspondência | 0 tuplas |
| Teste 4 | Leitura dos CSVs reais e SMJ completo | 510 tuplas |
| Teste 5 | Ordenação da tabela de Resultado pelo atributo "vinho" | 510 tuplas ordenadas  |

---

## Observações

- O wine `Springfield Estate 'Wild Yeast' Chardonnay` possui `chave_estrangeira` vazia no CSV original — por isso o resultado é **510** e não 511.
- O parser de CSV (`leitura.hpp`) trata corretamente campos entre aspas com vírgulas internas (ex: nomes de vinho como `"Bread & Butter, California"` e listas de sinônimos em `grape_synonym`).
- Caracteres acentuados podem aparecer incorretamente no terminal Windows. Para corrigir, foi adicionado `SetConsoleOutputCP(CP_UTF8)` no início do `main`.
