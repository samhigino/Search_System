#include "pesquisa_palavra.hpp"
#include <stdexcept>
#include <iostream>
#include <queue>

//------------------------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
//------------------------------------------------------------------------------------------

// Achar o máximo O(1)
int max(int a, int b) {
    if(a < b) return b;
    else return a;
}

// Retornar a palavra de um nó O(1)
std::string NodeWord(Node* node) {
    if(node == nullptr)
        return "*";
    return node->palavra.GetWord();
}

// Comparação de strings O(n)
bool StrCompare(std::string s1, std::string s2) {
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
            throw std::invalid_argument("Comparando duas strings iguais.");
        }
    }

    return s1.at(i) > s2.at(i);
}

// Achar o fator de balanceamento de um nó O(1)
int Balance(Node* node, std::ostream& out) {
    if(node == nullptr)
        return 0;
    else {
        if(node->l_child == nullptr && node->r_child == nullptr) {
            out << "(0 - 0)";
            return 0;
        }
        else if(node->l_child == nullptr) {
            out << "(" << node->r_child->altura << " - 0)";
            return node->r_child->altura;
        }
        else if(node->r_child == nullptr) {
            out << "(0 - " << node->l_child->altura << ")";
            return (-1)*node->l_child->altura;
        }
        else {
            out << "(" << node->r_child->altura << " - " << node->l_child->altura << ")";
            return (node->r_child->altura - node->l_child->altura);
        }
    }
}

// Atualizando os níveis de cada nó
void UpdateLevels(Node* node, int n) {
    if(node == nullptr) return;
    node->level = n;
    UpdateLevels(node->l_child, n+1);
    UpdateLevels(node->r_child, n+1);
    return;
}

// Atualiza as alturas a partir de um nó para cima
void UpdateHeightUp(Node* node) {
    if(node == nullptr) return;

    if(node->l_child == nullptr && node->r_child == nullptr) {
        node->altura = 1;
    }
    else if(node->l_child == nullptr) {
        node->altura = 1 + node->r_child->altura;
    }
    else if(node->r_child == nullptr) {
        node->altura = 1 + node->l_child->altura;
    }
    else {
        node->altura = 1 + max(node->l_child->altura, node->r_child->altura);
    }

    UpdateHeightUp(node->parent);
    return;
}

// Atualiza as alturas a partir de um nó para baixo
int UpdateHeightDown(Node* node) {
    if(node == nullptr) return 0;
    node->altura = 1 + max(UpdateHeightDown(node->l_child), UpdateHeightDown(node->r_child));
    return node->altura;
}

// Função para apagar todos os nós da árvore (destrutor)
void DeleteNodes(Node* node) {
    if(node == nullptr) return;
    DeleteNodes(node->l_child);
    DeleteNodes(node->r_child);
    delete node;
    return;
}

// Rotação para a direita em um nó
void BasePalavras::RotateRight(Node* node, std::ostream& out) {
    // Depuração
    out << "\t\t\t\tInitiating a right rotation.\n";
    out.flush();

    // Identificação do pai + atualização de memória
    Node* pai = node->parent;
    if(pai == nullptr) return;
    this->current_usage += sizeof(Node*);
    UpdateMemory();
    out << "\t\t\t\t\tIdentification: ok.\n";
    out.flush();

    // Rotação
    pai->l_child = node->r_child;
    node->r_child = pai;
    out << "\t\t\t\t\tRotation: ok.\n";
    out.flush();

    // Atualização dos pais
    node->parent = pai->parent;
    pai->parent = node;
    if(pai->l_child != nullptr) pai->l_child->parent = pai;
    out << "\t\t\t\t\tParenting update: ok.\n";
    out.flush();

    // Atualizações do avô
    if(node->parent != nullptr) {
        if(node->parent->l_child == pai) node->parent->l_child = node;
        if(node->parent->r_child == pai) node->parent->r_child = node;
    }
    out << "\t\t\t\t\tPops update: ok.\n";
    out.flush();

    // Atualização da raíz e das alturas
    if(this->root == pai) {
        out << "\t\t\t\t\tNeeded to reassign root!\n";
        this->root = node;
    }
    UpdateHeightDown(node);
    out << "\t\t\t\t\tRoot and heights: ok.\n";
    out.flush();

    // Atualização dos níveis e finalização
    UpdateLevels(node, pai->level);
    this->current_usage -= sizeof(Node*);
    out << "\t\t\t\t\tEverything seems ok.\n";
    out.flush();
}

