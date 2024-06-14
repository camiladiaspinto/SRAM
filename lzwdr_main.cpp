
#include "lzwdr_biblioteca.h"
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;

int pattern_found=0;
int counter_output = 0;
unsigned int total_pattern_count = 0;
unsigned int total_pattern_size = 0;


// compilar: make clean all
// executar: ./lzwdr


//Função para imprimir o pedido no início do programa 
void inicial_print(unsigned int max_dic_size){
    cout << "Autor: Camila Pinto,Eduarda Dinis, Barbara Fonseca" << endl;
    cout << "Data de criação: 15/04/2024" << endl;
    cout << "Nome do ficheiro de entrada: " << "ola.txt" << endl;
    cout << "Nome do ficheirode saída: " << "saida.txt" << endl;
    cout << "Tamanho máximo do dicionário: " << max_dic_size << endl;
    cout << "Tipo de dicionário inicial: " << "HashTable, inicializada com 256 padrões." << endl;
    cout << "Tipo de limpeza: " << "Reset do dicionário, mantendo apenas as 256 entradas padrão." << endl;
}

//Função para dividir e ler cada bloco do ficheiro 
std::vector<char> read_block(std::fstream &file, const size_t block_size)
{
    vector<char> block(block_size);        // Cria um vetor para armazenar o bloco;
    file.read(block.data(), block_size);   // Lê um bloco do ficheiro
    streamsize bytes_read = file.gcount(); // Verifica quantos bytes foram lidos
    block.resize(bytes_read);              // Redimensiona o vetor para o tamanho real lido
    return block;
}


//Função para concatenar duas strings 
std::string concat(const std::string &Pa, const std::string &Pb)
{
    return Pa + Pb;
}

//Função que inverte uma string 
std::string reverse(const std::string &P)
{
    std::string reversed_pattern;
    for (int i = P.length() - 1; i >= 0; --i)
    {
        reversed_pattern += P[i];
    }
    return reversed_pattern;
}

//Função que armazena o código num buffer
void output(int code, std::string &output_buffer)
{
    output_buffer += to_string(code) + ", ";
    counter_output++; 
}

//Função de comrpessão
void compression(HashTable *hashTable, const std::vector<char> &block_content, unsigned long max_dic_size, std::string &output_buffer)
{

    // 1.-- Inserting all symbols from A as patterns in D.
    //   -- The first Pa is just the first symbol of S and it is already in D.
    // for (i=1 up to K) insert(A[i],D)
    // Pa=S[1,1]; ind=2

    int block_length = block_content.size();
    string Pa = string(1, block_content[0]);
    int ind = 2;
    // 2.-- Processing S to find the bigger pattern Pb after Pa already in D.
    // code=search(Pa,D); Pb=S[ind,1]; i=1
    // while (ind+i≤N & search(concat(Pb,S[ind+i,1]),D))
    //     Pb=concat(Pb,S[ind+i,1]); i=i+1
    // int code = search(hashTable, Pa, max_dic_size);
    int i = 1;
    int iteration = 1;

    cout << "Pa: " << Pa << endl;

    while (ind + i - 1 <= block_length)
    {
        cout<<"Iteração: "<<iteration<<endl;
        int code = search(hashTable, Pa, max_dic_size);
        //cout<<"Código enviado para a saída: "<< code << endl;
        i = 1;
        string Pb = string(1, block_content[ind - 1]);
        cout << "Pb: " << Pb << endl;
        // string Pb_new = string(block_content.begin() + ind - 1, block_content.begin() + ind + i);
        while (ind + i <= block_length)
        {
            string caracter = string(1, block_content[ind + i - 1]); // caracater a seguir ao pb
            if (search(hashTable, concat(Pb, caracter), max_dic_size))
            {                              // se encontrar,
                Pb = concat(Pb, caracter); // atualiza valor Pb e incrementa i
                i++;                       // anda à frente do ind (Pb) para ver qual o valor maior do buffer de entrada
                pattern_found++;
            }
            else
            {
                break;
            }
        }
        // 3.-- Send the code of Pa to the output.
        output(code, output_buffer);

        // 4.-- Insert in D all the new patterns while D is not full
        //   --(D maxes out at T entries).
        // j=1; t=T-1
        // while (j<=i & t<T)
        //     t=insert(concat(Pa,Pb[1,j]),D)
        //     if (t<T)
        //         t=insert(reverse(concat(Pa,Pb[1,j])),D)
        //     j=j+1
        int j = 1;
        unsigned long t = max_dic_size - 1;
        while (j <= i && t < max_dic_size)
        {
           
            cout << "Padrão a ser inserido: " << concat(Pa, Pb.substr(0, j)) << endl;
            string current_pattern = concat(Pa, Pb.substr(0, j));
            t = insert(hashTable, code, concat(Pa, Pb.substr(0, j)), max_dic_size); // concat(Pa, Pb.substr(1 - 1, (1 - 1) + j - 1))

            total_pattern_count++;
            total_pattern_size += current_pattern.length();

            if (t < max_dic_size)
            {
                cout << "Padrão inverso a ser inserido: " << reverse(concat(Pa, Pb.substr(0, j))) << endl;
                t = insert(hashTable, code, reverse(concat(Pa, Pb.substr(0, j))), max_dic_size);

            }
            j++;
        }

        // 5.-- Update variables and return to 3. if it’s not the end of S,
        //   -- otherwise it will end by sending to the output the code of Pb;
        //   -- even if D is full, it lets keep processing S…
        // if (ind+i>N)
        //    output(search(Pb,D))
        // else
        //     ind=ind+i; Pa=Pb; go to 2.
        if (ind + i > block_length)
        {
            output(search(hashTable, Pb, max_dic_size), output_buffer);
            return;
        }
        else
        {
            ind += i;
            Pa = Pb;
            i = 1;
        }
        iteration++;
    }
}

