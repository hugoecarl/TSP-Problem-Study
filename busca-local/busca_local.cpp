#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h> 
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <omp.h>

using namespace std;

double troca_dist(int &i, int &j, int &n, vector<double> &x, vector<double> &y, vector<int> &pontos, vector<double> &flat_mat){
    if (i == 0){
        if (j == n - 1){
            return flat_mat[pontos[n-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[0]];
        }
        return flat_mat[pontos[n-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];
    } else if (j == 0){
        if (i == n - 1){
            return flat_mat[pontos[i-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[0]] + flat_mat[pontos[n-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];
        }
        return flat_mat[pontos[n-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];    
    } 
    if (j == n - 1){
        if (i == 0){
            return flat_mat[pontos[n-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[0]];
        }
        return flat_mat[pontos[i-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[0]];
    } else if (i == n - 1){
        if (j == 0){
            return flat_mat[pontos[i-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[0]] + flat_mat[pontos[n-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];
        }
        return flat_mat[pontos[i-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[0]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];
    }
    return flat_mat[pontos[i-1] * n + pontos[i]] + flat_mat[pontos[i] * n + pontos[i+1]] + flat_mat[pontos[j-1] * n + pontos[j]] + flat_mat[pontos[j] * n + pontos[j+1]];

}

void calcula_dist(vector<double> &x, vector<double> &y, vector<double> &flat_mat){
    int n = x.size();
    vector<int> pontos;
    vector<int> ite;
    double melhor_dist = 1000000;     
    int seeds;
    
    if (getenv("SEED") == NULL)
        seeds = 10;
    else
        seeds = atoi(getenv("SEED"));
    
    for (int i = 0; i < n; i++)
    {
        pontos.push_back(i);
    }

    ite = pontos;
    
    #pragma omp parallel for
    for (int k = 0; k < 10*n; k++)
    {
        vector<int> pontos_mcore = pontos;
        double local_dist = 0;
        
        #ifdef _OPENMP
        shuffle(pontos_mcore.begin(), pontos_mcore.end(), default_random_engine((omp_get_thread_num()+1) * seeds));
        #else
        shuffle(pontos.begin(), pontos.end(), default_random_engine(seeds));
        #endif

        for (int h = 1; h < n; h++){
            local_dist += flat_mat[pontos_mcore[h-1] * n + pontos_mcore[h]];                            
        }                
        local_dist += flat_mat[pontos_mcore[n-1] * n + pontos_mcore[0]];            
        int i = 0;
        
        for (i = 0; i < n; i++)
        {    
            for (int j = i + 1; j < n; j++)
            {
                double dist = 0; 
                double comp1 = 0;
                double comp2 = 0;

                comp1 = troca_dist(i, j, n, x, y, pontos_mcore, flat_mat);
                
                iter_swap(pontos_mcore.begin() + i, pontos_mcore.begin() + j);
                
                comp2 = troca_dist(i, j, n, x, y, pontos_mcore, flat_mat);
        
                if (comp2 < comp1) {
                    for (int h = 1; h < n; h++){
                    dist += flat_mat[pontos_mcore[h-1] * n + pontos_mcore[h]];                            
                    }                
                    dist += flat_mat[pontos_mcore[n-1] * n + pontos_mcore[0]];          
                    i = -1;
                    local_dist = dist;
                    break;
                } else { 
                    iter_swap(pontos_mcore.begin() + j, pontos_mcore.begin() + i);
                }
            }
        }
        
        #pragma omp critical
        {
            if (getenv("DEBUG")){
                cerr << "local: " << local_dist << " ";
                for (auto &k : pontos_mcore){
                    cerr << k << " ";
                }
                cerr << endl;
            }

            if (local_dist < melhor_dist) {
            melhor_dist = local_dist;
            ite = pontos_mcore;
            }
        }             
    }
    cout << melhor_dist << " " << 0 << endl;
    
    for (int i = 0; i < n; i++)
    {
        cout << ite[i] << " ";
    }
    cout << endl;   
}
    
int main(){

    vector<double> x, y, flat_mat;
    double xi, yi;
    int n;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> xi >> yi;
        x.push_back(xi);
        y.push_back(yi);
    }
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            flat_mat.push_back(sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)));
        }
    }

    calcula_dist(x, y, flat_mat);

}