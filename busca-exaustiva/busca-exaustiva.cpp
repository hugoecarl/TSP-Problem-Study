#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h> 
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <omp.h>

using namespace std;

struct data
{
int ite;
double valor;
};

bool is_less(const data& x, const data& y) { return x.valor < y.valor; }

void exaustivo(vector<double> &flat_mat, int &n, int ite, double &melhor_dist, vector<int> &pontos, vector<int> &melhor_pontos, long long int &num_leaf, double &local_dist, vector<bool> &visitados, int &debug, vector<data> &pontos_mais_proximos){
    
    if (ite == n){
            double dist = 0;
            
            for (int j = 1; j < n; j++)
            {
                dist += flat_mat[pontos[j-1] * n + pontos[j]];
            }
            dist += flat_mat[pontos[n-1] * n + pontos[0]];

            num_leaf++;
             #pragma omp critical 
            {
            if (dist < melhor_dist) {
                melhor_dist = dist;
                melhor_pontos = pontos;
            }
            }
            return;
        }
    
    if (!debug){
	    //bound 1
        for (int k = 0; k < n; k++)
        {      
            if (!visitados[pontos_mais_proximos[(ite-1)*n+k].ite]){
            
                if (flat_mat[pontos[ite-1]*n+pontos_mais_proximos[(ite-1)*n+k].ite] + local_dist > melhor_dist) {
                    return;
                }
            }
        }
        //bound 2
        if (local_dist > melhor_dist){
		    return;
	    }
        //paralelo 
        if (ite == 1){
            #pragma omp parallel for
            for (int i = 1; i < n; i++)
            {
                vector<int> pontos_mcore = pontos; 
                vector<bool> visitados_mcore = visitados;   
                double local_dist_mcore = local_dist; 
                if (!visitados[i])
                {      
                    pontos_mcore[ite] = i;
                    visitados_mcore[i] = true;
                    local_dist_mcore += flat_mat[pontos_mcore[ite-1] * n + pontos_mcore[ite]];
                    exaustivo(flat_mat, n, ite+1, melhor_dist, pontos_mcore, melhor_pontos, num_leaf, local_dist_mcore, visitados_mcore, debug, pontos_mais_proximos);
                    local_dist_mcore -= flat_mat[pontos_mcore[ite-1] * n + pontos_mcore[ite]];
                    visitados_mcore[i] = false;       
                }
            }
        } else {
            for (int i = 1; i < n; i++)
            {   
                if (!visitados[i])
                {      
                    pontos[ite] = i;
                    visitados[i] = true;
                    local_dist += flat_mat[pontos[ite-1] * n + pontos[ite]];
                    exaustivo(flat_mat, n, ite+1, melhor_dist, pontos, melhor_pontos, num_leaf, local_dist, visitados, debug, pontos_mais_proximos);
                    local_dist -= flat_mat[pontos[ite-1] * n + pontos[ite]];
                    visitados[i] = false;       
                }
            }
        }
    } else {
        for (int i = 1; i < n; i++)
        {   
            if (!visitados[i])
            {      
                pontos[ite] = i;
                visitados[i] = true;
                local_dist += flat_mat[pontos[ite-1] * n + pontos[ite]];
                exaustivo(flat_mat, n, ite+1, melhor_dist, pontos, melhor_pontos, num_leaf, local_dist, visitados, debug, pontos_mais_proximos);
                local_dist -= flat_mat[pontos[ite-1] * n + pontos[ite]];
                visitados[i] = false;       
            }
        }
    }
}    
                                                          
int main()
{
    vector<double> x, y, flat_mat;
    vector<bool> visitados;
    vector<int> pontos, melhor_pontos;
    double xi, yi;
    int n, debug;
    long long int num_leaf = 0;            
    const char* env_p = getenv("DEBUG");
    debug = 0;
    
    if (getenv("DEBUG")) {
    if (atoi(env_p)){
	debug = 1;
    } else if (!atoi(env_p)) {
	debug = 0;	
    	}
    }
    cin >> n;
        
    for (int i = 0; i < n; i++)
    {
        cin >> xi >> yi;
        x.push_back(xi);
        y.push_back(yi);
        pontos.push_back(-1);
        visitados.push_back(false);
    }
   
    vector<data> pontos_mais_proximos;
    for (int i = 0; i < n; i++)
    {
        data fl;
        for (int j = 0; j < n; j++)
        {
            flat_mat.push_back(sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)));
            fl.ite = j;
            fl.valor = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
            pontos_mais_proximos.push_back(fl);
        }
    }
 
    double menor = 9999999;
    int iter;
    

    for (int i = 0; i < n; i++)
    {
        sort(pontos_mais_proximos.begin()+n*i,pontos_mais_proximos.begin()+n*i+n,is_less);
    }

    double melhor_dist = 1000000;
    pontos[0] = 0;
    double local_dist = 0;
    
    exaustivo(flat_mat, n, 1, melhor_dist, pontos, melhor_pontos, num_leaf, local_dist, visitados, debug, pontos_mais_proximos);

    std::cerr << "num_leaf " << num_leaf << endl;
    std::cout << melhor_dist << " " << 1 << endl;
    
    for (int i = 0; i < n; i++)
    {
        std::cout << melhor_pontos[i] << " ";
    }

    std::cout << endl;   



}
