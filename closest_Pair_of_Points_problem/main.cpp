#include <bits/stdc++.h>

using namespace std;

struct Ponto{
    double x, y;
};

bool comparaX(Ponto x1,Ponto x2){
    if (x1.x<x2.x){return true;}else{return false;}
}

bool comparaY(Ponto y1,Ponto y2){
    if (y1.y<y2.y){return true;}else{return false;}
}

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

double menorDist(vector<Ponto> &pontos, int i, int tam){
    double menor = 10000000000;

    for(i ;i<tam-1;i++){
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

double menorDist_Div_Conq(vector<Ponto> &pontos, int inicio, int fim){
    double menorEsq, menorDir, menor = 100000.0;

    int tam = fim - inicio;

    if(tam<=3){
        return menorDist(pontos, inicio, fim);
    }

    int meio = inicio +(tam/2);
 
    menorDir = menorDist_Div_Conq(pontos, inicio, meio);
    menorEsq = menorDist_Div_Conq(pontos, meio, fim);

    cout << "----------------- Menores Distancias Encontradas nas esquerda e direita" << endl;
    cout << "Esquerda:" << menorEsq << endl; 
    cout << "Direita:" << menorDir << endl; 


    if(menorDir<menorEsq){menor = menorDir;}else{menor = menorEsq;}

    vector<Ponto> faixa;
    Ponto pontoMeio = pontos[meio];
    for(int z = inicio;z<fim;z++){
        if(abs(pontos[z].x - pontoMeio.x)< menor){faixa.push_back(pontos[z]);}

    }

    sort(faixa.begin(), faixa.end(), comparaY);

    for(int i = 0;i<faixa.size();i++){
        for(int j = i+1;j<faixa.size() && (faixa[j].y - faixa[i].y)<menor;j++){

            double dist = sqrt(pow(faixa[i].x - faixa[j].x, 2) + pow(faixa[i].y - faixa[j].y, 2));

            if(dist < menor){
                menor = dist;
            }
        }
    }

    return menor;
}

int main(){

    vector<Ponto> plano;
    int opcao, n;
 cout << "----------MENU----------\n";
    cout << "Closest Pair of Points problem\n";
    cout << "1 - Plano com força bruta\n";
    cout << "2 - Plano com divisao e conquista: \n";
    cout << "3 - Sair \n";
    cin >> opcao;


    switch (opcao)
    {
        
        case 1:
            cout << "Digite o número total de pontos";
            cin >> n;
            gerarPontos(plano, n);
            cout << "A menor distancia entre os pontos criado eh: " << menorDistanciaPadrao(plano,n) << endl;
            break;

        case 2: 
            cout << "Digite o número total de pontos";
            cin >> n;
            gerarPontos(plano, n);
            sort(plano.begin(), plano.end(), comparaX);
            cout << "A menor distancia entre os pontos criado eh: " << menorDist_Div_Conq(plano,0, n) << endl;
            break;

        case 3:
            break;
        
        default:
            break;
    }

    return 0;
}