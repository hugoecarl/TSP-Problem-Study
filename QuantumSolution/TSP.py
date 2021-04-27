import math

class TSP:
    
    def __init__(self, flat_mat, n):
        self.flat_mat = flat_mat
        self.n = n
        self.melhor_dist = 99999999
        self.pontos = [-1] * n
        self.pontos[0] = 0
        self.melhores_pontos = []

    def busca_exaustiva(self, flat_mat, n, ite):
    
        for i in range(n):
            if ite == n:
                dist = 0
                
                for j in range(1, n):
                    dist += flat_mat[self.pontos[j-1] * n + self.pontos[j]]
                dist += flat_mat[self.pontos[n-1] * n + self.pontos[0]]

                if dist < self.melhor_dist:
                    self.melhor_dist = dist
                    self.melhores_pontos = self.pontos[:]
                
                return

            if self.pontos[i] == -1:   
                self.pontos[i] = ite
                self.busca_exaustiva(flat_mat, n, ite + 1)
                self.pontos[i] = -1

    def get_results(self):
        self.busca_exaustiva(self.flat_mat, self.n, 1)
        return self.melhor_dist, self.melhores_pontos


if __name__ == "__main__":
    
    x = [] 
    y = []
    flat_mat = [] #matriz 1 dimensao contendo todas as distancias possiveis entre os pontos.

    while True:
        try:    
            temp = input().split()
            x.append(float(temp[0]))
            y.append(float(temp[1]))
        except:
            break
    
    for i in range(len(x)):
        for j in range(len(y)):
            flat_mat.append(math.sqrt((x[i] - x[j])**2 + (y[i] - y[j])**2))

    Tsp = TSP(flat_mat, len(x))
    distancia, pontos = Tsp.get_results()
    print("Melhor distancia encontrada: ", distancia)
    print("Melhor caminho encontrado: ", pontos)