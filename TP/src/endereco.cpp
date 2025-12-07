#include "endereco.hpp"

//------------------------------------------------------------------------------------------
// CLASSE ENDERECO
//------------------------------------------------------------------------------------------

// Construtor
Endereco::Endereco(std::string idend, int idlog, std::string tipolog, std::string log, int num, std::string bairro, std::string regiao, int cep, double lt, double lg) {
    this->idEnd = idend;
    this->idLog = idlog;
    this->tipoLog = tipolog;
    this->log = log;
    this->num = num;
    this->bairro = bairro;
    this->regiao = regiao;
    this->cep = cep;
    this->lt = lt;
    this->lg = lg;

    this->mem_usage = sizeof(*this);
}

//------------------------------------------------------------------------------------------
// CLASSE BASE ENDERECOS - FUNÇÕES AUXILIARES
//------------------------------------------------------------------------------------------

// Comparação de strings
bool StrCompareUtil(std::string s1, std::string s2) {
    int size1 = s1.size();
    int size2 = s2.size();

    int i = 0;
    while(s1.at(i) == s2.at(i)) {
        i++;

        if(i == size1 && i < size2) {
            return false;
        }
        else if(i < size1 && i == size2) {
            return true;
        }
        else if(i == size1 && i == size2) {
            throw std::logic_error("Comparando strings iguais.");
        }
    }

    return s1.at(i) > s2.at(i);
}

// Comparação de igualdade de strings
bool StrEqual(std::string s1, std::string s2) {
    int size1 = s1.size();
    int size2 = s2.size();
    if(size1 != size2) return false;

    for(int i = 0; i < size1; i++) {
        if(s1.at(i) != s2.at(i)) {
            return false;
        }
    }

    return true;
}

// SwitchAdresses: troca dois endereços na lista
void BaseEnderecos::SwitchAdresses(int i, int j) {
    Endereco* temp = this->enderecos[i];
    current_mem_usage += temp->mem_usage;
    UpdateMemory();
    this->enderecos[i] = this->enderecos[j];
    this->enderecos[j] = temp;
    current_mem_usage -= temp->mem_usage;
}

// SortAdresses: ordena os endereços por ordem de tipoLog, log, num (insertion sort)
void BaseEnderecos::SortAdresses() {
    for(int i = endereco_amount - 1; i >= 0; i--) {
        int prev = i - 1;
        try {
            // Comparação de tipoLog
            bool eh_depois = StrCompareUtil(enderecos[i]->tipoLog, enderecos[prev]->tipoLog);
            if(!eh_depois) {
                SwitchAdresses(i, prev);
                continue;
            }
            else break;
        }

        catch(std::logic_error& e) {
            try {
                // Mesmos tipoLog, comparar log
                bool eh_depois = StrCompareUtil(enderecos[i]->log, enderecos[prev]->log);
                if(!eh_depois) {
                    SwitchAdresses(i, prev);
                    continue;
                }
                else break;
            }
            
            catch(std::logic_error& e) {
                // Mesmos log, comparar num
                if(enderecos[i]->num < enderecos[prev]->num) {
                    SwitchAdresses(i, prev);
                    continue;
                }
                else if(enderecos[i]->num > enderecos[prev]->num) {
                    break;
                }
                else throw std::logic_error("Endereços idênticos encontrados durante a ordenação.");
            }
        }
    }
}

//------------------------------------------------------------------------------------------
// CLASSE BASE ENDERECOS - CONSTRUTOR E DESTRUTOR
//------------------------------------------------------------------------------------------

// Construtor
BaseEnderecos::BaseEnderecos(int max_enderecos) {
    // Controle de estado
    this->can_insert = true;
    this->can_search = false;

    // Inicialização da lista de endereços e do TAD consulta associado
    this->enderecos = new Endereco*[max_enderecos];
    this->endereco_amount = 0;

    // Inicialização da lista de logradouros únicos
    this->logradouros_unicos = new Logradouro*[max_enderecos];
    this->log_amount = 0;

    this->current_mem_usage = sizeof(*this) + (max_enderecos * sizeof(Endereco*)) + (max_enderecos * sizeof(Logradouro*));
    this->max_global_usage = this->current_mem_usage;
}

// Destrutor
BaseEnderecos::~BaseEnderecos() {
    // Deletando endereços
    for(int i = 0; i < this->endereco_amount; i++) {
        delete this->enderecos[i];
    }
    delete[] this->enderecos;

    // Deletando logradouros
    for(int i = 0; i < this->log_amount; i++) {
        delete this->logradouros_unicos[i];
    }
    delete[] this->logradouros_unicos;
}

