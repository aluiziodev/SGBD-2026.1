#ifndef ORDENACAO_HPP
#define ORDENACAO_HPP

#include "intercalacao.hpp"
#include "runs.hpp"

Tabela ordernar(Tabela &entrada, string atr_juncao) {
    int col_idx = entrada.esquema.indice(atr_juncao);

    vector<Tabela> runs = gerarRuns(entrada, col_idx);


    while (runs.size() > 1){
        runs = intercala(runs, col_idx, entrada.esquema);
    }

    Tabela resultado = runs[0];
    resultado.esquema = entrada.esquema;
    return resultado;
}



#endif