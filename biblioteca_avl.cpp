#include <iostream>
#include <string>
using namespace std;

// Estrutura do nó da árvore AVL
struct Node {
    string title;
    string author;
    string ISBN;
    int year;
    Node* left;
    Node* right;
    int height;

    Node(string t, string a, string i, int y)
        : title(t), author(a), ISBN(i), year(y), left(nullptr), right(nullptr), height(1) {}
};

// Função para obter a altura de um nó
int height(Node* n) {
    return n ? n->height : 0;
}

// Função para calcular o fator de balanceamento de um nó
int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

// Função para rotacionar o nó à direita
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Função para rotacionar o nó à esquerda
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Função para inserir um novo nó na árvore AVL
Node* insert(Node* node, string title, string author, string ISBN, int year) {
    if (!node)
        return new Node(title, author, ISBN, year);

    if (title < node->title)
        node->left = insert(node->left, title, author, ISBN, year);
    else if (title > node->title)
        node->right = insert(node->right, title, author, ISBN, year);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && title < node->left->title)
        return rightRotate(node);

    if (balance < -1 && title > node->right->title)
        return leftRotate(node);

    if (balance > 1 && title > node->left->title) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && title < node->right->title) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Função para encontrar o nó com menor valor na árvore
Node* minValueNode(Node* node) {
    Node* current = node;

    while (current->left)
        current = current->left;

    return current;
}

// Função para remover um nó da árvore AVL
Node* deleteNode(Node* root, string title) {
    if (!root)
        return root;

    if (title < root->title)
        root->left = deleteNode(root->left, title);
    else if (title > root->title)
        root->right = deleteNode(root->right, title);
    else {
        if ((!root->left) || (!root->right)) {
            Node* temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->title = temp->title;
            root->author = temp->author;
            root->ISBN = temp->ISBN;
            root->year = temp->year;
            root->right = deleteNode(root->right, temp->title);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Função para buscar um livro pelo título
Node* search(Node* root, string title) {
    if (!root || root->title == title)
        return root;

    if (title < root->title)
        return search(root->left, title);

    return search(root->right, title);
}

// Função para exibir os livros em ordem alfabética
void inOrder(Node* root) {
    if (root) {
        inOrder(root->left);
        cout << "Title: " << root->title << ", Author: " << root->author
             << ", ISBN: " << root->ISBN << ", Year: " << root->year << endl;
        inOrder(root->right);
    }
}

// Função para atualizar informações de um livro
Node* updateBook(Node* root, string title, string newAuthor, string newISBN, int newYear) {
    Node* book = search(root, title);
    if (book) {
        book->author = newAuthor;
        book->ISBN = newISBN;
        book->year = newYear;
    }
    return root;
}

void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Adicionar Livro\n";
    cout << "2. Remover Livro\n";
    cout << "3. Buscar Livro\n";
    cout << "4. Listar Todos os Livros\n";
    cout << "5. Atualizar Informações do Livro\n";
    cout << "6. Sair\n";
    cout << "Escolha uma opção: ";
}

int main() {
    Node* root = nullptr;
    int choice;
    string title, author, ISBN;
    int year;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Limpar o buffer do teclado

        switch (choice) {
            case 1:
                cout << "Adicionar Livro\n";
                cout << "Título: ";
                getline(cin, title);
                cout << "Autor: ";
                getline(cin, author);
                cout << "ISBN: ";
                getline(cin, ISBN);
                cout << "Ano: ";
                cin >> year;
                root = insert(root, title, author, ISBN, year);
                cout << "Livro adicionado com sucesso!\n";
                break;
            case 2:
                cout << "Remover Livro\n";
                cout << "Título: ";
                getline(cin, title);
                root = deleteNode(root, title);
                cout << "Livro removido com sucesso!\n";
                break;
            case 3:
                cout << "Buscar Livro\n";
                cout << "Título: ";
                getline(cin, title);
                if (Node* book = search(root, title)) {
                    cout << "Livro encontrado:\n";
                    cout << "Título: " << book->title << "\n";
                    cout << "Autor: " << book->author << "\n";
                    cout << "ISBN: " << book->ISBN << "\n";
                    cout << "Ano: " << book->year << "\n";
                } else {
                    cout << "Livro não encontrado.\n";
                }
                break;
            case 4:
                cout << "Listar Todos os Livros\n";
                inOrder(root);
                break;
            case 5:
                cout << "Atualizar Informações do Livro\n";
                cout << "Título: ";
                getline(cin, title);
                cout << "Novo Autor: ";
                getline(cin, author);
                cout << "Novo ISBN: ";
                getline(cin, ISBN);
                cout << "Novo Ano: ";
                cin >> year;
                root = updateBook(root, title, author, ISBN, year);
                cout << "Informações do livro atualizadas com sucesso!\n";
                break;
            case 6:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    } while (choice != 6);

    return 0;
}