//------------------------------------------------------------------------------------------
// CLASSE BASE ENDERECOS - OPERAÇÕES
//------------------------------------------------------------------------------------------

// InsertAdress: Insere um novo endereço na base de endereços - O(n) devido ao insertionsort
void BaseEnderecos::InsertAddress(std::string idend, int idlog, std::string tipolog, std::string log, int num, std::string bairro, std::string regiao, int cep, double lt, double lg) {
    // Se não estiver no estado de coleta de endereços, lançar exceção
    if(can_insert == false) {
        throw std::logic_error("Nao podemos inserir agora.");
    }

    // Criação do endereço e organização do vetor
    Endereco* new_endereco = new Endereco(idend, idlog, tipolog, log, num, bairro, regiao, cep, lt, lg);
    this->enderecos[this->endereco_amount] = new_endereco;
    this->endereco_amount++;

    // Atualização do uso de memória
    current_mem_usage += new_endereco->mem_usage;
    UpdateMemory();

    // Organização dos endereços
    SortAdresses();
}

// BuildData: Construir estruturas de dados auxiliares (logradouros únicos, árvore de palavras, etc)
void BaseEnderecos::BuildData(std::ostream& debug) {
    int i = 0;                                              // Índice para percorrer os endereços
    std::string current_log = this->enderecos[0]->log;      // Nome do logradouro atualmente avaliado
    Endereco* current_end;                                  // Endereço atualmente avaliado

    // Update de memória
    this->current_mem_usage += sizeof(Endereco*);
    UpdateMemory();

    // Percorrendo a lista de endereços para criar os logradouros únicos e palavras únicas
    while(i < this->endereco_amount) {
        current_end = this->enderecos[i];

        // Primeiro endereço - trivial
        if(i == 0) {
            // Criação do logradouro
            Logradouro* new_log = new Logradouro(current_end->idLog, current_end->log);
            this->logradouros_unicos[log_amount] = new_log;
            this->log_amount++;
            int m = new_log->InsertAdress(current_end->num, current_end->lt, current_end->lg);

            // Criação das palavras
            std::stringstream tlog(current_end->tipoLog);
            std::stringstream log(current_end->log);
            std::string w;
            bool inseriu;

            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "Inseriu " << w << std::endl;
                }
                else {
                    debug << "Palavra " << w << " já consta" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }
            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "Inseriu " << w << std::endl;
                }
                else {
                    debug << "Palavra " << w << " já consta" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }

            // Update de memória
            this->current_mem_usage += new_log->GetMemoryUsage() + m + sizeof(tlog) + sizeof(log) + sizeof(bool);
            UpdateMemory();
            this->current_mem_usage -= (m + sizeof(tlog) + sizeof(log) + sizeof(bool));

            i++;
            continue;
        }

        // Endereços restantes
        // Se o logradouro mudou, criar um novo logradouro
        if(!StrEqual(this->enderecos[i]->log, current_log)) {
            // Atualização do current_log
            current_log = this->enderecos[i]->log;

            // Criação do logradouro
            Logradouro* new_log = new Logradouro(current_end->idLog, current_end->log);
            this->logradouros_unicos[log_amount] = new_log;
            this->log_amount++;
            int m = new_log->InsertAdress(current_end->num, current_end->lt, current_end->lg);

            // Update de memória
            this->current_mem_usage += new_log->GetMemoryUsage() + m;
            UpdateMemory();
            this->current_mem_usage -= m;

            // Criação das palavras
            std::stringstream tlog(current_end->tipoLog);
            std::stringstream log(current_end->log);
            std::string w;
            bool inseriu;

            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "Inseriu " << w << std::endl;
                }
                else {
                    debug << "Palavra " << w << " já consta" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }
            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "Inseriu " << w << std::endl;
                }
                else {
                    debug << "Palavra " << w << " já consta" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }

            // Update de memória
            this->current_mem_usage += new_log->GetMemoryUsage() + m + sizeof(tlog) + sizeof(log) + sizeof(bool);
            UpdateMemory();
            this->current_mem_usage -= (m + sizeof(tlog) + sizeof(log) + sizeof(bool));

            i++;
            continue;
        }

        // Se não mudou, apenas adicionar esse novo número no logradouro existente
        else {
            Logradouro* log = this->logradouros_unicos[log_amount - 1];
            int m = log->InsertAdress(current_end->num, current_end->lt, current_end->lg);
            
            // Update de memória
            this->current_mem_usage += sizeof(Logradouro*) + m;
            UpdateMemory();
            this->current_mem_usage -= m;
        }

        i++;
    }

    // Calculando o centro de gravidade de cada logradouro
    for(int i = 0; i < log_amount; i++) {
        this->logradouros_unicos[i]->CalculateCenter();
    }

    // Update de memória
    this->current_mem_usage -= sizeof(Endereco*);

    // Atualização de estado
    this->can_insert = false;
    this->can_search = true;
}

