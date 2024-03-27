#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

//Passos: 1- Ler o ficheiro em blocos e armazenar numa string => feito e testado 
        //2-Criar um dicionário, usando a hash table como estrutura de dados, com tamanho minimo de 2^16
        //3-Aplicar o algoritmo (compressão) 
        //4- Processamento estatístico básico dos resultados? 

//definir a estrutura do item

typedef struct Ht_item
{
    unsigned int key; // código atribuído a uma sequencia de entrada 
    std::string value; //padrão
    Ht_item* next;// apontador para o próximo item na lista encadeada de colisões
} Ht_item;

//definir a hashtable, que tem um array de apontadores a apontar para Ht_item 

typedef struct HashTable
{
    Ht_item** items;
    unsigned int count; // Contador de itens na tabela
} HashTable;

//Função de hash simples => a chave é dividida pelo tamanho da tabela e o rsultado é o indice calculado na tabela de hash. 

int hashFunction(unsigned int key, int tableSize) {
    return key % tableSize;
}


//Função para inicializar tabela de hash , new é usado para criar memória dinamicamente 
HashTable* create_table(unsigned long max_dic_size, int init_entrys) {
    HashTable* table = new HashTable; //Alocar memória para a tabela 
    table->items = new Ht_item*[max_dic_size];//alocar um novo apontador 
    table->count=0; //inicializar contador 
    
    // Inicializa todos os slots com nullptr, ou seja, contador nulo
    for (int i = 0; i < max_dic_size; i++) {
        table->items[i] = nullptr;
    }

    // Preenche com as 256 combinações iniciais 
    for (int f = 0; f < init_entrys; f++) {
        Ht_item* item = new Ht_item; // Aloca memória para o item
        item ->key = f; //Define a chave como índice 
        item->value = std::to_string(f); // Converte o índice para string e define como valor
        table->items[f] = item; // Adiciona o item à tabela
        table->count++; // Incrementa o contador de itens
    }
    
    return table;
}
// Função para imprimir a tabela hash
void display_table(HashTable* table, unsigned long max_dic_size) {
    std::cout << "HashTable:" << std::endl; // Imprime o cabeçalho
    for (int i = 0; i < max_dic_size; ++i) {
        if (table->items[i] != nullptr) { // Verifica se há um item na posição atual
            std::cout << "Key " << i << ": " << table->items[i]->key << ", Value: " << table->items[i]->value << std::endl;
        }
    }
}
//Função para limpar a hash table 
void clean(HashTable* hashtable,unsigned long max_dict_size){
    int i; 
    for ( i = 0 ; i < max_dict_size;i++){
        hashtable->items[i] = NULL;
    }
}

//Função para inserir padrão na hash table , tratamento por encandeamento para tratar colisões
unsigned int insert(HashTable* hashtable, int key, const std::string& value, unsigned long max_dict_size) {
    unsigned int hash = hashFunction(key, max_dict_size);
    Ht_item* item = new Ht_item; // Alocando memória para o novo item
    if (item == nullptr) {
        std::cerr << "Erro ao alocar memória para item" << std::endl;
        return 0;
    }
    item->key = key;
    item->value = value;
    item->next = nullptr;

    if (hashtable->items[hash] == nullptr) {
        // Se não houver nenhum item no índice hash, insira o novo item
        hashtable->items[hash] = item;
    } else {
        Ht_item* current = hashtable->items[hash];
        Ht_item* previous = nullptr;

        // Percorra a lista ligada para encontrar a posição correta para inserir o novo item
        while (current != nullptr && key > current->key) {
            previous = current;
            current = current->next;
        }

        if (previous == nullptr) {
            // Se o novo item deve ser o primeiro na lista ligada
            item->next = hashtable->items[hash];
            hashtable->items[hash] = item;
        } else {
            // Insira o novo item após o item anterior encontrado
            previous->next = item;
            item->next = current;
        }
    }
    
    return item->key;
}

unsigned long search(HashTable* hashtable, const std::string& value, unsigned long max_dict_size) {
    for (int i = 0; i < max_dict_size; i++) {
        Ht_item* item = hashtable->items[i];
        // Verifica se o item não é nulo e se o valor do item é igual ao valor procurado
        if (item != nullptr && item->value == value) {
            return item->key; // Retorna a chave do item encontrado
        }
    }

    // Retorna 1 indicando que o item não foi encontrado
    return 1;
}

//Função para libertar memória , em C++ delete é como free em C 
void free_table(HashTable* hashtable, unsigned long max_dic_size) {
    // Percorre todos os itens na tabela hash
    for (unsigned long i = 0; i < max_dic_size; ++i) {
        Ht_item* current = hashtable->items[i];
        while (current != nullptr) {
            Ht_item* temp = current;
            current = current->next;
            delete temp; // Liberta a memória alocada para o item
        }
    }
    // Liberta a memória alocada para o array de itens na tabela hash
    delete[] hashtable->items;
    // Liberta a memória alocada para a própria tabela hash
    delete hashtable;
}

// Função para ler um bloco do ficheiro

vector<char> readBlock(fstream& file, const streamsize blockSize) {
    vector<char> block(blockSize); // Cria um vetor para armazenar o bloco
    file.read(block.data(), blockSize); // Lê um bloco do arquivo
    streamsize bytesRead = file.gcount(); // Verifica quantos bytes foram lidos
    block.resize(bytesRead); // Redimensiona o vetor para o tamanho real lido
    return block;
}

// Função para processar um bloco
void processBlock(const vector<char>& block) {
    // Exemplo: imprimir o conteúdo do bloco
    for (char c : block) {
        cout << c;
    }
}

int main() {
    int blockcount=0; 
    unsigned long max_dic_size = 1<<16; 
    unsigned int init_entrys=256;
    fstream inputfile;
    inputfile.open("ola.txt", ios::in | ios::binary); // Abre o ficheiro 

    if (!inputfile.is_open()) { //verifica se o ficheiro existe 
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    const streamsize blockSize = 64 * 1024; // Tamanho do bloco em bytes (64KB), em computação, e nao em SI, qual delas deve ser?

    // Ler e processar o ficheiro em blocos
    while (!inputfile.eof()) {
        vector<char> block = readBlock(inputfile, blockSize);
        //processBlock(block);
        // Verificar o tamanho do bloco processado
        cout << "Tamanho do bloco processado: " << block.size() << " bytes" << endl;
        blockcount ++; 
    }

    inputfile.close();

    HashTable* hashTable = create_table(max_dic_size,init_entrys);
   

  
    //Printar
    display_table(hashTable,max_dic_size);

    // Limpar a tabela hash
    clean(hashTable, max_dic_size);

    //Libertas memória 
    free_table(hashTable, max_dic_size);


    return 0;
}
