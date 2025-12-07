#include <iomanip>
#include <iostream>
#include "endereco.hpp"

int main() {
    // Setup
    std::cout << std::fixed << std::setprecision(2);

    // Leitura dos endereços e incialização da base de endereços
    int n;                  // Quantidade de endereços
    std::cout << "Enter number of addresses: ";
    std::cin >> n;
    BaseEnderecos manager(n);

    std::string line;       // Linha inteira de comando
    std::string IdEnd;      // Id do endereço
    int IdLog;              // Id do logradouro
    std::string TipoLog;    // Tipo de logradouro
    std::string Log;        // Logradouro
    int num;                // Número do endereço
    std::string Bairro;     // Bairro do endereço
    std::string Regiao;     // Região do endereço
    int CEP;                // CEP do endereço
    double lt;              // Latitude
    double lg;              // Longitude

    for(int i = 0; i < n; i++) {
        std::cout << "Enter address data: ";
        // Leitura da linha inteira
        std::getline(std::cin >> std::ws, line);

        // Auxiliares
        std::stringstream ss(line);
        std::string idlog_str;
        std::string num_str;
        std::string cep_str;
        std::string lt_str;
        std::string lg_str;

        // Separação da linha
        std::getline(ss, IdEnd, ';');
        std::getline(ss, idlog_str, ';');
        std::getline(ss, TipoLog, ';');
        std::getline(ss, Log, ';');
        std::getline(ss, num_str, ';');
        std::getline(ss, Bairro, ';');
        std::getline(ss, Regiao, ';');
        std::getline(ss, cep_str, ';');
        std::getline(ss, lt_str, ';');
        std::getline(ss, lg_str, ';');

        // Conversões de tipo
        IdLog = std::stoi(idlog_str);
        num = std::stoi(num_str);
        CEP = std::stoi(cep_str);
        lt = std::stod(lt_str);
        lg = std::stod(lg_str);

        // Impressão (debug)
        std::cout << IdEnd << std::endl
        << IdLog << std::endl
        << TipoLog << std::endl
        << Log << std::endl
        << num << std::endl
        << Bairro << std::endl
        << Regiao << std::endl
        << CEP << std::endl
        << lt << std::endl
        << lg << std::endl;

        // Inserção do endereço na base
        manager.InsertAddress(IdEnd, IdLog, TipoLog, Log, num, Bairro, Regiao, CEP, lt, lg);
    }

    // Consultas
    manager.BuildData(std::cout);
    int m;      // Número de consultas
    int r;      // Máximo de respostas
    std::cin >> m >> r;
    std::cout << m;

    int IdCon;              // Id da consulta
    std::string consulta;   // Consulta em si
    Point2D origin;         // Ponto de origem da consulta
    for(int i = 0; i < m; i++) {
        // Leitura da linha inteira
        std::getline(std::cin >> std::ws, line);

        // Auxiliares
        std::stringstream ss(line);
        std::string idcon_str;
        std::string consulta;
        std::string lt_str;
        std::string lg_str;

        // Separação da linha
        std::getline(ss, idcon_str, ';');
        std::getline(ss, consulta, ';');
        std::getline(ss, lt_str, ';');
        std::getline(ss, lg_str, ';');

        // Conversões de tipo
        IdCon = std::stoi(idcon_str);
        double lt = std::stod(lt_str);
        double lg = std::stod(lg_str);
        origin = Point2D(lt, lg);

        // Execução da consulta
        manager.Search(IdCon, origin, consulta, r, std::cout, std::cout);
    }

    return 0;
}