#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h> 
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/random/uniform_int_distribution.h>
#include <thrust/random/linear_congruential_engine.h>
#include <thrust/extrema.h>

using namespace std;
using namespace thrust;

struct custom_transform
{
   
    thrust::device_ptr<double> flat_mat;
    int *pontos;
    int n, seeds;

    custom_transform (thrust::device_ptr<double> flat_mat,int *pontos, int n, int seeds) : flat_mat(flat_mat), pontos(pontos), n(n), seeds(seeds) {}; 

    __host__ __device__
        double operator()(const int& x) {

            thrust::minstd_rand rng(x*1331+seeds);
            thrust::uniform_int_distribution<int> dist(0,n-1);
            
            double local_dist = 0;
            
            for (int i = 0; i < n; i++)
            {
                pontos[(n*x)+i] = i;
            }
            
            int v;
            int temp;  

            for (v = n-1; v > 0; --v) {
                int rnd = dist(rng);
                temp = pontos[(n*x)+v];
                pontos[(n*x)+v] = pontos[(n*x)+rnd];
                pontos[(n*x)+rnd] = temp;
            }
            
            for (int h = 1; h < n; h++){
                local_dist += flat_mat[pontos[(n*x)+h-1] * n + pontos[(n*x)+h]];
            }                
            local_dist += flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+0]];

            int i = 0;
            bool trocou = false;
            
            for (i = 0; i < n; i++)
            {    
                for (int j = i+1; j < n; j++)
                {
                    double dist = 0; 
                    double comp1 = 0;
                    double comp2 = 0;

                    if (i == 0){
                        if (j == n - 1){
                            comp1 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        } else {
                        comp1 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        }
                    } else if (j == 0){
                        if (i == n - 1){
                            comp1 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        } else {
                        comp1 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];    
                        }
                    } else if (j == n - 1){
                        if (i == 0){
                            comp1 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        } else {
                        comp1 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        }
                    } else if (i == n - 1){
                        if (j == 0){
                            comp1 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        } else {
                        comp1 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        }
                    } else {
                        comp1 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];                     
                    }
                    
                    thrust::swap(pontos[(n*x)+i], pontos[(n*x)+j]);
             
                    if (i == 0){
                        if (j == n - 1){
                            comp2 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        } else {
                        comp2 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        }
                    } else if (j == 0){
                        if (i == n - 1){
                            comp2 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        } else {
                        comp2 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];    
                        }
                    } else if (j == n - 1){
                        if (i == 0){
                            comp2 = flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        } else {
                        comp2 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+0]];
                        }
                    } else if (i == n - 1){
                        if (j == 0){
                            comp2 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        } else {
                        comp2 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+0]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];
                        }
                    } else {
                        comp2 = flat_mat[pontos[(n*x)+i-1] * n + pontos[(n*x)+i]] + flat_mat[pontos[(n*x)+i] * n + pontos[(n*x)+i+1]] + flat_mat[pontos[(n*x)+j-1] * n + pontos[(n*x)+j]] + flat_mat[pontos[(n*x)+j] * n + pontos[(n*x)+j+1]];                     
                    }
                        
                    if (comp2 < comp1) {
                        for (int h = 1; h < n; h++){
                        dist += flat_mat[pontos[(n*x)+h-1] * n + pontos[(n*x)+h]];                            
                        }                
                        dist += flat_mat[pontos[(n*x)+n-1] * n + pontos[(n*x)+0]];          
                        trocou = true;
                        local_dist = dist;
                        break;    
                    } else { 
                        thrust::swap(pontos[(n*x)+j], pontos[(n*x)+i]);
                    }
                }
                if (trocou && i == n - 1){
                    trocou = false;
                    i = -1;
                }
            }
            return local_dist;
        }
};

int main(){
    int seeds;
    
    if (getenv("SEED") == NULL)
        seeds = 10;
    else
        seeds = atoi(getenv("SEED"));

    vector<double> x, y, flat_mat1;
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
            flat_mat1.push_back(sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)));
        }
    }

    thrust::device_vector<double> flat_mat(flat_mat1);
    
    thrust::counting_iterator<int> iter(0);
    
    device_vector<double> vec(10*n, 0.0);    
    
    thrust::device_vector<int> pontos(10*n*n, 0);

    custom_transform cus(thrust::device_ptr<double>(flat_mat.data()), thrust::raw_pointer_cast(pontos.data()), n, seeds);
    
    thrust::transform(iter, iter + 10*n, vec.begin(), cus);
    
    thrust::device_vector<double>::iterator iter2 = thrust::min_element(vec.begin(), vec.end());

    int position = iter2 - vec.begin();
    
    if (getenv("DEBUG")){
        for (int i = 0; i < vec.size(); i++){
            cerr << "local: " << vec[i] << " ";
            for (int j = 0; j < n; j++)
            {
                cerr << pontos[i*n+j] << " ";
            }
            cerr << endl;
        }
    }
    
    cout << vec[position] << " " << 0 << endl;
    
    for (int i = 0; i < n; i++)
    {
        cout << pontos[position*n+i] << " ";
    }
    
    cout << endl;

    return 0;
}