// Rotação para a esquerda em um nó
void BasePalavras::RotateLeft(Node* node, std::ostream& out) {
    // Depuração
    out << "\t\t\t\tInitiating a left rotation.\n";
    out.flush();

    Node* pai = node->parent;
    if(pai == nullptr) return;
    this->current_usage += sizeof(Node*);
    UpdateMemory();
    out << "\t\t\t\t\tIdentification: ok.\n";
    out.flush();

    // Rotação
    pai->r_child = node->l_child;
    node->l_child = pai;
    out << "\t\t\t\t\tRotation: ok.\n";
    out.flush();

    // Atualização dos pais
    node->parent = pai->parent;
    pai->parent = node;
    if(pai->r_child != nullptr) pai->r_child->parent = pai;
    out << "\t\t\t\t\tParenting update: ok.\n";
    out.flush();

    // Atualizações do avô
    if(node->parent != nullptr) {
        if(node->parent->l_child == pai) node->parent->l_child = node;
        if(node->parent->r_child == pai) node->parent->r_child = node;
    }
    out << "\t\t\t\t\tPops update: ok.\n";
    out.flush();

    // Atualização da raíz e das alturas
    if(this->root == pai) {
        out << "\t\t\t\t\tNeeded to reassign root!\n";
        this->root = node;
    }
    UpdateHeightDown(node);
    out << "\t\t\t\t\tRoot and heights: ok.\n";
    out.flush();

    // Atualização dos níveis e finalização
    UpdateLevels(node, pai->level);
    this->current_usage -= sizeof(Node*);
    out << "\t\t\t\t\tEverything seems ok.\n";
    out.flush();
}

// Impressão da árvore por níveis
void BasePalavras::PrintByLevel(Node* node, std::ostream& out) {
    int prev_level = 0;
    std::queue<Node*> q;
    q.push(node);
    Node* p;

    out << "0: ";

    while(!q.empty()) {
        p = q.front();
        q.pop();

        if(p != nullptr) {
            if(p->level > prev_level) {
                out << std::endl;
                out << p->level << ": ";
            }
            out << (p->palavra.GetWord()) << " ";
            prev_level = p->level;

            if(p->l_child != nullptr) {
                // std::cout << "\nQueuing left child of " << p->palavra.GetWord() << ": " << p->l_child->palavra.GetWord() << std::endl;
                q.push(p->l_child);
            }
                
            if(p->r_child != nullptr) {
                // std::cout << "\nQueuing right child of " << p->palavra.GetWord() << ": " << p->r_child->palavra.GetWord() << std::endl;
                q.push(p->r_child);
            }
        }
    }

    out << "\n";
}

//------------------------------------------------------------------------------------------
// DEFINIÇÃO DOS MÉTODOS DA CLASSE BasePalavras
//------------------------------------------------------------------------------------------

// Construtor: apenas inicializa a raíz vazia
BasePalavras::BasePalavras() {
    this->root = nullptr;
    this->current_usage = sizeof(Node*);
}

// Destrutor: apaga todos os nós post-order
BasePalavras::~BasePalavras() {
    DeleteNodes(this->root);
}

