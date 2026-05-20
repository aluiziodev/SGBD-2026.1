#ifndef TABELA_HPP
#define TABELA_HPP

#include "esquema.hpp"
#include "pagina.hpp"


struct Tabela{
    vector<Pagina> pags;
    Esquema esquema;

    Tabela(vector<Pagina> pags = vector<Pagina>(), Esquema esquema = Esquema()) :   // Construtor
            pags(pags), esquema(esquema) {} 


    int qtdPags() {  // Retorna total de páginas
        return (int)pags.size();
    }

    int qtdTotalTuplas() { // Retorna total de tuplas em todas as páginas
        int total = 0;
        for(auto &p : pags) total += p.qtd_tuplas_ocup;
        return total;
    }

    Tupla tuplaEm(int pag_idx, int tup_idx) { // Acessa a tupla na posição global    
        return pags[pag_idx].tuplas[tup_idx];
    }

    void novaPagina() { // Adiciona uma página nova 
        pags.emplace_back();
    }

    void insereTupla(Tupla &tupla) { // Insere uma tupla, criando nova página se necessário
        if(pags.empty() || pags.back().isCheia()) novaPagina();
        pags.back().inserirTupla(tupla);

    }

};


#endif