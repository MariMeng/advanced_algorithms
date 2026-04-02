#include <bits/stdc++.h>

using namespace std;

struct Ponto{
    double x, y;
};

void gerarPontos(vector<Ponto> &pontos, int tam){

    random_device rd;
    mt19937 gen(17);
    uniform_real_distribution<double> dist(-100.0,100.0);

    for(int i = 0;i<tam;i++){
        pontos.push_back({dist(gen), dist(gen)});
    }

}

double menorDistanciaPadrao(vector<Ponto> &pontos, int tam){
    double menor = 10000000000;

    for(int i = 0;i<tam-1;i++){
        float x1 = pontos[i].x;
        float y1 = pontos[i].y;
        for(int t = i;t<tam-1;t++){
            float x2 = pontos[t+1].x;
            float y2 = pontos[t+1].y;
            double distancia = sqrt(((pow((x2-x1),2))+(pow((y2-y1),2))));
            if(distancia<menor){menor = distancia;}
        }
    }

    return menor;
}

double menorDistanciaOtima(vector<Ponto> &pontos, int tam){
    double menor = 0;


    return menor;
}

int main(){

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
            int n;
            cout << "Digite o número total de pontos";
            cin >> n;
            gerarPontos(plano, n);
            cout << "A menor distancia entre os pontos criado eh: " << menorDistanciaPadrao(plano,n) << endl;
            break;

        case 2: 
            break;

        case 3:
            break;
        
        default:
            break;
    }

    return 0;
}