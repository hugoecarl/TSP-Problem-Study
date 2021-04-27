#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h> 

using namespace std;


void calcula_dist(vector<double> &x, vector<double> &y){
    int n = x.size();
    vector<int> pontos;
    double dist = 0;
    

    vector<int> ite;
    for (int i = 0; i < n; i++){
        ite.push_back(i);
    }
    
    pontos.push_back(0);
    int var = 0;
    
    while (!ite.empty()){
            int i = ite[var];
            ite.erase(ite.begin()+var);
            if (ite.empty())
                break;
            
            vector<double> distancias;
            for (auto j : ite)
            {
                double cont = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));  
                distancias.push_back(cont);
            }
            auto menor = min_element(distancias.begin(), distancias.end()); 
            var = distance(distancias.begin(), menor);
            pontos.push_back(ite[var]);   
}
    
    for (int h = 1; h < n; h++){
    dist += sqrt(pow(x[pontos[h-1]] - x[pontos[h]], 2) + pow(y[pontos[h-1]] - y[pontos[h]], 2));                            
    }                
    dist += sqrt(pow(x[pontos[n-1]] - x[pontos[0]], 2) + pow(y[pontos[n-1]] - y[pontos[0]], 2));
    
    cout << dist << " 0" << endl;
    
    for (int i = 0; i < pontos.size(); i++)
    {
        cout << pontos[i] << " ";
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
