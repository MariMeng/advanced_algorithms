#include <stdio.h>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip> // Para organizar as colunas

using namespace std;

struct Ponto{
    double x, y;
};

void gerarPontos(vector<Ponto> &pontos, int quantidade)
{
    //inicializa o gerador de numeros aleatorios
    random_device rd;
    mt19937 gen(rd());
    //intervalo das coordenadas
    uniform_real_distribution<double> dist(-1000.0, 1000.0);

    for(int i = 0; i < quantidade; i++){
        pontos.push_back({dist(gen), dist(gen)});
    }
}

bool lerArquivo(vector<Ponto> &pontos, string nomeArquivo )
{
    ifstream arquivo(nomeArquivo);
    if(!arquivo.is_open())
        return false;
    
    double x, y;
    while(arquivo >> x >> y)
    {
        pontos.push_back({x, y});
    }

    arquivo.close();
    return true;
}

void imprimirListaPontos(const vector<Ponto>& pontos) {
    cout << "\n--- Lista de Pontos no Plano ---" << endl;
    cout << setw(10) << "Indice" << setw(15) << "X" << setw(15) << "Y" << endl;
    cout << "------------------------------------------" << endl;
    
    for (size_t i = 0; i < pontos.size(); ++i) {
        cout << setw(10) << i 
             << setw(15) << fixed << setprecision(2) << pontos[i].x 
             << setw(15) << fixed << setprecision(2) << pontos[i].y << endl;
        
        // Limite de impressão para não travar o terminal se houver 1 milhão de pontos
        if (i == 49 && pontos.size() > 50) {
            cout << "... (e mais " << pontos.size() - 50 << " pontos) ..." << endl;
            break;
        }
    }
}

int main()
{
    vector<Ponto> plano;
    int opcao;

    cout << "----------MENU----------\n";
    cout << "Closest Pair of Points problem\n";
    cout << "1 - Plano com 100 pontos: \n";
    cout << "2 - Plano aleatório com tamanho escolhido: \n";
    cout << "3 - Sair \n";
    cin >> opcao;


    switch (opcao)
    {
        
        case 1:
            if(lerArquivo(plano, "pontos.txt"))
            {
                cout << "Arquivo carregado com sucesso";
            }else{
                cout << "Erro ao carregar o arquivo";
            }
            break;
        
        case 2: 
            int n;
            cout << "Digite o número total de pontos";
            cin >> n;
            gerarPontos(plano, n);
            break;

        case 3:
            break;
        
        default:
            break;
    }
    
    cout << "Total de pontos no plano: " << plano.size();
    imprimirListaPontos(plano);

    return 0;
}