#ifndef LOGRADOURO_H
#define LOGRADOURO_H
#include "2D_point.hpp"
#include <string>
#include <stdexcept>

const static int MAX_PTS = 100;

// PtoLog representa um endereço numérico dentro do logradouro
class PtoLog {
    friend class Logradouro;

    protected:
        int num;
        Point2D cords;

    public:
        PtoLog() : PtoLog(-1, 0, 0) { };
        PtoLog(int n, double lt, double lg) : num(n) {
            cords = Point2D(lt, lg);
        }
        void operator=(PtoLog& other) {
            num = other.num;
            cords = other.cords;
        }
};

class Logradouro {
    private:
        // Atributos gerais
        long int id;
        int num_palavras;
        std::string nome;
        Point2D centro;
        bool center_calculated;
        double delta;
        PtoLog pontos[MAX_PTS];
        int ponto_amount;

        // Controle de memória
        int mem_usage;

    public:
        // Construtor
        Logradouro(long int id, std::string nome);

        // Operações
        long int GetId();
        Point2D& GetCenter();
        std::string GetName();
        double GetDelta();
        int GetMemoryUsage();
        int InsertAdress(int num, double lt, double lg);
        void CalculateCenter();
        void CalculateDelta(Point2D origin);
};

#endif