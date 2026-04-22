#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "raylib.h"

using namespace std;

int M = 7;
int N = 7;

const int cell_size = 70;
const int screenWidth = 490;
const int screenHeight = 490;

using namespace std;

void draw_grid(const vector<string>& mat, const vector<vector<int>>& status) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            Color color = WHITE;
            if (status[i][j] == 1) color = GREEN; // Caminho atual

            DrawRectangle(j * cell_size, i * cell_size, cell_size, cell_size, color);
            DrawRectangleLines(j * cell_size, i * cell_size, cell_size, cell_size, LIGHTGRAY);
            DrawText(TextFormat("%c", mat[i][j]), j * cell_size + 35, i * cell_size + 30, 40, DARKGRAY);
        }
    }
}

bool word_search(vector<string>& cop, string c,vector <vector<int>>& status, int local, int m, int n){

            if(local == c.length()){ return true;}
            else{
            if(m<0 || m>=M || n>=N || n<0) return false;
            if(cop[m][n] == c[local]){
                    status[m][n] = 1;
                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        draw_grid(cop, status); // Função auxiliar para desenhar a grade
                    EndDrawing();
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    char atual = cop[m][n];
                    cop[m][n] = '#';
                    if(word_search(cop,c, status, local+1, m, n+1)) {return true;}
                    if(word_search(cop,c,status, local+1, m, n-1)) {return true;}
                    if(word_search(cop,c, status, local+1, m+1, n)) {return true;}
                    if(word_search(cop,c, status, local+1, m-1, n)) {return true;}
                    cop[m][n] = atual;

                    status[m][n] = 0; 
                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        draw_grid(cop, status);
                    EndDrawing();
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                    return false;
            }else {
                return false;
            }
    }
    
}

int main(){

    InitWindow(screenWidth, screenHeight, "Word Search");
    SetTargetFPS(60);

    vector<vector<int>> status(M, vector<int>(N, 0));
    vector <string> mat2 = {
        "ALGORIT", 
        "ALGORIT", 
        "XXXXXXM", 
        "CNAVASO", 
        "AXXXXXX", 
        "DOSXXXX", 
        "XXXXXXX"  
    };
   string c2 = "ALGORITMOSAVANCADOS";
    vector<string> mat = {
        "ABCFXXX", 
        "ABXXXXX", 
        "AXXXXXX", 
        "XABCCFX", 
        "XXXXXEX", 
        "XXABCCE", 
        "XXXXXXD"  
    };
    
    string c = "ABCCED";

    vector <string> cop = mat;

   
    int local, m, n = 0;

    bool encontrou = false;
    bool buscaFinalizada = false;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE) && !buscaFinalizada) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (mat[i][j] == c[0]) {
                        if (word_search(cop, c, status, 0, i, j)) {
                            encontrou = true;
                            break;
                        }
                    }
                }
                if (encontrou) break;
            }
            buscaFinalizada = true;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_grid(mat, status);
            
            if (!buscaFinalizada) {
                DrawText("", 50, 140, 20, DARKBLUE);
            } else {
                DrawText(encontrou ? "SUCESSO!" : "FALHA!", 10, 10, 20, encontrou ? GREEN : RED);
            }
        EndDrawing();
    }

    if(encontrou){
        cout << "A palavra existe dentro da matriz!" << endl;
    }else {
        cout << "Palavra não existe dentro da matriz" << endl;
    }
    
    CloseWindow();
    return 0;
}