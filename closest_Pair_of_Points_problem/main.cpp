#include <bits/stdc++.h>

using namespace std;

struct Ponto{
    double x, y;
};

void gerarPontos(vector<Ponto> &pontos, int tam){

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-100.0,100.0);

    for(int i = 0;i<tam;i++){
        pontos.push_back({dist(gen), dist(gen)});
    }

}


int main(){

    cout << "oiii" << endl;


    return 0;
}