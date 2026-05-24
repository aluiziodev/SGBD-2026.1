#ifndef LEITURA_HPP
#define LEITURA_HPP
#include <fstream>
#include <sstream>
#include "../models/tabela.hpp"

vector<string> parseLinhaCSV(const string &linha) {
    vector<string> colunas;
    string campo = "";
    bool dentro_aspas = false;

    for(size_t i = 0; i < linha.length() ;  ++i){
        char c = linha[i];

        //Caso achemos uma aspas invertemos o valor de dentro_aspas
        if(c == '"'){
            dentro_aspas = !dentro_aspas;
        }
        else if(c == ',' && !dentro_aspas){
            colunas.push_back(campo);
            campo = "";
        }
        else{
            campo += c;
        }
    }
    colunas.push_back(campo);
    return colunas;
    
}

string trim(const string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}


//FUNÇÃO PRINCIPAL DE LEITURA DE CSV
Tabela lerCSV(const string &filename) {

    ifstream arquivo(filename);
    if (!arquivo.is_open()){
        throw runtime_error("Nao foi possivel abrir o arquivo: " + filename);
    }

    Tabela tabela;
    string linha;

    //Lê o cabeçalho 
    if (!getline(arquivo, linha)){
        throw runtime_error("Arquivo vazio: " + filename);
    }

    vector<string> nomes = parseLinhaCSV(linha);
    tabela.esquema.constroiEsquema(nomes);
    int totalcarregado = 0;

    // --- Lê as tuplas ---
    while (getline(arquivo, linha)) {
        if (linha.empty() || linha == "\r") continue;

        vector<string> vals = parseLinhaCSV(linha);
        Tupla tupla;

        for(const string &val : vals){
            tupla.cols.push_back(trim(val));
        }

        tabela.insereTupla(tupla);
        totalcarregado++;
    }

    arquivo.close();
    return tabela;
}

#endif
