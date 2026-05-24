#ifndef INTERCALACAO_HPP
#define INTERCALACAO_HPP

#include "runs.hpp"

#define MAX_BUFFERS_ENTRADA 4
#define MAX_BUFFERS_SAIDA 1


vector<Tabela> intercala(vector<Tabela> &runs_entrada,   
    const int col_idx, const Esquema &esquema){
    Disco disco; // Simula o disco para ler as runs de entrada e escrever as runs de saída  

    for (int i = 0; i < (int)runs_entrada.size(); i+= MAX_BUFFERS_ENTRADA){
        int fim = min(i + MAX_BUFFERS_ENTRADA, (int)runs_entrada.size());

        vector<int> indices(fim - i, 0); // Índices de leitura para cada run de entrada

        Tabela run_saida; // Frame de saída 
        run_saida.esquema = esquema;

        Pagina pagina_saida; // Página atual do frame de saída            

        while (true){
            int menor_idx = -1;
            Tupla menor_tupla;

            
            for (int j = i; j < fim; j++){     // Encontrar a menor tupla entre as runs de entrada
                int local = j - i;
                if (indices[local] < runs_entrada[j].qtdTotalTuplas()){
                    Tupla tupla_atual = runs_entrada[j].tuplaEm(indices[local]);
                    if (menor_idx == -1 || tupla_atual.cols[col_idx] < menor_tupla.cols[col_idx]){
                        menor_idx = j;
                        menor_tupla = tupla_atual;
                    }
                }
            }

            if (menor_idx == -1) break; // Todas as runs de entrada foram processadas

            pagina_saida.inserirTupla(menor_tupla); // Inserir a menor tupla na página de saída
            indices[menor_idx - i]++; // Avança o índice da run de onde a tupla foi retirada


            if (pagina_saida.isCheia()){ // Se a página de saída estiver cheia, grava no disco e inicia nova página
                run_saida.pags.push_back(pagina_saida);
                pagina_saida.limparPagina();
            }   
            
        }


        // Grava a última run de saída se tiver tuplas restantes
        if (!pagina_saida.isVazia()) run_saida.pags.push_back(pagina_saida);
            
        disco.gravarTabela(run_saida); // Grava a run de saída no disco (simula escrita no disco)

    }

    return disco.tabelas; // Retorna as runs de saída gravadas no disco
}



#endif