#ifndef PESQUISAPALAVRAS_H
#define PESQUISAPALAVRAS_H
#include "palavra.hpp"

struct Node {
    Palavra palavra;
    int altura;
    int level;
    Node* l_child;
    Node* r_child;
    Node* parent;

    Node(Palavra palavra) : palavra(palavra), altura(1), level(0), l_child(nullptr), r_child(nullptr), parent(nullptr) { }
};

class BasePalavras {
    private:
        // Atributos relacionados à arvore
        Node* root;                     // Raíz da árvore

        // Funções auxiliares
        void RotateRight(Node* node, std::ostream& out);   // Função de rotação para a direita
        void RotateLeft(Node* node, std::ostream& out);    // Função de rotação para a esquerda
        void PrintByLevel(Node* node, std::ostream& out);

        // Controle de memória
        void UpdateMemory();            // Atualiza a memória máxima usada
        int current_usage;              // Uso de memória atual
        int max_usage;                  // Uso máximo de memória global

    public:
        // Construtor e Destrutor
        BasePalavras();
        ~BasePalavras();

        // Operações
        void Insert(Palavra palavra, std::ostream& out);
        Palavra& Find(std::string palavra, std::ostream& out);
        void PrintByLevel(std::ostream& out);

        // Controle de memória
        int GetCurrentMemoryUsage();
        int GetMaxMemoryUsage();
};

#endif