#include <bits/stdc++.h>

using namespace std;

int M = 3;
int N = 4;

using namespace std;

bool word_search(vector<string>& cop, string c, int local, int m, int n){

            if(local == c.length()){ return true;}
            else{
            if(m<0 || m>=M || n>=N || n<0) return false;
            if(cop[m][n] == c[local]){
                    char atual = cop[m][n];
                    cop[m][n] = '#';
                    if(word_search(cop,c, local+1, m, n+1)) {return true;}
                    if(word_search(cop,c, local+1, m, n-1)) {return true;}
                    if(word_search(cop,c, local+1, m+1, n)) {return true;}
                    if(word_search(cop,c, local+1, m-1, n)) {return true;}
                    cop[m][n] = atual;

                    return false;
            }else {
                return false;
            }
    }
    
}

int main(){


    vector <string> mat = {
    "ABCE",
    "SFCS",
    "ADEE",
    };

    vector <string> cop = mat;

    string c = "ABCCED";
    int local, m, n = 0;

    bool encontrou = false;
    
    for(int i = 0;i< M;i++){
        for(int j = 0;j<N;j++){
            if(mat[i][j] == c[0]){
     
                if(word_search(cop, c, local, i, j)){
                    encontrou = true;
                    break;
                }
       
            }
        }
    }

    if(encontrou){
        cout << "A palavra existe dentro da matriz!" << endl;
    }else {
        cout << "Palavra não existe dentro da matriz" << endl;
    }
    

    return 0;
}