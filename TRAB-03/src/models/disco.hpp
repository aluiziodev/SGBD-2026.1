#ifndef DISCO_HPP
#define DISCO_HPP

#include "tabela.hpp"


struct Disco{
    vector<Tabela> tabelas;

    Disco(vector<Tabela> tabelas = vector<Tabela>()) : tabelas(tabelas) {}  // Construtor

    void gravarTabela(Tabela &tabela){ // Grava tabela no disco (simulado por um vetor)
        tabelas.push_back(tabela);
    }
};


#endif