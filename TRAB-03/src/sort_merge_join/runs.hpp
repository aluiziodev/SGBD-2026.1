#ifndef RUNS_HPP
#define RUNS_HPP

#include "../models/tabela.hpp"
#include "../models/disco.hpp"
#include <algorithm>

#define MAX_BUFFER_FRAMES 5


vector<Tabela> gerarRuns(Tabela &tabela, int col_idx) {
    Disco disco;
    int total_pags = tabela.qtdPags();
    int inicio = 0;

    while(inicio < total_pags){
        vector<Tupla> buffer;
        int fim = min(inicio + MAX_BUFFER_FRAMES, total_pags);     

        for(int p = inicio; p < fim; p++){    // Coleta tuplas das próximas 5 páginas e insere no buffer
            Pagina &pg = tabela.pags[p];
            for(int t = 0; t < pg.qtd_tuplas_ocup; t++)
                buffer.push_back(pg.tuplas[t]);
        }  


        sort(buffer.begin(), buffer.end(),      // Ordena em memória pelo atributo de junção
            [col_idx](const Tupla& a, const Tupla& b) {
                return a.cols[col_idx] < b.cols[col_idx];
        });

                                           
        Tabela run;                            // Grava como nova run (simula escrita no disco)
        run.esquema = tabela.esquema;
        for(auto &t : buffer) run.insereTupla(t);
        disco.gravarTabela(run);
        inicio = fim;

    }

    return disco.tabelas;
    
}




#endif