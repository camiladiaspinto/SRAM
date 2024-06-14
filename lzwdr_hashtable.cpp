#include "lzwdr_biblioteca.h"

using namespace std;

unsigned int num_dict_resets = 0;



//Função para a função hash 
int hash_function(const std::string &value, unsigned long max_dic_size)
{
    unsigned int hash = 0;
    for (char c : value)
    {
        hash += static_cast<unsigned int>(c); // Soma o valor ASCII de cada caracter
    }
    return hash % max_dic_size;
}

// Função para inicializar tabela  hash , new é usado para criar memória dinamicamente
HashTable *create_table(unsigned long max_dic_size, int init_entrys)
{
    HashTable *table = new HashTable;           // alocar memória para a tabela
    table->items = new Ht_item *[max_dic_size]; // alocar um novo apontador
    table->count = 1;                           // inicializar contador

    // Inicializa todos os slots com nullptr, ou seja, contador nulo
    for (unsigned long i = 0; i < max_dic_size; i++)
    {
        table->items[i] = nullptr;
    }

    // Preenche com os 256 padrões iniciais
    for (int f = 0; f < init_entrys; f++)
    {
        Ht_item *item = new Ht_item; // Aloca memória para o item
        item->key = f;               // Define a chave como índice
        item->value = std::string(1, static_cast<char>(f)); // Converte o índice para string e define como valor, converte o numero para valor ASCII
        table->items[f] = item;                             // Adiciona o item à tabela
        table->count++;                                     // Incrementa o contador
    }

    return table;
}

void print_pattern(const Ht_item *item)
{
    cout << "  Padrão: " << item->value << " Código: " << item->key << endl;
}

// Função para imprimir a tabela hash
void display_table(const HashTable *table, unsigned long max_dic_size)
{
    std::cout << std::endl;
    std::cout << " Dicionário Atual:" << std::endl;

    for (unsigned int i = 0; i < max_dic_size; ++i)
    {
        if (table->items[i] != nullptr)  //se o slot não estiver vazio 
        {
            Ht_item *current = table->items[i]; //apontador para o item atual
            std::cout << "Hash: " << i << std::endl;
            print_pattern(current);

            while (current->next != nullptr) //continua a percorrer os items que existirem nesse índice 
            {
                current = current->next;//proximo item da lista ligada 
                print_pattern(current);
            }
        }
    }
}

//Função para inserir um item na tabela hash 
unsigned int insert(HashTable *hashtable, int key, const std::string &value, unsigned long max_dict_size)
{
    // Verificar se a tabela está cheia após os 256 padrões iniciais
    if (hashtable->count >= max_dict_size)
    {
        reset(hashtable,max_dict_size); //faz reset da tabela hash com as 256 posiçõe iniciais
    }

    int index = hash_function(value, max_dict_size); 
    // Verificar se o padrão já está presente, isto é feito para eliminar duplicados 
    if (search(hashtable, value, max_dict_size) != 0)
    {
        return 0; 
    }

    Ht_item *item = new Ht_item; // Alocando memória para o novo item
    if (item == nullptr)
    {
        std::cerr << "Erro ao alocar memória para item" << std::endl;
        return 0;
    }
    item->key = key;
    item->value = value;
    item->next = nullptr;


    if (hashtable->items[index] == nullptr)
    {
        // Se não houver nenhum item no índice hash, insire o novo item
        item->key = hashtable->count; // o count começa em 1 e incrementa, key seja o numero do padrão que estou a inserir
        hashtable->items[index] = item;
        hashtable->count++;
        return item->key; // Retorna o índice hash
    }
    else
    {
        Ht_item *current = hashtable->items[index];

        // Percorra a lista ligada para encontrar se o padrão já está presente
        while (current->next != nullptr)
        {
            if (current->value == value)
            { // Verifica se o padrão já está presente
                return 0; // Retorna zero se o padrão já estiver presente
            }
            current = current->next;
        }

        // Chegamos ao final da lista ligada sem encontrar o padrão, então vamos inseri-lo no final
        item->key = hashtable->count; // o count começa em 1 e incrementa, key seja o numero do padrão que estou a inserir
        current->next = item;
        hashtable->count++;

    
        return item->key; //retorna o código
    }
}

//Função para procurar um padrão na tabela hash 
unsigned int search(HashTable *hashtable, const std::string &value, unsigned long max_dict_size)
{
    int index = hash_function(value, max_dict_size); // Calcula o índice hash do padrão

    // Verifica se há algum item na posição da tabela hash correspondente ao índice calculado
    Ht_item *current = hashtable->items[index];
    while (current != nullptr)
    {
        // Verifica se o valor do item é igual ao padrão 
        if (current->value == value)
        {
           // cout << "Padrão encontrado: " << value << ", Código: " << current->key << endl;
            return current->key; // Retorna o código do padrão se encontrado
        }
        current = current->next; // Avança para o próximo item na lista ligada 
    }

    // Se o padrão não for encontrado, retorna zero
    //cout << "Padrão não encontrado: " << value << endl;
    return 0;
}

// Função para resetar ("limpar") a hash table mantendo os 256 primeiros itens
int reset(HashTable *hashtable, unsigned long max_dict_size)
{
    for (unsigned long i = 256; i < max_dict_size; ++i)
    {
        Ht_item *current = hashtable->items[i];
        while (current != nullptr)
        {
            Ht_item *temp = current;
            current = current->next;
            delete temp; // Liberta a memória alocada para o item
        }
        hashtable->items[i] = nullptr; // Marca como nullptr para indicar que está vazio
    }
    //contagem de itens para 256
    hashtable->count = 256;
    num_dict_resets++;
    return num_dict_resets;
}
