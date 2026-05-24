#ifndef MERGE_JOIN_HPP
#define MERGE_JOIN_HPP
#include "ordenacao.hpp"
#include "../models/tabela.hpp"

Tabela mergeJoin(Tabela &R, Tabela &S, const string &atr_r, const string &atr_s){
    //Usando o esquema da tabela, obtemos os indices
    int col_r = R.esquema.indice(atr_r);
    int col_s = S.esquema.indice(atr_s);

    // Monta o esquema do resultado (R join S)
    vector<string> nomes_r(R.esquema.qtd_cols);
    for (auto &[nome, idx] : R.esquema.nome_para_indice)
        nomes_r[idx] = nome;

    vector<string> nomes_s(S.esquema.qtd_cols);
    for (auto &[nome, idx] : S.esquema.nome_para_indice)
        nomes_s[idx] = nome;

    vector<string> nomes_resultado = nomes_r;
    nomes_resultado.insert(nomes_resultado.end(), nomes_s.begin(), nomes_s.end());

    Esquema esquema_resultado;
    esquema_resultado.constroiEsquema(nomes_resultado);

    Tabela resultado;
    resultado.esquema = esquema_resultado;

    //FAZENDO O MERGE DAS TABELAS
    int total_r = R.qtdTotalTuplas();
    int total_s = S.qtdTotalTuplas();
    int i = 0, j = 0;

    while (i < total_r && j < total_s){
        Tupla tr = R.tuplaEm(i);
        Tupla ts = S.tuplaEm(j);

        string val_r = tr.cols[col_r];
        string val_s = ts.cols[col_s];

        if (val_r == val_s) {
            // Identifica o grupo inteiro com o mesmo valor em R 
            int i_fim = i;
            while (i_fim < total_r && R.tuplaEm(i_fim).cols[col_r] == val_r)
                i_fim++;

            // Identifica o grupo inteiro com o mesmo valor em S
            int j_fim = j;
            while (j_fim < total_s && S.tuplaEm(j_fim).cols[col_s] == val_s)
                j_fim++;

            // Produto cartesiano entre os dois grupos 
            for (int ii = i; ii < i_fim; ii++) {
                for (int jj = j; jj < j_fim; jj++) {
                    Tupla tii = R.tuplaEm(ii);
                    Tupla tjj = S.tuplaEm(jj);
                    Tupla joined = tii.concatenar(tjj);
                    resultado.insereTupla(joined);
                }
            }

            // Avança ambos os ponteiros para depois dos grupos
            i = i_fim;
            j = j_fim;

        } else if (val_r < val_s) {
            i++; // R está atrás — avança R
        } else {
            j++; // S está atrás — avança S
        }
    }

    return resultado;
}

Tabela sortMergeJoin(Tabela &R, Tabela &S, const string &atr_r, const string &atr_s) {

    //Ordena as tabelas pra poder usar o merge join
    Tabela R_ord = ordernar(R, atr_r);
    Tabela S_ord = ordernar(S, atr_s);

    // Merge Join nas tabelas já ordenadas
    return mergeJoin(R_ord, S_ord, atr_r, atr_s);
}

#endif