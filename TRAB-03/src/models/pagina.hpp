#ifndef PAGINA_HPP
#define PAGINA_HPP


#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;
#define MAX_TUPLAS_PAG 12 


struct Tupla{
    vector<string> cols;

    Tupla(vector<string> cols=vector<string>()) : cols(cols) {}   // Construtor 

    Tupla concatenar(const Tupla &tupla2) {  // Concatena duas tuplas
        vector<string> nova_cols = cols;
        nova_cols.insert(nova_cols.end(), tupla2.cols.begin(), tupla2.cols.end());
        return Tupla(nova_cols);
    }
};

struct Pagina{
    Tupla tuplas[MAX_TUPLAS_PAG];
    int qtd_tuplas_ocup;

    Pagina(int qtd_tuplas_ocup=0) : qtd_tuplas_ocup(qtd_tuplas_ocup) {}  // Construtor 


    bool isCheia() {  // Verifica se a página está cheia
        return qtd_tuplas_ocup == MAX_TUPLAS_PAG;
    }

    bool isVazia() {  // Verifica se a página está vazia 
        return qtd_tuplas_ocup == 0;
    }

    void inserirTupla(const Tupla &tupla){  // Insere uma tupla na página 
        if(isCheia()) throw overflow_error("Pagina cheia");
        tuplas[qtd_tuplas_ocup++] = tupla;
    }
    
    void limparPagina(){  // Limpa a página (zera a quantidade de tuplas ocupadas)
        qtd_tuplas_ocup = 0;
    }


};





#endif