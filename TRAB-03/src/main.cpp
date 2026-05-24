#include <iostream>
#include <iomanip>
#ifdef _WIN32
    #include <windows.h>   // só no Windows
#endif

#include "utils/leitura.hpp"
#include "sort_merge_join/sort_merge_join.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

using namespace std;


// Imprime as colunas (nomes) de um esquema na ordem correta 
void imprimirCabecalho(const Esquema &esq) {
    vector<string> nomes(esq.qtd_cols);
    for (auto &[nome, idx] : esq.nome_para_indice)
        nomes[idx] = nome;
    for (int i = 0; i < (int)nomes.size(); i++) {
        if (i) cout << " | ";
        string n = nomes[i].substr(0, 20);
        cout <<YELLOW  << setw(20) << left << nomes[i] << RESET;
    }
    cout << "\n" << string(esq.qtd_cols * 23, '-') << "\n";
}

/* Imprime todas as tuplas de uma tabela */
void imprimirTabela(Tabela &tab) {
    imprimirCabecalho(tab.esquema);
    for (int i = 0; i < tab.qtdTotalTuplas(); i++) {
        Tupla t = tab.tuplaEm(i);
        for (int c = 0; c < (int)t.cols.size(); c++) {
            if (c) cout << " | ";
            string val = t.cols[c];
            cout << setw(20) << left <<  val ;
        }
        cout << "\n";
    }
    cout << "\nTotal: " << tab.qtdTotalTuplas() << " tuplas em " << tab.qtdPags() << " pagina(s)\n\n";
}

void imprimirTabelaLimitado(Tabela &tab, int limite) {
    imprimirCabecalho(tab.esquema);
    for (int i = 0; i < min(limite, tab.qtdTotalTuplas()); i++) {
        Tupla t = tab.tuplaEm(i);
        for (int c = 0; c < (int)t.cols.size(); c++) {
            if (c) cout << " | ";
            string val = t.cols[c];
            if ((int)val.size() > 20) val = val.substr(0, 17) + "...";  //Pelos nomes longos, preferimos simplificar e deixar so 20 caracteres na exibição pra facilitar a leitura
            cout << setw(20) << left << val;
        }
        cout << "\n";
    }
    cout << "\nTotal: " << tab.qtdTotalTuplas() << " tuplas em " << tab.qtdPags() << " pagina(s)\n\n";
}

/* Monta uma Tabela manualmente a partir de vetores (para testes sem CSV) */
Tabela montarTabela(vector<string> colunas, vector<vector<string>> linhas) {
    Esquema esq;
    esq.constroiEsquema(colunas);
    Tabela tab;
    tab.esquema = esq;
    for (auto &l : linhas) {
        Tupla t(l);
        tab.insereTupla(t);
    }
    return tab;
}

// ─── Testes ─────────────────────────────────────────────────────────────────

// Teste 1 — Ordenação externa

void testeOrdenacao() {
    cout << BLUE << "\n========== TESTE 1: Ordenacao Externa ==========\n" << RESET;

    Tabela tab = montarTabela(
        {"id", "nome"},
        {}  // preenchido abaixo
    );
    for (int i = 70; i >= 1; i--) {
        Tupla t({to_string(i), "item" + to_string(i)});
        tab.insereTupla(t);
    }

    cout << "Antes: " << tab.qtdPags() << " paginas, "
         << tab.qtdTotalTuplas() << " tuplas\n";

    Tabela ord = ordernar(tab, "id");

    cout << "Apos ordenacao: " << ord.qtdPags() << " paginas\n";

    // Verificamos se realmente ordenou todos as tuplas
    bool ok = true;
    for (int i = 1; i < ord.qtdTotalTuplas(); i++) {
        if (ord.tuplaEm(i-1).cols[0] > ord.tuplaEm(i).cols[0]) {
            ok = false;
            break;
        }
    }

    if(ok){
        cout << GREEN << "[FUNCIONOU] Tabela ordenada corretamente\n" << RESET;
    }
    else{
        cout << RED << "[FALHOU] Tabela nao ordenada corretamente\n" << RESET;
    }
}

