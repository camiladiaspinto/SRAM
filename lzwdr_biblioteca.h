#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

extern int pattern_found;
extern unsigned int num_dict_resets ;

extern unsigned int total_pattern_count;
extern unsigned int total_pattern_size;


// Estruturas
/// @brief Estrutura que representa um padrão
typedef struct Ht_item
{
    unsigned int key;  // código atribuído a uma sequencia de entrada
    std::string value; // padrão
    Ht_item *next;     // apontador para o próximo item na lista ligada
} Ht_item;

/// @brief Estrutura que representa a hashtable
typedef struct HashTable
{
    Ht_item **items;    // array de apontadores a apontar para Ht_item
    unsigned int count; // Contador de itens na tabela
} HashTable;

// Funções de lzwdr_main.cpp

/// @brief Função para imprimir o pedido no início do programa 
/// @param max_dic_size
void inicial_print(unsigned int max_dic_size);

/// @brief Função para ler um bloco do ficheiro
/// @param file
/// @param block_size
/// @return Retorna o bloco 
std::vector<char> read_block(std::fstream &file, const size_t block_size);


/// @brief Função que concatena duas strings
/// @param Pa
/// @param Pb
/// @return
std::string concat(const std::string &Pa, const std::string &Pb);


/// @brief Função que inverte uma string 
/// @param P
/// @return
std::string reverse(const std::string &P);

/// @brief Função output, armazena o código em um buffer
/// @param code
/// @param outputBuffer
void output(int code, std::string &outputBuffer);

/// @brief Função de compressão
/// @param hashTable
/// @param blockContent
/// @param max_dic_size
/// @param outputBuffer
void compression(HashTable *hashTable, const std::vector<char> &blockContent, unsigned long max_dic_size, std::string &outputBuffer);


/// @brief Função que guarda o conteudo do buffer de saída num ficheiro 
/// @param outputBuffer
/// @param filename
void saveToFile(const std::string &outputBuffer, const std::string &filename);


// Funções de lzwdr_hashtable.cpp

/// @brief Função para calcular a função hash
/// @param value
/// @param max_dic_size
/// @return valor de hash
int hash_function(const std::string &value, unsigned long max_dic_size);

/// @brief Função para inicializar tabela de hash 
/// @param max_dic_size
/// @param init_entrys
/// @return a tabela de hash criada, com os 256 padroes iniciais 
HashTable *create_table(unsigned long max_dic_size, int init_entrys);

/// @brief Função para printar o padrão (value) e o código (key)
/// @param item
void print_pattern(const Ht_item *item);

/// @brief Função para imprimir a tabela hash
/// @param table
/// @param max_dic_size
void display_table(const HashTable *table, unsigned long max_dic_size);


/// @brief Função para inserir padrão na tabela de hash
/// @param hashtable
/// @param key
/// @param value
/// @param max_dict_size
/// @return Caso o padrão não se encontre na hashtable, retorna o código, caso contrário retorna 0 
unsigned int insert(HashTable *hashtable, int key, const std::string &value, unsigned long max_dict_size);

/// @brief Função para procurar um padrão na tabela hash 
/// @param hashtable
/// @param value
/// @param max_dict_size
/// @return Caso o padrão seja encontrado retorna o código, caso contrário retorna 0 
unsigned int search(HashTable *hashtable, const std::string &value, unsigned long max_dict_size);


/// @brief FUnção para resetar a tabela, ou seja, voltar ao dicionário inciial quando o número máximo de entradas é atingido 
/// @param hashtable
/// @param value
/// @param max_dict_size
/// @return Retorna o número de vezes que o dicionário ficou cheio 
int reset(HashTable *hashtable, unsigned long max_dict_size);