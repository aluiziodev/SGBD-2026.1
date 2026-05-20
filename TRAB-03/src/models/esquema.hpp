#ifndef ESQUEMA_HPP
#define ESQUEMA_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;



struct Esquema{
    int qtd_cols;
    unordered_map<string, int> nome_para_indice;

    Esquema(int qtd_cols=0, unordered_map<string,                       // Construtor 
            int> nome_para_indice=unordered_map<string, int>()) : 
        qtd_cols(qtd_cols), nome_para_indice(nome_para_indice) {}

    int indice(const string nome){ // Retorna índice da coluna a partir do nome
        auto it = nome_para_indice.find(nome);
        if(it == nome_para_indice.end())
            throw runtime_error("Coluna nao encontrada: " + nome);
        return it->second;
    }

    void limpaEsquema(){ // Limpa o esquema 
        nome_para_indice.clear();
        qtd_cols = 0;
    }

    void constroiEsquema(vector<string> &nomes){ // Constrói esquema a partir da lista de nomes
        limpaEsquema();
        qtd_cols = (int)nomes.size();
        for(int i = 0; i < (int)nomes.size(); i++){ 
            nome_para_indice[nomes[i]] = i;
        }
    }


};


#endif