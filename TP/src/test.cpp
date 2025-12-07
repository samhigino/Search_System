#include <fstream>
#include <iomanip>
#include <iostream>
#include "endereco.hpp"

using namespace std;

int main() {
    ifstream in("tests/input_1.txt", fstream::in);
    ofstream out("output/output.txt", fstream::out);
    ofstream debug("output/debug.txt", fstream::out);

    if(!debug.is_open()) {
        cout << "Error while opening debug.\n";
        return 1;
    }
    if(!in.is_open() || !out.is_open()) {
        debug << "Error while opening in or out.\n";
        return 2;
    }

    // Setup
    out << std::fixed << std::setprecision(2);
    debug << std::fixed << std::setprecision(2);
    debug << "Booted program.\n\n";
    debug.flush();

    // Leitura dos endereços e incialização da base de endereços
    debug << "Initializing manager...\n";
    debug.flush();
    int n;                  // Quantidade de endereços
    in >> n;
    BaseEnderecos manager(n);
    debug << "Manager initialized.\n\n";
    debug.flush();

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
        // Depuração
        debug << "Reading address " << i+1 << " of " << n << ":\n";
        debug.flush();

        // Leitura da linha inteira
        std::getline(in >> std::ws, line);

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
        debug << IdEnd << std::endl
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
        debug << "Inserting address...\n";
        debug.flush();
        manager.InsertAddress(IdEnd, IdLog, TipoLog, Log, num, Bairro, Regiao, CEP, lt, lg);
        debug << "Address inserted.\n\n";
        debug.flush();
    }

    // Consultas
    debug << "Building data...\n";
    debug.flush();
    manager.BuildData(debug);
    int m;      // Número de consultas
    int r;      // Máximo de respostas
    in >> m >> r;
    out << m;
    debug << "Data built.\n\n";
    debug.flush();

    int IdCon;              // Id da consulta
    std::string consulta;   // Consulta em si
    Point2D origin;         // Ponto de origem da consulta
    for(int i = 0; i < m; i++) {
        // Depuração
        debug << "Processing query " << i+1 << " of " << m << ":\n";
        debug.flush();

        // Leitura da linha inteira
        std::getline(in >> std::ws, line);

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
        double ltc = std::stod(lt_str);
        double lgc = std::stod(lg_str);
        origin = Point2D(lt, lg);

        // Impressão (debug)
        debug << idcon_str << std::endl
        << consulta << std::endl
        << ltc << std::endl
        << lgc << std::endl;

        // Execução da consulta
        debug << "Executing query...\n";
        debug.flush();
        manager.Search(IdCon, origin, consulta, r, out, debug);
        debug << "Query executed.\n\n";
        debug.flush();
    }

    debug << "Finished all operations.\n";
    debug.flush();

    debug.close();
    in.close();
    out.close();
    return 0;
}