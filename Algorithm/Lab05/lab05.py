#bin, bin2 구현
def bin(n, k):
    if k == 0 or n == k:
        return 1
    else:
        return bin(n - 1, k - 1) + bin(n - 1, k)

def bin2(n, k):
    B = [[0 for cols in range(k + 1)] for rows in range(n + 1)]
    for i in range(0, n + 1):
        for j in range(0, min(i, k) + 1):
            if j == 0 or j == i:
                B[i][j] = 1
            else:
                B[i][j] = B[i - 1][j - 1] + B[i - 1][j]
    
    return B[n][k]

print(bin(10, 5), bin2(10, 5))

#Floyd 알고리즘 구현
def allShortestPath(g, n):
    P = [[0 for cols in range(n)] for rows in range(n)]
    D = g
    for k in range(0, n):
        for i in range(0, n):
            for j in range(0, n):
                if D[i][k] + D[k][j] < D[i][j]:
                    P[i][j] = k + 1
                    D[i][j] = D[i][k] + D[k][j]
    
    return D, P

def printMatrix(d):
    n = len(d[0])
    for i in range(0, n):
        for j in range(0, n):
            print(d[i][j], end = " ")
        print()

#두 노드 간의 최단경로를 출력하는 path함수
import utility

def path(p, q, r):
    if p[q - 1][r - 1] != 0:
        path(p, q, p[q - 1][r - 1])
        print("v", p[q - 1][r - 1])
        path(p, p[q - 1][r - 1], r)

inf = 1000
g = [[0, 1, inf, 1, 5],
     [9, 0, 3, 2, inf],
     [inf, inf, 0, 4, inf],
     [inf, inf, 2, 0, 3],
     [3, inf, inf, inf, 0]]
print()
printMatrix(g)
d, p = allShortestPath(g, 5)
print()
printMatrix(d)
print()
printMatrix(p)
path(p, 5, 3)

#연쇄행렬 최소곱셈 알고리즘 구현
def order(p, i, j):
    if i == j:
        print("A", i, end = " ")
    else:
        k = p[i][j]
        print("(", end = " ")
        order(p, i, k)
        order(p, k + 1, j)
        print(")", end = " ")

d = [5, 2, 3, 4, 6, 7, 8]
n = len(d) - 1

m = [[0 for j in range(1, n + 2)] for i in range(1, n + 2)]
p = [[0 for j in range(1, n + 2)] for i in range(1, n + 2)]

import sys

def minmult(d, n, m, p):
    for i in range(1, n + 1):
        m[i][i] = 0

    for diagonal in range(1, n):
        for i in range(1, n - diagonal + 1):
            j = i + diagonal
            m[i][j] = sys.maxsize
            for k in range(i, j):    
                x = m[i][k] + m[k + 1][j] + d[i - 1]*d[k]*d[j]
                if x < m[i][j]:
                    m[i][j] = x
                    p[i][j] = k
    return m[1][n]

print()
minmult(d, n, m, p)

utility.printMatrix(m)
print()
utility.printMatrix(p)
order(p, 1, 6)