// Search: realiza uma consulta por logradouro e exibe o resultado em out
void BaseEnderecos::Search(int idc, Point2D origin, std::string consulta, int r, std::ostream& out, std::ostream& debug) {
    // Se não estiver no estado de pesquisa, lançar exceção
    if(!can_search) {
        throw std::logic_error("Nao podemos pesquisar agora.");
    }

    // Inicio da consulta
    std::stringstream cons(consulta);                       // Stringstream da linha de consulta
    std::string w;                                          // Palavra avaliada na consulta
    Palavra* p;                                             // Ponteiro para o TAD Palavra associado a palavra da consulta

    // Primeira palavra
    std::getline(cons, w, ' ');
    try {
        p = &this->consulta.Find(w, debug);
    }
    catch(std::logic_error& e) {
        debug << "Não foi possível encontrar a palavra " << w << ": " << e.what() << std::endl;
        return;
    }

    // Criação do vetor inicial de resultados
    int max_results = p->GetLogAmount();                    // Quantidade máxima possível de resultados
    int result_amount = max_results;                        // Quantidade de resultados
    Logradouro* results[max_results];
    for(int i = 0; i < max_results; i++) {
        results[i] = p->GetLog(i);
    }
    this->current_mem_usage += max_results * sizeof(Logradouro*) + 2*sizeof(int);
    UpdateMemory();

    // Restante das palavras - achar interseção dos logradouros
    while(std::getline(cons, w, ' ')) {
        try {
            p = &this->consulta.Find(w, debug);
        }
        catch(std::logic_error& e) {
            debug << "Não foi possível encontrar a palavra " << w << ": " << e.what() << std::endl;
            return;
        }
        for(int i = 0; i < max_results; i++) {
            if(results[i] == nullptr) continue;

            bool is_intersection = false;

            for(int j = 0; j < p->GetLogAmount(); j++) {
                if(p->GetLog(j) == results[i]) {
                    is_intersection = true;
                    break;
                }
            }

            if(!is_intersection) {
                results[i] = nullptr;
                result_amount--;
            }
        }
    }

    // Coletar o vetor de resultados verdadeiros
    Logradouro* results_def[result_amount];
    this->current_mem_usage += result_amount * sizeof(Logradouro*);
    UpdateMemory();

    int k = 0;
    for(int i = 0; i < max_results; i++) {
        if(results[i] == nullptr) continue;
        else {
            results_def[k] = results[i];
            results_def[k]->CalculateDelta(origin);
            for(int j = k - 1; j >= 0; j++) {
                if(results_def[j + 1]->GetDelta() < results[j]->GetDelta()) {
                    Logradouro* temp = results_def[j + 1];
                    results_def[j + 1] = results[j];
                    results[j] = temp;
                }
            }
            k++;
        }
    }

    if(k != result_amount) throw std::logic_error("k != result_amount");
    

    // Imprimir resultado da consulta
    int count = 0;
    out << idc << ";" << result_amount << std::endl;
    for(int i = 0; i < result_amount; i++) {
        out << results_def[i]->GetId() << ";" << results_def[i]->GetName() << std::endl;
    }

    // Finalização
    this->current_mem_usage -= (max_results * sizeof(Logradouro*) + result_amount * sizeof(Logradouro*) + 2*sizeof(int));
}

//------------------------------------------------------------------------------------------
// CLASSE BASE ENDERECOS - CONTROLE DE MEMÓRIA
//------------------------------------------------------------------------------------------

void BaseEnderecos::UpdateMemory() {
    if(this->current_mem_usage > this->max_global_usage) {
        this->max_global_usage = this->current_mem_usage;
    }
}

int BaseEnderecos::GetCurrentMemoryUsage() {
    return this->current_mem_usage;
}

int BaseEnderecos::GetMaxGlobalUsage() {
    return this->max_global_usage;
}