// Teste 2 — Merge Join simples (sem duplicatas)
void testeMergeJoinSimples() {
    cout << BLUE << "\n========== TESTE 2: Merge Join Simples ==========\n" << RESET;

    Tabela grapes = montarTabela(
        {"chave_primaria", "grape_synonym", "grape_country"},
        {
            {"1", "Cabernet",  "France"},
            {"2", "Merlot",    "France"},
            {"3", "Malbec",    "Argentina"},
            {"4", "Chardonnay","France"},
        }
    );

    Tabela wines = montarTabela(
        {"vinho", "chave_estrangeira", "origem", "tipo", "estilo"},
        {
            {"Vinho B", "2", "France",    "Tinto",  "Seco"},
            {"Vinho A", "1", "France",    "Tinto",  "Seco"},
            {"Vinho C", "3", "Argentina", "Tinto",  "Encorpado"},
            {"Vinho D", "4", "France",    "Branco", "Seco"},
        }
    );

    Tabela resultado = sortMergeJoin(wines, grapes, "chave_estrangeira", "chave_primaria");

    cout << "Resultado (" << resultado.qtdTotalTuplas() << " tuplas):\n";
    imprimirTabela(resultado);

    // Verifica número de tuplas esperado
    bool ok = (resultado.qtdTotalTuplas() == 4);

    if(ok){
        cout << GREEN << "[FUNCIONOU] 4 Tuplas retornadas:\n" << RESET;
    }
    else{
        cout << RED << "[FALHOU] Tuplas retornadas incorreta:\n" << RESET;
    }
}

// Teste 3 — Merge Join sem correspondência entre tabelas

void testeMergeJoinSemMatch() {
    cout << BLUE << "\n======= TESTE 3: Merge Join Sem Correspondencia de nenhuma tupla =====\n" << RESET;

    //CRIANDO 2 TABELAS QUE NAO TERAM CORRESPONDENCIA PARA TESTAR ESSE CASO
    Tabela wines = montarTabela(
        {"vinho", "chave_estrangeira", "origem", "tipo", "estilo"},
        {{"Vinho X", "99", "Brasil", "Rose", "Seco"}}
    );

    Tabela grapes = montarTabela(
        {"chave_primaria", "grape_synonym", "grape_country"},
        {{"1", "Cabernet", "France"}}
    );

    Tabela resultado = sortMergeJoin(wines, grapes, "chave_estrangeira", "chave_primaria");

    //Testa se o resultado está vazio (0 tuplas) e informa
    bool ok = (resultado.qtdTotalTuplas() == 0);
    if(ok) {
        cout << GREEN << "[FUNCIONOU] 0 tuplas retornadas (sem correspondencia)\n" << RESET;
    } else {
        cout<< RED << "[FALHOU] " << resultado.qtdTotalTuplas() << " tuplas encontradas (esperado 0)\n" << RESET;
    }
}

//Teste 4 — TESTE COM O CSV
void testeCSV() {
    cout << BLUE << "\n========== TESTE 4: Leitura de CSV ==========\n" << RESET;
    try {
        Tabela wines  = lerCSV("src/data/wines.csv");
        Tabela grapes = lerCSV("src/data/grapes.csv");

        cout << "Wines:  " << wines.qtdTotalTuplas()  << " tuplas, " << wines.qtdPags()  << " pagina(s)\n";
        cout << "Grapes: " << grapes.qtdTotalTuplas() << " tuplas, " << grapes.qtdPags() << " pagina(s)\n";

        Tabela resultado = sortMergeJoin(wines, grapes, "chave_estrangeira", "chave_primaria");

        int limite = 10;

        cout << "Resultado: " << resultado.qtdTotalTuplas() << " tuplas\n";
        cout << "Primeiras " << limite << " tuplas:\n\n";
        
        imprimirTabelaLimitado(resultado, limite);
        cout << GREEN << "\n\n[FUNCIONOU] CSV carregado e join executado\n" << RESET;

        //TESTE 5 - ORDENANDO A TABELA RESULTADO PELO ATRIBUTO "vinho"
        cout << BLUE << "\n========== TESTE 5: Ordenacao do resultado do join por 'vinho' ==========" << RESET;
        Tabela resultado_ordenado = ordernar(resultado, "vinho");
        cout << "\n\nResultado ordenado por 'vinho':\n\n";
        imprimirTabelaLimitado(resultado_ordenado, limite);

        cout << GREEN << "\n\n[FUNCIONOU] Resultado do join ordenado por 'vinho'\n" << RESET;

    } catch (exception &e) {
        cout << RED << "[ERRO] " << e.what() << RESET << endl;
    }
}

// ─── Main ───────────────────────────────────────────────────────────────────

int main() {
    //Configura a saída para aparecer os acentos corretamente
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); 
    #else
        setlocale(LC_ALL, "");
    #endif

    cout << "===== Sort-Merge Join =====\n";
    cout << "Buffer: B=5 frames | Pagina: 12 tuplas\n";

    testeOrdenacao();
    testeMergeJoinSimples();
    testeMergeJoinSemMatch();
    testeCSV();

    cout << "\n===== Fim dos testes =====\n\n";
    return 0;
}
