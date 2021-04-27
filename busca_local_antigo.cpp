
#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h> 
#include <random>
#include <algorithm>
#include <stdlib.h>

using namespace std;

void calcula_dist(vector<double> &x, vector<double> &y){
    int n = x.size();
    vector<int> pontos;
    vector<int> ite;
    double melhor_dist = 1000000;     
    char* seed;
    int seeds;
    seed = getenv("SEED");
    
    if (seed == NULL)
        seeds = 10;
    else
        seeds = atoi(seed);
    
    for (int i = 0; i < n; i++)
    {
        pontos.push_back(i);
    }

    ite = pontos;
    for (int k = 0; k < 10*n; k++)
    {
        double local_dist = 1000000;
        shuffle(pontos.begin(), pontos.end(), default_random_engine(seeds));          
        int i = 0;
        for (i = 0; i < n; i++)
        {    
            for (int j = i ; j < n; j++)
            {
                double dist = 0; 
                iter_swap(pontos.begin() + i, pontos.begin() + j);
                
                for (int h = 1; h < n; h++){
                    dist += sqrt(pow(x[pontos[h-1]] - x[pontos[h]], 2) + pow(y[pontos[h-1]] - y[pontos[h]], 2));                            
                }                
                dist += sqrt(pow(x[pontos[n-1]] - x[pontos[0]], 2) + pow(y[pontos[n-1]] - y[pontos[0]], 2));         
        
                if (dist < local_dist) {
                    i = -1;
                    local_dist = dist;
                    break;
                } else { 
                    iter_swap(pontos.begin() + j, pontos.begin() + i);
                }
            }
        }
        cerr << "local: " << local_dist << " ";
        for (auto &k : pontos){
            cerr << k << " ";
        }
        cerr << endl;

        if (local_dist < melhor_dist) {
        melhor_dist = local_dist;
        ite = pontos;
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

    vector<double> x, y;
    double xi, yi;
    int n;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> xi >> yi;
        x.push_back(xi);
        y.push_back(yi);
    }
    
    calcula_dist(x, y);

}