#ifndef SORT_MERGE_JOIN_HPP
#define SORT_MERGE_JOIN_HPP

#include "ordenacao.hpp"
#include "merge_join.hpp"
#include "../models/tabela.hpp"


Tabela sortMergeJoin(Tabela &R, Tabela &S, const string &atr_r, const string &atr_s) {

    //Ordena as tabelas pra poder usar o merge join
    Tabela R_ord = ordernar(R, atr_r);
    Tabela S_ord = ordernar(S, atr_s);

    // Merge Join nas tabelas já ordenadas
    return mergeJoin(R_ord, S_ord, atr_r, atr_s);
}


#endif