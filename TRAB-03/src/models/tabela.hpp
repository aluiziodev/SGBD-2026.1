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

    Tupla tuplaEm(int global_idx) { // Acessa a tupla na posição global    
        //Recebe o indice global da tupla (considerando todas as páginas)
        int restante = global_idx;
        for (auto &p : pags) {
            //Verifica se a tupla está na pag atual (Temos que ver a qtd de tuplas ocupadas na página)
            if (restante < p.qtd_tuplas_ocup)
                return p.tuplas[restante];
            restante -= p.qtd_tuplas_ocup;
        }
        throw out_of_range("Índice fora dos limites");
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