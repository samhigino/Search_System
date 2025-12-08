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

// Mínimo entre inteiros
int min(int a, int b) {
    if(a < b) return a;
    else return b;
}

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

// SortAdresses: ordena os endereços por ordem de tipoLog, log, idLog, num, idEnd (insertion sort)
void BaseEnderecos::SortAdresses() {
    for(int i = endereco_amount - 1; i > 0; i--) {
        int prev = i - 1;
        if(prev < 0) return;

        // Primeira comparação: idLog
        if(enderecos[i]->idLog > enderecos[prev]->idLog) {
            SwitchAdresses(i, prev);
            continue;
        }
        else if(enderecos[i]->idLog < enderecos[prev]->idLog) {
            continue;
        }
        else {
            try {
                // Mesmo idLog, comparar tipoLog
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
                    // Mesmos log, comparar idLog
                    // if(enderecos[i]->idLog > enderecos[prev]->idLog) {
                    //     SwitchAdresses(i, prev);
                    //     continue;
                    // }
                    // else if(enderecos[i]->idLog < enderecos[prev]->idLog) {
                    //     continue;
                    // }
                    // else {
                        // Mesmos idLog, comparar num
                        if(enderecos[i]->num < enderecos[prev]->num) {
                            SwitchAdresses(i, prev);
                            continue;
                        }
                        else if(enderecos[i]->num > enderecos[prev]->num) {
                            break;
                        }
                        else {
                            // Mesmos num, comparar idEnd
                            bool eh_depois = StrCompareUtil(enderecos[i]->idEnd, enderecos[prev]->idEnd);
                            if(!eh_depois) {
                                SwitchAdresses(i, prev);
                                continue;
                            }
                            else break;
                        }
                    // }
                }
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
void BaseEnderecos::InsertAddress(std::string idend, int idlog, std::string tipolog, std::string log, int num, std::string bairro, std::string regiao, int cep, double lt, double lg, std::ostream& debug) {
    // Se não estiver no estado de coleta de endereços, lançar exceção
    if(can_insert == false) {
        throw std::logic_error("Nao podemos inserir agora.");
    }

    // Criação do endereço e organização do vetor
    debug << "\tCreating new address...\n";
    debug.flush();
    Endereco* new_endereco = new Endereco(idend, idlog, tipolog, log, num, bairro, regiao, cep, lt, lg);
    this->enderecos[this->endereco_amount] = new_endereco;
    this->endereco_amount++;
    debug << "\tAddress created and inserted in position " << this->endereco_amount - 1 << ".\n";
    debug.flush();

    // Atualização do uso de memória
    current_mem_usage += new_endereco->mem_usage;
    UpdateMemory();

    // Organização dos endereços
    debug << "\tSorting addresses...\n";
    debug.flush();
    SortAdresses();
    debug << "\tAddresses sorted.\n";
    debug.flush();
}

// BuildData: Construir estruturas de dados auxiliares (logradouros únicos, árvore de palavras, etc)
void BaseEnderecos::BuildData(std::ostream& debug) {
    // Depuração
    debug << "\tBuilding auxiliary data structures...\n";
    debug.flush();

    // Auxiliares
    int i = 0;                                              // Índice para percorrer os endereços
    Endereco* current_log = this->enderecos[0];             // Logradouro atualmente avaliado associado a um endereço
    Endereco* current_end;                                  // Endereço atualmente avaliado

    // Update de memória
    this->current_mem_usage += sizeof(Endereco*);
    UpdateMemory();

    // Percorrendo a lista de endereços para criar os logradouros únicos e palavras únicas
    while(i < this->endereco_amount) {
        current_end = this->enderecos[i];
        debug << "\t\tNow evaluating address " << current_end->idEnd << ": " << current_end->tipoLog << " " << current_end->log << " " << current_end->idLog << ", " << current_end->num << "\n";
        debug.flush();

        // Primeiro endereço - trivial
        if(i == 0) {
            debug << "\t\t\tFirst address, creating first unique logradouro...\n";
            debug.flush();
            // Criação do logradouro
            Logradouro* new_log = new Logradouro(current_end->idLog, current_end->log);
            this->logradouros_unicos[log_amount] = new_log;
            this->log_amount++;
            int m = new_log->InsertAdress(current_end->num, current_end->lt, current_end->lg);
            debug << "\t\t\tUnique logradouro created: " << new_log->GetName() << " with id " << new_log->GetId() << ".\n";
            debug.flush();

            // Criação das palavras
            std::stringstream tlog(current_end->tipoLog);
            std::stringstream log(current_end->log);
            std::string w;
            bool inseriu;
            debug << "\t\t\tInserting words into the tree...\n";
            debug.flush();

            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "\t\t\t\tInserted " << w << std::endl;
                }
                else {
                    debug << "\t\t\t\tWord " << w << " already exsts" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }
            while(std::getline(log, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "\t\t\t\tInserted " << w << std::endl;
                }
                else {
                    debug << "\t\t\t\tWord " << w << " already exists" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }

            // Update de memória
            this->current_mem_usage += new_log->GetMemoryUsage() + m + sizeof(tlog) + sizeof(log) + sizeof(bool);
            UpdateMemory();
            this->current_mem_usage -= (m + sizeof(tlog) + sizeof(log) + sizeof(bool));

            i++;
            debug << "\t\t\tFirst address processed.\n";
            debug.flush();
            continue;
        }

        // Endereços restantes
        // Se o logradouro mudou, criar um novo logradouro
        if(current_end->idLog != current_log->idLog) {
            debug << "\t\t\tLogradouro changed, creating new unique logradouro...\n";
            debug << "\t\t\tComparison: " << current_end->log << " != " << current_log->log << " or " << current_end->idLog << " != " << current_log->idLog << std::endl;
            debug.flush();

            // Atualização do current_log
            current_log = this->enderecos[i];

            // Criação do logradouro
            Logradouro* new_log = new Logradouro(current_end->idLog, current_end->log);
            this->logradouros_unicos[log_amount] = new_log;
            this->log_amount++;
            int m = new_log->InsertAdress(current_end->num, current_end->lt, current_end->lg);
            debug << "\t\t\tUnique logradouro created: " << new_log->GetName() << " with id " << new_log->GetId() << ".\n";
            debug.flush();

            // Criação das palavras
            std::stringstream tlog(current_end->tipoLog);
            std::stringstream log(current_end->log);
            std::string w;
            bool inseriu;
            debug << "\t\t\tInserting words into the tree...\n";
            debug.flush();

            while(std::getline(tlog, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "\t\t\t\tInserted " << w << std::endl;
                }
                else {
                    debug << "\t\t\t\tWord " << w << " already exsts" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }
            while(std::getline(log, w, ' ')) {
                Palavra pal(w);
                inseriu = this->consulta.InsertWord(pal, debug);
                if(inseriu) {
                    debug << "\t\t\t\tInserted " << w << std::endl;
                }
                else {
                    debug << "\t\t\t\tWord  " << w << " already exists" << std::endl;
                }
                this->consulta.InsertLog(w, *new_log, debug);
            }

            // Update de memória
            this->current_mem_usage += new_log->GetMemoryUsage() + m + sizeof(tlog) + sizeof(log) + sizeof(bool);
            UpdateMemory();
            this->current_mem_usage -= (m + sizeof(tlog) + sizeof(log) + sizeof(bool));

            i++;
            debug << "\t\t\tAddress processed.\n";
            debug.flush();
            continue;
        }

        // Se não mudou, apenas adicionar esse novo número no logradouro existente
        else {
            debug << "\t\t\tLogradouro unchanged, inserting address number into existing unique logradouro...\n";
            debug.flush();
            Logradouro* log = this->logradouros_unicos[log_amount - 1];
            int m = log->InsertAdress(current_end->num, current_end->lt, current_end->lg);
            debug << "\t\t\tAddress number " << current_end->num << " inserted into logradouro " << log->GetName() << ".\n";
            debug.flush();
            
            // Update de memória
            this->current_mem_usage += sizeof(Logradouro*) + m;
            UpdateMemory();
            this->current_mem_usage -= m;
        }

        i++;
    }

    debug << "\t\tComplete list of unique Logradouros:\n";
    for(int i = 0; i < this->log_amount; i++) {
        debug << "\t\t\t" << this->logradouros_unicos[i]->GetId() << " " << this->logradouros_unicos[i]->GetName() << std::endl;
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
    // Depuração
    debug << "\tStarting search for query \"" << consulta << "\" with origin (" << origin.GetX() << ", " << origin.GetY() << ") and id " << idc << "...\n";
    debug.flush();

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
    debug << "\t\tFirst word in query: " << w << std::endl;
    debug.flush();
    try {
        p = &(this->consulta.Find(w));
    }
    catch(std::logic_error& e) {
        debug << "\t\tCouldn't find word " << w << ": " << e.what() << std::endl;
        debug.flush();
        out << idc << ";" << 0 << std::endl;
        return;
    }

    // Criação do vetor inicial de resultados
    debug << "\t\tLocated word. Associated logs:" << std::endl;
    debug.flush();
    int max_results = p->GetLogAmount();                    // Quantidade máxima possível de resultados
    int result_amount = max_results;                        // Quantidade de resultados
    Logradouro* results[max_results];
    for(int i = 0; i < max_results; i++) {
        results[i] = p->GetLog(i);
        debug << "\t\t\t" << results[i]->GetId() << " " << results[i]->GetName() << std::endl;
    }
    this->current_mem_usage += max_results * sizeof(Logradouro*) + 2*sizeof(int);
    UpdateMemory();

    debug << "\t\tFound " << max_results << " associated logs for word " << w << std::endl;

    // Restante das palavras - achar interseção dos logradouros
    while(std::getline(cons, w, ' ')) {
        debug << "\t\tNext word in query: " << w << std::endl;
        debug.flush();

        try {
            p = &this->consulta.Find(w);
        }
        catch(std::logic_error& e) {
            debug << "\t\tCouldn't find word " << w << ": " << e.what() << std::endl;
            out << idc << ";" << 0 << std::endl;
            return;
        }
        for(int i = 0; i < max_results; i++) {
            if(results[i] == nullptr) continue;

            bool is_intersection = false;

            for(int j = 0; j < p->GetLogAmount(); j++) {
                if(p->GetLog(j) == results[i]) {
                    debug << "\t\t\tFound intersection with logradouro " << results[i]->GetName() << std::endl;
                    debug.flush();
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
    debug << "\t\tFound " << result_amount << " results after intersection.\n";
    debug << "\t\tBuilding the actual results vector and sorting it.\n";
    debug.flush();

    int k = 0;
    for(int i = 0; i < max_results; i++) {
        if(results[i] == nullptr) continue;
        else {
            debug << "\t\t\tPosition " << i << " of the original vector, we found " << k << " not null results previously.\n";
            results_def[k] = results[i];
            results_def[k]->CalculateDelta(origin);
            debug << "\t\t\tThis is log " << results_def[k]->GetName() << " with delta " << results_def[k]->GetDelta() << std::endl;
            for(int j = k; j > 0; j--) {
                // debug << "\t\t\t\tInsertion sort phase " << k - j << std::endl;
                // debug << "\t\t\t\tComparing (" << results_def[j]->GetName() << ", delta " << results_def[j]->GetDelta() << ") with (" << results_def[j - 1]->GetName() << ", delta " << results_def[j - 1]->GetDelta() << ")\n";
                if(results_def[j]->GetDelta() < results_def[j - 1]->GetDelta()) {
                    Logradouro* temp = results_def[j];
                    results_def[j] = results_def[j - 1];
                    results_def[j - 1] = temp;
                }
                else break;
            }
            k++;
        }
    }

    debug << "\t\tresult_amount = " << result_amount << ", k = " << k << std::endl;
    debug.flush();
    if(k != result_amount) throw std::logic_error("k != result_amount");

    debug << "\t\tResults sorted by distance to origin. Here are them:\n";
    debug.flush();
    for(int i = 0; i < result_amount; i++) {
        debug << "\t\t\t" << results_def[i]->GetName() << " with id " << results_def[i]->GetId() << " at distance " << results_def[i]->GetDelta() << std::endl;
        debug.flush();
    }
    

    // Imprimir resultado da consulta
    debug << "\t\tPrinting results to output stream...\n";
    debug.flush();
    int count = 0;
    out << idc << ";" << min(result_amount, r) << std::endl;
    for(int i = 0; i < result_amount; i++) {
        if(i == r) break;
        out << results_def[i]->GetId() << ";" << results_def[i]->GetName() << std::endl;
    }

    // Finalização
    debug << "\t\tSearch finished." << std::endl;
    debug.flush();
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
