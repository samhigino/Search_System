#include "logradouro.hpp"

//------------------------------------------------------------------------------------------
// GETTERS
//------------------------------------------------------------------------------------------

// Construtor
Logradouro::Logradouro(long int id, std::string nome) {
    this->id = id;
    this->nome = nome;
    this->center_calculated = false;

    // Detecta quantas palavras o Logradouro tem
    int n = 1;
    for(int i = 0; i < nome.size(); i++) {
        if(nome.at(i) == ' ') n++;
    }

    this->num_palavras = n;
    this->ponto_amount = 0;
    this->mem_usage = sizeof(*this);
}

// GetId
long int Logradouro::GetId() {
    return this->id;
}

// GetCenter
Point2D& Logradouro::GetCenter() {
    if(!center_calculated) {
        throw std::logic_error("Trying to get a center before calculating it.");
    }
    return this->centro;
}

// GetName
std::string Logradouro::GetName() {
    return this->nome;
}

// GetDelta
double Logradouro::GetDelta() {
    return this->delta;
}

// GetMemoryUsage
int Logradouro::GetMemoryUsage() {
    return this->mem_usage;
}

//------------------------------------------------------------------------------------------
// OUTRAS OPERAÇÕES
//------------------------------------------------------------------------------------------

// InsertAdress: insere um novo endereço numérico nesse Logradouro
int Logradouro::InsertAdress(int num, double lt, double lg) {
    // Exceção para tamanho máximo
    if(this->ponto_amount == MAX_PTS - 1) {
        throw std::out_of_range("Numero maximo de enderecos atingido nesse logradouro.");
    }

    // Exceção para centro de gravidade já calculado
    if(this->center_calculated) {
        throw std::logic_error("Impossivel inserir mais um endereco nesse logradouro: centro de gravidade ja calculado.");
    }

    PtoLog new_pto(num, lt, lg);
    this->pontos[this->ponto_amount] = new_pto;
    this->ponto_amount++;

    // Retorna o uso de memória auxiliar
    return sizeof(new_pto);
}

// CalculateCenter: calcula o centro de gravidade desse Logradouro
void Logradouro::CalculateCenter() {
    if(center_calculated) return;

    double med_lt = 0;
    double med_lg = 0;

    for(int i = 0; i < ponto_amount; i++) {
        med_lt += this->pontos->cords.GetX();
        med_lg += this->pontos->cords.GetY();
    }

    med_lt = med_lt / ponto_amount;
    med_lg = med_lg / ponto_amount;

    this->centro = Point2D(med_lt, med_lg);
    this->center_calculated = true;
}

// CalculateDelta: calcula a distância do centro desse logradouro até a origem informada
void Logradouro::CalculateDelta(Point2D origin) {
    if(!center_calculated) {
        throw std::logic_error("Impossivel calcular delta antes do centro de gravidade.");
    }

    this->delta = this->centro.Distance(origin);
    return;
}