//Função que guarda o conteudo do buffer de saída num ficheiro 
void save_to_file(const std::string &output_buffer, const std::string &filename)
{
    ofstream outputFile(filename);
    if (outputFile.is_open())
    {
        outputFile << "Resultado final na saída: ";
        outputFile << output_buffer;
        outputFile << "0*" << endl;
        outputFile.close();
    }
    else
    {
        cerr << "Erro ao abrir o ficheiro de saída." << endl;
    }
}

// Função para obter o tempo atual em segundos
long long now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

//Função que imprime os dados finais (c/estatisticas)
void final_print(int block_count, int total_bytes, int deltaT_r, int deltaT_w, int compress_time, int total_duration_seconds,const vector<int>& block_sizes){
    //std::cout << "Tempo da operação de leitura: " << deltaT_r << " milissegundos" << std::endl;
    //std::cout << "Tempo da operação escrita : " << deltaT_w << " milissegundos" << std::endl;
    //std::cout << "Tempo total: " << total_duration << " milissegundos" << std::endl; //print que o prof quer 
    //std::cout << "Tempo compressão: " << compress_time<< " milissegundos" << std::endl;
    std::cout << "Tempo total da operação : " << total_duration_seconds<< " segundos" << std::endl;
    cout<<"*******************Estatíticas :  ********************          "<<endl;
    cout << "Número total de bytes processados: " << total_bytes<< " bytes" << endl;
    cout << "Número de padrões encontrados: " << pattern_found << endl;
    cout<<"Número de códigos de padrões enviados para o output: "<<counter_output<< endl;
    double average_pattern_size = static_cast<double>(total_pattern_size) / total_pattern_count;
    cout << "Tamanho médio dos padrões inseridos no dicionário: " << average_pattern_size << endl;

    cout << "Número de resets do dicionário: " << num_dict_resets  << endl;
    for (int i = 0; i < static_cast<int>(block_sizes.size()); i++)
    {
        cout << "Tamanho do bloco " << i + 1 << ": " << block_sizes[i] << " bytes" << endl;
    }
    cout<<"Número de blocos processados: "<<block_count<<endl;

}


int main(int argc, char *argv[])
{
    int block_count = 0;
    int total_bytes = 0;
    int  block_size = stoi(argv[1]); // Tamanho do bloco em bytes
    unsigned long max_dic_size = stoul(argv[2]); // Tamanho máximo do dicionário
    unsigned int init_entrys = 256; //numero de entradas inicial 
    string output_buffer; //string para armazenar os codigos de saida 
    fstream input_file; 
    vector<int> block_sizes; // vetor para armazenar o tamanho de cada bloco processado

    inicial_print(max_dic_size);

    input_file.open("teste.jpeg", ios::in | ios::binary); // Abre o ficheiro

    if (!input_file.is_open())
    { 
        std::cerr << "Erro ao abrir o ficheiro." << endl;
        return 1;
    }
    HashTable *hashTable = create_table(max_dic_size, init_entrys);


    //int start_r= 0, end_r= 0,deltaT_r,start_w,end_w,deltaT_w,readwrite_time;
    std::chrono::time_point<std::chrono::steady_clock> start_read, end_read,start_write,end_write,start_total,end_total,start_compress,end_compress;
    //std::chrono::time_point<std::chrono::steady_clock> start_read, end_read, start_write, end_write, total_start, total_end;
    double deltaT_r = 0.0, deltaT_w = 0.0;
    //long long start_total = now(), execution_time = 0;

    start_total = std::chrono::steady_clock::now();
    while (!input_file.eof())
    {
        start_read = std::chrono::steady_clock::now();
      
        

        vector<char> block = read_block(input_file, block_size);


        end_read = std::chrono::steady_clock::now();
     

        deltaT_r += std::chrono::duration_cast<std::chrono::milliseconds>(end_read - start_read).count();

        total_bytes+= block.size();
        int block_size_bytes = block.size(); 
        block_sizes.push_back(block_size_bytes); 


        compression(hashTable, block, max_dic_size, output_buffer);
        block_count++;

    
        start_write = std::chrono::steady_clock::now();
        save_to_file(output_buffer, "saida.txt");
        end_write = std::chrono::steady_clock::now();
        deltaT_w += std::chrono::duration_cast<std::chrono::milliseconds>(end_write - start_write).count();
       
        
    }
    end_total = std::chrono::steady_clock::now();
    long long total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - start_total).count();
    long long total_timerw = deltaT_r + deltaT_w;
    long long compress_time=total_duration-total_timerw;

    double total_duration_seconds = total_duration/ 1000.0;

   

    input_file.close();
    //display_table(hashTable,max_dic_size);
    final_print(block_count,total_bytes,deltaT_r,deltaT_w,compress_time,total_duration_seconds,block_sizes);
   
    return 0;
}