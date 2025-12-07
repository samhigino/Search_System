#ifndef CONSULTA_H
#define CONSULTA_H
#include "pesquisa_palavra.hpp"

class Consulta {
    private:
        // Atributos
        BasePalavras palavras;

        // Controle de memória
        void UpdateMemory();
        int current_mem_usage;
        int max_global_usage;

    public:
        // Construtores
        Consulta();

        // Operações
        bool InsertWord(Palavra palavra, std::ostream& out);
        bool InsertLog(std::string palavra, Logradouro& log, std::ostream& out);
        Palavra& Find(std::string palavra, std::ostream& out);
        
        // Controle de memória
        int GetCurrentMemoryUsage();
        int GetMaxGlobalUsage();
};

#endif