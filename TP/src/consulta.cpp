#include "consulta.hpp"
#include <iostream>

//------------------------------------------------------------------------------------------
// CONSTRUTOR
//------------------------------------------------------------------------------------------

Consulta::Consulta() {
    this->current_mem_usage = sizeof(*this);
    this->max_global_usage = this->current_mem_usage;
}

//------------------------------------------------------------------------------------------
// OPERAÇÕES
//------------------------------------------------------------------------------------------

// Insere uma palavra na árvore de pesquisa de palavras, retorna false se a palavra já existir na lista
bool Consulta::InsertWord(Palavra palavra, std::ostream& out) {
    try {
        this->palavras.Insert(palavra, std::cout);
        return true;
    }
    catch(std::logic_error& e) {
        return false;
    }
    UpdateMemory();
}

// Procura a palavra fornecida e, se exisitir, associa o logradouro a ela
bool Consulta::InsertLog(std::string palavra, Logradouro& log, std::ostream& out) {
    try {
        this->palavras.Find(palavra).AddLog(log);
        return true;
    }
    catch(std::logic_error& e) {
        out << "Palavra " << palavra << " não existe na consulta: " << e.what() << std::endl;
        return false;
    }
    catch(std::out_of_range& e) {
        out << "Não foi possível inserir o logradouro em " << palavra << ": " << e.what() << std::endl;
        return false;
    }
}

// Acha uma palavra
Palavra& Consulta::Find(std::string palavra) {
    return this->palavras.Find(palavra);
}

//------------------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//------------------------------------------------------------------------------------------

// Atualiza o uso de memória atual baseado na AVL de palavras e o máximo global, se necessário
void Consulta::UpdateMemory() {
    this->current_mem_usage = this->palavras.GetMaxMemoryUsage();

    if(this->current_mem_usage > this->max_global_usage) {
        this->max_global_usage = this->current_mem_usage;
    }
}

int Consulta::GetCurrentMemoryUsage() {
    return this->current_mem_usage;
}

int Consulta::GetMaxGlobalUsage() {
    return this->max_global_usage;
}