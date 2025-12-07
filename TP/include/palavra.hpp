#ifndef PALAVRA_H
#define PALAVRA_H
#include "logradouro.hpp"

const static int MAX_LOGS = 200;

class Palavra {
    private:
        std::string palavra;
        Logradouro* logs[MAX_LOGS];
        int log_amount;

    public:
        // Construtores
        Palavra(std::string palavra);
        Palavra(Palavra& other);

        // Operações
        void operator=(Palavra& other);
        std::string GetWord();
        Logradouro* GetLog(int i);
        int GetLogAmount();
        void AddLog(Logradouro& log);
        void SortLogs();
};

#endif