// Inserção de uma palavra na árvore
void BasePalavras::Insert(Palavra palavra, std::ostream& out) {
    // Depuração
    out << "Trying to insert: " << palavra.GetWord() << std::endl;
    out.flush();

    // Criação do nó
    Node* new_node = new Node(palavra);
    this->current_usage += sizeof(Node*) + sizeof(*new_node);
    UpdateMemory();

    // Inserção na raíz caso seja o primeiro nó
    if(this->root == nullptr) {
        this->root = new_node;
        this->current_usage -= sizeof(Node*);
        out << "\tTrivial case. New root: " << NodeWord(this->root) << std::endl;
        out.flush();
        return;
    }

    // Depuração
    out << "\tNot trivial case. Finding spot to insert.\n";
    out.flush();

    // Inserção abaixo de um nó folha
    Node* spot = this->root;
    bool left_insertion;

    while(spot != nullptr) {
        // Caso a palavra inserida seja igual a uma já na árvore
        if(palavra.GetWord() == NodeWord(spot)) {
            throw std::logic_error("Inserindo palavra ja existente na arvore.");
        }
        
        // Insere na subárvore da esquerda
        if(!StrCompare(palavra.GetWord(), NodeWord(spot))) {
            if(spot->l_child == nullptr) {
                spot->l_child = new_node;
                new_node->parent = spot;
                left_insertion = true;
                out << "\t\tInserting on the left of " << NodeWord(spot) << std::endl;
                out.flush();
                break;
            }
            else spot = spot->l_child;
        }
        // Insere na subárvore da direita
        else {
            if(spot->r_child == nullptr) {
                spot->r_child = new_node;
                new_node->parent = spot;
                left_insertion = false;
                out << "\t\tInserting on the right of " << NodeWord(spot) << std::endl;
                out.flush();
                break;
            }
            else spot = spot->r_child;
        }
    }

    // Atualização das alturas e nivel do novo nó
    new_node->level = new_node->parent->level + 1;
    out << "\tUpdating heights...\n";
    out.flush();
    UpdateHeightUp(new_node);
    out << "\tHeights updated.\n";
    out << "\tNow evaluating balance...\n";
    out.flush();

    // Balanceamento
    Node* node = new_node->parent->parent;
    Node* next_node;
    int balance;

    while(node != nullptr) {
        next_node = node->parent;
        out << "\t\tEvaluating balance of " << NodeWord(node) << ": ";
        out.flush();
        balance = Balance(node, out);
        out << " = " << balance << std::endl;
        
        // Nó está desbalanceado, fazer rotações
        if(balance < -1 || balance > 1) {
            // Caso 1: left-left
            if(left_insertion && balance < -1) {
                try {
                    out << "\t\t\tNeeds balancing: Case 1\n";
                    out.flush();
                    RotateRight(node->l_child, out);
                }
                catch(...) {
                    throw std::logic_error("Erro no caso 1.");
                }
            }
            // Caso 2: right-right
            else if(!left_insertion && balance > 1) {
                try {
                    out << "\t\t\tNeeds balancing: Case 2\n";
                    out.flush();
                    RotateLeft(node->r_child, out);
                }
                catch(...) {
                    throw std::logic_error("Erro no caso 2.");
                }
            }
            // Caso 3: left-right
            else if(!left_insertion && balance < -1) {
                try {
                    out << "\t\t\tNeeds balancing: Case 3\n";
                    out.flush();

                    RotateLeft(node->l_child->r_child, out);

                    out << "\t\t\tAfter left rotation:\n\n";
                    PrintByLevel(node, out);
                    out.flush();

                    RotateRight(node->l_child, out);

                    out << "\t\t\tAfter right rotation:\n\n";
                    PrintByLevel(node->parent, out);
                    out.flush();
                }
                catch(...) {
                    throw std::logic_error("Erro no caso 3.");
                }
            }
            // Caso 4: right-left
            else if(left_insertion && balance > 1) {
                try {
                    out << "\t\t\tNeeds balancing: Case 4\n";
                    out.flush();

                    RotateRight(node->r_child->l_child, out);

                    out << "\t\t\tAfter right rotation:\n\n";
                    PrintByLevel(node, out);
                    out.flush();

                    RotateLeft(node->r_child, out);

                    out << "\t\t\tAfter left rotation:\n\n";
                    PrintByLevel(node->parent, out);
                    out.flush();
                }
                catch(...) {
                    throw std::logic_error("Erro no caso 4.");
                }
            }
        }
        else {
            out << "\t\tNo balancing needed.\n";
        }
        
        UpdateHeightUp(node);
        node = next_node;
    }

    // Finalização
    UpdateLevels(this->root, 0);
    this->current_usage -= sizeof(Node*);
    out << "\tBalancing ok (at first glance) finished execution.\n";
    out.flush();
}

// Busca por uma palavra na árvore
Palavra& BasePalavras::Find(std::string palavra) {
    Node* current = this->root;
    while(current != nullptr) {
        // Verifica se o nó atual é a palavra procurada
        if(NodeWord(current) == palavra) {
            return current->palavra;
        }
        // Se não, se a palavra procurada for menor, vá para a subarvore da esquerda
        else if(!StrCompare(palavra, NodeWord(current))) {
            current = current->l_child;
        }
        // Se não, se a palavra procurada for maior, vá para a subárvore da direita
        else {
            current = current->r_child;
        }
    }

    throw std::logic_error("Palavra não encontrada na árvore.");
}

void BasePalavras::PrintByLevel(std::ostream& out) {
    PrintByLevel(this->root, out);
}

//------------------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//------------------------------------------------------------------------------------------

// Atualização de memória máxima usada
void BasePalavras::UpdateMemory() {
    if(this->current_usage > this->max_usage) {
        this->max_usage = current_usage;
    }
}

int BasePalavras::GetCurrentMemoryUsage() {
    return this->current_usage;
}

int BasePalavras::GetMaxMemoryUsage() {
    return this->max_usage;
}