#include "palavra.hpp"

// Construtor
Palavra::Palavra(std::string palavra) {
    this->palavra = palavra;
    this->log_amount = 0;

    for(int i = 0; i < MAX_LOGS; i++) {
        this->logs[i] = nullptr;
    }
}

// Construtor de cópia
Palavra::Palavra(Palavra& other) {
    this->palavra = other.GetWord();
    this->log_amount = other.log_amount;

    for(int i = 0; i < MAX_LOGS; i++) {
        if(i < other.log_amount) this->logs[i] = other.GetLog(i);
        else this->logs[i] = nullptr;
    }
}

// Sobrecarga de atribuição para cópias
void Palavra::operator=(Palavra& other) {
    this->palavra = other.GetWord();
    this->log_amount = other.log_amount;

    for(int i = 0; i < MAX_LOGS; i++) {
        if(i < other.log_amount) this->logs[i] = other.GetLog(i);
        else this->logs[i] = nullptr;
    }
}

// Retorna a palavra em si
std::string Palavra::GetWord() {
    return this->palavra;
}

// Retorna o Logradouro associado na posição i do vetor, caso exista
Logradouro* Palavra::GetLog(int i) {
    if(i >= this->log_amount || i < 0) {
        throw std::out_of_range("Tentando coletar um logradouro inexistente.");
    }
    else return this->logs[i];
}

// Retorna a quantidade de logradouros associados (popularidade da palavra)
int Palavra::GetLogAmount() {
    return this->log_amount;
}

// Adiciona um logradouro à lista de logradouros associados
void Palavra::AddLog(Logradouro& log) {
    if(log_amount == MAX_LOGS - 1) {
        throw std::out_of_range("Numero maximo de logradouros atingido nessa palavra.");
    }
    this->logs[this->log_amount] = &log;
    this->log_amount++;
}

// Organiza os logradouros no vetor por id
void Palavra::SortLogs() {
    
}