#ifndef ENDERECO_H
#define ENDERECO_H
#include "palavra.hpp"
#include "consulta.hpp"
#include <sstream>

class Endereco {
    friend class BaseEnderecos;

    protected:
        // Atributos gerais
        std::string idEnd;      // Id do endereço
        int idLog;              // Id do logradouro
        std::string tipoLog;    // Tipo de logradouro
        std::string log;        // Logradouro
        int num;                // Número
        std::string bairro;     // Bairro
        std::string regiao;     // Região
        int cep;                // CEP
        double lt;              // Latitude
        double lg;              // Longitude

        // Controle de memória
        int mem_usage;

    public:
        // Construtor
        Endereco(std::string idend, int idlog, std::string tipolog, std::string log, int num, std::string bairro, std::string regiao, int cep, double lt, double lg);

        // Atribuidor
        void operator=(Endereco& other) {
            this->idEnd = other.idEnd;
            this->idLog = other.idLog;
            this->tipoLog = other.tipoLog;
            this->log = other.log;
            this->num = other.num;
            this->bairro = other.bairro;
            this->regiao = other.regiao;
            this->cep = other.cep;
            this->lt = other.lt;
            this->lg = other.lg;
            this->mem_usage = other.mem_usage;
        }
};

class BaseEnderecos {
    friend class Consulta;

    protected:
        // Controle de estado
        bool can_insert;
        bool can_search;

        // Funções auxiliares
        void SwitchAdresses(int i, int j);
        void SortAdresses();

        // Armazenamento dos endereços e do TAD de consulta
        Endereco** enderecos;
        int endereco_amount;
        Consulta consulta;

        // Armazenamento dos logradouros únicos
        Logradouro** logradouros_unicos;
        int log_amount;

        // Árvore de pesquisa de palavras

        // Memória
        void UpdateMemory();
        int current_mem_usage;
        int max_global_usage;

    public:
        // Construtor e Destrutor
        BaseEnderecos(int max_enderecos);
        ~BaseEnderecos();

        // Operações
        void InsertAddress(std::string idend, int idlog, std::string tipolog, std::string log, int num, std::string bairro, std::string regiao, int cep, double lt, double lg);
        void BuildData(std::ostream& debug);
        void Search(int idc, Point2D origin, std::string consulta, int r, std::ostream& out, std::ostream& debug);

        // Controle de memória
        int GetCurrentMemoryUsage();
        int GetMaxGlobalUsage();
};

#endif